//#include "myHeader.h"

int **z,*x,**y;
int *v,*u,w;

int main(){
	y = &v;
	z = &u;
	x = &w;
	if(w > 10)
		*z = x;
	else
		z = y;
	return *u;
}
