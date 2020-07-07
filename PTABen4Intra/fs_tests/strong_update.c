//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


/*void bar(int***k, int***s){

	*k = *s; 

}*/

	int *p1,*q1,*r1,*a1,*b1,*c1,q2,a2;
	int **p = &p1;
	int **q = &q1;
int main(){
	q1 = &q2;
	int ***k = &p;
	int ***s = &q;
	*k = *s;
	doesNotPointsTo(p,p1);
	mustPointsTo(p,q1);
	mustPointsTo(q,q1);
	return **p+**q;
	//bar(&p,&q);
	//NOALIAS(p,&p1);
}
