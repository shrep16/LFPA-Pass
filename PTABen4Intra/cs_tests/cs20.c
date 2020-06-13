//#include "aliascheck.h"
#include "myHeader.h"
int *obj;
int **x,*b,*w,d;
int **z, *a,*y,c;
/*void Zulu(int**p, int *q);
 
void Xray(){
	x = &b;
	w = &d;
	Zulu(x,w);
    MUSTALIAS(b,&d);
    NOALIAS(b,&c);
}

void Yank(){
	z = &a;
	y = &c;
	Zulu(z,y);
    MUSTALIAS(a,&c);
    NOALIAS(a,&d);
}

void Zulu(int**p,int *q){
	*q = 100;
	*p = q;

}*/

int main(){
	x = &b;
	int **p, *q;
	w = &d;
	p = x;
	q = w;
	*q = 100;
	*p =q;

	mustPointsTo(b,d);
	d = **p;



	z = &a;
	y = &c;
	q = y;
	p = z;
	*q = 100;
	*p = q;
	mustPointsTo(a,c);
	d += **p;
	return d;



	//Xray();
	//Yank();

}
