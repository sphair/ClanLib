
#pragma once

#if defined(min)
#define llvm_min_bug min
#undef min
#endif
#if defined(max)
#define llvm_max_bug max
#undef max
#endif

#pragma warning(disable: 4146) // warning C4146: unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4624) // warning C4624: 'llvm::AugmentedUse' : destructor could not be generated because a base class destructor is inaccessible
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
#pragma warning(disable: 4800) // warning C4800: 'const unsigned int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4996) // warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_Sclan::SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'

#include <llvm/DerivedTypes.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/PassManager.h>
//#include <llvm/LinkAllPasses.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Intrinsics.h>
#include <llvm/CodeGen/AsmPrinter.h>

#if defined(llvm_min_bug)
#define min llvm_min_bug
#undef llvm_min_bug
#endif
#if defined(llvm_max_bug)
#define max llvm_max_bug
#undef llvm_max_bug
#endif
