#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

/*void foo(int q){
  int i = 10;
  int k = i;

}*/

int *s,*r,*x,**y,t,z,k;
int main(){

	s = &t;
	r = &z;
	y = &r;
	mustPointsTo(s,t);
	k = *s;
	s = r;
	mustPointsTo(s,z);
	k += *s;

	mustPointsTo(y,r);
	mustPointsTo(r,z);
	x = *y;
	mustPointsTo(x,z);
	k += *x;
  
	
return k;
}
