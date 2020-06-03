//liveness based flow sensitive pointer analysis - LFPA
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include <bits/stdc++.h>
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include <cxxabi.h>
#include <cassert>

using namespace llvm;


/*
* IN and OUT (fix-point solutions) for each instruction
*/
typedef struct LFPA {
	std::set<Value *> L_in;						//IN set for Liveness Analysis
	std::set<Value *> L_out;					//OUT set for Liveness Analysis
	std::map<Value *, std::pair<std::set<Value *>, bool>> A_in;	//IN set for Pointer Analysis
	std::map<Value *, std::pair<std::set<Value *>, bool>> A_out;	//OUT set for Pointer Analysis
}LFPA;

/*
* Applicable only to store instructions
*/
typedef struct lhsRhs {
	std::pair<Value *,int> lhs;					//LHS operand of the inst
	std::set<std::pair<Value *, int>> rhs;				//RHS operand of the inst
}lhsRhs;

/*
* for testing purposes
*/
enum PointsTo {mayPointsTo, mustPointsTo, doesNotPointsTo};
 
namespace {
	class Lfpa {
		private :
			using pointerPointeeMap = std::map<Value *, std::pair<std::set<Value *>, bool>>;
			using operandInd = std::set<std::pair<Value *, int>>;
			std::map<Instruction *, LFPA> instAnalysisData;
			std::map<Instruction *, lhsRhs> instOperandData;
			std::map<Instruction *,std::tuple<Value *, Value *, PointsTo>> pointsToTest;
			std::map<Instruction *, operandInd> useMap;
			
		public :
			void run(Function *);
			void initContext(Function *);
			void initLFPAstruct(Instruction *);
			std::string getOriginalName(Function *);
			std::string demangle(const char *);
			void printPointsToTests();
			void findOperands(Value *, int, Instruction *);
			void printLhsRhs();
			bool strongLivenessAnalysis(Function *);
			bool pointerAnalysis(Function *);
			void printPointsToTestsAnswers();
			bool whetherDefnFreePathExists(Value *, LFPA);
			std::set<Instruction *> getPredecessors(BasicBlock::iterator, BasicBlock *);
			std::set<Value *> getDefRefSet(Value *, pointerPointeeMap, int *, bool *);
			std::set<Instruction *> getSuccessors(BasicBlock::reverse_iterator, BasicBlock *);
			std::set<Value *> getMayRefVariables(pointerPointeeMap ,operandInd ,std::set<Value *>, bool);
			bool isLiveIn(std::set<Value *>, std::set<Value *>);
			std::set<Value *> getMayKillSet(pointerPointeeMap, Value *, std::set<Value *> *, bool *, int *);
	};//end Lfpa

	/*
	* run - (starter function)
	*	initializes the context
	*	performs bidirectional - backward(Strong Liveness Analysis) and forward(Pointer Analysis)
	*	prints the results (of the Points to tests)
	*/
	void Lfpa::run(Function *fp) {
/*--------------------Initialize Context--------------------*/
		initContext(fp);
		//printPointsToTests();
		//printLhsRhs();

/*--------------------Perform Analysis----------------------*/
		//int itCnt = 0;
		bool changed,cL,cP;
		do {
			changed = false;
			cL = false;
			cP = false;
//errs()<<"iteration"<<itCnt<<"\n";
//if strongLivenessAnalysis is true but pointerAnalysis is false then changed is false
			//changed = strongLivenessAnalysis(fp);
			//pointerAnalysis(fp);
			cL = strongLivenessAnalysis(fp);
			cP = pointerAnalysis(fp);
			if(/*strongLivenessAnalysis(fp) && pointerAnalysis(fp)*/cL && cP) {
				changed = true;
				//errs()<<"cL = "<<cL<<" cP = "<<cP<<"\n";
			} else{
				printPointsToTestsAnswers();
			}
			//itCnt++;
		}while(changed);
			
/*--------------------Perform Analysis----------------------*/
	}//Lfpa::run


