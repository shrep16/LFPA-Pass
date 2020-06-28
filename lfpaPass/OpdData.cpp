using namespace llvm;

class OpdData{
	public :
		enum InstType {
			Store = 0,	//store inst
			Use,		//compare or return inst
			Call,		//function call - interprocedural
			MayPointsTo,	//may points to utility
			MustPointsTo,	//must points to utility
			DoesNotPointsTo	//does not points to utility
		};
	
	private :
		InstType type;

		using pointerPointeeMap = std::map<Value *, std::set<Value *>>;
		using operand = std::pair<Value *, int>;
		using operandSet = std::set<operand>;

		std::pair<Value *,int> lhs;
		std::set<std::pair<Value *, int>> opdList;
		std::vector<std::pair<Value *,int>> argList;
	public :
		OpdData();		
		OpdData(InstType ttype) {
			type = ttype;
			//inlined
		}

		std::pair<Value *, int> 	  getLHS();
		std::set<std::pair<Value *, int>> getRHS();
		std::set<std::pair<Value *, int>> getUse();
		std::vector<std::pair<Value *, int>> getFuncArg();
		void setLHS(operand);
		void setRHS(operandSet);
		void setFuncArg(operandSet);
		void setUse(operandSet);
		void printOperand(operand);
		InstType whetherPointsTo(std::set<Value *>, std::set<Value *>, pointerPointeeMap);
		void printLhsRhs();
		void printUse();
		void printFuncArg();
		operandSet findOperands(Value *, bool);
		InstType getInstType();
		void setInstType(InstType);
}; //end OpdData

std::set<std::pair<Value *, int>> OpdData::findOperands(Value *op, bool use) {
	std::stack<operand> st;
	operandSet opdSet;
	if(!use)
		st.push(std::make_pair(op,-1));
	else
		st.push(std::make_pair(op,-2));
		
	while(!st.empty()) {

		auto ele = st.top();
		st.pop();

		if(isa<Instruction>(ele.first)) {
			Instruction *tip = (Instruction *)ele.first;
			if(isa<AllocaInst>(tip)) {
				opdSet.insert(ele);
			}else if(isa<LoadInst>(tip)) {
				Value *ti = tip->getOperand(0);
				if(!isa<ConstantData>(ti))
					 st.push(std::make_pair(ti,ele.second+1));
			}else {
				for(int i=0, numOp = tip->getNumOperands(); i < numOp; i++) {
					Value *ti = tip->getOperand(i);						
					if(!isa<ConstantData>(ti)) {
						st.push(std::make_pair(ti,ele.second));
					}
				}
			}
		} else if (isa<GlobalVariable>(ele.first)) {
			opdSet.insert(ele);
		}
	}
	return opdSet;
		
}//OpdData::findOperands

void OpdData::printOperand(operand opd) {
	int ind = opd.second;
	while(ind > -1) {
		errs()<<"*";
		ind--;
	}
	if(((Constant *)opd.first)->isNullValue())
		errs()<<"?";
	else
		errs()<<opd.first->getName();
		
}//OpdData::printOperand

OpdData::InstType OpdData::whetherPointsTo(std::set<Value *> opd1Set, std::set<Value *> opd2Set, pointerPointeeMap A_in) {
	InstType ptsTo = DoesNotPointsTo;
	for(auto ptr : opd1Set) {
		if(A_in.find(ptr) != A_in.end()) {//ptr could be an integer as well
			for(auto pte : opd2Set) {
				if(A_in[ptr].find(pte) != A_in[ptr].end()) {
					ptsTo = MayPointsTo;
					break;
				}
			}
		}
	}

	if(ptsTo == MayPointsTo) {
		//check for must points to
		if(opd1Set.size() == 1 && opd2Set.size() == 1) {
			if(A_in[*(opd1Set.begin())].size() == 1)
				ptsTo = MustPointsTo;
		}
	}
	return ptsTo;
}//OpdData::whetherPointsTo

void OpdData::printLhsRhs() {
	if(type != Store)
		assert(false && "Not a store instruction!");
	printOperand(lhs);
	errs()<<" = ";
	for(auto it : opdList) {
		printOperand(it);
		errs()<<", ";
	}
	errs()<<"\n";
}//OpdData::printLhsRhs

void OpdData::printFuncArg() {
	if(!(type == Call || type == MayPointsTo || type == MustPointsTo || type == DoesNotPointsTo))
		assert(false && "Not a call instruction!");

	for(auto it : argList) {
		printOperand(it);
		errs()<<", ";
	}
	errs()<<"\n";	
}//OpdData::printFuncArg

void OpdData::printUse() {
	if(type != Use)
		assert(false && "Not a compare or return instruction!");

	for(auto it : opdList) {
		printOperand(it);
		errs()<<", ";
	}
	errs()<<"\n";
}//OpdData::printUse

inline std::pair<Value *, int> OpdData::getLHS() {
	if(type != Store)
		assert(false && "Not a store instruction!");
	return lhs;
}//OpdData::getLHS

inline std::set<std::pair<Value *, int>> OpdData::getRHS() {
	if(type != Store)
		assert(false && "Not a store instruction!");
	return opdList;
}//OpdData::getRHS

inline std::vector<std::pair<Value *, int>> OpdData::getFuncArg() {
	if(!(type == Call || type == MayPointsTo || type == MustPointsTo || type == DoesNotPointsTo))
		assert(false && "Not a call instruction!");
	return argList;
}//OpdData::getFuncArg

inline std::set<std::pair<Value *, int>> OpdData::getUse() {
	if(type != Use)
		assert(false && "Not a compare or store instruction!");
	return opdList;
}//OpdData::getUse

inline void OpdData::setLHS(operand opd) {
	if(type != Store)
		assert(false && "Not a store instruction!");
	lhs = opd;
}//OpdData::setLHS

void OpdData::setRHS(operandSet opdSet) {
	if(type != Store)
		assert(false && "Not a store instruction!");
	for(auto it : opdSet)
		opdList.insert(it);
}//OpdData::setRHS

void OpdData::setUse(operandSet opdSet) {
	if(type != Use)
		assert(false && "Not a compare or store instruction!");
	for(auto it : opdSet)
		opdList.insert(it);
}//OpdData::setUse

void OpdData::setFuncArg(operandSet opdSet) {
	if(!(type == Call || type == MayPointsTo || type == MustPointsTo || type == DoesNotPointsTo))
		assert(false && "Not a call instruction!");
	for(auto it : opdSet)
		argList.push_back(it);
}//OpdData::setFuncArg

inline OpdData::InstType OpdData::getInstType() {
	return type;
}//OpdData::getInstType

inline void OpdData::setInstType(InstType ttype) {
	type = ttype;
}//OpdData::setInstType
