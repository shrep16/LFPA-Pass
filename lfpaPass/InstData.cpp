using namespace llvm;

class InstData{
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
		InstData() {}		
		InstData(InstType ttype) {
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
		InstType getInstType();
		void setInstType(InstType);
		operandSet findOperands(Value *);
		bool IsCallInst();
}; //end InstData

void InstData::printOperand(operand opd) {
	int ind = opd.second;
	while(ind > StartInd) {
		errs()<<"*";
		ind--;
	}
	if(((Constant *)opd.first)->isNullValue())
		errs()<<"?";
	else
		errs()<<opd.first->getName();
		
}//InstData::printOperand

InstData::InstType InstData::whetherPointsTo(std::set<Value *> opd1Set, std::set<Value *> opd2Set, pointerPointeeMap A_in) {
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
}//InstData::whetherPointsTo

void InstData::printLhsRhs() {
	if(type != Store)
		assert(false && "Not a store instruction!");
	printOperand(lhs);
	errs()<<" = ";
	for(auto it : opdList) {
		printOperand(it);
		errs()<<", ";
	}
	errs()<<"\n";
}//InstData::printLhsRhs

void InstData::printFuncArg() {
	if(!IsCallInst())
		assert(false && "Not a call instruction!");

	for(auto it : argList) {
		printOperand(it);
		errs()<<", ";
	}
	errs()<<"\n";	
}//InstData::printFuncArg

void InstData::printUse() {
	if(type != Use)
		assert(false && "Not a compare or return instruction!");

	for(auto it : opdList) {
		printOperand(it);
		errs()<<", ";
	}
	errs()<<"\n";
}//InstData::printUse

inline std::pair<Value *, int> InstData::getLHS() {
	if(type != Store)
		assert(false && "Not a store instruction!");
	return lhs;
}//InstData::getLHS

inline std::set<std::pair<Value *, int>> InstData::getRHS() {
	if(type != Store)
		assert(false && "Not a store instruction!");
	return opdList;
}//InstData::getRHS

inline std::vector<std::pair<Value *, int>> InstData::getFuncArg() {
	if(!IsCallInst())
		assert(false && "Not a call instruction!");
	return argList;
}//InstData::getFuncArg

inline std::set<std::pair<Value *, int>> InstData::getUse() {
	if(type != Use)
		assert(false && "Not a compare or store instruction!");
	return opdList;
}//InstData::getUse

inline void InstData::setLHS(operand opd) {
	if(type != Store)
		assert(false && "Not a store instruction!");
	lhs = opd;
}//InstData::setLHS

void InstData::setRHS(operandSet opdSet) {
	if(type != Store)
		assert(false && "Not a store instruction!");
	for(auto it : opdSet)
		opdList.insert(it);
}//InstData::setRHS

void InstData::setUse(operandSet opdSet) {
	if(type != Use)
		assert(false && "Not a compare or store instruction!");
	for(auto it : opdSet)
		opdList.insert(it);
}//InstData::setUse

void InstData::setFuncArg(operandSet opdSet) {
	if(!IsCallInst())
		assert(false && "Not a call instruction!");
	for(auto it : opdSet)
		argList.push_back(it);
}//InstData::setFuncArg

inline InstData::InstType InstData::getInstType() {
	return type;
}//InstData::getInstType

inline void InstData::setInstType(InstType ttype) {
	type = ttype;
}//InstData::setInstType

inline bool InstData::IsCallInst() {
	if(type == Call || type == MustPointsTo || type == MayPointsTo || type == DoesNotPointsTo)
		return true;
	return false;
}//InstData::IsCallInst

std::set<std::pair<Value *, int>> InstData::findOperands(Value *op) {
	std::stack<operand> st;
	operandSet opdSet;
	if(type == Store)
		st.push(std::make_pair(op,StartInd));
	else
		st.push(std::make_pair(op,StartInd-1));
		
	while(!st.empty()) {

		auto ele = st.top();
		st.pop();

		if(isa<Instruction>(ele.first)) {
			Instruction *tip = (Instruction *)ele.first;
			if(isa<AllocaInst>(tip)) {
				if(ele.first->getType()->getContainedType(0)->isPointerTy() 
				   || IsCallInst()
				   || type == Store)
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
			if(ele.first->getType()->getContainedType(0)->isPointerTy() 
		           || IsCallInst()
			   || type == Store)
				opdSet.insert(ele);
		}
	}
	return opdSet;
		
}//InstData::findOperands

