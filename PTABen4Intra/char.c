#include "myHeader.h"
int main(){
    char *a = "a";

    *a = "f";
    mustPointsTo(a,"f");
    return 0;


}
