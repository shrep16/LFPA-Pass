/*
 * Global variables test.
 * Author: Sen Ye
 * Date: 03/05/2014
 */
//#include "aliascheck.h"
#include "myHeader.h"

int a_int = 10;
int* p_int = &a_int;
int** pp_int = &p_int;

int main() {
	int b_int = a_int;
	int* q_int = p_int;
	int** qq_int = pp_int;
	mustPointsTo(p_int,a_int);
	mustPointsTo(q_int,a_int);
	mustPointsTo(pp_int,p_int);
	mustPointsTo(qq_int,p_int);
	mustPointsTo(*qq_int, a_int);
	//MUSTALIAS(*qq_int, q_int);
	//MUSTALIAS(q_int, &a_int);
	b_int = *q_int + **qq_int + *p_int + **pp_int;
        doesNotPointsTo(p_int,a_int);
	return b_int;
}
