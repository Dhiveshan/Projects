/* File:     pth_life.c
 * Purpose:  Implement John Conway's Game of Life.  The game is ``played''
 *           on a board or *world* consisting of a rectangular grid with
 *           m rows and n columns.  Each cell in the grid is ``alive'' 
 *           or ``dead.'' An initial generation (generation 0) is either 
 *           entered by the user or generated using a random number 
 *           generator.  
 *
 *           Subsequent generations are computed according to the 
 *           following rules:
 *
 *              - Any live cell with fewer than two live neighbors 
 *              dies, as if caused by under-population.
 *              - Any live cell with two or three live neighbors 
 *              lives on to the next generation.
 *              - Any live cell with more than three live neighbors 
 *              dies, as if by over-population.
 *              - Any dead cell with exactly three live neighbors 
 *              becomes a live cell, as if by reproduction.
 *
 *           Updates take place all at once.
 * 
 * Compile:  gcc -g -Wall -o life life.c -lpthread
 * Run:      ./life <th_rows> <th_cols> <rows> <cols> <max gens> <'i'|'g'>
 *              th_rows = number of rows of threads
 *              th_cols = number of cols of threads
 *              rows = number of rows in the world
 *              cols = number of cols in the world
 *              max gens = max number of generations
 *              'i' = user will enter generation 0
 *              'g' = program should generate generation 0
 *
 * Input:    If command line has the "input" char ('i'), the first
 *              generation.  Each row should be entered on a separate
 *              line of input.  Live cells should be indicated with
 *              a capital 'X', and dead cells with a blank, ' '.
 *           If command line had the "generate" char ('g'), the
 *              probability that a cell will be alive.
 *
 * Output:   The initial world (generation 0) and the world after
 *           each subsequent generation up to and including
 *           generation = max_gen.  If all of the cells die,
 *           the program will terminate.
 *
 * Notes:
 * 1.  This implementation uses a "toroidal world" in which the
 *     top row of cells borders the bottom, and the left row
 *     borders the right.
 * 2.  In order to avoid issues with counting a single cell as
 *     a neighbor multiple times, we assume that the number of
 *     rows and the number of columns are at least 3.
 * 3.  The program uses a logical block submatrix partition of the
 *     world.  
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define LIVE 1
#define DEAD 0
#define LIVE_IO 'X'
#define DEAD_IO ' '
#define MAX_TITLE 1000

/* Shared */
int th_rows, th_cols;
int thread_count;
int m, n, max_gens;
int *w1, *w2, *wp, *twp;

int bar_count = 0;
int live_count = 0;
int quit = 0;
pthread_cond_t bar_cond;
pthread_mutex_t bar_mutex;


void Usage(char prog_name[]);
void Read_world(char prompt[]);
void Gen_world(char prompt[]);
void Print_world(char title[]);
void* Play_life(void* rank);
int  Count_nbhrs(long my_rank, int i, int j);
void Barrier_and_update(int my_live_count, int* curr_gen_p);

int main(int argc, char* argv[]) {
   char ig;
   pthread_t* threads;
   long th;

   if (argc != 7) Usage(argv[0]);
   th_rows = strtol(argv[1], NULL, 10);
   th_cols = strtol(argv[2], NULL, 10);
   m = strtol(argv[3], NULL, 10);
   n = strtol(argv[4], NULL, 10);
   max_gens = strtol(argv[5], NULL, 10);
   ig = argv[6][0];
#  ifdef DEBUG
   printf("m = %d, n = %d, max_gens = %d, ig = %c\n",
         m, n, max_gens, ig);
#  endif

   w1 = malloc(m*n*sizeof(int));
   w2 = malloc(m*n*sizeof(int));
   wp = w1;
   twp = w2;

   thread_count = th_rows*th_cols;
   threads = malloc(thread_count*sizeof(pthread_t));
   pthread_cond_init(&bar_cond, NULL);
   pthread_mutex_init(&bar_mutex, NULL);

   if (ig == 'i') 
      Read_world("Enter generation 0");
   else
      Gen_world("What's the prob that a cell is alive?");

   printf("\n");
   Print_world("Generation 0");

   for (th = 0; th < thread_count; th++)
      pthread_create(&threads[th], NULL, Play_life, (void*) th);

   for (th = 0; th < thread_count; th++)
      pthread_join(threads[th], NULL);

   free(w1);
   free(w2);
   free(threads);

   pthread_cond_destroy(&bar_cond);
   pthread_mutex_destroy(&bar_mutex);

   return 0;
}  /* main */


