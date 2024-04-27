#pragma once 

#include "llvm/IR/PassManager.h"

namespace my_llvm
{

class dce_pass_t : public llvm::PassInfoMixin<dce_pass_t> 
{
        public:
                llvm::PreservedAnalyses run (llvm::Function &func, 
                                             llvm::FunctionAnalysisManager &func_an_man);
        private:
                bool delete_instruction
                (llvm::Instruction *instr, std::set<llvm::Instruction*> &next2delete, 
                 llvm::TargetLibraryInfo &info);

                bool elim_dead_code 
                (llvm::Function &func, llvm::TargetLibraryInfo &info);
};

}