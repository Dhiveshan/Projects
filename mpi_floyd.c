/* Author:    Dhiveshan Chetty
 * Purpose:   Implement a parallel version of Floyd's algorithm for finding
 *            the least cost path between each pair of cities
 * 
 * Compile:   mpicc -g -Wall -o mpi_floyd mpi_floyd.c
 * Run:       mpiexec -n <number of processes> ./mpi_floyd
 *
 * Input:     n, the number of vertices
 *            mat, the matrix
 *           
 * Output:    mat, after being updated by floyd so that it contains the
 *            costs of the cheapest paths between all pairs of vertices.
 *
 * Notes:
 * 1.  n, the number of vertices should be evenly divisible by p, the
 *     number of processes.
 * 2.  The entries in the matrix should be nonnegative ints:  0 on the
 *     diagonal, positive off the diagonal.  Infinity should be indicated
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for debugging */
#include <mpi.h>

const int INFINITY = 1000000;

void Read_matrix(int local_mat[], int n, int my_rank, int p,MPI_Comm comm);
void Print_matrix(int local_mat[], int n, int my_rank, int p,MPI_Comm comm);
void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm);
int Owner(int k, int p, int n);
void Copy_row(int local_mat[], int n, int p, int row_k[], int k);

int main(int argc, char* argv[]) {
   int  num;
   int* local_mat;
   MPI_Comm comm;
   int p, my_rank;
//Initialize MPI
   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);
//input for the amount of cities
   if (my_rank == 0) {
      printf("How many cities?\n");
      scanf("%d", &num);
   }
   MPI_Bcast(&num, 1, MPI_INT, 0, comm);
   local_mat = malloc(num*num/p*sizeof(int));
//prompt for input and call to readMatrix
   if (my_rank == 0) printf("Enter the local_matrix\n");
   Read_matrix(local_mat, num, my_rank, p, comm);
   if (my_rank == 0) printf("The matrix we entered: \n");
   Print_matrix(local_mat, num, my_rank, p, comm);
   if (my_rank == 0) printf("\n");

   Floyd(local_mat, num, my_rank, p, comm);

   if (my_rank == 0) printf("The solution is:\n");
   Print_matrix(local_mat, num, my_rank, p, comm);

   MPI_Finalize();
   free(local_mat);

   return 0;
}  /* main */

   /*---------------------------------------------------------------------
 * Function:  Print_matrix
 * Purpose:   Gather the distributed matrix onto process 0 and print it.
 * In args:   All
 */
void Print_matrix(int local_mat[], int n, int my_rank, int p, 
      MPI_Comm comm) {
   int i, j;
   int* temp_mat = NULL;

   if (my_rank == 0) {
      temp_mat = malloc(n*n*sizeof(int));
      MPI_Gather(local_mat, n*n/p, MPI_INT, 
                 temp_mat, n*n/p, MPI_INT, 0, comm);
      //iterates of the matrix and prints an i in the place of 1000000
      for (i = 0; i < n; i++) {
         for (j = 0; j < n; j++)
            if (temp_mat[i*n+j] == INFINITY)
               printf("i ");
            else
               printf("%d ", temp_mat[i*n+j]);
         printf("\n");
      }
      free(temp_mat);
   } else {
      MPI_Gather(local_mat, n*n/p, MPI_INT, 
                 temp_mat, n*n/p, MPI_INT, 0, comm);
   }
}  /* Print_matrix */


/*---------------------------------------------------------------------
 * Function:  Read_matrix
 * Purpose:   Read in a matrix on process 0 and scatter it
 * In args:   All except local_mat
 * Out arg:   local_mat
 */
void Read_matrix(int local_mat[], int num, int my_rank, int p, 
      MPI_Comm comm) { 
   int i, j;
   int* temp_mat = NULL;

   if (my_rank == 0) {
      //temp variable that's is malloced at n*n size
      temp_mat = malloc(num*num*sizeof(int));
      for (i = 0; i < num; i++)
         for (j = 0; j < num; j++)
            scanf("%d", &temp_mat[i*num+j]);
      MPI_Scatter(temp_mat, num*num/p, MPI_INT, 
                  local_mat, num*num/p, MPI_INT, 0, comm);
      free(temp_mat);
   } else {
      MPI_Scatter(temp_mat, num*num/p, MPI_INT, 
                  local_mat, num*num/p, MPI_INT, 0, comm);
   }

}  /* Read_matrix */


/*---------------------------------------------------------------------
 * Function:    Floyd
 * Purpose:     Implement a distributed version of Floyd's algorithm for
 *              finding the shortest path between all pairs of cities.
 *              The adjacency matrix is distributed by block rows.
 * In args:     All except local_mat
 * In/out arg:  local_mat:  on input the adjacency matrix.  On output
 *              the matrix of lowests costs between all pairs of
 *              cities
 */
void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm) {
   int int_city, city1, city2, temp;
   int my_first_city =0;
   int root;
   int* row_k = malloc(n*sizeof(int));
   int my_last_city = n/p;

   for (int_city = 0; int_city < n; int_city++) {
      //checks the process that is the owner of each city or row
      root = Owner(int_city, p, n);
      if (my_rank == root)
         //copies the row into row_k
         Copy_row(local_mat, n, p, row_k, int_city);
      MPI_Bcast(row_k, n, MPI_INT, root, comm);
      for (city1 = my_first_city; city1 < my_last_city; city1++)
         for (city2 = 0; city2 < n; city2++) {
               temp = local_mat[city1*n + int_city] + row_k[city2];
               if (temp < local_mat[city1*n+city2])
                  local_mat[city1*n + city2] = temp;
         }
   }
   free(row_k);
}  /* Floyd */

/*---------------------------------------------------------------------
 * Function:  Owner
 * Purpose:   Return rank of process that owns global row k
 * In args:   All
 */
int Owner(int k, int p, int n) {
   return k/(n/p);
}  /* Owner */

/*---------------------------------------------------------------------
 * Function:  Copy_row
 * Purpose:   Copy the row with *global* subscript k into row_k
 * In args:   All except row_k
 * Out arg:   row_k
 */
void Copy_row(int local_mat[], int n, int p, int row_k[], int k) {
   int j;
   int local_k = k % (n/p);

   for (j = 0; j < n; j++)
      row_k[j] = local_mat[local_k*n + j];
}  /* Copy_row */ 