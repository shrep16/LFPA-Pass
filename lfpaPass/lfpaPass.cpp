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
			std::map<Instruction *, LFPA> instAnalysisData;
			std::map<Instruction *, lhsRhs> instOperandData;
			std::map<Instruction *,std::tuple<Value *, Value *, PointsTo>> pointsToTest;
			std::map<Instruction *,std::set<std::pair<Value *, int>>> useMap;
			
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
			void printAnswersPerIteration();
			void printPointsToTestsAnswers();
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
/*--------------------Initialize Context--------------------*/

/*--------------------Perform Analysis----------------------*/
		int itCnt = 0;
		bool changed;
		do {
			errs()<<"----------------------------iteration"<<itCnt<<"-------------------------\n";
			changed = false;
			changed = strongLivenessAnalysis(fp);
			pointerAnalysis(fp);
			if(!changed) {
				printPointsToTestsAnswers();	
			}	
			//errs()<<"changed = "<<changed<<"\n";
			itCnt++;

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

	void Lfpa::printAnswersPerIteration() {
		//iterate through instAnalysisData
		int instCnt = 1;
		for(auto itVal : instAnalysisData) {
			auto ipStruct = itVal.second;
			
			auto INla = ipStruct.in_LA;
			auto OUTla = ipStruct.out_LA;
			auto INpa = ipStruct.in_PA;
			auto OUTpa = ipStruct.out_PA;
			errs()<<"-----------------------------inst"<<itVal.first<<"--------------------------\n";
			errs()<<"IN la :\n";
			for(auto it : INla) 
				errs()<<it->getName()<<", ";
			errs()<<"\n";

			errs()<<"OUT la :\n";
			for(auto it : OUTla)
				errs()<<it->getName()<<", ";
			errs()<<"\n";

			/*errs()<<"IN pa :\n";
			for(auto it : INpa) {
				errs()<<it.first->getName()<<" -> ";
				auto itSet = it.second.first;
				for(auto itVal : itSet)
					errs()<<itVal->getName()<<", ";
				errs()<<"\n";
			}

			errs()<<"OUT pa :\n";
			for(auto it : OUTpa) {
				errs()<<it.first->getName()<<" -> ";
				auto itSet = it.second.first;
				for(auto itVal : itSet)
					errs()<<itVal->getName()<<", ";
				errs()<<"\n";
			}*/
			instCnt++;
			
		}
	}//Lfpa::printAnswersPerIteration


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
				auto tINla = ipStruct.in_LA;	//is it required
				auto tOUTla = ipStruct.out_LA;

				/*---------------------------calculate OUT----------------------------*/
				if(ii == bp->rbegin()) { //error
					for(BasicBlock *suc : successors(bp)) {
						Instruction *suc_i = &(*suc->begin());
						auto suc_i_struct = instAnalysisData[suc_i];
						for(auto itVal : suc_i_struct.in_LA)
							tOUTla.insert(itVal);
					}
 				} else{
					auto tii = ii;
					tii--; //error
					Instruction *tip = &(*tii);
					auto suc_i_struct = instAnalysisData[tip];
					for(auto itVal : suc_i_struct.in_LA)
						tOUTla.insert(itVal);
				}
		
				/*--------------------------calculate IN-----------------------------*/
				
				//for(auto itVal : tOutla)
				//	tINla.insert(itVal);


				if(isa<StoreInst>(ip)) {
					auto op = instOperandData[ip];
					auto lOp = op.lhs;
					auto rOp = op.rhs;

					auto INpa = ipStruct.in_PA;
					
					std::set<Value *> newSet;

					


					
					//if(ip->getOperand(1)->getType()->getContainedType(0)->isPointerTy()) {
						
					if(lOp.second == -1) {


						tINla = tOUTla;

						
						if(tOUTla.find(lOp.first) != tOUTla.end()) {
							tINla.erase(lOp.first);	//erase the LHS
							//findValues(RHS);

							std::set<Value *> oldSet;

							for(auto rIt : rOp) {
								oldSet.clear();//check- stmt required
								oldSet.insert(rIt.first);
								tINla.insert(rIt.first);
								int ind = rIt.second;
								while(ind > -1 && oldSet.size() > 0) {
									for(auto itVal : oldSet) {
										if(INpa.find(itVal) != INpa.end()) {
											for(auto it : INpa.find(itVal)->second.first) {
												newSet.insert(it);
												tINla.insert(it);
											}
										
										}
									}
									oldSet = newSet;
									newSet.clear();
									ind--;
								}
							}//findValues

						}
					} else {
						//findValues(LHS);
						std::set<Value *> lhsSet,rhsSet;
						lhsSet.insert(lOp.first);
						//tINla.clear();
						//tINla.insert(lOp.second);
						int ind = lOp.second;
						while(ind > -1 && lhsSet.size() > 0 ) {
							for(auto itVal : lhsSet) {
								tINla.insert(itVal);
								if(INpa.find(itVal) != INpa.end()) {
									for(auto it : INpa.find(itVal)->second.first) {
										newSet.insert(it);
									}
								}
							}
							lhsSet = newSet;
							newSet.clear();
							ind--;
						}//findValues(LHS)
						if(ind == -1) {
							
							for(auto itVal : lhsSet) {
								if(tOUTla.find(itVal) != tOUTla.end()) {
									//tINla.erase(itVal);	//erase the lhs	//lhs already deleted
									
									//findValues(RHS);
									for(auto rIt : rOp) {//there will be only one rhs
										//rhsSet.clear();//check
										rhsSet.insert(rIt.first);
										tINla.insert(rIt.first);
										ind = rIt.second;
										while(ind > -1 && rhsSet.size() > 0) {
											for(auto itValue : rhsSet) {
												if(INpa.find(itValue) != INpa.end()) {
													for(auto it : INpa.find(itValue)->second.first) {
														newSet.insert(it);
														tINla.insert(it);
													}
										
												}
											}
											rhsSet = newSet;
											newSet.clear();
											ind--;
										}
									}
								}
							}
						}
						
						
					}
				} else if(isa<ICmpInst>(ip) || isa<ReturnInst>(ip)) {

					tINla = tOUTla;


					auto use = useMap[ip];
					auto INpa = ipStruct.in_PA;
					std::set<Value *> oldSet;
					std::set<Value *> newSet;
					
					//findValues(use);
					for(auto itVal : use) {
						//if(itVal->getType()->getContainedType(0)->isPointerTy()) {
						oldSet.clear();
						oldSet.insert(itVal.first);
						tINla.insert(itVal.first);
						int ind = itVal.second;

						while(ind > -1 && oldSet.size() > 0) {
							for(auto itValue : oldSet) {
								if(INpa.find(itValue) != INpa.end()) {
									for(auto it : INpa.find(itValue)->second.first) {
										newSet.insert(it);
										tINla.insert(it);
									}
								}
							}
							ind--;
							oldSet = newSet;
							newSet.clear();
						}
					}
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

	void Lfpa::pointerAnalysis(Function *fp) {
		BasicBlock *bp;
		Instruction *ip;
		//iterating in forward manner
		for(auto bb = fp->begin(); bb != fp->end(); bb++) {
			bp = &(*bb);
			for(auto ii = bp->begin(); ii != bp->end(); ii++) {
				ip = &(*ii);
				auto ipStruct = instAnalysisData[ip];
				auto tINpa = ipStruct.in_PA;
				auto tOUTpa = ipStruct.out_PA;
				
				/*-----------------------calculate IN--------------------------*/

				if(ii == bp->begin()) {
					for(auto itVal : ipStruct.in_LA) {
						if(itVal->getType()->getContainedType(0)->isPointerTy()) {
						if(tINpa.find(itVal) != tINpa.end()) {
							std::set<Value *> pointees;
									
							tINpa.insert(std::pair<Value *, std::pair<std::set<Value*>, bool>>(itVal, std::make_pair(pointees, false)));	//since the variable has not been defined
						}

						bool defnFreePath = false;
						for(auto pred : predecessors(bp)) {
							auto tii = pred->rbegin();
							Instruction *tip = &(*tii);
							auto pred_struct = instAnalysisData[tip];
							if((pred_struct.out_PA.find(itVal) == pred_struct.out_PA.end()) || (pred_struct.out_PA[itVal].first.size() == 0) || (pred_struct.out_PA[itVal].second == true))
								defnFreePath = true;
							
							for(auto itValue : pred_struct.out_PA[itVal].first)
								tINpa[itVal].first.insert(itValue);
							
							
						}
						tINpa[itVal].second = defnFreePath;
						}
					}
				} else {

					for(auto itVal : ipStruct.in_LA) {
						if(itVal->getType()->getContainedType(0)->isPointerTy()) {
						if(tINpa.find(itVal) != tINpa.end()) {
							std::set<Value *> pointees;
									
							tINpa.insert(std::pair<Value *, std::pair<std::set<Value*>, bool>>(itVal, std::make_pair(pointees, false)));	//since the variable has not been defined
						}

						bool defnFreePath = false;
						
						auto tii = ii;
						tii--;
						Instruction *tip = &(*tii);
						auto pred_struct = instAnalysisData[tip];
						if((pred_struct.out_PA.find(itVal) == pred_struct.out_PA.end())|| (pred_struct.out_PA[itVal].first.size() == 0) || (pred_struct.out_PA[itVal].second == true))
							defnFreePath = true;
						
						for(auto itValue : pred_struct.out_PA[itVal].first)
							tINpa[itVal].first.insert(itValue);
						
						tINpa[itVal].second = defnFreePath;
						}
							
					}
						
				}


				/*-----------------------calculate OUT--------------------------*/

				

				for(auto itVal : tINpa) {
					if(ipStruct.out_LA.find(itVal.first) != ipStruct.out_LA.end()) {
						if(tOUTpa.find(itVal.first) == tOUTpa.end()) {
							std::set<Value *> pointees;			
							tOUTpa.insert(std::pair<Value *, std::pair<std::set<Value*>, bool>>(itVal.first, std::make_pair(pointees, false)));	//since the variable has not been defined
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
							

							//find lhs
							int indL = lOp.second;
							lhsSet.insert(lOp.first);
							while(indL > -1 && lhsSet.size()>0) {
								for(auto itVal : lhsSet) {
									if(tINpa.find(itVal) != tINpa.end()) {
										for(auto it : tINpa.find(itVal)->second.first)
											newSet.insert(it);
									}
								}
								lhsSet = newSet;
								newSet.clear();
								indL--;
							}
							int indR;
							//find rhs //do for multiple rhs //only one rhs will be present
							for(auto rIt : rOp) {
								indR = rIt.second;
								rhsSet.insert(rIt.first);
								while(indR > -1 && rhsSet.size()>0) {
									for(auto itVal : rhsSet) {
										if(tINpa.find(itVal) != tINpa.end()) {
											for(auto it : tINpa.find(itVal)->second.first)
												newSet.insert(it);
										}
									}
									rhsSet = newSet;
									newSet.clear();
									indR--;
								}
							}

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
									if(lhsSet.size() == 1 && rhsSet.size() == 1)
										tOUTpa[itVal].first.clear();
									if(rhsSet.size() == 0)
										tOUTpa[itVal].second = true; //no definition obtained
									else //if(ipStruct.out_PA[itVal].second != true)
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


