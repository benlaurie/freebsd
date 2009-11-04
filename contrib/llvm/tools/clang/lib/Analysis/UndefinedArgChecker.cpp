//===--- UndefinedArgChecker.h - Undefined arguments checker ----*- C++ -*--==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This defines BadCallChecker, a builtin check in GRExprEngine that performs
// checks for undefined arguments.
//
//===----------------------------------------------------------------------===//

#include "clang/Analysis/PathSensitive/Checkers/UndefinedArgChecker.h"
#include "clang/Analysis/PathSensitive/BugReporter.h"

using namespace clang;

void *UndefinedArgChecker::getTag() {
  static int x = 0;
  return &x;
}

void UndefinedArgChecker::PreVisitCallExpr(CheckerContext &C, 
                                           const CallExpr *CE){
  for (CallExpr::const_arg_iterator I = CE->arg_begin(), E = CE->arg_end();
       I != E; ++I) {
    if (C.getState()->getSVal(*I).isUndef()) {
      if (ExplodedNode *N = C.GenerateNode(CE, true)) {
        if (!BT)
          BT = new BugType("Pass-by-value argument in function call is "
                           "undefined", "Logic error");
        // Generate a report for this bug.
        EnhancedBugReport *R = new EnhancedBugReport(*BT, BT->getName().c_str(),
                                                     N);
        R->addRange((*I)->getSourceRange());
        R->addVisitorCreator(bugreporter::registerTrackNullOrUndefValue, *I);
        C.EmitReport(R);
      }
    }
  }
}
