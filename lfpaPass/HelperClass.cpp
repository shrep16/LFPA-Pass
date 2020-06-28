using namespace llvm;

class HelperClass {
	public :
		std::set<Instruction *> getPredecessors(BasicBlock::iterator, BasicBlock *);
		std::set<Instruction *> getSuccessors(BasicBlock::reverse_iterator, BasicBlock *);
		std::string getOriginalName(Function *);
		std::string demangle(const char *);
};//end HelperClass

inline std::string HelperClass::getOriginalName(Function *calledFunction) {
	std::string s1 = demangle(calledFunction->getName().str().c_str());
        size_t found = s1.find('<');
        size_t found1 = s1.find(' ');
        if(found!=std::string::npos and found1!=std::string::npos)
           	s1 = std::string(s1.begin()+found1+1, s1.begin()+found);
        return s1;
}//HelperClass::getOriginalName

inline std::string HelperClass::demangle(const char *name) {
	int status = -1;

        std::unique_ptr<char, void(*)(void*)> res { abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
        return (status == 0) ? res.get() : std::string(name);
}//HelperClass::demangle

std::set<Instruction *> HelperClass::getPredecessors(BasicBlock::iterator ii, BasicBlock *bp) {
	std::set<Instruction *> predInstSet;
	if(ii == bp->begin()) {
		for(auto pred : predecessors(bp)) {
			predInstSet.insert(&(*pred->rbegin()));
		}
	} else {
		ii--;
		predInstSet.insert(&(*ii));
	}
		
	return predInstSet;
}//HelperClass::getPredecessors

std::set<Instruction *> HelperClass::getSuccessors(BasicBlock::reverse_iterator ii, BasicBlock *bp) {
	std::set<Instruction *> succInstSet;
	if(ii == bp->rbegin()) {
		for(BasicBlock *suc : successors(bp))
			succInstSet.insert(&(*suc->begin()));	
	} else {
		ii--;
		succInstSet.insert(&(*ii));
	}
	return succInstSet;
		
}//HelperClass::getSuccessors

