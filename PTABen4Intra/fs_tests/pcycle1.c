//#include "aliascheck.h"
#include "myHeader.h"  
int ***m,**n,*z,*y,z1,y1;
int main(){

    m=&n;
    n=&z;
    *m=&y;
    mustPointsTo(n,y);
    doesNotPointsTo(n,z);
    z1 = ***m;
    //MUSTALIAS(n,&y);
    //NOALIAS(n,&z);
    z=&z1;
    y=&y1;
    mustPointsTo(z,z1);
    z1+= *z;
    ***m=10;
    z=**m;
    mustPointsTo(z,y1);
    z1+= *z;
    return z1;
    //NOALIAS(z,&z1);
}
