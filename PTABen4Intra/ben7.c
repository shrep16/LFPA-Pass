#include "myHeader.h"
int *p,a,b;
/*void foo(int* p){

    *p=100;

}*/
int *q,s,c,d;

int main(){
    q = &s;
    mustPointsTo(q,s);
    p=q;
    mustPointsTo(p,s);

    if(p){
	p =&c;
    }
    else{
	p=&d;
    }
    mayPointsTo(p,c);
    mayPointsTo(p,d);
    doesNotPointsTo(p,s);
    *p = 100;
//    foo(p);

    return 0;
}
