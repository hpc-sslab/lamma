#include<time.h>
#include<stdio.h>
#include<malloc.h>

#include<hbwmalloc.h>
#include<mkl_cblas.h>
#include<mkl_vsl.h>
#include<omp.h>


// col-major store
void col_major_matrix(double      *mm,
                      const int    nrow,
                      const int    ncol,
                      const int    seed,
                      const double lb,
                      const double rb)
{
  int i;
  VSLStreamStatePtr stm;
  //
  vslNewStream(&stm, VSL_BRNG_MT19937, seed);
  #pragma ivdep
  for(i = 0; i < ncol; ++i)
  {
    vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD_ACCURATE,stm,nrow,&mm[i*nrow],lb,rb);
  }
  vslDeleteStream(&stm);
}

// column-major store
void row_major_matrix(double      *mm,
                      const int    nrow,
                      const int    ncol,
                      const int    seed,
                      const double lb,
                      const double rb)
{
  int i;
  VSLStreamStatePtr stm;
  //
  vslNewStream(&stm, VSL_BRNG_MT19937, seed);
  #pragma ivdep
  for(i = 0; i < nrow; ++i)
  {
    vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD_ACCURATE,stm,ncol,&mm[i*ncol],lb,rb);
  }
  vslDeleteStream(&stm);
}

// General Matrix-Matrix Multiplication (GEMM)
// Test routine

int main(int    argc,
         char **argv)
{
  // test type
  const int iter = 1; 
  int       i;
	int m2, n2, k2;
  // iteration, time, validity
  double startTime;
  double endTime;
  double gap;
  double flops;
  double norm;
  // matrices
  double *A;
  double *B;
  double *C;
  double *D;

#ifdef MSIZE
	for(m2 = MSIZE; m2<2000; m2+=MSIZE);
#endif
#ifdef NSIZE
	for(n2 = NSIZE; n2<2000; n2+=NSIZE);
#endif
#ifdef KSIZE
	for(k2 = KSIZE; k2<2000; k2+=KSIZE);
#endif
  // matrix dimensions
  const int m = m2; 
  const int n = n2; 
	const int k = k2;
/*  A = (double *) malloc(sizeof(double)*m*k);
  B = (double *) malloc(sizeof(double)*k*n);
  C = (double *) malloc(sizeof(double)*m*n);
  D = (double *) malloc(sizeof(double)*m*n);*/
  // MCDRAM version
  A = (double *) hbw_malloc(sizeof(double)*m*k);
  hbw_posix_memalign((void *) A, 64, sizeof(double)*m*k);
  B = (double *) hbw_malloc(sizeof(double)*k*n);
  hbw_posix_memalign((void *) B, 64, sizeof(double)*k*n);
  C = (double *) hbw_malloc(sizeof(double)*m*n);
  hbw_posix_memalign((void *) C, 64, sizeof(double)*m*n);
  D = (double *) hbw_malloc(sizeof(double)*m*n);
  hbw_posix_memalign((void *) D, 64, sizeof(double)*m*n);
  // generate matrices
  row_major_matrix(&A[0], m, k, 777,  -1.0,  1.0);
  row_major_matrix(&B[0], k, n, 888, -10.0, 10.0);
  row_major_matrix(&C[0], m, n, 999, -20.0, 20.0);
  /*for(i=0; i<m*k; ++i)
  {
    printf("A[%d]=%f\n",i,A[i]);
  }
  int mq = (m+3-1) / 3;
  int md = m % 3;
  double _A[mq*3*9] __attribute__((aligned(64)));
  packarc(m,k,&A[0],k,_A);
  for(i=0; i<mq*3*9; ++i)
  {
    printf("A[%d]=%f\n",i,_A[i]);
  }*/
  //col_major_matrix(&A[0], m, k, 777,  -1.0,  1.0);
  //col_major_matrix(&B[0], k, n, 888, -10.0, 10.0);
  //col_major_matrix(&C[0], m, n, 999, -20.0, 20.0);
  D[0:m*n] = C[0:m*n];
  static const double *bufToFlushLlc = NULL;

  // test
  for(i = 0; i < iter; ++i)
  {
		printf("valid_test start\n");
    startTime = omp_get_wtime();

    // dgemm
    //user_dgemm(m ,n, k, A, B, C);
    user_dgemm(m ,n, k, A, k, B, n, C, n);
	//p1d_dgemm(m,n,k,A,k,B,n,C,n);
    // mkl
    //cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
    //  m, n, k, 1.0, A, k, B, n, 1.0, C, n);

    endTime = omp_get_wtime();

    // flops
    gap = (double)( endTime - startTime );
    flops = (2.0 * (double)m * (double)n * (double)k) * 1.0e-9 / gap;
    printf("dgemm time (sec)\t%f, Gflops\t%f \n", gap, flops);
  }
  {
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
      m, n, k, 1.0, A, k, B, n, 1.0, D, n);
    cblas_daxpy(m*n, -1.0, C, 1, D, 1);
    norm = cblas_dnrm2(m*n, D, 1);
    printf("|| C-C_mkl || = %e\n",norm);
  }
//  free(A);
//  free(B);
//  free(C);
//  free(D);
  // MCDRAM version
  hbw_free(A);
  hbw_free(B);
  hbw_free(C);
  hbw_free(D);
}
