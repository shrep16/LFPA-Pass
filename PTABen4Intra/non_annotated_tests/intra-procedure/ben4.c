#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

/*void foo(){

	int **s,*k;

	*s = k;

}*/

int **q,*p;
int main(){


		q = &p;
		mustPointsTo(q,p);
		return **q;

}
