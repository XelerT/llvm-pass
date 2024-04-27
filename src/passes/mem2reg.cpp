#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Dominators.h"

#include "ai_passes.hpp"

using llvm::dyn_cast;
using llvm::isa;

llvm::PreservedAnalyses AIMem2Reg::run (llvm::Function &F, llvm::FunctionAnalysisManager &AM)
{
    auto& DT = AM.getResult<llvm::DominatorTreeAnalysis>(F);

	for (auto &BB : F)
	{
		for (auto &I : BB)
		{
            auto *SI = dyn_cast<llvm::StoreInst>(&I);
            if (nullptr == SI) continue;

            auto *AI = dyn_cast<llvm::AllocaInst>(SI->getPointerOperand());

            bool matches = true;
            std::vector<llvm::Value*> users(AI->user_begin(), AI->user_end());

            for (auto user: users) 
            {
                if (SI == user) continue;
                if (isa<llvm::LoadInst>(user)) continue;
                auto def_instr = dyn_cast<llvm::Instruction>(user);
                if (DT.dominates(&I, def_instr)) continue;
                matches = false;
            }

            if (!matches) continue;

            for (auto user: users) 
            {
                if (!isa<llvm::LoadInst>(user)) continue;
                user->replaceAllUsesWith(SI->getValueOperand());
            }
		}
	}

	return llvm::PreservedAnalyses::all();
}
