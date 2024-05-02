#include <iostream>
#include <vector>

#include  "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/AssumeBundleBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Attributes.h"

#include "dce.hpp"

namespace my_llvm
{

bool dce_pass_t::elim_dead_code (llvm::Function &func, llvm::TargetLibraryInfo &info)
{
        bool changed = false;
        std::vector<llvm::Instruction*> next2delete;

        for (llvm::Instruction &instr : llvm::make_early_inc_range(llvm::instructions(func))) {
                if (!instr.hasNoInfs())
                        changed |= delete_instruction(&instr, std::back_inserter(next2delete), info);
        }

        while (!next2delete.empty()) {
                auto instr = next2delete.back();
                next2delete.pop_back();
                changed |= delete_instruction(instr,  std::back_inserter(next2delete), info);
        }

        return changed;
}

llvm::PreservedAnalyses dce_pass_t::run (llvm::Function &func, 
                                         llvm::FunctionAnalysisManager &func_an_man)
{
        if (elim_dead_code(func, func_an_man.getResult<llvm::TargetLibraryAnalysis>(func))) {
                llvm::PreservedAnalyses pres_anal {};
                pres_anal.preserveSet<llvm::CFGAnalyses>();
                return pres_anal;
        }

        return llvm::PreservedAnalyses::all();
}

}
