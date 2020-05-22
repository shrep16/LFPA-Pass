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
	std::set<Value *> in_LA;					//IN set for Liveness Analysis
	std::set<Value *> out_LA;					//OUT set for Liveness Analysis
	std::map<Value *, std::pair<std::set<Value *>, bool>> in_PA;	//IN set for Pointer Analysis
	std::map<Value *, std::pair<std::set<Value *>, bool>> out_PA;	//OUT set for Pointer Analysis
}LFPA;

/*
* Applicable only to store instructions
*/
typedef struct lhsRhs {
	std::pair<Value *,int> lhs;					//LHS operand of the inst
	std::set<std::pair<Value *, int>> rhs;				//RHS operand of the inst
	//int l_ind;					//indirection of lhs operand
	//int r_ind;					//indirection of rhs operand
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
			//std::map<Instruction *,std::set<std::pair<Value *, int>>> useMap;
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
			void pointerAnalysis(Function *);
			void printPointsToTestsAnswers();
			bool whetherDefnFreePathExists(Value *, LFPA);
			std::set<Instruction *> getPredecessors(BasicBlock::iterator, BasicBlock *);
			std::set<Value *> getDefRefSet(Value *, pointerPointeeMap, int *, bool *);
			std::set<Instruction *> getSuccessors(BasicBlock::reverse_iterator, BasicBlock *);
			std::set<Value *> getMayRefVariables(pointerPointeeMap ,operandInd ,std::set<Value *>);
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
		bool changed;
		do {
			changed = false;
//errs()<<"iteration"<<itCnt<<"\n";
			changed = strongLivenessAnalysis(fp);
			pointerAnalysis(fp);
			if(!changed) {
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
			auto inPA = ipStruct.in_PA;
			Value *op1, *op2;
			PointsTo type;
			std::tie(op1, op2, type) = test.second;

			if(type == mayPointsTo) {
				errs()<<op1->getName()<<" may points to "<<op2->getName() <<" : ";
				if(inPA.find(op1) != inPA.end()) {
					if(inPA[op1].first.find(op2) != inPA[op1].first.end()) {
						errs()<<"Yes\n";
					} else {
						errs()<<"No\n";
					}
				} else {
					errs()<<"No\n";
				}
			} else if(type == mustPointsTo) {
				errs()<<op1->getName()<<" must points to "<<op2->getName() <<" : ";
				if(inPA.find(op1) != inPA.end()) {
					if(inPA[op1].first.find(op2) != inPA[op1].first.end()) {
						if((inPA[op1].first.size() == 1) && (inPA[op1].second == false)) {
							errs()<<"Yes\n";
						} else {
							errs()<<"No\n";
						}
					} else {
						errs()<<"No\n";
					}
				} else {
					errs()<<"No\n";
				}
			} else {
				errs()<<op1->getName()<<" does not points to "<<op2->getName() <<" : ";
				if(inPA.find(op1) != inPA.end()) {
					if(inPA[op1].first.find(op2) == inPA[op1].first.end())
						errs()<<"Yes\n";
					else
						errs()<<"No\n";
				} else 
					errs()<<"Yes\n";
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

	std::set<Value *> Lfpa::getMayRefVariables(pointerPointeeMap INpa, operandInd opd, std::set<Value *> INla) {
		std::set<Value *> refSet, tempSet;
		int ind;
		for(auto it : opd) {//there will be only one rhs
			refSet.clear();//check
			refSet.insert(it.first);
			INla.insert(it.first);
			ind = it.second;
			while(ind > -1 && refSet.size() > 0) {
				for(auto itValue : refSet) {
					if(INpa.find(itValue) != INpa.end()) {
						for(auto itVal : INpa.find(itValue)->second.first) {
							tempSet.insert(itVal);
							INla.insert(itVal);
						}
										
					}
				}
				refSet = tempSet;
				tempSet.clear();
				ind--;
			}
		}
		return INla;
	}//Lfpa::getMayRefVariables

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
				std::set<Value *> tINla,tOUTla;

				/*---------------------------calculate OUT----------------------------*/
				for(auto succIp : getSuccessors(ii, bp)) {
					auto suc_i_struct = instAnalysisData[succIp];
					for(auto itVal : suc_i_struct.in_LA)
						tOUTla.insert(itVal);
				}
		
				/*--------------------------calculate IN-----------------------------*/
				if(isa<StoreInst>(ip)) {
					auto op = instOperandData[ip];
					auto lOp = op.lhs;
					auto rOp = op.rhs;
					auto INpa = ipStruct.in_PA;
	
					if(lOp.second == -1) {
						tINla = tOUTla;
						if(tOUTla.find(lOp.first) != tOUTla.end()) {
							tINla.erase(lOp.first);	//erase the LHS
							tINla = getMayRefVariables(INpa,rOp,tINla);
						}
					} else {
						//mayDef-begin
						std::set<Value *> lhsSet, tempSet;
						lhsSet.insert(lOp.first);
						int ind = lOp.second;
						while(ind > -1 && lhsSet.size() > 0 ) {
							for(auto itVal : lhsSet) {
								tINla.insert(itVal);
								if(INpa.find(itVal) != INpa.end()) {
									for(auto it : INpa.find(itVal)->second.first) {
										tempSet.insert(it);
									}
								}
							}
							lhsSet = tempSet;
							tempSet.clear();
							ind--;
						}//mayDef-end
						if(ind == -1) { //*x=y case
							for(auto itVal : lhsSet) {
								if(tOUTla.find(itVal) != tOUTla.end()) {
									tINla = getMayRefVariables(INpa,rOp,tINla);
								}
							}
						}	
					}
				} else if(isa<ICmpInst>(ip) || isa<ReturnInst>(ip)) {

					tINla = tOUTla;
					auto use = useMap[ip];
					auto INpa = ipStruct.in_PA;
					tINla = getMayRefVariables(INpa,use,tINla);

				} else {
					tINla = tOUTla;
				}

				if(tINla != ipStruct.in_LA || tOUTla != ipStruct.out_LA) {
					changed = true;
					ipStruct.in_LA = tINla;
					ipStruct.out_LA = tOUTla;
					instAnalysisData[ip] = ipStruct;
				}


				/*errs()<<"-----------------------------inst"<<ip<<"--------------------------\n";
			errs()<<"IN la :\n";
			for(auto it : tINla) 
				errs()<<it->getName()<<", ";
			errs()<<"\n";

			errs()<<"OUT la :\n";
			for(auto it : tOUTla)
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

	//for pointer analysis
	std::set<Value *> Lfpa::getDefRefSet(Value *op, pointerPointeeMap INpa, int *ind, bool *defnFreePath) {
		std::set<Value *> defRefSet, tempSet;
		defRefSet.insert(op);
		while(*ind > -1 && defRefSet.size()>0) {
			for(auto itVal : defRefSet) {
				if(INpa.find(itVal) != INpa.end()) {
					if(INpa.find(itVal)->second.second == true)
						*defnFreePath = true;
					for(auto it : INpa.find(itVal)->second.first)
						tempSet.insert(it);
				}
			}
			defRefSet = tempSet;
			tempSet.clear();
			(*ind)--;
		}
		return defRefSet;
	}//Lfpa::getDefRefSet

	void Lfpa::pointerAnalysis(Function *fp) {
		BasicBlock *bp;
		Instruction *ip;
		//iterating in forward manner
		for(auto bb = fp->begin(); bb != fp->end(); bb++) {
			bp = &(*bb);
			for(auto ii = bp->begin(); ii != bp->end(); ii++) {
				ip = &(*ii);
				auto ipStruct = instAnalysisData[ip];
				//std::map<Value *, std::pair<std::set<Value *>, bool>> tINpa, tOUTpa;
				pointerPointeeMap tINpa, tOUTpa;
				
				/*-----------------------calculate IN--------------------------*/
				
				for(auto itVal : ipStruct.in_LA) {
					if(itVal->getType()->getContainedType(0)->isPointerTy()) {
						if(tINpa.find(itVal) != tINpa.end()) {
							std::set<Value *> pointees;		
							tINpa.insert(std::pair<Value *, std::pair<std::set<Value*>, bool>>(itVal, std::make_pair(pointees, false)));	//since the variable has not been defined
						}

						bool defnFreePath = false;
						for(auto predIp : getPredecessors(ii,bp)) {
							auto pred_struct = instAnalysisData[predIp];
							if(whetherDefnFreePathExists(itVal,pred_struct))
								defnFreePath = true;
							
							for(auto itValue : pred_struct.out_PA[itVal].first)
								tINpa[itVal].first.insert(itValue);		
						}
						tINpa[itVal].second = defnFreePath;
					}
				}

				/*-----------------------calculate OUT--------------------------*/

				

				for(auto itVal : tINpa) {
					if(ipStruct.out_LA.find(itVal.first) != ipStruct.out_LA.end()) {
						if(tOUTpa.find(itVal.first) == tOUTpa.end()) {
							std::set<Value *> pointees;			
							tOUTpa.insert(std::pair<Value *, std::pair<std::set<Value*>, bool>>(itVal.first, std::make_pair(pointees, false)));	
						}
						tOUTpa[itVal.first].second = tINpa[itVal.first].second;
						for(auto itValue : tINpa[itVal.first].first)
							tOUTpa[itVal.first].first.insert(itValue);
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

							lhsSet = getDefRefSet(lOp.first, tINpa, &indL, &defnFreeLhs); //def set
							rhsSet = getDefRefSet(rOp.begin()->first, tINpa, &indR, &defnFreeRhs); //ref set
							
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
									if(ipStruct.out_LA.find(itVal) != ipStruct.out_LA.end()) {
										if(tOUTpa.find(itVal) == tOUTpa.end()) {
											std::set<Value *> pointees;
											tOUTpa.insert(std::pair<Value *, std::pair<std::set<Value*>, bool>>(itVal,std::make_pair(pointees, false)));	
										}
										if(lhsSet.size() == 1 && rhsSet.size() == 1 &&
										   !defnFreeRhs && !defnFreeLhs)
											tOUTpa[itVal].first.clear();

										if(rhsSet.size() == 0 || defnFreeRhs)
											tOUTpa[itVal].second = true; //no definition obtained
										else
											tOUTpa[itVal].second = false;

										for(auto itVals : rhsSet){
											tOUTpa[itVal].first.insert(itVals);
										 
										}
									}
								}

							}
						}
					}
				}
				ipStruct.in_PA = tINpa;
				ipStruct.out_PA = tOUTpa;
				instAnalysisData[ip] = ipStruct;


/*errs()<<"-----------------------------inst"<<ip<<"--------------------------\n";

errs()<<"IN pa :\n";
			for(auto it : tINpa) {
				errs()<<it.first->getName()<<" -> ";
				auto itSet = it.second.first;
				for(auto itVal : itSet)
					errs()<<itVal->getName()<<", ";
				errs()<<"\n";
errs()<<" Definition free path :"<<it.second.second<<"\n";
				
			}

			errs()<<"OUT pa :\n";
			for(auto it : tOUTpa) {
				errs()<<it.first->getName()<<" -> ";
				auto itSet = it.second.first;
				for(auto itVal : itSet)
					errs()<<itVal->getName()<<", ";
				errs()<<"\n";
errs()<<" Definition free path :"<<it.second.second<<"\n";
			}*/


			
			}
		}
	}//Lfpa::pointerAnalysis

	inline bool Lfpa::whetherDefnFreePathExists(Value *itVal, LFPA pred_struct) {
		if((pred_struct.out_PA.find(itVal) == pred_struct.out_PA.end()) || 
		   (pred_struct.out_PA[itVal].first.size() == 0) || 
                   (pred_struct.out_PA[itVal].second == true))
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
					int i=0;
					for(User::op_iterator oi = ip->op_begin(), eo = ip->op_end(); eo != oi; oi++,i++) {
						Value *op = ip->getOperand(i);
						if(op != (Value *)ip && !isa<ConstantData>(op))
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
					int i=0;
					for(User::op_iterator oi = tip->op_begin(), eo = tip->op_end(); eo != oi; oi++,i++) {
						Value *ti = tip->getOperand(i);						
						if((ele.first != ti) && !isa<ConstantData>(ti)) {
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
		std::set<Value *> INla, OUTla;
		std::map<Value *, std::pair<std::set<Value *>,bool>> INpa, OUTpa;
		LFPA lfpaStruct = {INla, OUTla, INpa, OUTpa};
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


