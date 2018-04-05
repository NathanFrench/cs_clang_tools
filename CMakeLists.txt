project                (cs_clang_plugins)
cmake_minimum_required(VERSION 3.9)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")
list(APPEND CMAKE_MODULE_PATH "${LLVM_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -O0 -g -std=c++0x -std=c++11")

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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# LLVM_DIR must be set to the prefix of /share/llvm/cmake via commandline
find_package(LLVM REQUIRED CONFIG HINTS ${LLVM_DIR})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# We incorporate the CMake features provided by LLVM:
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
include(AddClang)

#option(LLVM_ENABLE_CXX1Y   "Enable C++1Y" ON)
#option(LLVM_INCLUDE_TOOLS  "Generate build targets for the LLVM tools." ON)
#option(LLVM_ENABLE_PLUGINS "Generate build targets for LLVM plugins." ON)
#option(LLVM_BUILD_TOOLS    "Build the LLVM tools. If OFF, just generate build targets." ON)

# LLVM_DIR must be set to the prefix of /share/llvm/cmake via commandline
find_package(Clang REQUIRED)

message("LLVM STATUS:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}")

# Now set the LLVM header and library paths:
include_directories (${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
link_directories    (${LLVM_LIBRARY_DIRS})
add_definitions     (${LLVM_DEFINITIONS})

############## FINAL PROJECT CONFIG #################

# And the project header and library paths
include_directories (${CMAKE_SOURCE_DIR}/include)
link_directories    (${LIBRARY_OUTPUT_PATH})
set                 (CMAKE_TEMP_LIBRARY_PATH "${PROJECT_BINARY_DIR}/lib")

add_subdirectory(src)