	void Lfpa::printPointsToTestsAnswers() {
		for(auto test : pointsToTest) {
			Instruction *ip = test.first;
			auto ipStruct = instAnalysisData[ip];
			auto A_in = ipStruct.A_in;
			Value *op1, *op2;
			PointsTo type;
			std::tie(op1, op2, type) = test.second;

			if(type == mayPointsTo) {
				errs()<<op1->getName()<<" may points to "<<op2->getName() <<" : ";
				if(A_in.find(op1) != A_in.end()) {
					if(A_in[op1].first.find(op2) != A_in[op1].first.end()) {
						errs()<<"Yes\n";
					} else {
						errs()<<"No\n";
					}
				} else {
					errs()<<"Not live\n";
				}
			} else if(type == mustPointsTo) {
				errs()<<op1->getName()<<" must points to "<<op2->getName() <<" : ";
				if(A_in.find(op1) != A_in.end()) {
					if(A_in[op1].first.find(op2) != A_in[op1].first.end()) {
						if((A_in[op1].first.size() == 1) && (A_in[op1].second == false)) {
							errs()<<"Yes\n";
						} else {
							errs()<<"No\n";
						}
					} else {
						errs()<<"No\n";
					}
				} else {
					errs()<<"Not live\n";
				}
			} else {
				errs()<<op1->getName()<<" does not points to "<<op2->getName() <<" : ";
				if(A_in.find(op1) != A_in.end()) {
					if(A_in[op1].first.find(op2) == A_in[op1].first.end())
						errs()<<"Yes\n";
					else
						errs()<<"No\n";
				} else 
					errs()<<"Yes not live\n";
			}
		}
	}//Lfpa::printPointsToTestsAnswers

	
	std::set<Instruction *> Lfpa::getSuccessors(BasicBlock::reverse_iterator ii, BasicBlock *bp) {
		std::set<Instruction *> succInstSet;
		if(ii == bp->rbegin()) {
			for(BasicBlock *suc : successors(bp))
				succInstSet.insert(&(*suc->begin()));	
		} else {
			ii--;
			succInstSet.insert(&(*ii));
		}
		return succInstSet;
		
	}//Lfpa::getSuccessors

	/*
	* - calculates ref_n
	* and inserts in IN
	*/
	std::set<Value *> Lfpa::getMayRefVariables(pointerPointeeMap A_in, operandInd opd, std::set<Value *> L_in, bool useInst) {
		std::set<Value *> refSet, tempSet;
		int ind;
		for(auto it : opd) {
			refSet.clear();
			refSet.insert(it.first);
			ind = it.second;

			if(useInst || (!useInst && ind != -1))
				L_in.insert(it.first);


			while(ind > -1 && refSet.size() > 0) {
				ind--;
				for(auto itValue : refSet) {
					if(A_in.find(itValue) != A_in.end()) {
						for(auto itVal : A_in.find(itValue)->second.first) {
							tempSet.insert(itVal);
							if(useInst || (!useInst && ind != -1))
								L_in.insert(itVal);
						}
										
					}
				}
				refSet = tempSet;
				tempSet.clear();
				
			}
		}
		return L_in;
	}//Lfpa::getMayRefVariables

	/*
	* returns true, if any one of the variables are present in L_out, so that the rhs should be found only if lhs is live in L_out
	*/
	inline bool Lfpa::isLiveIn(std::set<Value *> lhsSet, std::set<Value *> L_out) {
		for(auto itVal : lhsSet) {
			if(L_out.find(itVal) != L_out.end()) 
				return true;
		}
		return false;
		
	}//Lfpa::isLiveIn

	std::set<Value *> Lfpa::getMayKillSet(pointerPointeeMap A_in, Value *lOp, std::set<Value *> *L_in, bool *defnFreeLhs, int *ind) {
		std::set<Value *> mayKillSet, tempSet;
		mayKillSet.insert(lOp);
						
		while(*ind > -1 && mayKillSet.size() > 0 ) {
			for(auto itVal : mayKillSet) {
				L_in->insert(itVal);
				if(A_in.find(itVal) != A_in.end()) {
					if(A_in.find(itVal)->second.second == true)
						*defnFreeLhs = true;
					for(auto it : A_in.find(itVal)->second.first) {
						tempSet.insert(it);
					}
				}
			}
			mayKillSet = tempSet;
			tempSet.clear();
			*ind = *ind -1;
		}
		return mayKillSet;
						
	}//Lfpa::getMayKillSet

