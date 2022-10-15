/***************************************************************************
 *
 * Sequential version of Matrix Inverse
 * An adapted version of the code by H�kan Grahn
 ***************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MAX_SIZE 4096
#define NUM_THREADS 4
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
typedef double matrix[MAX_SIZE][MAX_SIZE];

int N;              /* matrix size		*/
int maxnum;         /* max number of element*/
char *Init;         /* matrix init type	*/
int PRINT;          /* print switch		*/
matrix A;           /* matrix A		*/
matrix I = {{0.0}}; /* The A inverse matrix, which will be initialized to the identity matrix */
int client_nr = 0;
char *Output_file;

/* forward declarations */

// void *subtract_rows(void *params);
void write_matrix_to_file(matrix M, char name[], FILE *fptr);
void *matrix_inverse(void *params);

void find_inverse();
void Init_Matrix(void);
void Print_Matrix(matrix M, char name[]);

void Init_Default(void);
void Read_Options(int, char **);

struct threadArgs
{
    int p;
    int start_row;
    int end_row;
};

int main(int argc, char **argv)
{
    // pthread_mutex_init(&lock, NULL);

    printf("Matrix Inverse\n");
    // int i, timestart, timeend, iter;

    Init_Default();           /* Init default values	*/
    Read_Options(argc, argv); /* Read arguments	*/
    Init_Matrix();            /* Init the matrix	*/
    find_inverse();

    // pthread_mutex_destroy(&lock);

    if (PRINT == 1)
    {
        //(Print_Matrix(A, "End: Input");
        FILE *fptr;

        char *file = Output_file;
        fptr = fopen(file, "w");

        if (fptr == NULL)
        {
            printf("Error!");
            exit(1);
        }

        write_matrix_to_file(I, "Inversed", fptr);
        fclose(fptr);
    }
}

void *matrix_inverse(void *params)
{
    struct threadArgs *args = (struct threadArgs *)params;

    int start_row = args->start_row;
    int end_row = args->end_row;
    int p = args->p;

    for (int row = start_row; row < end_row; row++)
    {
        double multiplier = A[row][p];
        if (row != p) // Perform elimination on all except the current pivot row
        {
            for (int col = 0; col < N; col++)
            {
                A[row][col] = A[row][col] - A[p][col] * multiplier; /* Elimination step on A */
                I[row][col] = I[row][col] - I[p][col] * multiplier; /* Elimination step on I */
            }
            assert(A[row][p] == 0.0);
        }
    }

    pthread_exit(NULL);
}

void find_inverse()
{
    pthread_t *threads = malloc(NUM_THREADS * sizeof(pthread_t));
    struct threadArgs *args = malloc(NUM_THREADS * sizeof(struct threadArgs));

    int col;

    /* Bringing the matrix A to the identity form */
    for (int p = 0; p < N; p++)
    { /* Outer loop */

        double pivalue = A[p][p];
        for (col = 0; col < N; col++)
        {
            A[p][col] = A[p][col] / pivalue; /* Division step on A */
            I[p][col] = I[p][col] / pivalue; /* Division step on I */
        }
        assert(A[p][p] == 1.0);

        for (size_t i = 0; i < NUM_THREADS; i++)
        {

            args[i].p = p;
            args[i].start_row = ceil(i * ((double)N / NUM_THREADS));
            args[i].end_row = ceil((i + 1) * ((double)N / NUM_THREADS));
            // if (p == 0)
            // {
            //     printf("%zu> start: %d, end: %d\n", i, args[i].start_row, args[i].end_row);
            // }

            pthread_create(&(threads[i]),
                           NULL,
                           matrix_inverse,
                           (void *)&args[i]); // args to that function
        }

        for (int id = 0; id < N; id++)
        {
            pthread_join(threads[id], NULL);
        }
    }

    free(args);    // deallocate args vector
    free(threads); // deallocate array
}

void Init_Matrix()
{
    int row, col;

    // Set the diagonal elements of the inverse matrix to 1.0
    // So that you get an identity matrix to begin with
    for (row = 0; row < N; row++)
    {
        for (col = 0; col < N; col++)
        {
            if (row == col)
                I[row][col] = 1.0;
        }
    }

    printf("\nsize      = %dx%d ", N, N);
    printf("\nmaxnum    = %d \n", maxnum);
    printf("Init	  = %s \n", Init);
    printf("Initializing matrix...");

    if (strcmp(Init, "rand") == 0)
    {
        for (row = 0; row < N; row++)
        {
            for (col = 0; col < N; col++)
            {
                if (row == col) /* diagonal dominance */
                    A[row][col] = (double)(rand() % maxnum) + 5.0;
                else
                    A[row][col] = (double)(rand() % maxnum) + 1.0;
            }
        }
    }
    if (strcmp(Init, "fast") == 0)
    {
        for (row = 0; row < N; row++)
        {
            for (col = 0; col < N; col++)
            {
                if (row == col) /* diagonal dominance */
                    A[row][col] = 5.0;
                else
                    A[row][col] = 2.0;
            }
        }
    }

    printf("done \n\n");
    if (PRINT == 1)
    {
        // Print_Matrix(A, "Begin: Input");
        // Print_Matrix(I, "Begin: Inverse");
    }
}

void write_matrix_to_file(matrix M, char name[], FILE *fptr)
{
    int row, col;

    fprintf(fptr, "%s Matrix:\n", name);
    for (row = 0; row < N; row++)
    {
        for (col = 0; col < N; col++)
            fprintf(fptr, " %5.2f", M[row][col]);
        fprintf(fptr, "\n");
    }
    fprintf(fptr, "\n\n");
}

void Print_Matrix(matrix M, char name[])
{
    int row, col;

    printf("%s Matrix:\n", name);
    for (row = 0; row < N; row++)
    {
        for (col = 0; col < N; col++)
            printf(" %5.2f", M[row][col]);
        printf("\n");
    }
    printf("\n\n");
}

void Init_Default()
{
    N = 5;
    Init = "fast";
    maxnum = 15.0;
    PRINT = 1;
    Output_file = "matrix_result.txt";
}

void Read_Options(int argc, char **argv)
{
    char *prog;

    prog = *argv;

    while (++argv, --argc > 0)
        if (**argv == '-')
            switch (*++*argv)
            {
            case 'n':
                --argc;
                N = atoi(*++argv);
                break;
            case 'h':
                printf("\nHELP: try matinv -u \n\n");
                exit(0);
                break;
            case 'u':
                printf("\nUsage: matinv [-n problemsize]\n");
                printf("           [-D] show default values \n");
                printf("           [-h] help \n");
                printf("           [-I init_type] fast/rand \n");
                printf("           [-m maxnum] max random no \n");
                printf("           [-P print_switch] 0/1 \n");
                exit(0);
                break;
            case 'D':
                printf("\nDefault:  n         = %d ", N);
                printf("\n          Init      = rand");
                printf("\n          maxnum    = 5 ");
                printf("\n          P         = 0 \n\n");
                exit(0);
                break;
            case 'I':
                --argc;
                Init = *++argv;
                break;
            case 'm':
                --argc;
                maxnum = atoi(*++argv);
                break;
            case 'P':
                --argc;
                PRINT = atoi(*++argv);
                break;
            case 'o':
                --argc;
                Output_file = *++argv;
                break;
            default:
                printf("%s: ignored option: -%s\n", prog, *argv);
                printf("HELP: try %s -u \n\n", prog);
                break;
            }
}
