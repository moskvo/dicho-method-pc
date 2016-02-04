#include "burkov.h"

void (*dicho_tree) (node_t*, const int, item_t*) = dicho_tree_notrecursive;
node_t* (*burkovtree)(const task_t*) = optimal_dichotomic_tree;
void (*treesolver) (node_t*, knint) = notrecursive_treesolver;


/*
*  building optimum dichotomy tree on Burkov,Burkova's works
*/
node_t* optimal_dichotomic_tree ( const task_t *task){

  //{ finding q <= countItems - number of elements on which the maximal symmetric hierarchy must be created
    int q = find_q (task->b);
    q = (q > task->length) ? task->length : q;
  //}

  // maximum symmetric hierarchy must be created from top to down!
  // optimally sorting items
  item_t *diitems, // items for dichotomic part
         *dpitems; // items for dynamic programming branch
  prep_items(task->length, task->items, q, &diitems, &dpitems);

  // head of optimal dichotomic tree
  node_t* head = createnodes (2*task->length-1); // number of all nodes of any tree is doubled number of it's leafs minus one.
  head->hnode = NULL; // parentof the tree's head

  // DP branch
  node_t *p = head;
  item_t *pl = dpitems;//, *tmp;
  int i;
  for ( i = 0 ; i < task->length-q ; i++ ) { // in fact p move as p = p + 2
    p->lnode = (p+1);
    p->lnode->items = copyitem(pl);
//	p->lnode->items->flag = TRUE_ELEM;
    pl++;
    p->lnode->length = 1;
    p->lnode->hnode = p;

    p->rnode = (p+2);
    p->rnode->hnode = p;

    p = p->rnode;
  }

  // Dichotomic branch
  if( q > 0 ) dicho_tree(p, q, diitems);

  return head;
}

// must be: q <= size
// diitems - items for dichotomic part of tree
// dpitems - items for dynamic programming part of tree
void prep_items ( const int size, item_t *list, const int q, item_t **diitems, item_t **dpitems ){
  // it's just a plug
  (*dpitems) = list;
  (*diitems) = list + (size-q); // dangerous:
}

// b(q)
knint bq[] = {0LL, 0LL, 4LL, 6LL, 12LL, 20LL, 38LL, 70LL, 140LL, 268LL, 532LL, 1044LL, 2086LL, 4134LL, 8262LL, 16454LL, 32908LL, 65676LL, 131340LL, 262412LL, 524820LL, 1049108LL, 2098196LL, 4195348LL, 8390694LL,
   16779302LL, 33558566LL, 67112998LL, 134225990LL, 268443718LL, 536887366LL, 1073758278LL, 2147516556LL, 4295000204LL, 8590000268LL, 17179934860LL, 34359869708LL, 68719608076LL, 137439215884LL, 274878169356LL};
const short bq_size = sizeof(bq) / sizeof(bq[0]);
int find_q (knint b) {
  knint *i = bq;
  for ( ; (i<bq+bq_size && (*i) < b ) ; i++ ) {}
  return (i-bq);
}

