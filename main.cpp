#include <cstdio>
#include <memory>
#include <iostream>

#include "llvm/Analysis/Passes.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"

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
        FPM.addPass(llvm::PromotePass());
        FPM.addPass(my_llvm::dce_pass_t());
        
        PB.registerFunctionAnalyses(FAM);

        for (auto &F: *M)
        {
                FPM.run(F, FAM);
                llvm::errs() << F << "\n";
        }

        return 0;
}
