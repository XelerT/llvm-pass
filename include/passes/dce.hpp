#pragma once 

#include "llvm/Transforms/Utils/Local.h"
#include "llvm/IR/PassManager.h"

namespace my_llvm
{

class dce_pass_t : public llvm::PassInfoMixin<dce_pass_t> 
{
        public:
                llvm::PreservedAnalyses run (llvm::Function &func, 
                                             llvm::FunctionAnalysisManager &func_an_man);
        private:
                template <typename Container>
                bool delete_instruction
                (llvm::Instruction *instr, std::back_insert_iterator<Container> next2delete, 
                 llvm::TargetLibraryInfo &info);

                bool elim_dead_code 
                (llvm::Function &func, llvm::TargetLibraryInfo &info);

                std::vector<llvm::BasicBlock*> 
                get_rev_dom_frontier 
                (llvm::Function &func, llvm::BasicBlock *basic_block);

                bool sweep (llvm::Function &func, llvm::TargetLibraryInfo &info); 
                bool is_jump (llvm::Instruction *instr);
};

template <typename Container>
inline bool dce_pass_t::delete_instruction 
(llvm::Instruction *instr, std::back_insert_iterator<Container> next2delete, 
 llvm::TargetLibraryInfo &info)
{
        if (llvm::isInstructionTriviallyDead(instr, &info)) {
                unsigned char end_indx = instr->getNumOperands();
                for (unsigned char i = 0; i != end_indx; ++i) {
                        llvm::Value *operand = instr->getOperand(i);
                        instr->setOperand(i, nullptr);
                        
                        if (!operand->use_empty() || instr == operand) // check if we need to delete uses
                                continue;
                
                        if (auto operand_instr = llvm::dyn_cast<llvm::Instruction>(operand)) // add dead instructions to delete them further
                                if (llvm::isInstructionTriviallyDead(operand_instr, &info)) {
                                        operand_instr->setHasNoInfs(true);
                                        next2delete = operand_instr;
                                }
                }
                instr->eraseFromParent();
                return true;
        }
        return false;
} 

}