// RUN: %clang_cc1 %s -verify -pedantic -fsyntax-only

typedef __attribute__((ext_vector_type(2)))  unsigned int uint2;
typedef __attribute__((ext_vector_type(2)))  int int2;

void unsignedCompareOps()
{
  uint2 A, B;
  int2 result = A != B;
}

