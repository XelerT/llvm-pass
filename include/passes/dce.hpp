#pragma once 

#include "llvm/IR/PassManager.h"

namespace my_llvm
{

class dce_pass : public llvm::PassInfoMixin<dce_pass> 
{
        public:
                llvm::PreservedAnalyses run (llvm::Function &func, 
                                             llvm::FunctionAnalysisManager &func_anman);
};

}