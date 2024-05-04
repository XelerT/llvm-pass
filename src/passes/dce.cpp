#include <iostream>
#include <vector>
#include <utility>

#include  "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/AssumeBundleBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/GenericDomTree.h"
#include "llvm/Analysis/PostDominators.h"

#include "dce.hpp"

namespace my_llvm
{

#ifdef DCE_NO_MARK_AND_SWEEP

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
                changed |= delete_instruction(instr, std::back_inserter(next2delete), info);
        }

        return changed;
}

#endif /* DCE_NO_MARK_AND_SWEEP */

bool dce_pass_t::is_jump (llvm::Instruction *instr)
{
        llvm::Instruction *dyn_instr = llvm::dyn_cast<llvm::ReturnInst>(instr);
        if (!dyn_instr)
                dyn_instr = llvm::dyn_cast<llvm::CallBase>(instr);
        else if (!dyn_instr)
                dyn_instr = llvm::dyn_cast<llvm::SwitchInst>(instr);
        else if (!dyn_instr)
                dyn_instr = llvm::dyn_cast<llvm::IndirectBrInst>(instr);

        return dyn_instr;
}

bool dce_pass_t::sweep (llvm::Function &func, llvm::TargetLibraryInfo &info)
{
        bool changed = false;

        for (llvm::Instruction &instr : llvm::make_early_inc_range(llvm::instructions(func)))
                if (instr.hasNoInfs())
                        if (!is_jump(&instr)) {
                                instr.eraseFromParent();
                                changed |= true;
                        }

        return changed;
}

std::vector<llvm::BasicBlock*> 
dce_pass_t::get_rev_dom_frontier 
(llvm::Function &func, llvm::BasicBlock *basic_block) 
{
        std::vector<std::pair<llvm::BasicBlock*, llvm::BasicBlock*>> work_list;
        llvm::DominatorTree dom_tree(*basic_block->getParent());
        llvm::PostDominatorTree post_dom_tree(func);

        std::vector<llvm::BasicBlock*> rdf {};        

        std::unordered_map<llvm::BasicBlock*, llvm::BasicBlock*> visited {};

        work_list.push_back({basic_block, nullptr});
        while (work_list.size()) {
                auto [cur_block, prev_block] = work_list.back();

                if (visited.find(cur_block) != visited.end()) {
                        visited[cur_block];
                        for (auto pred_block : llvm::predecessors(cur_block)) {
                                if (dom_tree.findNearestCommonDominator(pred_block, pred_block) != cur_block) {
                                        rdf.push_back(pred_block);
                                }
                        }
                }

                bool visited_post_dom = false;
                if (auto post_dom_cur = post_dom_tree.findNearestCommonDominator(cur_block, cur_block)) {
                        if (visited.find(post_dom_cur) != visited.end()) {
                                work_list.push_back({post_dom_cur, cur_block});
                                visited_post_dom = true;
                        }
                }

                if (!visited_post_dom) {
                        if (!prev_block) 
                                break;

                        for (auto pdf_block : get_rev_dom_frontier(func, cur_block)) {
                                if (!post_dom_tree.dominates(prev_block, pdf_block)) {
                                        rdf.push_back(pdf_block);
                                        work_list.pop_back();
                                }
                        }
                }
        }
        
        return rdf;
}

bool dce_pass_t::elim_dead_code (llvm::Function &func, llvm::TargetLibraryInfo &info)
{
        std::vector<llvm::Instruction*> next2delete;

        for (llvm::Instruction &instr : llvm::make_early_inc_range(llvm::instructions(func))) {
                if (!llvm::wouldInstructionBeTriviallyDead(&instr, &info)) {
                        instr.setHasNoInfs(false);
                        next2delete.push_back(&instr);
                } else {
                        instr.setHasNoInfs(true);
                }
        }

        while (!next2delete.empty()) {
                auto instr = next2delete.back();
                next2delete.pop_back();
                for (auto& oper : instr->operands()) {
                        auto *operand_insrt = llvm::dyn_cast<llvm::Instruction>(oper);
                        if (operand_insrt && operand_insrt->hasNoInfs()) {
                                operand_insrt->setHasNoInfs(false);
                                next2delete.push_back(operand_insrt);
                        }
                }

                for (auto& block : get_rev_dom_frontier(func, instr->getParent())) {
                        auto& rev_dom_instr = block->back();
                        rev_dom_instr.setHasNoInfs(false);
                        next2delete.push_back(&rev_dom_instr);
                }
        }

        return sweep(func, info);
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
