//void D(char ***ppp) { ***ppp = 0; }

//void C(char ***ppp) { ***ppp = 0; }

//void B(char ***ppp) { D(ppp); }
#include "myHeader.h"

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
