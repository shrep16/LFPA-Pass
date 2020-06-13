//#include "aliascheck.h"
#include "myHeader.h"

//void foo(int ***,int**);
int obj;
    int ***p,**q, **a, **b, *c, *m,*n,d;
int main(){

    m = &d;
    a = b = &c;
    if(a){
        p = &a;
        q = &c;
	*p = q;
        //foo(p,q);
    }
    else{
        p = &b;
        q = &c;
	*p = q;
        //foo(p,q);
    }

   *a = m;
   n = *b;
   mustPointsTo(a,c);
   mustPointsTo(b,c);
   mustPointsTo(c,d);
   mustPointsTo(m,d);
   mustPointsTo(q,c);
   mustPointsTo(n,d);
   return **a+**b+**q+*n+*m;
   //MUSTALIAS(a,b);
   //MUSTALIAS(m,n);
}

//void foo(int ***x,int **y){
  //  *x = y;
//}
