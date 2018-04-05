#include <string>
#include <clang/AST/AST.h>
#include <clang/AST/ASTConsumer.h>
#include "clang/Analysis/Analyses/FormatString.h"
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/raw_ostream.h>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory cs_clang_catagory_("Debugging");

class ReturnStmtHandler: public MatchFinder::MatchCallback {
    public:
        ReturnStmtHandler(clang::Rewriter & Rewrite) : Rewrite(Rewrite) {
        }

        virtual void run(const MatchFinder::MatchResult &Result) {
            const clang::ReturnStmt * retStmt = Result.Nodes.getNodeAs < clang::ReturnStmt > ("returnStmt");

            if (retStmt) {
                Rewrite.InsertText(retStmt->getLocStart(), "// log here\n", true, true);
            }
        }

    private:
        clang::Rewriter &Rewrite;
};

class csASTConsumer: public clang::ASTConsumer {
    public:
        csASTConsumer(clang::Rewriter & R) : HandleRet(R) {
            Matcher.addMatcher(returnStmt().bind("returnStmt"), &HandleRet);
        }

        void HandleTranslationUnit(clang::ASTContext &Context) override {
            Matcher.matchAST(Context);
        }

    private:
        ReturnStmtHandler HandleRet;
        MatchFinder       Matcher;
};

class csFrontendAction: public clang::ASTFrontendAction {
    public:
        csFrontendAction() {
        }

        void EndSourceFileAction() override {
            TheRewriter.getEditBuffer(TheRewriter.getSourceMgr().getMainFileID()).write(llvm::outs());
        }

        std::unique_ptr < clang::ASTConsumer > CreateASTConsumer(clang::CompilerInstance &CI, StringRef file) override {
            TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
            return llvm::make_unique < csASTConsumer > (TheRewriter);
        }
    private:
        clang::Rewriter TheRewriter;
};

int main(int argc, const char ** argv) {
    /* clang::tooling CommonOptionParser */
    CommonOptionsParser op_parser(argc, argv, cs_clang_catagory_);
    /* clang::tooling ClangTool, returns ref to loaded compilations db */
    ClangTool cl_tool(op_parser.getCompilations(), op_parser.getSourcePathList());

    return cl_tool.run(newFrontendActionFactory < csFrontendAction > ().get());
}
