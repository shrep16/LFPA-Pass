//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int g;
int* obj = &g; 
/*void Zulu(int**p, int *q);

void Xray(){

	int **x,*b,*w,d;
	x = &b; 
	w = &d; 
	Zulu(x,w);
	NOALIAS(b,w);
	MUSTALIAS(b,&g);
}


void Zulu(int**p,int *q){
	*p = q;
	*p = obj;
}*/

	int **x,*b,*w,d;
int main(){
	//Xray();
	x = &b;
	w = &d;
	int **p = x;
	int *q = w;
	*p = q;
	*p = obj;
	mustPointsTo(b,g);
	mustPointsTo(w,d);
	return **p + *w;
}
