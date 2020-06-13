//#include "aliascheck.h"
#include "myHeader.h"
int obj,b;
/*void bar(int **s){
    *s = &b;
}

void foo(int **p){
	*p = &obj;
	bar(p);
}*/
	int **x;
	int *a, *c;
int main(){
	x=&a; 
	int **p;
	p = x;
	*p = &obj;
	int **s;
	s = p;
	*s = &b;
	mustPointsTo(a,b);
	doesNotPointsTo(a,obj);
	obj = **p;
	//foo(x);
    //MUSTALIAS(a,&b);
    //NOALIAS(a,&obj);
	x = &c;
	p = x;
	*p = &obj;
	s = p;
	*s = &b;
	mustPointsTo(c,b);
	doesNotPointsTo(c,obj);
	obj += **s;
	return obj;

	//foo(x);
    //MUSTALIAS(c,&b);
    //NOALIAS(c,&obj);
	
}

