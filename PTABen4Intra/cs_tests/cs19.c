//#include "aliascheck.h"
#include "myHeader.h"
int g;
int* obj = &g;
//void Zulu(int**p, int *q);
/* 
void Xray(){

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

	x = &b;
	w = &d;
	int **p = x;
	int *q = w;
	*p = q;
	*p = obj;
	mustPointsTo(b,g);
	mustPointsTo(w,d);
	return **p+*w;


	//Xray();

}
