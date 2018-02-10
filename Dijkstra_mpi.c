/* Author: Dhiveshan Chetty
 * File:     mpi_dijk.c
 * Purpose:  Implement Dijkstra's algorithm using MPI.
 *           The adjacency matrix is distributed among the
 *           processes by using block columns, meaning that 
 *           the traversal of the matrix is columns instead of rows.
 *
 * Compile:  mpicc -g -Wall -o mpi_dijk mpi_dijk.c
 * Run:      mpiexec -n <p> ./mpi_dijk 
 *           
 *
 * Input:    n:  the number of rows and the number of columns 
 *               in the matrix
 *           mat:  the matrix:  Infinity should be put
 *                              in as 1000000
 * Output:   The length of a shortest path from vertex 0 to
 *               every vertex
 *           The path found from 0 to each vertex.
 *
 * Notes:
 * 1.  The number of processes, p, should evenly divide n.
 * 2.  You should free the MPI_Datatype object created by
 *     the program with a call to MPI_Type_free
 * 4.  The cost of travelling from a vertex to itself is 0.  So the adjacency
 *     matrix has zeroes on the main diagonal.
 * 5.  Edges joining different vertices have positive weights.
 * 6.  No error checking is done on the input.
 * 7.  The adjacency matrix is stored as a 1-dimensional array and subscripts
 *     are computed using the formula:  the entry in the ith row and jth
 *     column is mat[i*n + j]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_STRING 10000
#define INFINITY 1000000

int Read_n(int my_rank, MPI_Comm comm);
MPI_Datatype Build_blk_col_type(int n, int loc_n);
void Read_matrix(int loc_mat[], int n, int loc_n, 
      MPI_Datatype blk_col_mpi_t, int my_rank, MPI_Comm comm);
void Print_local_matrix(int loc_mat[], int n, int loc_n, int my_rank);
void Print_matrix(int loc_mat[], int n, int loc_n, 
      MPI_Datatype blk_col_mpi_t, int my_rank, MPI_Comm comm);

int Find_min_dist(int loc_dist[], int loc_known[], int loc_n);
int owner(int u, int my_rank, int loc_n);
int local_vertex(int u, int loc_n);
int global_vertex(int loc_u, int my_rank, int loc_n);

void Dijkstra(int loc_mat[], int loc_dist[], int loc_pred[], 
      int n, int loc_n, int my_rank, MPI_Comm comm);
void Print_dists(int loc_dist[], int n, int loc_n, 
      int my_rank, MPI_Comm comm);
void Print_paths(int loc_pred[], int n, int loc_n, 
      int my_rank, MPI_Comm comm);

int main(int argc, char* argv[]) {
   int *loc_mat;
   int n, loc_n, p, my_rank;
   int *loc_dist, *loc_pred;
   MPI_Comm comm;
   MPI_Datatype blk_col_mpi_t;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);
   
   n = Read_n(my_rank, comm);
   loc_n = n/p;
   loc_mat = malloc(n*loc_n*sizeof(int));
   loc_dist = malloc(loc_n*sizeof(int));
   loc_pred = malloc(loc_n*sizeof(int));


   /* Build the special MPI_Datatype before doing matrix I/O */
   blk_col_mpi_t = Build_blk_col_type(n, loc_n);

   if(my_rank ==0)
   {
      printf("Please enter the values of the matrix\n");
   }
   Read_matrix(loc_mat, n, loc_n, blk_col_mpi_t, my_rank, comm);

   Dijkstra(loc_mat, loc_dist, loc_pred, n, loc_n, my_rank, comm);

   Print_dists(loc_dist, n, loc_n, my_rank, comm);
   Print_paths(loc_pred, n, loc_n, my_rank, comm);

// Print_matrix(loc_mat, n, loc_n, blk_col_mpi_t, my_rank, comm);

   free(loc_mat);
   free(loc_dist);
   free(loc_pred);

   /* When you're done with the MPI_Datatype, free it */
   MPI_Type_free(&blk_col_mpi_t);

   MPI_Finalize();
   return 0;
}  /* main */


