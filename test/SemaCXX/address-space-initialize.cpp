// RUN: %clang_cc1 -fsyntax-only -verify %s

__attribute__((address_space(42)))
const float withc = 1.0f;

__attribute__((address_space(42)))
volatile float withv = 1.0f;

__attribute__((address_space(42)))
float nocv = 1.0f;

__attribute__((address_space(42)))
float nocv_array[10] = { 1.0f };

__attribute__((address_space(42)))
int nocv_iarray[10] = { 4 };


__attribute__((address_space(9999)))
int* as_ptr = nocv_iarray; // expected-error{{cannot initialize a variable of type '__attribute__((address_space(9999))) int *' with an lvalue of type '__attribute__((address_space(42))) int [10]'}}


__attribute__((address_space(42))) int* __attribute__((address_space(42))) ptr_in_same_addr_space = nocv_iarray;
__attribute__((address_space(42))) int* __attribute__((address_space(999))) ptr_in_different_addr_space = nocv_iarray;


struct A { virtual ~A() {} }; // \
  expected-note 1 {{candidate constructor (the implicit copy constructor)}} \
  expected-note 1 {{candidate constructor (the implicit default constructor)}}
struct C { float x, y; };
struct D : C { D(const D &i) {} }; // \
  expected-note 1 {{candidate constructor not viable}}

typedef A *A_ptr;
typedef A __attribute__((address_space(1))) *A_ptr_1;
typedef C *C_ptr;
typedef C __attribute__((address_space(1))) *C_ptr_1;
typedef D *D_ptr;
typedef D __attribute__((address_space(1))) *D_ptr_1;

void test_assignment(A_ptr ap, A_ptr_1 ap1,
                     C_ptr cp, C_ptr_1 cp1,
                     D_ptr dp, D_ptr_1 dp1) {
  // No address space conversions
  A a1 = ap[0];
  C c1 = cp[0];
  D d1 = dp[0];

  // Implicit address space conversion allowed only for POD without user defined copy constructor (C)
  A a2 = ap1[0]; // expected-error{{no matching constructor for initialization}}
  C c2 = cp1[0];
  D d2 = dp1[0]; // expected-error{{no matching constructor for initialization}}
}
