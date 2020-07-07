//void D(char ***ppp) { ***ppp = 0; }

//void C(char ***ppp) { ***ppp = 0; }

//void B(char ***ppp) { D(ppp); }
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


  int c;
  int *p ;
  int **pp ;
int A() {
	p = &c;
	pp = &p;
  mustPointsTo(p,c);
  mustPointsTo(pp,p);
  //B(&pp);
  **pp = 0;

  //C(&pp);
  *pp = 0;
  mustPointsTo(pp,p);
  doesNotPointsTo(p,c);
  //**pp = c;
  
  return **pp;
} 