/*---------------------------------------------------------------------
 * Function:  Read_n
 * Purpose:   Read in the number of rows in the matrix on process 0
 *            and broadcast this value to the other processes
 * In args:   my_rank:  the calling process' rank
 *            comm:  Communicator containing all calling processes
 * Return value:   n:  the number of rows in the matrix
 */
int Read_n(int my_rank, MPI_Comm comm) {
   int n;

   if (my_rank == 0)
   {
      printf("Please Enter size n of the matrix\n");
      scanf("%d", &n);
   }
   MPI_Bcast(&n, 1, MPI_INT, 0, comm);
   return n;
}  /* Read_n */


/*---------------------------------------------------------------------
 * Function:  Build_blk_col_type
 * Purpose:   Build an MPI_Datatype that represents a block column of
 *            a matrix
 * In args:   n:  number of rows in the matrix and the block column
 *            loc_n = n/p:  number cols in the block column
 * Ret val:   blk_col_mpi_t:  MPI_Datatype that represents a block
 *            column
 */
MPI_Datatype Build_blk_col_type(int n, int loc_n) {
   MPI_Aint lb, extent;
   MPI_Datatype block_mpi_t;
   MPI_Datatype first_bc_mpi_t;
   MPI_Datatype blk_col_mpi_t;

   MPI_Type_contiguous(loc_n, MPI_INT, &block_mpi_t);
   MPI_Type_get_extent(block_mpi_t, &lb, &extent);

   MPI_Type_vector(n, loc_n, n, MPI_INT, &first_bc_mpi_t);
   MPI_Type_create_resized(first_bc_mpi_t, lb, extent,
         &blk_col_mpi_t);
   MPI_Type_commit(&blk_col_mpi_t);

   MPI_Type_free(&block_mpi_t);
   MPI_Type_free(&first_bc_mpi_t);

   return blk_col_mpi_t;
}  /* Build_blk_col_type */


/*-------------------------------------------------------------------
 * Function:    Dijkstra
 * Purpose:     Apply Dijkstra's algorithm to the matrix loc_mat
 * In args:     n:  the number of vertices
 *              loc_n:    n divided by the number of processes n/p
 *              my_rank:  every rank calls Dijkstra, and has an individual rank
 *              loc_mat:  adjacency matrix for the graph
 * Out args:    loc_dist:  loc_dist[v] = distance 0 to v.
 *              loc_pred:  loc_pred[v] = predecessor of v on a 
 *                  shortest path 0->v.
 */
void Dijkstra(int loc_mat[], int loc_dist[], int loc_pred[], 
      int n, int loc_n, int my_rank, MPI_Comm comm) {
   int loc_known[loc_n], loc_u, loc_v, i, u, dist_u, new_dist;
   int min_dist_loc[2];
   int min_dist[2];

   for (loc_v = 0; loc_v < loc_n; loc_v++) {

      loc_dist[loc_v] = loc_mat[0*loc_n + loc_v];
      loc_pred[loc_v] = 0;
      loc_known[loc_v] = 0;
   }
   if (my_rank == 0) loc_known[0] = 1;

   for (i = 1; i < n; i++) {

      loc_u = Find_min_dist(loc_dist, loc_known, loc_n);
      if (loc_u >= 0) {
         min_dist_loc[0] = loc_dist[loc_u];
         min_dist_loc[1] = global_vertex(loc_u, my_rank,loc_n);
      } else {
         min_dist_loc[0] = INFINITY;
         min_dist_loc[1] = -1;
      }
      MPI_Allreduce(min_dist_loc, min_dist, 1, MPI_2INT, 
            MPI_MINLOC, comm);
      dist_u = min_dist[0];
      u = min_dist[1];
      if (owner(u, my_rank, loc_n)) loc_known[loc_u] = 1;

      for (loc_v = 0; loc_v < loc_n; loc_v++)
         if (!loc_known[loc_v]) {
            new_dist = dist_u + loc_mat[u*loc_n + loc_v];
            if (new_dist < loc_dist[loc_v]) {
               loc_dist[loc_v] = new_dist;
               loc_pred[loc_v] = u;
            }
         }
   }

}  /* Dijkstra */