	bool Lfpa::strongLivenessAnalysis(Function *fp) {
		BasicBlock *bp;
		Instruction *ip;
		bool changed = false;
		//iterating in reverse fashion
		auto bbList = &(fp->getBasicBlockList());
		for(auto bb = bbList->rbegin(); bb != bbList->rend(); bb++) {
			bp = &(*bb);
			for(auto ii = bp->rbegin(); ii != bp->rend(); ii++) {
				ip = &(*ii);
				
				auto ipStruct = instAnalysisData[ip];
				std::set<Value *> tL_in,tL_out;

				/*---------------------------calculate OUT----------------------------*/
				for(auto succIp : getSuccessors(ii, bp)) {
					auto suc_i_struct = instAnalysisData[succIp];
					for(auto itVal : suc_i_struct.L_in)
						tL_out.insert(itVal);
				}

				
				/*--------------------------calculate IN-----------------------------*/
				
				tL_in = tL_out;

				if(isa<StoreInst>(ip)) {
					auto op = instOperandData[ip];
					auto lOp = op.lhs;
					auto rOp = op.rhs;
					auto A_in = ipStruct.A_in;
					
					std::set<Value *> lhsSet;
					bool defnFreeLhs = false;
					int ind = lOp.second;
					Value *v = lOp.first;
					lhsSet = getMayKillSet(A_in, lOp.first, &tL_in, &defnFreeLhs, &ind);
					if(ind == -1) {
						if(lhsSet.size() == 1 && !defnFreeLhs) { //must-kill
							auto killVal = lhsSet.begin();
							tL_in.erase(*killVal);
						}
						if(isLiveIn(lhsSet,tL_out)) //strong liveness
							tL_in = getMayRefVariables(A_in,rOp,tL_in,false);
					}

				} else if(isa<ICmpInst>(ip) || isa<ReturnInst>(ip)) {
					auto use = useMap[ip];
					auto A_in = ipStruct.A_in;
					tL_in = getMayRefVariables(A_in,use,tL_in,true);

				} 

				if(tL_in != ipStruct.L_in || tL_out != ipStruct.L_out) {
					changed = true;
					ipStruct.L_in = tL_in;
					ipStruct.L_out = tL_out;
					instAnalysisData[ip] = ipStruct;
				}


				/*errs()<<"-----------------------------inst"<<ip<<"--------------------------\n";
			errs()<<"IN la :\n";
			for(auto it : tL_in) 
				errs()<<it->getName()<<", ";
			errs()<<"\n";

			errs()<<"OUT la :\n";
			for(auto it : tL_out)
				errs()<<it->getName()<<", ";
			errs()<<"\n";*/


			}
		}
		

		

		return changed;
	}//Lfpa::strongLivenessAnalysis

	std::set<Instruction *> Lfpa::getPredecessors(BasicBlock::iterator ii, BasicBlock *bp) {
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
	}//Lfpa::getPredecessors

	
	/*
	* calculates pointee_n and def_n	
	*/
	std::set<Value *> Lfpa::getDefRefSet(Value *op, pointerPointeeMap A_in, int *ind, bool *defnFreePath) {
		std::set<Value *> defRefSet, tempSet;
		defRefSet.insert(op);
		while(*ind > -1 && defRefSet.size()>0) {
			for(auto itVal : defRefSet) {
				if(A_in.find(itVal) != A_in.end()) {
					if(A_in.find(itVal)->second.second == true)
						*defnFreePath = true;
					for(auto it : A_in.find(itVal)->second.first)
						tempSet.insert(it);
				}
			}
			defRefSet = tempSet;
			tempSet.clear();
			(*ind)--;
		}
		return defRefSet;
	}//Lfpa::getDefRefSet

