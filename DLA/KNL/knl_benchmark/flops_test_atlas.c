#include<time.h>
#include<stdio.h>
#include<malloc.h>

#include<hbwmalloc.h>
//#include<mkl_vsl.h>
#include"cblas.h"
#include<omp.h>


// row-major store
void row_major(double       **maddress,
               const int      nrow,
               const int      ncol,
               const double   seed)
{
  double *matrix = *maddress;
  const int nnz = nrow * ncol;
  const double factor = 1.0 / (double)nnz;
  int i;

  //
  for(i = 0; i < nnz; ++i)
  {
    matrix[i] = seed + (double)i * factor;
  }
}

// column-major store
void col_major(double       **maddress,
               const int      nrow,
               const int      ncol,
               const double   seed)
{
  double *matrix = *maddress;
  const int nnz = nrow * ncol;
  const double factor = 1.0 / (double)nnz;
  int i;
  int j;

  //
  for(j = 0; j < nrow; ++j)
  {
    for(i = 0; i < ncol; ++i)
    {
      matrix[i*nrow+j] = seed + (double)(j*ncol+i) * factor;
    }
  }
}

// General Matrix-Matrix Multiplication (GEMM)
// Test routine

int main(int    argc,
         char **argv)
{
#define NUM_ITER 5
  // test type
  const int iter = NUM_ITER;
  int       i, j;
	int m2, n2, k2;
  // iteration, time, validity
  double startTime;
  double endTime;
  double gap;
  double flops, maxFlops = 0;
	double flopsArray[NUM_ITER];
  double norm;
  // matrices
  double *A;
  double *B;
  double *C;

#ifdef MSIZE
	for(m2 = MSIZE; m2<1950; m2+=MSIZE);
#endif
#ifdef NSIZE
	for(n2 = NSIZE; n2<1950; n2+=NSIZE);
#endif
#ifdef KSIZE
	for(k2 = KSIZE; k2<1950; k2+=KSIZE);
#endif
  // matrix dimensions
  const int m = m2;
  const int n = n2;
  const int k = k2;
// const int k = (2400/KB)*KB;
// const int k = 12*KB;

  /*A = (double *) malloc(sizeof(double)*m*k);
  B = (double *) malloc(sizeof(double)*k*n);
  C = (double *) malloc(sizeof(double)*m*n);//  */
  // MCDRAM version
  A = (double *) hbw_malloc(sizeof(double)*m*k);
  hbw_posix_memalign((void *) A, 64, sizeof(double)*m*k);
  B = (double *) hbw_malloc(sizeof(double)*k*n);
  hbw_posix_memalign((void *) B, 64, sizeof(double)*k*n);
  C = (double *) hbw_malloc(sizeof(double)*m*n);
  hbw_posix_memalign((void *) C, 64, sizeof(double)*m*n);  // */
  // generate matrices
  row_major(&A, m, k, 1.0);
  row_major(&B, k, n, 2.0);
  row_major(&C, m, n, 3.0);
  //col_major_matrix(&A[0], m, k, 777,  -1.0,  1.0);
  //col_major_matrix(&B[0], k, n, 888, -10.0, 10.0);
  //col_major_matrix(&C[0], m, n, 999, -20.0, 20.0);
//  printf("m=%d n=%d k=%d\n mb=%d kb=%d\n",m,n,k,MB,KB);
  // test
//  double * bufToFlushLlc;
  static const double *bufToFlushLlc = NULL;
	printf("m, n, k : %d %d %d\n", m, n, k);
  for(i = 0; i < iter; ++i)
  {
    bufToFlushLlc = (double *)calloc(34*1024*1024, 64);
	//printf("calloc\n");
    startTime = omp_get_wtime();
    // dgemm
    //user_dgemm(m ,n, k, A, k, B, n, C, n);
   cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
     m, n, k, 1.0, A, k, B, n, 1.0, C, n);
  //    
   // p1d_dgemm(m ,n, k, A, k, B, n, C, n);
    endTime = omp_get_wtime();
	//printf("user_dgemm\n");
    // flops
    gap = (double)( endTime - startTime );
    flops = (2.0 * (double)m * (double)n * (double)k) * 1.0e-9 / gap;
		//get maxFlops
		if(maxFlops < flops){
			maxFlops = flops;
		}
		//sort flops
		for(j = i; j>0; --j){
			if(flops > flopsArray[j-1]){
				flopsArray[j] = flopsArray[j-1];
			}else{
				break;
			}
		}
		flopsArray[j] = flops;
    printf("dgemm time (sec)\t%f, Gflops\t%f \n", gap, flops);
  }
	flops = 0;
	for(i = 0; i < iter-2; ++i){
		flops += flopsArray[i];
	}
  //printf("done\n");
	printf("maximum Gflops : %f\n", maxFlops);
	printf("average Gflops : %f\n", flops/(iter-2));


  /*free(A);
  free(B);
  free(C); // */
	//printf("free\n");
  // MCDRAM version
  hbw_free(A);
  hbw_free(B);
  hbw_free(C);  // */
  //free(bufToFlushLlc);
  }