/*---------------------------------------------------------------------
 * Function:   Usage
 * Purpose:    Show user how to start the program and quit
 * In arg:     prog_name
 */
void Usage(char prog_name[]) {
   fprintf(stderr, "usage: %s <th rows> <th cols> <rows> <cols> <max> <i|g>\n",
         prog_name);
   fprintf(stderr, " th_rows = number of rows of threads\n");
   fprintf(stderr, " th_cols = number of cols of threads\n");
   fprintf(stderr, "    rows = number of rows in the world\n");
   fprintf(stderr, "    cols = number of cols in the world\n");
   fprintf(stderr, "     max = max number of generations\n");
   fprintf(stderr, "       i = user will enter generation 0\n");
   fprintf(stderr, "       g = program should generate generation 0\n");
   exit(0);
}  /* Usage */

/*---------------------------------------------------------------------
 * Function:    Read_world
 * Purpose:     Get generation 0 from the user
 * In arg :     prompt
 * Globals in:  m:  number of rows in visible world
 *              n:  number of cols in visible world
 * Global out:  wp:  stores generation 0
 */
void Read_world(char prompt[]) {
   int i, j;
   char c;

   printf("%s\n", prompt);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++) {
         scanf("%c", &c);
         if (c == LIVE_IO)
            wp[i*n + j] = LIVE;
         else
            wp[i*n + j] = DEAD;
      }
      /* Read end of line character */
      scanf("%c", &c);
  }
}  /* Read_world */


/*---------------------------------------------------------------------
 * Function:    Gen_world
 * Purpose:     Use a random number generator to create generation 0 
 * In arg :     prompt
 * Globals in:  m:  number of rows in visible world
 *              n:  number of cols in visible world
 * Global out:  wp:  stores generation 0
 *
 */
void Gen_world(char prompt[]) {
   int i, j;
   double prob;
#  ifdef DEBUG
   int live_count = 0;
#  endif
   
   printf("%s\n", prompt);
   scanf("%lf", &prob);

   srandom(1);
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         if (random()/((double) RAND_MAX) <= prob) {
            wp[i*n + j] = LIVE;
#           ifdef DEBUG
            live_count++;
#           endif
         } else {
            wp[i*n + j] = DEAD;
         }

#  ifdef DEBUG
   printf("Live count = %d, request prob = %f, actual prob = %f\n",
         live_count, prob, ((double) live_count)/(m*n));
#  endif
}  /* Gen_world */


/*---------------------------------------------------------------------
 * Function:    Print_world
 * Purpose:     Print the current world
 * In arg:      title
 * Globals in:  m:  number of rows in visible world
 *              n:  number of cols in visible world
 *              wp:  current gen
 */
void Print_world(char title[]) {
   int i, j;
   
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         if (wp[i*n + j] == LIVE)
            printf("%c", LIVE_IO);
         else
            printf("%c", DEAD_IO);
      printf("\n");
   }
   printf("%s\n\n", title);
}  /* Print_world */


/*---------------------------------------------------------------------
 * Function:        Play_life
 * Purpose:         Play Conway's game of life.  (See header doc)
 * In arg:          rank:  thread rank
 * Globals in:      m:  number of rows in visible world
 *                  n:  number of cols in visible world 
 *                  max_gens:  the maximum number of generations
 *                    to create.
 * Global in/out:   wp:  pointer to the current world 
 * Global scratch:  twp:  pointer to the next gen world being created
 */
