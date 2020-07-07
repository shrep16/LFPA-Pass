#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int main(){
    char *a = "a";

    *a = "f";
    mustPointsTo(a,"f");
    return 0;


}
