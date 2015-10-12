#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dichosolver.h"

#define NO_DEBUG 0
#define LOW_DEBUG 1
#define MID_DEBUG 2
#define HI_DEBUG 3

#define DBGLVL LOW_DEBUG

int main(int argc, char** argv){
	time_t clock1,clock2;
	clock1 = time(NULL);

  if ( argc < 2 ){
    printf("Discho solver: not enough arguments: filename needed\n",myrank);
    exit(1);
  }

  task_t *mytask;
  node_t *root = NULL;

/* get task */
  mytask = readtask (argv[1]);

    #if DBGLVL >= LOW_DEBUG
    	printf("Task readed. b=%ld, size=%d.\n",mytask->b,mytask->length); fflush(stdout);
    #endif
    #if DBGLVL >= LOW_DEBUG
	puts("build tree..."); fflush(stdout);
    #endif

    //{ solve mytask
    if( (root = burkovtree ( mytask )) == NULL )
    	{ puts("Can't build optdichotree"); fflush(stdout); }

    #if DBGLVL >= HI_DEBUG
	print_tree (root); fflush(stdout);
    #endif
    #if DBGLVL >= LOW_DEBUG
    	puts("solve task..."); fflush(stdout);
    #endif

    treesolver (root,mytask->b);

    #if DBGLVL >= LOW_DEBUG
    	printf("task solved.\n"); fflush(stdout);
    #endif
    //}

  // print complete solution
    if ( root->length == -1 ) { puts("length == -1"); fflush(stdout); }
    else {
      if ( root->items == NULL ) puts ("Wwarning! There's no items!");
      if ( root->items->p == NULL ) puts("Wwarning! There's no solutions!");
      // print it
      item_t *decis;
      for ( decis = root->items ; decis->next != NULL ; decis = decis->next );
      printf ( "knapsack: (p=%ld w=%ld)\n", *(decis->p), *(decis->w) ); //fflush(stdout);

	clock2 = time(NULL);
	printf ("Elapsed time: %ld sec\n", clock2-clock1);

    }

  #if DBGLVL >= LOW_DEBUG
	printf ("finalizing...\n"); fflush (stdout);
  #endif

  /*
  #if DBGLVL >= LOW_DEBUG
  puts ("free tree"); fflush(stdout);
  #endif
  clean_tree (root);

  node_t *t;
  for( ; cnt > 0 ; cnt-- ){
    t = root;
    root = root->lnode;
    free (t);
  }

  free (root); //free_tree (root);

  #if DBGLVL >= LOW_DEBUG
    puts ("free some var"); fflush(stdout);
  #endif
  free_task(&mytask);
  free(msgreq);
  free(msgstat);
  */
  #if DBGLVL >= LOW_DEBUG
  	printf("ok\n"); fflush (stdout);
  #endif
  return 0;
} // main()