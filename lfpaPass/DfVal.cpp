/*
* IN and OUT (fix-point solutions) for each instruction
*/
using namespace llvm;
class DfVal {
	private :
		using pointerPointeeMap = std::map<Value *, std::set<Value *>>;
		using operand = std::pair<Value *, int>;
		using operandSet = std::set<operand>;
		std::set<Value *> L_in;					//IN set for Liveness Analysis
		std::set<Value *> L_out;				//OUT set for Liveness Analysis
		std::map<Value *, std::set<Value *>> A_in;		//IN set for Pointer Analysis
		std::map<Value *, std::set<Value *>> A_out;		//OUT set for Pointer Analysis
	public :
		bool isLiveIn(std::set<Value *>, std::set<Value *>);
		std::set<Value *> getDefRefSet(Value *, pointerPointeeMap, int *);
		std::set<Value *> getMayRefVariables(pointerPointeeMap ,operandSet ,std::set<Value *>, bool);
		std::set<Value *> getMayDefVariables(pointerPointeeMap, Value *, std::set<Value *> *, int *);
		bool whetherDefnFreePathExists(Value *);
		void setLin(std::set<Value *>);
		void setLout(std::set<Value *>);
		void setAin(pointerPointeeMap);
		void setAout(pointerPointeeMap);
		std::map<Value *, std::set<Value *>> getAin();
		std::map<Value *, std::set<Value *>> getAout();
		std::set<Value *> getLin();
		std::set<Value *> getLout();
		void printL(std::set<Value *>);
		void printA(pointerPointeeMap);
		
};//end DfVal


void DfVal::printA(pointerPointeeMap A) {
	for(auto it : A) {
		if(((Constant *)(it.first))->isNullValue())
			errs()<<"? -> ";
		else
			errs()<<it.first->getName()<<" -> ";
		auto itSet = it.second;
		for(auto itVal : itSet) {
			if(((Constant *)itVal)->isNullValue())
				errs()<<"?, ";
			else
				errs()<<itVal->getName()<<", ";
		}
		errs()<<"\n";			
	}
}//DfVal::printA

void DfVal::printL(std::set<Value *> L) {
	for(auto it : L) {
		if(((Constant *)it)->isNullValue())
			errs()<<"?, ";
		else
			errs()<<it->getName()<<", ";
	}
	errs()<<"\n";
}//DfVal::printL

inline std::set<Value *> DfVal::getLout() {
	return L_out;
}//DfVal::getLout

inline std::set<Value *> DfVal::getLin() {
	return L_in;
}//DfVal::getLin

inline std::map<Value *, std::set<Value *>> DfVal::getAout() {
	return A_out;
}//DfVal::getAout

inline std::map<Value *, std::set<Value *>> DfVal::getAin() {
	return A_in;
}//DfVal::getAin

inline void DfVal::setAin(pointerPointeeMap tA_in) {
	A_in = tA_in;
}//DfVal::setAin

inline void DfVal::setAout(pointerPointeeMap tA_out) {
	A_out =tA_out;
}//DfVal::setAout

inline void DfVal::setLin(std::set<Value *> tL_in) {
	L_in = tL_in;
}//DfVal::setLin

inline void DfVal::setLout(std::set<Value *> tL_out) {
	L_out = tL_out;
}//DfVal::setLout

inline bool DfVal::whetherDefnFreePathExists(Value *itVal) {
	if((A_out.find(itVal) == A_out.end()) || (A_out[itVal].size() == 0))
		return true;
	return false;
}//DfVal::whetherDefnFreePathExists

std::set<Value *> DfVal::getMayDefVariables(pointerPointeeMap A, Value *Op, std::set<Value *> *L, int *ind) {
	std::set<Value *> mayDefSet, tempSet;
	mayDefSet.insert(Op);
						
	while(*ind > StartInd && mayDefSet.size() > 0 ) {
		for(auto itVal : mayDefSet) {
			L->insert(itVal);
			if(A.find(itVal) != A.end()) {
				for(auto it : A.find(itVal)->second) {
					tempSet.insert(it);
				}
			}
		}
		mayDefSet = tempSet;
		tempSet.clear();
		*ind = *ind -1;
	}
	return mayDefSet;
}//DfVal::getMayDefVariables

/*
* - calculates ref_n
* and inserts in IN
*/
std::set<Value *> DfVal::getMayRefVariables(pointerPointeeMap A, operandSet opd, std::set<Value *> L, bool useInst) {
	std::set<Value *> refSet, tempSet;
	int ind;
	for(auto it : opd) {
		refSet.clear();
		refSet.insert(it.first);
		ind = it.second;

		if(useInst || (!useInst && ind != StartInd))
			L.insert(it.first);


		while(ind > StartInd && refSet.size() > 0) {
			ind--;
			for(auto itValue : refSet) {
				if(A.find(itValue) != A.end()) {
					for(auto itVal : A.find(itValue)->second) {
						tempSet.insert(itVal);
						if(useInst || (!useInst && ind != StartInd))
							if(itVal->getType()->getContainedType(0)->isPointerTy())
								L.insert(itVal); //don't insert scalars
					}				
				}
			}
			refSet = tempSet;
			tempSet.clear();			
		}
	}
	return L;
}//DfVal::getMayRefVariables

/*
* calculates pointee_n and def_n	
*/
std::set<Value *> DfVal::getDefRefSet(Value *op, pointerPointeeMap A, int *ind) {
	std::set<Value *> defRefSet, tempSet;
	defRefSet.insert(op);
	while(*ind > StartInd && defRefSet.size()>0) {
		for(auto itVal : defRefSet) {
			if(A.find(itVal) != A.end()) {
				for(auto it : A.find(itVal)->second)
					tempSet.insert(it);
			}
		}
		defRefSet = tempSet;
		tempSet.clear();
		(*ind)--;
	}
	return defRefSet;
}//DfVal::getDefRefSet
/*
* returns true, if any one of the variables are present in L
*/
bool DfVal::isLiveIn(std::set<Value *> valSet, std::set<Value *> L) {
	for(auto itVal : valSet) {
		if(L.find(itVal) != L.end()) 
			return true;
	}
	return false;		
}//DfVal::isLiveIn