	bool Lfpa::pointerAnalysis(Function *fp) {
		BasicBlock *bp;
		Instruction *ip;
		bool changed = false;
		//iterating in forward manner
		for(auto bb = fp->begin(); bb != fp->end(); bb++) {
			bp = &(*bb);
			for(auto ii = bp->begin(); ii != bp->end(); ii++) {
				ip = &(*ii);
				auto ipStruct = instAnalysisData[ip];
				pointerPointeeMap tA_in, tA_out;
				
				/*-----------------------calculate IN--------------------------*/
				
				for(auto itVal : ipStruct.L_in) {
					if(itVal->getType()->getContainedType(0)->isPointerTy()) {
						if(tA_in.find(itVal) != tA_in.end()) {
							std::set<Value *> pointees;		
							tA_in.insert(std::pair<Value *, std::pair<std::set<Value*>, bool>>(itVal, std::make_pair(pointees, false)));	
						}

						bool defnFreePath = false;
						auto predInstSet = getPredecessors(ii,bp);
				
						for(auto predIp : predInstSet) {
							auto pred_struct = instAnalysisData[predIp];
							if(whetherDefnFreePathExists(itVal,pred_struct))
								defnFreePath = true;
							
							for(auto itValue : pred_struct.A_out[itVal].first)
								tA_in[itVal].first.insert(itValue);		
						}
						tA_in[itVal].second = defnFreePath;
					}
				}

				/*-----------------------calculate OUT--------------------------*/

				

				for(auto itVal : tA_in) {
					if(ipStruct.L_out.find(itVal.first) != ipStruct.L_out.end()) {
						if(tA_out.find(itVal.first) == tA_out.end()) {
							std::set<Value *> pointees;			
							tA_out.insert(std::pair<Value *, std::pair<std::set<Value*>, bool>>(itVal.first, std::make_pair(pointees, false)));	
						}
						tA_out[itVal.first].second = tA_in[itVal.first].second;
						for(auto itValue : tA_in[itVal.first].first)
							tA_out[itVal.first].first.insert(itValue);
					}
				}

				if(isa<StoreInst>(ip)) {
					if(ip->getOperand(1)->getType()->isPointerTy()) {
						if(ip->getOperand(1)->getType()->getContainedType(0)->isPointerTy()) {
							std::set<Value *> lhsSet, rhsSet,newSet;
							auto op = instOperandData[ip];
							auto rOp = op.rhs;
							auto lOp = op.lhs;
							int indL,indR;
							indL = lOp.second;
							indR = rOp.begin()->second;
							bool defnFreeLhs = false, defnFreeRhs = false;

							lhsSet = getDefRefSet(lOp.first, tA_in, &indL, &defnFreeLhs); //def set
							rhsSet = getDefRefSet(rOp.begin()->first, tA_in, &indR, &defnFreeRhs); //pointee set
							
/*errs()<<"lhs set :";
for(auto it : lhsSet)
	errs()<<it->getName()<<", ";
errs()<<"\n";

errs()<<"rhs set :";
for(auto it : rhsSet)
	errs()<<it->getName()<<", ";
errs()<<"\n";*/
							if(indR == -1 && indL == -1) {
								for(auto itVal : lhsSet) {
									if(ipStruct.L_out.find(itVal) != ipStruct.L_out.end()) {
										if(tA_out.find(itVal) == tA_out.end() && rhsSet.size() != 0) {
											std::set<Value *> pointees;
											tA_out.insert(std::pair<Value *, std::pair<std::set<Value*>, bool>>(itVal,std::make_pair(pointees, false)));	
										}
										if(lhsSet.size() == 1 /*&& rhsSet.size() == 1*/ 
										   /*!defnFreeRhs*/ && !defnFreeLhs)
											tA_out[itVal].first.clear();//must update //kill

										if(rhsSet.size() == 0 || defnFreeRhs)
											tA_out[itVal].second = true; //no definition obtained
										else
											tA_out[itVal].second = false;

										for(auto itVals : rhsSet){	//def_n X pointee_n
											tA_out[itVal].first.insert(itVals);
										 
										}
									}
								}

							}
						}
					}
				}
				

				if(tA_in != ipStruct.A_in || tA_out != ipStruct.A_out) {
					changed = true;
					ipStruct.A_in = tA_in;
					ipStruct.A_out = tA_out;
					instAnalysisData[ip] = ipStruct;
				}


/*errs()<<"-----------------------------inst"<<ip<<"--------------------------\n";

errs()<<"IN pa :\n";
			for(auto it : tA_in) {
				errs()<<it.first->getName()<<" -> ";
				auto itSet = it.second.first;
				for(auto itVal : itSet)
					errs()<<itVal->getName()<<", ";
				errs()<<"\n";
errs()<<" Definition free path :"<<it.second.second<<"\n";
				
			}

			errs()<<"OUT pa :\n";
			for(auto it : tA_out) {
				errs()<<it.first->getName()<<" -> ";
				auto itSet = it.second.first;
				for(auto itVal : itSet)
					errs()<<itVal->getName()<<", ";
				errs()<<"\n";
errs()<<" Definition free path :"<<it.second.second<<"\n";
			}*/


			
			}
		}
		return changed;
	}//Lfpa::pointerAnalysis

