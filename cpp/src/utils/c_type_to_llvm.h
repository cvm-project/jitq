#ifndef UTILS_C_TYPE_TO_LLVM_H
#define UTILS_C_TYPE_TO_LLVM_H

//
// Created by sabir on 29.08.17.
//

#include <string>

#include <cassert>

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

llvm::Type *c_type_to_llvm(const std::string type_,
                           llvm::LLVMContext *context) {
    assert(context != nullptr);
    if (type_ == "long") return llvm::Type::getInt64Ty(*context);
    if (type_ == "int") return llvm::Type::getInt32Ty(*context);
    if (type_ == "bool") return llvm::Type::getInt1Ty(*context);
    if (type_ == "float") return llvm::Type::getFloatTy(*context);
    if (type_ == "double") return llvm::Type::getDoubleTy(*context);
    return llvm::Type::getDoubleTy(*context);
}

#endif
