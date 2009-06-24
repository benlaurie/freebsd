; RUN: llvm-as < %s | llc -march=arm -mattr=+neon > %t
; RUN: grep {vclz\\.i8} %t | count 2
; RUN: grep {vclz\\.i16} %t | count 2
; RUN: grep {vclz\\.i32} %t | count 2

define <8 x i8> @vclz8(<8 x i8>* %A) nounwind {
	%tmp1 = load <8 x i8>* %A
	%tmp2 = call <8 x i8> @llvm.arm.neon.vclz.v8i8(<8 x i8> %tmp1)
	ret <8 x i8> %tmp2
}

define <4 x i16> @vclz16(<4 x i16>* %A) nounwind {
	%tmp1 = load <4 x i16>* %A
	%tmp2 = call <4 x i16> @llvm.arm.neon.vclz.v4i16(<4 x i16> %tmp1)
	ret <4 x i16> %tmp2
}

define <2 x i32> @vclz32(<2 x i32>* %A) nounwind {
	%tmp1 = load <2 x i32>* %A
	%tmp2 = call <2 x i32> @llvm.arm.neon.vclz.v2i32(<2 x i32> %tmp1)
	ret <2 x i32> %tmp2
}

define <16 x i8> @vclzQ8(<16 x i8>* %A) nounwind {
	%tmp1 = load <16 x i8>* %A
	%tmp2 = call <16 x i8> @llvm.arm.neon.vclz.v16i8(<16 x i8> %tmp1)
	ret <16 x i8> %tmp2
}

define <8 x i16> @vclzQ16(<8 x i16>* %A) nounwind {
	%tmp1 = load <8 x i16>* %A
	%tmp2 = call <8 x i16> @llvm.arm.neon.vclz.v8i16(<8 x i16> %tmp1)
	ret <8 x i16> %tmp2
}

define <4 x i32> @vclzQ32(<4 x i32>* %A) nounwind {
	%tmp1 = load <4 x i32>* %A
	%tmp2 = call <4 x i32> @llvm.arm.neon.vclz.v4i32(<4 x i32> %tmp1)
	ret <4 x i32> %tmp2
}

declare <8 x i8>  @llvm.arm.neon.vclz.v8i8(<8 x i8>) nounwind readnone
declare <4 x i16> @llvm.arm.neon.vclz.v4i16(<4 x i16>) nounwind readnone
declare <2 x i32> @llvm.arm.neon.vclz.v2i32(<2 x i32>) nounwind readnone

declare <16 x i8> @llvm.arm.neon.vclz.v16i8(<16 x i8>) nounwind readnone
declare <8 x i16> @llvm.arm.neon.vclz.v8i16(<8 x i16>) nounwind readnone
declare <4 x i32> @llvm.arm.neon.vclz.v4i32(<4 x i32>) nounwind readnone
