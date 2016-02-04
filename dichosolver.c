#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dichosolver.h"

#define NO_DEBUG 0
#define LOW_DEBUG 1
#define MID_DEBUG 2
#define HI_DEBUG 3

#define DBGLVL LOW_DEBUG

int main ( int argc, char** argv ) {

	time_t clock1, clock2;
	clock1 = time(NULL);
	
  if ( argc < 2 ) {
    printf("not enough arguments: filename needed\n");
    exit(-1);
  }

  task_t *mytask;
  node_t *root = NULL;

/* get task */
  mytask = readtask (argv[1]);

	if( mytask->length < 1 ) { 
		puts("There is no solution");
		exit(0);
	}
    #if DBGLVL >= LOW_DEBUG
		printf("Task readed. b=%lld, size=%d.\n",mytask->b,mytask->length);
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

        clock2 = time(NULL);
    	//printf("task solved in %ld sec, print\n",clock2-clock1); fflush(stdout);
    //}

  // print complete solution
	//FILE *file;
    if ( root->length == -1 ) { puts("length == -1"); fflush(stdout); }
    else {
      if ( root->items == NULL ) puts ("Wwarning! There's no items!");
      if ( root->items->p == NULL ) puts("Wwarning! There's no solutions!");
      // print it
      item_t *decis;
      for ( decis = root->items ; decis->next != NULL ; decis = decis->next );

        //if( (file = fopen("out.txt","w")) == 0 ) return -1;
		//printf("solution: (%lld, %lld), time: %ld sec\n", *(decis->p), *(decis->w), clock2-clock1);
		printf("%lld\n%lld\n%ld\n", *(decis->p), *(decis->w), clock2-clock1);
      //fprintf (file, "%lld %lld %ld\n", *(decis->p), *(decis->w), clock2-clock1 ); //fflush(stdout);
	  //fclose(file);
		//puts("ok");

    }

  #if DBGLVL >= LOW_DEBUG
	printf ("finalizing...\n"); fflush (stdout);
  #endif


  #if DBGLVL >= LOW_DEBUG
  puts ("free tree"); fflush(stdout);
  #endif

  clean_tree (root);
  free (root); //free_tree (root);

  #if DBGLVL >= LOW_DEBUG
    puts ("free some var"); fflush(stdout);
  #endif
  free_task(&mytask);

  #if DBGLVL >= LOW_DEBUG
  	printf("ok\n"); fflush (stdout);
  #endif
  return 0;
} // main()