/*---------------------------------------------------------------------
 * Function:  Read_matrix
 * Purpose:   Read in an nxn matrix of ints on process 0, and
 *            distribute it among the processes so that each
 *            process gets a block column with n rows and n/p
 *            columns
 * In args:   n:  the number of rows in the matrix and the submatrices
 *            loc_n = n/p:  the number of columns in the submatrices
 *            blk_col_mpi_t:  the MPI_Datatype used on process 0
 *            my_rank:  the caller's rank in comm
 *            comm:  Communicator consisting of all the processes
 * Out arg:   loc_mat:  the calling process' submatrix (needs to be 
 *               allocated by the caller)
 */
void Read_matrix(int loc_mat[], int n, int loc_n, 
      MPI_Datatype blk_col_mpi_t, int my_rank, MPI_Comm comm) {
   int* mat = NULL, i, j;

   if (my_rank == 0) {
      mat = malloc(n*n*sizeof(int));
      for (i = 0; i < n; i++)
         for (j = 0; j < n; j++)
            scanf("%d", &mat[i*n + j]);
   }

   MPI_Scatter(mat, 1, blk_col_mpi_t,
           loc_mat, n*loc_n, MPI_INT, 0, comm);

   if (my_rank == 0) free(mat);
}  /* Read_matrix */


/*---------------------------------------------------------------------
 * Function:  Print_local_matrix
 * Purpose:   Store a process' submatrix as a string and print the
 *            string.  Printing as a string reduces the chance 
 *            that another process' output will interrupt the output.
 *            from the calling process.
 * In args:   loc_mat:  the calling process' submatrix
 *            n:  the number of rows in the submatrix
 *            loc_n:  the number of cols in the submatrix
 *            my_rank:  the calling process' rank
 */
void Print_local_matrix(int loc_mat[], int n, int loc_n, int my_rank) {
   char temp[MAX_STRING];
   char *cp = temp;
   int i, j;

   sprintf(cp, "Proc %d >\n", my_rank);
   cp = temp + strlen(temp);
   for (i = 0; i < n; i++) {
      for (j = 0; j < loc_n; j++) {
         if (loc_mat[i*loc_n + j] == INFINITY)
            sprintf(cp, " i ");
         else
            sprintf(cp, "%2d ", loc_mat[i*loc_n + j]);
         cp = temp + strlen(temp);
      }
      sprintf(cp, "\n");
      cp = temp + strlen(temp);
   }

   printf("%s\n", temp);
}  /* Print_local_matrix */


/*---------------------------------------------------------------------
 * Function:  Print_matrix
 * Purpose:   Print the matrix that's been distributed among the 
 *            processes.
 * In args:   loc_mat:  the calling process' submatrix
 *            n:  number of rows in the matrix and the submatrices
 *            loc_n:  the number of cols in the submatrix
 *            blk_col_mpi_t:  MPI_Datatype used on process 0 to
 *               receive a process' submatrix
 *            my_rank:  the calling process' rank
 *            comm:  Communicator consisting of all the processes
 */
void Print_matrix(int loc_mat[], int n, int loc_n,
      MPI_Datatype blk_col_mpi_t, int my_rank, MPI_Comm comm) {
   int* mat = NULL, i, j;

   if (my_rank == 0) mat = malloc(n*n*sizeof(int));
   MPI_Gather(loc_mat, n*loc_n, MPI_INT,
         mat, 1, blk_col_mpi_t, 0, comm);
   if (my_rank == 0) {
      for (i = 0; i < n; i++) {
         for (j = 0; j < n; j++)
            if (mat[i*n + j] == INFINITY)
               printf(" i ");
            else
               printf("%2d ", mat[i*n + j]);
         printf("\n");
      }
      free(mat);
   }
}  /* Print_matrix */


