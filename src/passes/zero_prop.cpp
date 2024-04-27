#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "ai_passes.hpp"

using llvm::dyn_cast;

llvm::PreservedAnalyses
AIZeroProp::run(llvm::Function &F,
		        llvm::FunctionAnalysisManager &AM)
{
	for (auto &BB : F)
	{
		for (auto &I : BB)
		{
			auto *BinOp = dyn_cast<llvm::BinaryOperator>(&I);
			if (BinOp == nullptr) continue;
			if (BinOp->getOpcode() != llvm::Instruction::Mul) continue;

			bool is_zero = false;
			for (int i = 0; i < I.getNumOperands(); ++i)
			{
				auto *ConstOpnd = dyn_cast<llvm::ConstantInt>(I.getOperand(i));
				if (ConstOpnd == nullptr) continue;
				if (!ConstOpnd->isZero()) continue;

				is_zero = true;
				break;		   
			}	   
			if (is_zero)
			{	
				I.replaceAllUsesWith(llvm::Constant::getNullValue(I.getType()));
			}
		}
	}

	return llvm::PreservedAnalyses::all();
}