	inline bool Lfpa::whetherDefnFreePathExists(Value *itVal, LFPA pred_struct) {
		if((pred_struct.A_out.find(itVal) == pred_struct.A_out.end()) || 
		   (pred_struct.A_out[itVal].first.size() == 0) || 
                    pred_struct.A_out[itVal].second == true)
			return true;
		return false;
	}//Lfpa::whetherDefnFreePathExists

	/*
	* initContext - figures out the function calls for Points to tests
	*		sets the LHS and RHS for store instructions
	*		initializes structure lfpa for each inst
	*/
	void Lfpa::initContext(Function *fp) {
		for(auto bb = fp->begin(); bb != fp->end(); bb++) {
			BasicBlock *bp = &(*bb);
			for(auto ii = bp->begin(); ii != bp->end(); ii++) {
				Instruction *ip = &(*ii);
				
				initLFPAstruct(ip);

				if(isa<StoreInst>(ip)) {

					std::pair<Value *,int> op1;
					std::set<std::pair<Value *, int>> op2;
					lhsRhs opStruct = {op1, op2};
					instOperandData[ip] = opStruct;
					
					findOperands(ip->getOperand(1),1,ip);
					findOperands(ip->getOperand(0),0,ip);

				} else if(isa<CallInst>(ip)) {

					Function *calledFp = cast<CallInst>(ip)->getCalledFunction();
					std::string originalName = getOriginalName(calledFp);

					PointsTo type;
					if(originalName == "mayPointsTo") {
						type = mayPointsTo;
					} else if (originalName == "mustPointsTo") {
						type = mustPointsTo;
					} else if (originalName == "doesNotPointsTo") {
						type = doesNotPointsTo;
					} else {
						assert(false && "other function calls not supported");
					}

					Value *op1 = ip->getOperand(0);
					Value *op2 = ip->getOperand(1);
					pointsToTest[ip] = std::make_tuple(op1, op2, type);

				} else if(isa<ICmpInst>(ip) || isa<ReturnInst>(ip)) {
					useMap[ip];
					
					for(int i=0, numOp = ip->getNumOperands(); i < numOp; i++) {
						Value *op = ip->getOperand(i);
						if(!isa<ConstantData>(op))
							findOperands(op,2,ip);
					}
					
				}
			}
		}
	}//Lfpa::initContext