void dicho_tree_notrecursive(node_t *head, const int size, item_t *items){
  int p_size = 1, pnext_size = 2;
  node_t *p = head, *pnext, *t1, *t2;
  int *sizes = (int*)malloc(2*sizeof(int)), // size of accordingly subtree items
      *indexes = sizes + 1, // start index of items that belongs to accordingly subtree
      *tmpsizes, *tmpindexes, *ss, *ii, *ss2, *ii2;
    *sizes = size; *indexes = 0;

  // creating a maximum symmetric tree
  int dp = (int)log2f ((float)size); // tree's depth
  int i;
  for ( i = 0 ; i < dp ; i++ ){
    pnext = p + p_size; // the next level of tree
    ss = (int*)malloc (2*pnext_size*sizeof(int));
    ii = ss + pnext_size;

    t2 = pnext;
    ss2 = ss;
    ii2 = ii;
    tmpsizes = sizes; // for free()
    int j = 0, d;
    for( t1 = p ; t1 < p + p_size ; t1++ ){
      d = (*sizes) / 2;
        *ss2 = d;
        *(ss2+1) = (*sizes) - d;
        ss2 = ss2 + 2;
        *ii2 = (*indexes);
        *(ii2+1) = (*indexes) + d;
        ii2 = ii2 + 2;
        sizes++;
        indexes++;

      t1->lnode = t2;
      t2->hnode = t1; t2++;
      t1->rnode = t2;
      t2->hnode = t1; t2++;
      j++;
    }
    sizes = ss;
    indexes = ii;
    free (tmpsizes);
    //free(tmpindexes); commented because ss = malloc; ii = ss + ...

    p = pnext;
    p_size = pnext_size;
    pnext_size <<= 1;
  } // for i

  // hang up all remaining items
  pnext = p + p_size;
  for( i = 0 ; i < p_size ; i++ ){
    if( sizes[i] > 2 ) { printf("size of leaf more than 2\n"); fflush(stdout); }
    if( sizes[i] == 2 ){
      pnext->items = copyitem (&items[indexes[i]]);
//	  pnext->items->flag = TRUE_ELEM;
      pnext->length = 1;
      p->lnode = pnext;
      pnext->hnode = p; pnext++;

      pnext->items = copyitem (&items[indexes[i]+1]);
//	  pnext->items->flag = TRUE_ELEM;
      pnext->length = 1;
      p->rnode = pnext;
      pnext->hnode = p; pnext++;
    } else if( sizes[i] == 1 ){
      p->items = copyitem (&items[indexes[i]]);
//	  p->items->flag = TRUE_ELEM;
      p->length = 1;
    } else {
      // error
      printf("size of leaf is non-positive?\n");
      fflush(stdout);
    }
    p++;
  }

}// dicho_tree_notrecursive()

/*  funcs for solving */

void notrecursive_treesolver ( node_t* root, knint cons ){
  node_t *runner = root, *smallnode, *bignode;
  if ( root->rnode == NULL || root->lnode == NULL ) return;
  int depth = 0;
  while ( runner != NULL ) {
  	while ( (runner->rnode->length == 0) || (runner->lnode->length == 0) ) {
  		while ( runner->rnode->length == 0 ) { runner = runner->rnode; depth++; }
  		while ( runner->lnode->length == 0 ) { runner = runner->lnode; depth++; }
  	}

	if ( runner->lnode->length > runner->rnode->length ) {
		bignode = runner->lnode;
		smallnode = runner->rnode;
	} else {
		bignode = runner->rnode;
		smallnode = runner->lnode;
	}

	dichosolve (runner, bignode, smallnode, cons );

	//clear_node (runner->lnode);
    //clear_node (runner->rnode);

  	//printf ( "depth = %d. length = %d, right length = %d, left length = %d\n", depth, runner->length, runner->rnode->length, runner->lnode->length );
  	//print_hash (runner->rnode->items);
  	//print_hash (runner->lnode->items);
  	//fflush (stdout);

  	runner = runner->hnode;
  	depth--;
  } // while
}

void recursive_treesolver(node_t* root, knint cons){
  if( root->length != 0 ) return;

  treesolver (root->lnode,cons);
  treesolver (root->rnode,cons);

  if ( root->lnode->length > root->rnode->length )
  	dichosolve(root, root->lnode, root->rnode, cons );
  else
  	dichosolve(root, root->rnode, root->lnode, cons );
//  print_tree(root);
}


