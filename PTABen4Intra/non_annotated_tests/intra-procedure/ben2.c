#include "myHeader.h"


    int* a;
    int* b;
    int**c;
    
    int x,y;

    int *w;
    int **s;
    int u;

 int main() {
    a=&x;
    b=&y;
    c=&a;
    s=&w;
    w=&u;
    u = 9;
    c=s;
     
    if(x==5){
	mustPointsTo(c,w);
	mustPointsTo(w,u);
    	**c = 10;
    }
    else{
	mustPointsTo(b,y);
	*b = 100;
    }

    mustPointsTo(w,u);
    while(u>10){
	mayPointsTo(w,u);
	mustPointsTo(w,u);
	*w = 100;
    }
    return 0;

}
