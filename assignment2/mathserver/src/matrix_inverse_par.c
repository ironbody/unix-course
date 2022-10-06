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

#define MAX_SIZE 4096
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
char *Output;

/* forward declarations */

// void *subtract_rows(void *params);
void *matrix_inverse(void *params);
void find_inverse();
void Init_Matrix(void);
void Print_Matrix(matrix M, char name[]);
void Init_Default(void);
void Read_Options(int, char **);
void Write_to_file(char* filename);

struct threadArgs
{
    int p;
};

int main(int argc, char **argv)
{
    pthread_mutex_init(&lock, NULL);

    printf("Matrix Inverse\n");
    // int i, timestart, timeend, iter;

    Init_Default();           /* Init default values	*/
    Read_Options(argc, argv); /* Read arguments	*/
    Init_Matrix();            /* Init the matrix	*/
    find_inverse();

    pthread_mutex_destroy(&lock);

    if (PRINT == 1)
    {
        //(Print_Matrix(A, "End: Input");
        Print_Matrix(I, "Inversed");
    }
}

void *matrix_inverse(void *params)
{   
    struct threadArgs *args = (struct threadArgs *)params;
    int row, col;
    
    pthread_mutex_lock(&lock);
    double pivalue = A[args->p][args->p];
    for (col = 0; col < N; col++)
    {
        A[args->p][col] = A[args->p][col] / pivalue; /* Division step on A */
        I[args->p][col] = I[args->p][col] / pivalue; /* Division step on I */
    }
    assert(A[args->p][args->p] == 1.0);

    double multiplier;
    for (row = 0; row < N; row++) {
        multiplier = A[row][args->p];
        if (row != args->p) // Perform elimination on all except the current pivot row 
        {
            for (col = 0; col < N; col++)
            {
                A[row][col] = A[row][col] - A[args->p][col] * multiplier; /* Elimination step on A */
                I[row][col] = I[row][col] - I[args->p][col] * multiplier; /* Elimination step on I */
            }      
            assert(A[row][args->p] == 0.0);
        }
    }
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

void find_inverse()
{
    pthread_t *threads = malloc(N * sizeof(pthread_t));
    struct threadArgs *args = malloc(N * sizeof(struct threadArgs));

    /* Bringing the matrix A to the identity form */
    for (int p = 0; p < N; p++) { /* Outer loop */
        args[p].p = p;
        pthread_create(&(threads[p]),
                        NULL,
                        matrix_inverse,
                        (void *)&args[p]); // args to that function
    }            
    
    for (int id = 0; id < N; id++)
    {
        pthread_join(threads[id], NULL);
    }

    free(args);    // deallocate args vector
    free(threads); // deallocate array
}

// void *subtract_rows(void *params)
// {
//     struct threadArgs *args = (struct threadArgs *)params;
//     int row, p;
//     row = args->row;
//     p = args->p;

//     double multiplier = A[row][p];
//     if (row != p)
//     {
//         for (int col = 0; col < N; col++)
//         {
//             A[row][col] = A[row][col] - A[p][col] * multiplier; /* Elimination step on A */
//             I[row][col] = I[row][col] - I[p][col] * multiplier; /* Elimination step on I */
//         }
//         assert(A[row][p] == 0.0);
//     }
//     pthread_exit(NULL);
// }

// void find_inverse()
// {
//     int row, col, p; // 'p' stands for pivot (numbered from 0 to N-1)
//     double pivalue;  // pivot value

//     pthread_t *threads = malloc(N * sizeof(pthread_t));
//     struct threadArgs *args = malloc(N * sizeof(struct threadArgs));

//     /* Bringing the matrix A to the identity form */
//     for (p = 0; p < N; p++)
//     { /* Outer loop */
//         pivalue = A[p][p];
//         for (col = 0; col < N; col++)
//         {
//             A[p][col] = A[p][col] / pivalue; /* Division step on A */
//             I[p][col] = I[p][col] / pivalue; /* Division step on I */
//         }
//         assert(A[p][p] == 1.0);

//         // double multiplier;
//         for (row = 0; row < N; row++)
//         {
//             if (row != p) // Perform elimination on all except the current pivot row
//             {
//                 args[row].row = row;
//                 args[row].p = p;
//                 pthread_create(&(threads[row]),
//                                NULL,
//                                subtract_rows,
//                                (void *)&args[row]); // args to that function
//             }
//         }

//         for (int id = 0; id < N; id++)
//         {
//             pthread_join(threads[id], NULL);
//         }
//     }

//     free(args);    // deallocate args vector
//     free(threads); // deallocate array
// }

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

    Output = "\nsize      = %dx%d ", N, N;
    char temp[] = "\nmaxnum    = %d \n", maxnum;
    strcat(Output, temp);

    char temp1[] = "Init	  = %s \n", Init;
    strcat(Output, temp);

    strcat(Output, "Initializing matrix...");

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

    strcat(Output,"done \n\n");
    if (PRINT == 1)
    {
        // Print_Matrix(A, "Begin: Input");
        // Print_Matrix(I, "Begin: Inverse");
    }
}

void Write_to_file(char* filename)
{
    FILE* file_ptr;
    file_ptr = fopen(filename, "w");

    if (file_ptr == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open '%s' file.\n", filename);
        exit(EXIT_FAILURE);
    }

    fputs(Output, file_ptr);
    fclose(file_ptr);
}

void Print_Matrix(matrix M, char name[])
{
    int row, col;

    char* temp2;
    strcat(temp2, name);
    strcat(temp2," Matrix:\n");
    strcat(Output, temp2);
    for (row = 0; row < N; row++)
    {
        for (col = 0; col < N; col++) {
            char temp1[] = " %5.2f", M[row][col];
            strcat(Output, temp1);
        }
        strcat(Output, "\n");
    }
    strcat(Output, "\n\n");
}

void Init_Default()
{
    N = 5;
    Init = "fast";
    maxnum = 15.0;
    PRINT = 1;
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
                Write_to_file(Output_file);
                break;
            default:
                printf("%s: ignored option: -%s\n", prog, *argv);
                printf("HELP: try %s -u \n\n", prog);
                break;
            }
}
