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
#include "ui.hpp"

int main (int argc, const char** argv)
{
        if (argc < 2)
        {
                llvm::errs() << "No IR file to optimize!" << "\n";
                return -1;
        }

        std::string ifile_path {argv[1]};

        auto ir_src = read_file(ifile_path);

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

        return 0;
}

