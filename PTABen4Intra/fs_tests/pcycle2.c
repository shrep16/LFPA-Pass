//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2
 
int ***a,**b,*c;
    int k;
int main(){

    a=&b;
    *a=&c;
    *b=&k;
    mustPointsTo(c,k);
    mustPointsTo(b,c);
    mustPointsTo(a,b);
    return ***a;
    //MUSTALIAS(c,&a);
    //MUSTALIAS(b,&c);
    //MUSTALIAS(a,&b);
    
}
