//capture may points to of s in the loop
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int *a, *b, *w;
int **c, **s;
int x, y;
int u;
int main(){

    
    a=&x;
    b=&y;
    c=&a;
    s = c;
    mustPointsTo(s,a);
    //s=&w;
    w=&u;
    mustPointsTo(w,u);
    u = 9;
     
    if(x==5){
    	mustPointsTo(c,a);
	mustPointsTo(a,x);
    	**c = 10;
    }
    else{
	*b = 100;
    }

    mustPointsTo(c,a);
    mustPointsTo(s,a);
    while(u>10){
	mustPointsTo(s,a);
	mayPointsTo(s,a);
	**s = 100;
    }

    return 0;

}
