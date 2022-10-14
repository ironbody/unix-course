#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 4096
#define MAX_CLUSTERS 32
#define NUM_THREADS 4

typedef struct point
{
  float x;     // The x-coordinate of the point
  float y;     // The y-coordinate of the point
  int cluster; // The cluster that the point belongs to
} point;

struct thread_args
{
  int idx;
  int from;
  int to;
  int old_cluster;
  int new_cluster;
  bool something_changed;
};

int N; // number of entries in the data
int k; // number of centroids
char *filename;
char *outputfile;
point data[MAX_POINTS];      // Data coordinates
point cluster[MAX_CLUSTERS]; // The coordinates of each cluster center (also
                             // called centroid)

int lines_in_fp()
{
  FILE *fp = fopen(filename, "r");
  int count = 0;

  char buf[200];
  while (fgets(buf, sizeof(buf), fp) != NULL)
  {
    count++;
  }

  return count;
}

void read_data()
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror("Cannot open the file");
    exit(EXIT_FAILURE);
  }

  N = lines_in_fp();

  // Initialize points from the data file
  // float temp;
  for (int i = 0; i < N; i++)
  {
    fscanf(fp, "%f %f", &data[i].x, &data[i].y);
    data[i].cluster = -1; // Initialize the cluster number to -1
  }
  printf("Read the problem data!\n");
  // Initialize centroids randomly
  srand(0); // Setting 0 as the random number generation seed
  for (int i = 0; i < k; i++)
  {
    int r = rand() % N;
    cluster[i].x = data[r].x;
    cluster[i].y = data[r].y;
  }
  fclose(fp);
}

int get_closest_centroid(int i, int k)
{
  /* find the nearest centroid */
  int nearest_cluster = -1;
  double xdist, ydist, dist, min_dist;
  min_dist = dist = INT_MAX;
  for (int c = 0; c < k; c++)
  { // For each centroid
    // Calculate the square of the Euclidean distance between that centroid and
    // the point
    xdist = data[i].x - cluster[c].x;
    ydist = data[i].y - cluster[c].y;
    dist = xdist * xdist + ydist * ydist; // The square of Euclidean distance
    // printf("%.2lf \n", dist);
    if (dist <= min_dist)
    {
      min_dist = dist;
      nearest_cluster = c;
    }
  }
  // printf("-----------\n");
  return nearest_cluster;
}

// Used for parallelism.
// Input: thread_args
// Output: void
void *find_point_cluster(void *params)
{
  struct thread_args *args = (struct thread_args *)params;

  args->new_cluster = get_closest_centroid(args->idx, k);
  if (args->old_cluster != args->new_cluster)
  {
    args->something_changed = true;
  }
  pthread_exit(NULL);
}

bool assign_clusters_to_points()
{
  struct thread_args *targs = malloc(NUM_THREADS * sizeof(struct thread_args));
  pthread_t *children = malloc(NUM_THREADS * sizeof(pthread_t));

  bool something_changed = false;

  for (size_t i = 0; i < NUM_THREADS; i++)
  {
    targs[i].from = ceil(i * (N / NUM_THREADS));
    targs[i].to = ceil((i + 1) * (N / NUM_THREADS));
    // targs[i].old_cluster = data[i].cluster;
    // targs[i].new_cluster = -1;
    targs[i].something_changed = false;

    pthread_create(&(children[i]),
                   NULL,
                   find_point_cluster,
                   (void *)&(targs[i]));
  }

  // create threads for calculating the new cluster for each point
  for (int i = 0; i < N; i++)
  { // For each data point
    // struct thread_args* arg = malloc(sizeof(struct thread_args));
  }

  // join threads
  for (int i = 0; i < N; i++)
  {
    pthread_join(children[i], NULL);
  }

  // update data and check if something changed
  for (int i = 0; i < N; i++)
  {
    int idx, cluster;
    idx = targs[i].idx;
    cluster = targs[i].new_cluster;
    data[idx].cluster = cluster;

    if (targs[i].something_changed)
    {
      something_changed = true;
    }
  }

  free(children);
  free(targs);
  return something_changed;
}

void update_cluster_centers()
{
  /* Update the cluster centers */
  int c;
  int count[MAX_CLUSTERS] = {
      0}; // Array to keep track of the number of points in each cluster
  point temp[MAX_CLUSTERS] = {{0.0}};

  for (int i = 0; i < N; i++)
  {
    c = data[i].cluster;
    count[c]++;
    temp[c].x += data[i].x;
    temp[c].y += data[i].y;
  }

  for (int i = 0; i < k; i++)
  {
    cluster[i].x = temp[i].x / count[i];
    cluster[i].y = temp[i].y / count[i];
  }
}

void kmeans()
{
  bool somechange;
  int iter = 0;
  do
  {
    iter++; // Keep track of number of iterations
    somechange = assign_clusters_to_points();
    update_cluster_centers();
  } while (somechange);
  printf("Number of iterations taken = %d\n", iter);
  printf("Computed cluster numbers successfully!\n");
}

void write_results()
{

  FILE *fp = fopen(outputfile, "w");
  if (fp == NULL)
  {
    perror("Cannot open the file");
    exit(EXIT_FAILURE);
  }
  else
  {
    for (int i = 0; i < N; i++)
    {
      fprintf(fp, "%.2f %.2f %d\n", data[i].x, data[i].y, data[i].cluster);
    }
  }
  printf("Wrote the results to a file!\n");
}

// following function is adapted from Håkan Grahn
void Read_Options(int argc, char **argv)
{
  char *prog;

  prog = *argv;
  while (++argv, --argc > 0)
    if (**argv == '-')
      switch (*++*argv)
      {
      case 'h':
        printf("\nUsage: kmeans [-k] number of clusters\n");
        printf("           [-f] file name for input data \n");
        printf("           [-h] help \n");
        exit(0);
        break;
      case 'f':
        --argc;
        filename = *++argv;
        break;
      case 'k':
        --argc;
        int num = atoi(*++argv);
        if (num > MAX_CLUSTERS)
        {
          printf("k cannot be more than: %u", MAX_CLUSTERS);
          exit(0);
        }
        k = num;
        break;
      case 'o':
        --argc;
        outputfile = *++argv;
        break;
      default:
        printf("%s: ignored option: -%s\n", prog, *argv);
        printf("HELP: try %s -u \n\n", prog);
        break;
      }
}

int main(int argc, char **argv)
{
  k = 9;
  filename = "kmeans-data.txt";
  outputfile = "kmeans-results.txt";

  Read_Options(argc, argv);
  read_data();
  kmeans();
  write_results();
}