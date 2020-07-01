//liveness based flow sensitive pointer analysis - LFPA
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include <bits/stdc++.h>
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constant.h"
#include <cxxabi.h>
#include <cassert>

#include "DfVal.cpp"
#include "HelperClass.cpp"
#include "OpdData.cpp"

using namespace llvm;

 
namespace {
	class Lfpa : public HelperClass {
		private :
			using pointerPointeeMap = std::map<Value *, std::set<Value *>>;
			using operand = std::pair<Value *, int>;
			using operandSet = std::set<operand>;
			std::map<Instruction *, DfVal> instAnalysisData;
			std::map<Instruction *, OpdData> instOperandData;
			std::map<Instruction *, OpdData> pointsToTest;
			Constant *nullValue;
			
		public :
			void run(Function *);
			void initContext(Function *);
			bool strongLivenessAnalysis(Function *);
			bool pointerAnalysis(Function *);
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
		
/*--------------------Perform Analysis----------------------*/
		//int itCnt = 0;
		bool changed,cL,cP;
		do {
			changed = false;
			cL = false;
			cP = false;
//errs()<<"iteration"<<itCnt<<"\n";
//if strongLivenessAnalysis is true but pointerAnalysis is false then changed is false
			cL = strongLivenessAnalysis(fp);
			cP = pointerAnalysis(fp);
			if(cL || cP) {
				changed = true;
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
			auto A_in = ipStruct.getAin();
			auto opdObj = test.second;
			
			OpdData::InstType type, ptsTo;
			operand opd1, opd2;
			
			auto opdSet = opdObj.getFuncArg();
			type = opdObj.getInstType();
			opd1 = opdSet[0];
			opd2 = opdSet[1];

			int opd1Ind = opd1.second;
			int opd2Ind = opd2.second;
			auto opd1Set = ipStruct.getDefRefSet(opd1.first, A_in, &opd1Ind);
			auto opd2Set = ipStruct.getDefRefSet(opd2.first, A_in, &opd2Ind);
			
			if(opd1Ind != -1 || opd2Ind != -1) {
				errs()<<"No variables found!\n";
				continue;
			}

			auto L_in = ipStruct.getLin();

			opdObj.printOperand(opd1);
			if(ipStruct.isLiveIn(opd1Set, L_in)) {
				ptsTo = opdObj.whetherPointsTo(opd1Set, opd2Set, A_in);
			} else {
				errs()<<" not live\n";
				continue;
			}

			if(type == OpdData::MayPointsTo) {

				
				errs()<<" may points to ";
				opdObj.printOperand(opd2);
				if(ptsTo != OpdData::DoesNotPointsTo)
					errs()<<" :Yes\n";
				else
					errs()<<" :No \n";

			} else if(type == OpdData::MustPointsTo) {

				errs()<<" must points to ";
				opdObj.printOperand(opd2);
				
				if(ptsTo == OpdData::MustPointsTo)
					errs()<<" :Yes\n";
				else
					errs()<<" :No\n";

			} else {

				errs()<<" does not points to ";
				opdObj.printOperand(opd2);

				if(ptsTo == OpdData::DoesNotPointsTo)	
					errs()<<" :Yes\n";
				else
					errs()<<" :No\n";

			}
		}
	}//Lfpa::printPointsToTestsAnswers

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
					for(auto itVal : suc_i_struct.getLin())
						tL_out.insert(itVal);
				}
				
				/*--------------------------calculate IN-----------------------------*/
				
				tL_in = tL_out;
				if(isa<StoreInst>(ip)) {
					auto opdObj = instOperandData[ip];
					auto lOp = opdObj.getLHS();
					auto rOp = opdObj.getRHS();
					auto A_in = ipStruct.getAin();
					
					std::set<Value *> lhsSet;
					int ind = lOp.second;
					Value *v = lOp.first;
					lhsSet = ipStruct.getMayDefVariables(A_in, lOp.first, &tL_in,&ind);

					if(ind == -1) {
						if(ipStruct.isLiveIn(lhsSet,tL_out)) {//strong liveness
							if(lhsSet.size() == 1) { //must-kill
								auto killVal = lhsSet.begin();
								tL_in.erase(*killVal);
							}
							tL_in = ipStruct.getMayRefVariables(A_in,rOp,tL_in,false);
						}
					}

				} else if(isa<ICmpInst>(ip) || isa<ReturnInst>(ip)) {
					auto opdObj = instOperandData[ip];
					auto A_in = ipStruct.getAin();
					tL_in = ipStruct.getMayRefVariables(A_in,opdObj.getUse(),tL_in,true);
				} 