void dichosolve ( node_t* to, node_t* big, node_t* small, knint cons ) {

  to->items = big->items;
  to->length = big->length;

  big->items = NULL;
  big->length = 0;

  if ( small->length < 1 ) { return; }

  //item_t *its = createitems0 (cons), *fp, *sp;
  item_t *fp, *sp, *tmp;
  knint w, p;

  //puts ("dichosolve. pairwise addition"); fflush(stdout);
  item_t *lastelem, *preelem = NULL;
  for( fp = to->items ; fp != NULL ; preelem = fp, fp = fp->next ) {
	// if element was computed in this pairwise addition cycle leave it
    if ( fp->flag == NEW_ELEM ) { fp->flag = OLD_ELEM; continue; }
    lastelem = fp;
    //puts("before for"); fflush(stdout);
    for( sp = small->items ; sp != NULL && (p = *(fp->p) + *(sp->p), w = *(fp->w) + *(sp->w), w<=cons) ; sp = sp->next ) {
	//printf ("lastelemw=%ld w=%ld\n",*(lastelem->w),w);
    	lastelem = find_preplace_badcutter (lastelem, &w, &(to->length));
		if ( lastelem == NULL ) {
			puts("lastelem null!");
			//printf("w=%ld, preelemw=%ld, fpw=%ld\n",w,*(preelem->w), w-*(sp->w));
			fflush(stdout);
		}
    	tmp = copyitem (lastelem);
    	*(tmp->p) = p;
    	*(tmp->w) = w;
	//puts("before put_item"); fflush(stdout);
    	put_item (lastelem, &tmp, &(to->length));
    } // for sp
    //puts("after for"); fflush(stdout);
    if ( fp->flag == ONESHOT_ELEM ) {
    	preelem->next = fp->next; // preelem isn't NULL cause ONESHOT_ELEM cann't be a first, see put_item().
    	free_items (&fp);
    	fp = preelem;
    	to->length--;
    }
  } // for fp

  //puts("put new elements of second table or replace elements having less value");fflush(stdout);
  item_t *desert = createitems0(1); // head of bad items to hold them in small list later
  int desertlen = 0;
  lastelem = to->items;
  fp = small->items;
  small->items = small->items->next;
  if ( (tmp = find_preplace_badcutter_simple(lastelem, fp->w, &(to->length))) == NULL ) { // if we must put item with fp->w weight to first place
	if ( *(to->items->w) == *(fp->w) ) {		
		if ( *(to->items->p) < *(fp->p) ) { // replace head of to->items with fp
			// TODO may be put fp to desert?
			*(to->items->p) = *(fp->p);
			free_items (&fp);
		} else {
			fp->next = desert->next;
			desert->next = fp;
			desertlen++;
		}
	} else { // Put fp to first place
		/*if ( fp->flag != TRUE_ELEM ) */fp->flag = OLD_ELEM;
		to->items = fp;
		to->items->next = lastelem;
		lastelem = to->items;
		to->length++;
	}
  } else {
	if ( put_item_simple (tmp, &fp, &(to->length)) == 0 ) {
		/*if ( fp->flag != TRUE_ELEM ) */fp->flag = OLD_ELEM;
		lastelem = fp;
	} else { // put_item drops fp
		lastelem = tmp;
		fp->next = desert->next;
		desert->next = fp;
		desertlen++;
	}
  }

  //puts ("cycle 'put new elements ...'"); fflush(stdout);
  for( fp = small->items ; fp != NULL /*&& *(fp->w) <= cons*/ ; ) {
    lastelem = find_preplace_badcutter_simple (lastelem, fp->w, &(to->length));
    tmp = fp->next;
    if ( put_item_simple (lastelem, &fp, &(to->length)) == 0 ) {
    	/*if ( fp->flag != TRUE_ELEM ) */fp->flag = OLD_ELEM;
    } else {
    	fp->next = desert->next;
		desert->next = fp;
		desertlen++;
    }
    fp = tmp;
  }
  small->items = desert->next; // hold bad items (unsorted?)
  small->length = desertlen;
  free_items (&desert);

  //puts("delete inefficient elems in tail");fflush(stdout);
	knint edge;
	do {
		edge = *(lastelem->p);
		while ( lastelem->next != NULL && /*lastelem->next->flag != TRUE_ELEM && */edge >= *(lastelem->next->p) ) {
			tmp = lastelem->next;
			lastelem->next = lastelem->next->next;
			free_items (&tmp);
			to->length--;
		}

		lastelem = lastelem->next;
	} while ( lastelem );

} // dichosolve()
