#ifndef AI_PASSES_H
#define AI_PASSES_H

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"

class AIZeroProp : public llvm::PassInfoMixin<AIZeroProp> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);
};

class AIMem2Reg : public llvm::PassInfoMixin<AIMem2Reg> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);
};

#endif // AI_PASSES_H