				/*errs()<<"-----------------------------inst"<<ip<<"--------------------------\n";
				errs()<<"IN la after : \n";
				ipStruct.printL(tL_in);
			
				errs()<<"OUT la after: \n";
				ipStruct.printL(tL_out);*/

				
				if(tL_in != ipStruct.getLin() || tL_out != ipStruct.getLout()) {
					changed = true;
					ipStruct.setLin(tL_in);
					ipStruct.setLout(tL_out);
					instAnalysisData[ip] = ipStruct;
				}

			}
		}
		

		

		return changed;
	}//Lfpa::strongLivenessAnalysis

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
				auto Lout = ipStruct.getLout();
				auto Lin = ipStruct.getLin();
				
				/*-----------------------calculate IN--------------------------*/
				auto predInstSet = getPredecessors(ii,bp);
				for(auto itVal : Lin) {
					if(itVal->getType()->getContainedType(0)->isPointerTy()) {
						if(tA_in.find(itVal) != tA_in.end()) {
							std::set<Value *> pointees;		
							tA_in.insert(std::pair<Value *, std::set<Value*>>(itVal, pointees));	
						}
						if(predInstSet.size()==0) {
							if(isa<GlobalVariable>(itVal)) {
								GlobalVariable *it = (GlobalVariable *)(itVal);
								if(it->hasInitializer()) {
									auto opd = ((Instruction *)(itVal))->getOperand(0);
									if(((Constant *)(opd))->isNullValue()) { 
										tA_in[itVal].insert(nullValue);
									} else {
										tA_in[itVal].insert(opd);
									}
								}
							} else 
								tA_in[itVal].insert(nullValue);
						} else {
							for(auto predIp : predInstSet) {
								auto pred_struct = instAnalysisData[predIp];
								if(pred_struct.whetherDefnFreePathExists(itVal))
									tA_in[itVal].insert(nullValue);
								else {
									auto A = pred_struct.getAout();
									for(auto itValue : A[itVal])
										tA_in[itVal].insert(itValue);
								}		
							}
						}
					}
				}

				/*-----------------------calculate OUT--------------------------*/

				

				for(auto itVal : tA_in) {
					if(Lout.find(itVal.first) != Lout.end()) {
						if(tA_out.find(itVal.first) == tA_out.end()) {
							std::set<Value *> pointees;			
							tA_out.insert(std::pair<Value *, std::set<Value*>>(itVal.first, pointees));	
						}
						for(auto itValue : tA_in[itVal.first])
							tA_out[itVal.first].insert(itValue);
					}
				}



				if(isa<StoreInst>(ip)) {
					if(ip->getOperand(1)->getType()->isPointerTy()) {
						if(ip->getOperand(1)->getType()->getContainedType(0)->isPointerTy()) {
							std::set<Value *> lhsSet, rhsSet,newSet;
							auto opdObj = instOperandData[ip];
							auto rOp = opdObj.getRHS();
							auto lOp = opdObj.getLHS();
							int indL,indR;
							indL = lOp.second;
							indR = rOp.begin()->second;
							

							lhsSet = ipStruct.getDefRefSet(lOp.first, tA_in, &indL); //def set
							rhsSet = ipStruct.getDefRefSet(rOp.begin()->first, tA_in, &indR); //pointee set
							
							if(indR == -1 && indL == -1) {

								for(auto itVal : lhsSet) {
									if(Lout.find(itVal) != Lout.end()) {
										if(tA_out.find(itVal) == tA_out.end() && rhsSet.size() != 0) {
											std::set<Value *> pointees;
											tA_out.insert(std::pair<Value *,std::set<Value*>>
												      (itVal,pointees));	
										}
										if(lhsSet.size() == 1)
											tA_out[itVal].clear();//must update //kill

										if(rhsSet.size() == 0)
											tA_out[itVal].insert(nullValue);
										else {
											for(auto itVals : rhsSet) //def_n X pointee_n
												tA_out[itVal].insert(itVals);
										}
									}
								}
							}
						}
					}
				}
				

				if(tA_in != ipStruct.getAin() || tA_out != ipStruct.getAout()) {
					changed = true;
					ipStruct.setAin(tA_in);
					ipStruct.setAout(tA_out);
					instAnalysisData[ip] = ipStruct;
				}


