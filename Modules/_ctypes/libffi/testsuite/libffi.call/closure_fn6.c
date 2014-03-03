/* Area:	closure_call
   Purpose:	Check multiple values passing from different type.
		Also, exceed the limit of gpr and fpr registers on PowerPC.
   Limitations:	none.
   PR:		PR23404
   Originator:	<andreast@gcc.gnu.org> 20050830	 */

/* { dg-do run } */
#include "ffitest.h"

static void
closure_test_fn0(ffi_cif* cif __UNUSED__, void* resp, void** args,
		 void* userdata)
{
  *(ffi_arg*)resp =
    (int)*(unsigned REALLYLONG *)args[0] +
    (int)(*(unsigned REALLYLONG *)args[1]) +
    (int)(*(unsigned REALLYLONG *)args[2]) +
    (int)*(unsigned REALLYLONG *)args[3] +
    (int)(*(int *)args[4]) + (int)(*(double *)args[5]) +
    (int)*(double *)args[6] + (int)(*(float *)args[7]) +
    (int)(*(double *)args[8]) + (int)*(double *)args[9] +
    (int)(*(int *)args[10]) + (int)(*(float *)args[11]) +
    (int)*(int *)args[12] + (int)(*(int *)args[13]) +
    (int)(*(double *)args[14]) +  (int)*(double *)args[15] +
    (intptr_t)userdata;

  printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d: %d\n",
	 (int)*(unsigned REALLYLONG  *)args[0],
	 (int)(*(unsigned REALLYLONG  *)args[1]),
	 (int)(*(unsigned REALLYLONG  *)args[2]),
	 (int)*(unsigned REALLYLONG  *)args[3],
	 (int)(*(int *)args[4]), (int)(*(double *)args[5]),
	 (int)*(double *)args[6], (int)(*(float *)args[7]),
	 (int)(*(double *)args[8]), (int)*(double *)args[9],
	 (int)(*(int *)args[10]), (int)(*(float *)args[11]),
	 (int)*(int *)args[12], (int)(*(int *)args[13]),
	 (int)(*(double *)args[14]), (int)(*(double *)args[15]),
	 (int)(intptr_t)userdata, (int)*(ffi_arg *)resp);

}

typedef int (*closure_test_type0)(unsigned REALLYLONG,
				  unsigned REALLYLONG,
				  unsigned REALLYLONG,
				  unsigned REALLYLONG,
				  int, double, double, float, double, double,
				  int, float, int, int, double, double);

int main (void)
{
  ffi_cif cif;
  void *code;
  ffi_closure *pcl = ffi_closure_alloc(sizeof(ffi_closure), &code);
  ffi_type * cl_arg_types[17];
  int res;

  cl_arg_types[0] = &ffi_type_uint64;
  cl_arg_types[1] = &ffi_type_uint64;
  cl_arg_types[2] = &ffi_type_uint64;
  cl_arg_types[3] = &ffi_type_uint64;
  cl_arg_types[4] = &ffi_type_sint;
  cl_arg_types[5] = &ffi_type_double;
  cl_arg_types[6] = &ffi_type_double;
  cl_arg_types[7] = &ffi_type_float;
  cl_arg_types[8] = &ffi_type_double;
  cl_arg_types[9] = &ffi_type_double;
  cl_arg_types[10] = &ffi_type_sint;
  cl_arg_types[11] = &ffi_type_float;
  cl_arg_types[12] = &ffi_type_sint;
  cl_arg_types[13] = &ffi_type_sint;
  cl_arg_types[14] = &ffi_type_double;
  cl_arg_types[15] = &ffi_type_double;
  cl_arg_types[16] = NULL;

  /* Initialize the cif */
  CHECK(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 16,
		     &ffi_type_sint, cl_arg_types) == FFI_OK);

  CHECK(ffi_prep_closure_loc(pcl, &cif, closure_test_fn0,
                             (void *) 3 /* userdata */, code) == FFI_OK);

  res = (*((closure_test_type0)code))
    (1, 2, 3, 4, 127, 429., 7., 8., 9.5, 10., 11, 12., 13,
     19, 21., 1.);
  /* { dg-output "1 2 3 4 127 429 7 8 9 10 11 12 13 19 21 1 3: 680" } */
  printf("res: %d\n",res);
  /* { dg-output "\nres: 680" } */
  exit(0);
}
