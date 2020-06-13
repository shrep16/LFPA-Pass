/*
 * Heap
 * Author: Sen Ye
 * Date: 12/10/2013
 * Description: heap objects are identified according to their
 *		allocation sites.
 */
//#include "aliascheck.h"
#include "myHeader.h"
#include "malloc.h"
// return two malloc object
/*void malloc_two(int **p, int **q) {
	*p = (int*) malloc(sizeof(int));
	*q = (int*) malloc(sizeof(int));
}*/

int main() {
	int **o1 = (int **)malloc(sizeof(int*));
    int **o2 = (int**)malloc(sizeof(int*));
    *o2 = (int*)malloc(sizeof(int));
    *o1 = (int*)malloc(sizeof(int));
    //does not recognize malloc
    //hence no pointee found for o1 nor for o2
    doesNotPointsTo(*o1, **o1);
    doesNotPointsTo(*o2, **o2);
//	malloc_two(o1, o2);
//	NOALIAS(*o1, *o2);
	return **o1+**o2;
}