/*errs()<<"-----------------------------inst"<<ip<<"--------------------------\n";
				errs()<<"IN pa :\n";
				ipStruct.printA(tA_in);

				errs()<<"OUT pa :\n";
				ipStruct.printA(tA_out);*/
			}
		}
		return changed;
	}//Lfpa::pointerAnalysis

	/*
	* initContext - figures out the function calls for Points to tests
	*		sets the LHS and RHS for store instructions
	*		initializes structure lfpa for each inst
	*		creates a null operand
	*/
	void Lfpa::initContext(Function *fp) {
		Type *tp = fp->getType();
		nullValue = Constant::getNullValue(tp);

		for(auto bb = fp->begin(); bb != fp->end(); bb++) {
			BasicBlock *bp = &(*bb);
			for(auto ii = bp->begin(); ii != bp->end(); ii++) {
				Instruction *ip = &(*ii);
				
				DfVal dfValObj;
				instAnalysisData.insert(std::pair<Instruction *, DfVal>(ip, dfValObj));

				if(isa<StoreInst>(ip)) {
					OpdData::InstType type= OpdData::Store;
					OpdData opdObj(type);
					
					auto lhsSet = opdObj.findOperands(ip->getOperand(1),false);
					opdObj.setLHS(*(lhsSet.begin()));
					auto rhsSet = opdObj.findOperands(ip->getOperand(0),false);
					opdObj.setRHS(rhsSet);
					instOperandData.insert(std::pair<Instruction *, OpdData>(ip, opdObj));

				} else if(isa<CallInst>(ip)) {

					Function *calledFp = cast<CallInst>(ip)->getCalledFunction();
					std::string originalName = getOriginalName(calledFp);

					OpdData::InstType type= OpdData::Call;
					if(originalName == "mayPointsTo") {
						type = OpdData::MayPointsTo;
					} else if (originalName == "mustPointsTo") {
						type = OpdData::MustPointsTo;
					} else if (originalName == "doesNotPointsTo") {
						type = OpdData::DoesNotPointsTo;
					}
					OpdData opdObj(type);
					for(int i=0, numOp = ip->getNumOperands(); i< numOp; i++) {
						Value *op = ip->getOperand(i);
						operandSet opdSet;
						if(!isa<ConstantData>(op)) {
							opdSet = opdObj.findOperands(op, false);
						} else {
							opdSet.insert(std::pair<Value *, int>(op,-1));
						}
						opdObj.setFuncArg(opdSet);
					}
					if(type == OpdData::Call) {
						instOperandData.insert(std::pair<Instruction *, OpdData>(ip, opdObj));
					} else {
						pointsToTest.insert(std::pair<Instruction *, OpdData>(ip, opdObj));
					}

				} else if(isa<ICmpInst>(ip) || isa<ReturnInst>(ip)) {
					OpdData::InstType type= OpdData::Use;
					OpdData opdObj(type);
					for(int i=0, numOp = ip->getNumOperands(); i < numOp; i++) {
						Value *op = ip->getOperand(i);
						if(!isa<ConstantData>(op)) {
							auto opdSet = opdObj.findOperands(op,true);
							opdObj.setUse(opdSet);
						}
					}
					instOperandData.insert(std::pair<Instruction *, OpdData>(ip, opdObj));
					
				}
			}
		}
	}//Lfpa::initContext

	class LfpaPass : public FunctionPass {
		public:
			static char ID;
			LfpaPass() : FunctionPass(ID) {}
			virtual bool runOnFunction(Function &F) override {
				errs()<<"------------------------------------------------------------------------------\n";
				errs()<<"LFPA analysis for "<<F.getName()<<"\n";
				Lfpa lfpaObj;//create the object outside if inter-procedural
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


