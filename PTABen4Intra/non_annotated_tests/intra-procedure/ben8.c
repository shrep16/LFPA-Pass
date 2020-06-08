//capture must points to of x at different program points
//capture must points to of *x at different program points
//good program :)
#include "myHeader.h"
int t;
int **x, **y;
int *a, *b, *c, *d,e;
int main(){
	x=&a;
        mustPointsTo(x,a);	
	*x = &e;
	mustPointsTo(a,e);
	for(t = 0;t< *a; t++) {
		mayPointsTo(x,c); //does not points to
		mayPointsTo(x,a); //does not points to
		if(1) 
		{ 
			x =&c;
		} 
		else 
		{ 
			x= &d;
		}
	       mustPointsTo(x,c);
       	       doesNotPointsTo(x,d);	       
	}
	*x = &t;
	mustPointsTo(c,t);
	return *c;
	
	
}

