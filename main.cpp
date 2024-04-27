#include <cstdio>
#include <memory>
#include <iostream>

#include "llvm/Analysis/Passes.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Transforms/Scalar/DCE.h"

#include "ai_passes.hpp"
#include "dce.hpp"

enum AICC_ERROR_CODES
{
  SUCCESS,
  BAD_FILENAME,
};

inline auto read_file (std::FILE* fp)
{
  std::fseek(fp, 0u, SEEK_END);
  const auto size = std::ftell(fp);
  std::fseek(fp, 0u, SEEK_SET);

  std::string s(size + 1, '\0');
  const auto read = std::fread(&s[0], sizeof(char), size, fp);
  assert(read == size);
  
  std::fclose(fp);
  return s;
}

int main (int argc, const char** argv)
{
  if (argc - 1 < 1)
  {
    llvm::errs() << "Please provide a file to optimize" << "\n";
    return BAD_FILENAME;
  }

  auto fp = std::fopen(argv[1], "rb");
  if (nullptr == fp)
  {
    llvm::errs() << "File not found" << "\n";
    return BAD_FILENAME;
  }

  auto ir_src = read_file(fp);

  llvm::LLVMContext ctx;
  llvm::SMDiagnostic Err;
  auto M = llvm::parseAssemblyString(ir_src, Err, ctx);

  llvm::PassBuilder PB;
  llvm::FunctionPassManager FPM;
  llvm::FunctionAnalysisManager FAM;

  FPM.addPass(AIZeroProp());
  FPM.addPass(AIMem2Reg());
  FPM.addPass(my_llvm::dce_pass_t());
  
  PB.registerFunctionAnalyses(FAM);

  for (auto &F: *M)
  {
    FPM.run(F, FAM);
    llvm::errs() << F << "\n";
  }
}

