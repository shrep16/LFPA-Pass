//#include "aliascheck.h"
#include "myHeader.h"

/*int* foo(int* x){
   return x;
}*/

  int *x,*y,*p,*q,a,b;
int main(){
  mustPointsTo(p,a); //not live
  doesNotPointsTo(q,b); //not live
  p = &a;
  q = &b;
  x = p;
  y = q;
  mustPointsTo(x,a);
  mustPointsTo(*x,a);//fails
  doesNotPointsTo(x,b);
  mustPointsTo(y,b);
  mustPointsTo(q,b);
  mayPointsTo(p,a);

  

  //x = foo(p);
  //y = foo(q);
  /*MUSTALIAS(x,p);
  MUSTALIAS(y,q);
  NOALIAS(x,q);
  NOALIAS(y,p);*/
  return *p+*q+*x+*y;

}
