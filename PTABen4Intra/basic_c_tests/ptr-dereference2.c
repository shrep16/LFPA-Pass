#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int **a, *b, *x ,c;

int main(){

  c = 10;
  a = &b;
  b = &c;
  mustPointsTo(a,b);
  mustPointsTo(b,c);
  x = *a;
  
  mustPointsTo(x,c);
  int y = *x;
  mustPointsTo(x,c);


  return y;
}
