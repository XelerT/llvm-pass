#pragma once 

#include "llvm/IR/PassManager.h"

namespace my_llvm
{

class dce_pass_t : public llvm::PassInfoMixin<dce_pass_t> 
{
        public:
                llvm::PreservedAnalyses run (llvm::Function &func, 
                                             llvm::FunctionAnalysisManager &func_an_man);
};

}