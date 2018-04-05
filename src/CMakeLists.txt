#llvm_map_components_to_libnames(llvm_libs asmparser bitreader support mc option)
add_clang_executable(clang-insert-logs cs_clang_logger.cpp)

# clang++-5.0 -L/usr/lib/llvm-5.0/lib -I/usr/lib/llvm-5.0/include -std=c++0x
# -Werror=date-time -std=c++11 -ffunction-sections -fdata-sections -O2 -DNDEBUG
# -g1  -fno-exceptions -DLLVM_BUILD_GLOBAL_ISEL -D_GNU_SOURCE
# -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
# ../src/cs_clang_logger.cpp -lclangTooling -lclangFrontendTool -lclangFrontend
# -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema
# -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers
# -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewrite
# -lclangRewriteFrontend -lclangEdit -lclangAST -lclangASTMatchers -lclangLex
# -lclangBasic -lclang -o tool -lLLVM-5.0

target_link_libraries(clang-insert-logs
    clangTooling
    clangFrontendTool
    clangFrontend
    clangDriver
    clangSerialization
    clangCodeGen
    clangParse
    clangSema
    clangStaticAnalyzerFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangAnalysis
    clangARCMigrate
    clangRewrite
    clangRewriteFrontend
    clangEdit
    clangAST
    clangASTMatchers
    clangLex
    clangBasic
    clang
    LLVM-5.0
)
