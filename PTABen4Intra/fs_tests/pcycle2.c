//#include "aliascheck.h"
#include "myHeader.h" 
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
