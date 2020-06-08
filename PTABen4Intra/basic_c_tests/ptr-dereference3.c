#include "myHeader.h"
/*void foo(int q){
  int i = 10;
  int k = i;

}*/

int *s,*r,**x,**y,t,z,k;
int main(){

	s = &t;
	r = &z;
	y = &r;
	mustPointsTo(s,t);
	k = *s;
	s = r;
	mustPointsTo(s,z);
	k += *s;

	x = y;
	mustPointsTo(x,r);
	k += **x;
  
	
return k;
}
