/*
 * Global pointer in flow-sensitive analysis.
 * Author: Sen Ye
 * Date: 08/11/2013
 */

//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int x, y; int *p = &x; int *q = &y;
int **pp = &p; int**qq = &q;

/*void foo() {
     NOALIAS(*pp, *qq);
}
void bar() {
     qq = &q;
     q = &x;
}*/
int main() {
//    foo();
   // bar();
    qq = &q;
    q = &x;
    mustPointsTo(*qq,x);
    mustPointsTo(*pp,x);
    return **qq+**pp;
    //MUSTALIAS(*pp, *qq);
}

