# $FreeBSD$

.include <bsd.own.mk>

SUBDIR=	clang clang-tblgen tblgen

.if ${MK_CLANG_EXTRAS} != "no"
SUBDIR+=bugpoint \
	llc \
	lli \
	llvm-ar \
	llvm-as \
	llvm-bcanalyzer \
	llvm-diff \
	llvm-dis \
	llvm-extract \
	llvm-ld \
	llvm-link \
	llvm-mc \
	llvm-nm \
	llvm-objdump \
	llvm-prof \
	llvm-ranlib \
	llvm-rtdyld \
	llvm-stub \
	macho-dump \
	opt
.endif

.include <bsd.subdir.mk>
