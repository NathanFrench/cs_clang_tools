#include <string>
#include <clang/AST/AST.h>
#include <clang/AST/ASTConsumer.h>
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

class cs_retstmt_handler: public MatchFinder::MatchCallback {
    public:
        cs_retstmt_handler(Rewriter & Rewrite) : Rewrite(Rewrite) {
        }

        virtual void run(const MatchFinder::MatchResult& Result) {
            const ReturnStmt * retStmt =
                Result.Nodes.getNodeAs < ReturnStmt > ("returnStmt");

            if (retStmt) {
                Rewrite.InsertText(retStmt->getLocStart(),
                                   "// log here\n", true, true);
            }
        }

    private:
        Rewriter &Rewrite;
};

class cs_declref_handler: public MatchFinder::MatchCallback {
    public:
        cs_declref_handler(Rewriter & Rewrite) : Rewrite(Rewrite) {
        }

        virtual void run(const MatchFinder::MatchResult& Result) {
            const FunctionDecl * fDecl =
                Result.Nodes.getNodeAs < FunctionDecl > ("functionDecl");

            if (fDecl) {
                Rewrite.InsertText(fDecl->getLocStart(),
                                   "// log entry here\n", false, false);
            }
        }

    private:
        Rewriter &Rewrite;
};

class cs_ast_consumer: public ASTConsumer {
    public:
        cs_ast_consumer(Rewriter & R) : HandleRet(R), HandleEntry(R) {
            /* Match any exit points. */
            Matcher.addMatcher(returnStmt().bind("returnStmt"), &HandleRet);
            /* Match any executable statements with code. */
            Matcher.addMatcher(functionDecl(isDefinition()).bind("functionDecl"), &HandleEntry);
        }

        void HandleTranslationUnit(ASTContext& Context) override {
            Matcher.matchAST(Context);
        }

    private:
        cs_retstmt_handler HandleRet;
        cs_declref_handler HandleEntry;
        MatchFinder        Matcher;
};

class cs_frontend_action: public ASTFrontendAction {
    public:
        cs_frontend_action() {
        }

        void EndSourceFileAction() override {
            _rewriter.getEditBuffer(
                _rewriter.getSourceMgr()
                .getMainFileID()).write(llvm::outs());
        }

        std::unique_ptr < ASTConsumer >
        CreateASTConsumer(CompilerInstance & CI, StringRef file) override {
            _rewriter.setSourceMgr(
                CI.getSourceManager(),
                CI.getLangOpts());

            return llvm::make_unique < cs_ast_consumer > (_rewriter);
        }
    private:
        Rewriter _rewriter;
};

int
main(int argc, const char ** argv) {
    /* clang::tooling CommonOptionParser */
    CommonOptionsParser op_parser(
        argc,
        argv,
        cs_clang_catagory_);

    /* clang::tooling ClangTool, returns ref to loaded compilations db */
    ClangTool cl_tool(
        op_parser.getCompilations(),
        op_parser.getSourcePathList());

    return cl_tool.run(newFrontendActionFactory < cs_frontend_action > ().get());
}