/*-------------------------------------------------------------------
 * Function:    Print_dists
 * Purpose:     Print the length of the shortest path from 0 to each
 *              vertex
 * In args:     n:  the number of vertices
 *              dist:  distances from 0 to each vertex v:  dist[v]
 *                 is the length of the shortest path 0->v
 */
void Print_dists(int loc_dist[], int n, int loc_n,
      int my_rank, MPI_Comm comm) {
   int* dist = NULL, v;

   if (my_rank == 0) dist = malloc(n*sizeof(int)); 

   MPI_Gather(loc_dist, loc_n, MPI_INT, 
         dist, loc_n, MPI_INT, 0, comm);
   
   if (my_rank == 0) {
      printf("  v    dist 0->v\n");
      printf("----   ---------\n");
                  
      for (v = 1; v < n; v++)
         printf("%3d       %4d\n", v, dist[v]);
      printf("\n");
      free(dist);
   }

}  /* Print_dists */
/*---------------------------------------------------------------------
 * Function:     global_vertex
 * Purpose:      converts a local vertex into a global vertex
 */
int global_vertex(int loc_u, int my_rank, int loc_n) {
   return loc_u + my_rank*loc_n;
}  /* global_vertex */

/*---------------------------------------------------------------------
 * Function:     local_vertex
 * Purpose:      converts a global vertex into a local vertex
 */
int local_vertex(int u, int loc_n) {
   return u % loc_n;
}  /* local_vertex */


/*---------------------------------------------------------------------
 * Function:     owner
 * Purpose:      determines what process owns 
 *               what column
 */
int owner(int u, int my_rank, int loc_n) {
   if (u/loc_n == my_rank)
      return 1;
   else
      return 0;
}  /* owner */



/*-------------------------------------------------------------------
 * Function:    Find_min_dist
 * Purpose:     Find the vertex u with minimum distance to 0
 *              (loc_dist[loc_u]) among the vertices whose distance 
 *              to 0 is not known.
 * In args:     dist:  loc_dist[v] = current estimate of distance
 *                 0->v
 *              known:  whether the minimum distance 0->loc_v is
 *                 known
 *              n:  the total number of vertices
 * Ret val:     The vertex u whose distance to 0, loc_dist[loc_u]
 *              is a minimum among vertices whose distance
 *              to 0 is not known.
 */
int Find_min_dist(int loc_dist[], int loc_known[], int loc_n) {
   // infin should be INFINITY+1:  so loc_u=-1 isn't
   // chosen, when no remaining vertices can be reached
   int loc_v, loc_u = -1, infin = INFINITY;

   for (loc_v = 0; loc_v < loc_n; loc_v++)
      if (!loc_known[loc_v])
         if (loc_dist[loc_v] < infin) {
            loc_u = loc_v;
            infin = loc_dist[loc_v];
         }

   return loc_u;
}  /* Find_min_dist */   


/*-------------------------------------------------------------------
 * Function:    Print_paths
 * Purpose:     Print the shortest path from 0 to each vertex
 * In args:     n:  the number of vertices
 *              loc_n:  n split up by the number of processes
 *              pred:  list of predecessors:  loc_pred[v] = u if
 *                 u precedes v on the shortest path 0->v
 */
void Print_paths(int loc_pred[], int n, int loc_n, 
      int my_rank, MPI_Comm comm) {
   int *pred = NULL, v, w, *path, count, i;

   if (my_rank == 0) pred = malloc(n*sizeof(int));

   MPI_Gather(loc_pred, loc_n, MPI_INT, 
         pred, loc_n, MPI_INT, 0, comm);

   if (my_rank == 0) {
      path = malloc(n*sizeof(int));

      printf("  v     Path 0->v\n");
      printf("----    ---------\n");
      for (v = 1; v < n; v++) {
         printf("%3d:    ", v);
         count = 0;
         w = v;
         while (w != 0) {
            path[count] = w;
            count++;
            w = pred[w];
         }
         printf("0 ");
         for (i = count-1; i >= 0; i--)
            printf("%d ", path[i]);
         printf("\n");
      }
   
      free(path);
      free(pred);
   }
}  /* Print_paths */


