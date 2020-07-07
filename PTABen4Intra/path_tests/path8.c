//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


    int **p,*q;
    int *b,*c,d,e;
int main(){


    p = &c;

c1:    if(e){
           p = &b;
           q = &d;
       }
       else if(b) {
           q = &e;
       }
       else if(c){
	       *c = 10;
         //printf("dummy branch\n");
       }
       else{
           goto c1;
       }

       *p = q;
       mayPointsTo(p,c);
       mayPointsTo(p,b);
       mayPointsTo(q,d);
       mayPointsTo(q,e);
       mayPointsTo(c,d);
       mayPointsTo(c,e);
       mayPointsTo(b,e);
       mayPointsTo(b,d);
       return **p+*q;
       /*MAYALIAS(p,&c);
       MAYALIAS(p,&b);
       MAYALIAS(c,&e);
       MAYALIAS(b,&d);
       NOALIAS(c,&d);
       NOALIAS(b,&e);*/
}
