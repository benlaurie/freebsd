; RUN: llc < %s -code-model=small  | FileCheck -check-prefix CHECK-SMALL %s
; RUN: llc < %s -code-model=kernel | FileCheck -check-prefix CHECK-KERNEL %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-unknown-linux-gnu"
@data = external global [0 x i32]		; <[0 x i32]*> [#uses=5]

define i32 @foo() nounwind readonly {
entry:
; CHECK-SMALL:  foo:
; CHECK-SMALL:   movl data(%rip), %eax
; CHECK-KERNEL: foo:
; CHECK-KERNEL:  movl data, %eax
	%0 = load i32* getelementptr ([0 x i32]* @data, i64 0, i64 0), align 4		; <i32> [#uses=1]
	ret i32 %0
}

define i32 @foo2() nounwind readonly {
entry:
; CHECK-SMALL:  foo2:
; CHECK-SMALL:   movl data+40(%rip), %eax
; CHECK-KERNEL: foo2:
; CHECK-KERNEL:  movl data+40, %eax
	%0 = load i32* getelementptr ([0 x i32]* @data, i32 0, i64 10), align 4		; <i32> [#uses=1]
	ret i32 %0
}

define i32 @foo3() nounwind readonly {
entry:
; CHECK-SMALL:  foo3:
; CHECK-SMALL:   movl data-40(%rip), %eax
; CHECK-KERNEL: foo3:
; CHECK-KERNEL:  movq $-40, %rax
	%0 = load i32* getelementptr ([0 x i32]* @data, i32 0, i64 -10), align 4		; <i32> [#uses=1]
	ret i32 %0
}

define i32 @foo4() nounwind readonly {
entry:
; FIXME: We really can use movabsl here!
; CHECK-SMALL:  foo4:
; CHECK-SMALL:   movl $16777216, %eax
; CHECK-SMALL:   movl data(%rax), %eax
; CHECK-KERNEL: foo4:
; CHECK-KERNEL:  movl data+16777216, %eax
	%0 = load i32* getelementptr ([0 x i32]* @data, i32 0, i64 4194304), align 4		; <i32> [#uses=1]
	ret i32 %0
}

define i32 @foo1() nounwind readonly {
entry:
; CHECK-SMALL:  foo1:
; CHECK-SMALL:   movl data+16777212(%rip), %eax
; CHECK-KERNEL: foo1:
; CHECK-KERNEL:  movl data+16777212, %eax
        %0 = load i32* getelementptr ([0 x i32]* @data, i32 0, i64 4194303), align 4            ; <i32> [#uses=1]
        ret i32 %0
}
define i32 @foo5() nounwind readonly {
entry:
; CHECK-SMALL:  foo5:
; CHECK-SMALL:   movl data-16777216(%rip), %eax
; CHECK-KERNEL: foo5:
; CHECK-KERNEL:  movq $-16777216, %rax
	%0 = load i32* getelementptr ([0 x i32]* @data, i32 0, i64 -4194304), align 4		; <i32> [#uses=1]
	ret i32 %0
}
