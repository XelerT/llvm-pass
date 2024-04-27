#include <set>
#include  "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/AssumeBundleBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Local.h"

#include "dce.hpp"

namespace my_llvm
{

bool delete_instruction (llvm::Instruction       *instr, 
                std::set<llvm::Instruction*>     &next2delete, 
                         llvm::TargetLibraryInfo &info)
{
        if (llvm::isInstructionTriviallyDead(instr, &info)) {
                unsigned char end_indx = instr->getNumOperands();
                for (unsigned char i = 0; i != end_indx; ++i) {
                        llvm::Value *operand = instr->getOperand(i);
                        instr->setOperand(i, nullptr);
                        
                        if (!operand->use_empty() || instr == operand) // check if we need to delete something connected
                                continue;
                
                        if (auto operand_instr = llvm::dyn_cast<llvm::Instruction>(operand)) // add dead instructions to delete them further
                                if (llvm::isInstructionTriviallyDead(operand_instr, &info))
                                        next2delete.insert(operand_instr);
                }
                instr->eraseFromParent();
                return true;
        }
        return false;
} 

bool elim_dead_code (llvm::Function &func, llvm::TargetLibraryInfo &info)
{
        bool changed = false;
        std::set<llvm::Instruction*> next2delete;

        for (llvm::Instruction &instr : llvm::make_early_inc_range(llvm::instructions(func))) {
                if (!next2delete.count(&instr))
                        changed |= delete_instruction(&instr, next2delete, info);
        }

        while (!next2delete.empty()) {
                auto it = next2delete.end();
                auto instr = *it;
                next2delete.erase(it);
                changed |= delete_instruction(instr, next2delete, info);
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