	void Lfpa::findOperands(Value *op, int opNo, Instruction *ip) {

		std::stack<std::pair<Value *, int>> st;
		if(opNo != 2)
			st.push(std::make_pair(op,-1));
		else
			st.push(std::make_pair(op,-2));
		lhsRhs opStruct;
		if(opNo != 2) //why without inclusion giving error
			opStruct = instOperandData[ip];

		while(!st.empty()) {

			auto ele = st.top();
			st.pop();

			if(isa<Instruction>(ele.first)) {
				Instruction *tip = (Instruction *)ele.first;
				if(isa<AllocaInst>(tip)) {
					if(opNo == 1) {
						//LHS
						opStruct.lhs = ele;
					} else if(opNo == 0){
						//RHS
						opStruct.rhs.insert(ele);
					} else {
						useMap[ip].insert(ele);
					}
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
				if(opNo == 1) { //LHS
					opStruct.lhs = ele;
				} else if(opNo == 0) { //RHS
					opStruct.rhs.insert(ele);
				} else {
					useMap[ip].insert(ele);
				}
			}
		}
		if(opNo != 2)
			instOperandData[ip] = opStruct;
		
	}//Lfpa::findOperands

	void Lfpa::printLhsRhs() {
		
		errs()<<"lhs rhs for store instructions\n";
		for(auto lr_it : instOperandData) {
			lhsRhs opStruct = lr_it.second;
			auto l_op = opStruct.lhs;
			auto r_op = opStruct.rhs;
			errs()<<"lhs op = "<<l_op.first->getName()<<" ind = "<<l_op.second<<"\n";
			errs()<<"rhs :\n";
			for(auto r_it : r_op) {
				errs()<< "op = "<<r_it.first->getName() <<" ind ="<<r_it.second<<",";
			}
			errs()<<"\n";
			
		}
		
		errs()<<"use operands for compare and return insts\n";
		for(auto use_it : useMap) {
			auto use_set = use_it.second;
			for(auto it : use_set) {
				errs()<<"op = "<<it.first->getName()<<" ind = "<<it.second<<", ";
			}
			errs()<<"\n";
		}
		
	}//Lfpa::printLhsRhs

	void Lfpa::printPointsToTests() {
		for(auto pt_it : pointsToTest) {
			Value *op1, *op2;
			PointsTo type;
			std::tie(op1, op2, type) = pt_it.second;
			errs()<<"type = "<<type<<"\nOperand 1 = "<<op1->getName()<<"\nOperand 2 = "<<op2->getName()<<"\n";
		}
	}//Lfpa::printPointsToTests

	/*
	* initLFPAstruct - inserts the various data structures in LFPA struct
	*/
	inline void Lfpa::initLFPAstruct(Instruction *ip) {
		std::set<Value *> L_in, L_out;
		pointerPointeeMap A_in, A_out;
		LFPA lfpaStruct = {L_in, L_out, A_in, A_out};
		instAnalysisData.insert(std::pair<Instruction *, LFPA>(ip, lfpaStruct));
	}//Lfpa::initLFPAstruct

	std::string Lfpa::getOriginalName(Function *calledFunction) {
		std::string s1 = demangle(calledFunction->getName().str().c_str());
        	size_t found = s1.find('<');
        	size_t found1 = s1.find(' ');
        	if(found!=std::string::npos and found1!=std::string::npos)
            		s1 = std::string(s1.begin()+found1+1, s1.begin()+found);
        	return s1;
	}//Lfpa::getOriginalName

	inline std::string Lfpa::demangle(const char *name) {
		int status = -1;

        	std::unique_ptr<char, void(*)(void*)> res { abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
        	return (status == 0) ? res.get() : std::string(name);
	}//Lfpa::demangle

	class LfpaPass : public FunctionPass {
		public:
			static char ID;
			LfpaPass() : FunctionPass(ID) {}
			virtual bool runOnFunction(Function &F) override {
				errs()<<"------------------------------------------------------------------------------\n";
				errs()<<"LFPA analysis for "<<F.getName()<<"\n";
				Lfpa lfpaObj;
				lfpaObj.run(&F);
				return false;
			}
	};
}//end namespace

char LfpaPass::ID = 0;
static RegisterPass<LfpaPass> X("lfpa",
				    "Liveness based flow sensitive pointer analysis Pass",
				    false,
				    false);


