#include "myHeader.h"
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