void* Play_life(void* rank) {
   int i, j, count, curr_gen = 0;

   long my_rank = (long) rank;
   int my_row_rank = my_rank/th_cols;
   int my_col_rank = my_rank % th_cols;

   int my_rows = m/th_rows;
   int my_cols = n/th_cols;
   int my_first_row = my_row_rank*my_rows;
   int my_last_row = my_first_row + my_rows;
   int my_first_col = my_col_rank*my_cols;
   int my_last_col = my_first_col + my_cols;

   int my_live_count;

#  ifdef DEBUG
   printf("Th %ld > my_row_rank = %d, my_col_rank = %d\n",
         my_rank, my_row_rank, my_col_rank);
   printf("Th %ld > my_rows = %d, my_cols = %d\n",
         my_rank, my_rows, my_cols);
   printf("Th %ld > my_first_row = %d, my_last_row = %d\n",
         my_rank, my_first_row, my_last_row);
   printf("Th %ld > my_first_col = %d, my_last_col = %d\n",
         my_rank, my_first_col, my_last_col);
#  endif

   while (curr_gen < max_gens) {
      my_live_count = 0;
      for (i = my_first_row; i < my_last_row; i++) {
         for (j = my_first_col; j < my_last_col; j++) {
            count = Count_nbhrs(my_rank, i, j);
#           ifdef DEBUG
            printf("Th %ld > curr_gen = %d, i = %d, j = %d, count = %d\n",
                  my_rank, curr_gen, i, j, count);
#           endif
            if (count < 2 || count > 3)
               twp[i*n + j] = DEAD;
            else if (count == 2)
               twp[i*n + j] = wp[i*n + j];
            else /* count == 3 */
               twp[i*n + j] = LIVE;
            if (twp[i*n + j] == LIVE) my_live_count++;
         }
      }
      Barrier_and_update(my_live_count, &curr_gen);
      if (quit) break;
   }

   return NULL;
}  /* Play_life */

/*---------------------------------------------------------------------
 * Function:    Count_nbhrs
 * Purpose:     Count the number of living nbhrs of the cell (i,j)
 * In args:     my_rank:  for debugging
 *              i:   row number of current cell
 *              j:   col number of current cell
 * Globals in:  wp:  current world
 *              m:   number of rows in world
 *              n:   number of cols in world
 *
 * Note:        Since the top row of cells is adjacent to the bottom
 *              row, and since the left col of cells is adjacent to the
 *              right col, in a very small world, it's possible to
 *              count a cell as a neighbor twice.  So we assume that
 *              m and n are at least 3.
 */
int Count_nbhrs(long my_rank, int i, int j) {
   int i1, j1, i2, j2;
   int count = 0;

#  ifdef DEBUG
   printf("Th %ld > i = %d, j = %d\n",
         my_rank, i, j);
   Print_world("");
#  endif
   for (i1 = i-1; i1 <= i+1; i1++)
      for (j1 = j-1; j1 <= j+1; j1++) {
         i2 = (i1 + m) % m;
         j2 = (j1 + n) % n;
         count += wp[i2*n + j2];
#        ifdef DEBUG
         printf("Th %ld > i1 = %d, j1 = %d, i2 = %d, j2 = %d, wp[i2*n+j2] = %d, count = %d\n",
            my_rank, i1, j1, i2, j2, wp[i2*n + j2], count);
#        endif
      }
   count -= wp[i*n + j];

   return count;
}  /* Count_nbhrs */


/*---------------------------------------------------------------------
 * Function:       Barrier_and_update
 * Purpose:        Update global live_count, quit, print current generation,
 *                    swap pointers to current gen and new gen
 * In arg:         my_live_count
 * In/out arg:     curr_gen_p
 * Globals in:     thread_count
 * Globals in/out: live_count:  number of cells that are alive in the
 *                    new generation
 *                 bar_count:   number of threads that have entered
 *                    this function;  set to 0 by last thread
 *                 bar_mutex;
 *                 bar_cond;
 * Globals out:    quit:  set to 1 if no cells are alive in the new
 *                    generation
 */
void Barrier_and_update(int my_live_count, int* curr_gen_p) {

   pthread_mutex_lock(&bar_mutex);
   live_count += my_live_count;  
   *curr_gen_p += 1;   /* Private to each thread */

   if (bar_count < thread_count-1) {
      bar_count++;
      while (pthread_cond_wait(&bar_cond, &bar_mutex) != 0);

   } else { /* Last thread to enter barrier */
      int* tmp;
      char title[MAX_TITLE];

      bar_count = 0;

      /* Swap wp and twp */
      tmp = wp;
      wp = twp;
      twp = tmp;
      if (live_count == 0) {
         quit = 1;
      } else {
         sprintf(title, "Generation %d", *curr_gen_p);
         Print_world(title);
         fflush(stdout);
         live_count = 0;  /* For next generation */
      }
      pthread_cond_broadcast(&bar_cond);
   }
   pthread_mutex_unlock(&bar_mutex);
}  /* Barrier_and_update */