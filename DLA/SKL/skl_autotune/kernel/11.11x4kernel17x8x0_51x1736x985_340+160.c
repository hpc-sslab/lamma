#include "immintrin.h"
#include "x86intrin.h"
#include "zmmintrin.h"
//#include <hbwmalloc.h>

#define MR 17
#define NR 8
#define MB 51
#define NB 1736
#define KB 985

#define L1_DIST_A 340
#define L1_DIST_B 160

void micro_kernel0(int k, const double * A, const double * B, double * C, int ncol)
{
  int i;
  register __m512d _B0;
  register __m512d _C0_0, _C0_1, _C0_2, _C0_3, _C0_4, _C0_5, _C0_6, _C0_7, _C0_8, _C0_9, _C0_10, _C0_11, _C0_12, _C0_13, _C0_14, _C0_15, _C0_16;
    _C0_0 = _mm512_loadu_pd(&C[0*ncol+0]);
    _C0_1 = _mm512_loadu_pd(&C[1*ncol+0]);
    _C0_2 = _mm512_loadu_pd(&C[2*ncol+0]);
    _C0_3 = _mm512_loadu_pd(&C[3*ncol+0]);
    _C0_4 = _mm512_loadu_pd(&C[4*ncol+0]);
    _C0_5 = _mm512_loadu_pd(&C[5*ncol+0]);
    _C0_6 = _mm512_loadu_pd(&C[6*ncol+0]);
    _C0_7 = _mm512_loadu_pd(&C[7*ncol+0]);
    _C0_8 = _mm512_loadu_pd(&C[8*ncol+0]);
    _C0_9 = _mm512_loadu_pd(&C[9*ncol+0]);
    _C0_10 = _mm512_loadu_pd(&C[10*ncol+0]);
    _C0_11 = _mm512_loadu_pd(&C[11*ncol+0]);
    _C0_12 = _mm512_loadu_pd(&C[12*ncol+0]);
    _C0_13 = _mm512_loadu_pd(&C[13*ncol+0]);
    _C0_14 = _mm512_loadu_pd(&C[14*ncol+0]);
    _C0_15 = _mm512_loadu_pd(&C[15*ncol+0]);
    _C0_16 = _mm512_loadu_pd(&C[16*ncol+0]);
  // _C += A*B
  for(i=0; i<k ; i++)
  {
    // A L1 prefetch
    _mm_prefetch((const void*) &A[L1_DIST_A+0],_MM_HINT_T0);
    _mm_prefetch((const void*) &A[L1_DIST_A+8],_MM_HINT_T0);
    _mm_prefetch((const void*) &A[L1_DIST_A+16],_MM_HINT_T0);
    // B L1 prefetch
    _mm_prefetch((const void*) &B[L1_DIST_B+0],_MM_HINT_T0);
    _B0 = _mm512_loadu_pd(&B[0]);
    _C0_0 = _mm512_fmadd_pd(_mm512_set1_pd(A[0]), _B0, _C0_0);
    _C0_1 = _mm512_fmadd_pd(_mm512_set1_pd(A[1]), _B0, _C0_1);
    _C0_2 = _mm512_fmadd_pd(_mm512_set1_pd(A[2]), _B0, _C0_2);
    _C0_3 = _mm512_fmadd_pd(_mm512_set1_pd(A[3]), _B0, _C0_3);
    _C0_4 = _mm512_fmadd_pd(_mm512_set1_pd(A[4]), _B0, _C0_4);
    _C0_5 = _mm512_fmadd_pd(_mm512_set1_pd(A[5]), _B0, _C0_5);
    _C0_6 = _mm512_fmadd_pd(_mm512_set1_pd(A[6]), _B0, _C0_6);
    _C0_7 = _mm512_fmadd_pd(_mm512_set1_pd(A[7]), _B0, _C0_7);
    _C0_8 = _mm512_fmadd_pd(_mm512_set1_pd(A[8]), _B0, _C0_8);
    _C0_9 = _mm512_fmadd_pd(_mm512_set1_pd(A[9]), _B0, _C0_9);
    _C0_10 = _mm512_fmadd_pd(_mm512_set1_pd(A[10]), _B0, _C0_10);
    _C0_11 = _mm512_fmadd_pd(_mm512_set1_pd(A[11]), _B0, _C0_11);
    _C0_12 = _mm512_fmadd_pd(_mm512_set1_pd(A[12]), _B0, _C0_12);
    _C0_13 = _mm512_fmadd_pd(_mm512_set1_pd(A[13]), _B0, _C0_13);
    _C0_14 = _mm512_fmadd_pd(_mm512_set1_pd(A[14]), _B0, _C0_14);
    _C0_15 = _mm512_fmadd_pd(_mm512_set1_pd(A[15]), _B0, _C0_15);
    _C0_16 = _mm512_fmadd_pd(_mm512_set1_pd(A[16]), _B0, _C0_16);
    A += MR;
    B += NR;
  }
  // store _C -> C
  _mm512_storeu_pd(&C[0*ncol+0], _C0_0);
  _mm512_storeu_pd(&C[1*ncol+0], _C0_1);
  _mm512_storeu_pd(&C[2*ncol+0], _C0_2);
  _mm512_storeu_pd(&C[3*ncol+0], _C0_3);
  _mm512_storeu_pd(&C[4*ncol+0], _C0_4);
  _mm512_storeu_pd(&C[5*ncol+0], _C0_5);
  _mm512_storeu_pd(&C[6*ncol+0], _C0_6);
  _mm512_storeu_pd(&C[7*ncol+0], _C0_7);
  _mm512_storeu_pd(&C[8*ncol+0], _C0_8);
  _mm512_storeu_pd(&C[9*ncol+0], _C0_9);
  _mm512_storeu_pd(&C[10*ncol+0], _C0_10);
  _mm512_storeu_pd(&C[11*ncol+0], _C0_11);
  _mm512_storeu_pd(&C[12*ncol+0], _C0_12);
  _mm512_storeu_pd(&C[13*ncol+0], _C0_13);
  _mm512_storeu_pd(&C[14*ncol+0], _C0_14);
  _mm512_storeu_pd(&C[15*ncol+0], _C0_15);
  _mm512_storeu_pd(&C[16*ncol+0], _C0_16);
}
void micro_kernel1(int k, const double * A, const double * B, double * C, int ncol)
{
  int i;
  register __m512d _B0;
  register __m512d _C0_0, _C0_1, _C0_2, _C0_3, _C0_4, _C0_5, _C0_6, _C0_7, _C0_8, _C0_9, _C0_10, _C0_11, _C0_12, _C0_13, _C0_14, _C0_15, _C0_16;
    _C0_0 = _mm512_setzero_pd();
    _C0_1 = _mm512_setzero_pd();
    _C0_2 = _mm512_setzero_pd();
    _C0_3 = _mm512_setzero_pd();
    _C0_4 = _mm512_setzero_pd();
    _C0_5 = _mm512_setzero_pd();
    _C0_6 = _mm512_setzero_pd();
    _C0_7 = _mm512_setzero_pd();
    _C0_8 = _mm512_setzero_pd();
    _C0_9 = _mm512_setzero_pd();
    _C0_10 = _mm512_setzero_pd();
    _C0_11 = _mm512_setzero_pd();
    _C0_12 = _mm512_setzero_pd();
    _C0_13 = _mm512_setzero_pd();
    _C0_14 = _mm512_setzero_pd();
    _C0_15 = _mm512_setzero_pd();
    _C0_16 = _mm512_setzero_pd();
  // _C += A*B
  for(i=0; i<k ; i++)
  {
    // A L1 prefetch
    _mm_prefetch((const void*) &A[L1_DIST_A+0],_MM_HINT_T0);
    _mm_prefetch((const void*) &A[L1_DIST_A+8],_MM_HINT_T0);
    _mm_prefetch((const void*) &A[L1_DIST_A+16],_MM_HINT_T0);
    // B L1 prefetch
    _mm_prefetch((const void*) &B[L1_DIST_B+0],_MM_HINT_T0);
    _B0 = _mm512_loadu_pd(&B[0]);
    _C0_0 = _mm512_fmadd_pd(_mm512_set1_pd(A[0]), _B0, _C0_0);
    _C0_1 = _mm512_fmadd_pd(_mm512_set1_pd(A[1]), _B0, _C0_1);
    _C0_2 = _mm512_fmadd_pd(_mm512_set1_pd(A[2]), _B0, _C0_2);
    _C0_3 = _mm512_fmadd_pd(_mm512_set1_pd(A[3]), _B0, _C0_3);
    _C0_4 = _mm512_fmadd_pd(_mm512_set1_pd(A[4]), _B0, _C0_4);
    _C0_5 = _mm512_fmadd_pd(_mm512_set1_pd(A[5]), _B0, _C0_5);
    _C0_6 = _mm512_fmadd_pd(_mm512_set1_pd(A[6]), _B0, _C0_6);
    _C0_7 = _mm512_fmadd_pd(_mm512_set1_pd(A[7]), _B0, _C0_7);
    _C0_8 = _mm512_fmadd_pd(_mm512_set1_pd(A[8]), _B0, _C0_8);
    _C0_9 = _mm512_fmadd_pd(_mm512_set1_pd(A[9]), _B0, _C0_9);
    _C0_10 = _mm512_fmadd_pd(_mm512_set1_pd(A[10]), _B0, _C0_10);
    _C0_11 = _mm512_fmadd_pd(_mm512_set1_pd(A[11]), _B0, _C0_11);
    _C0_12 = _mm512_fmadd_pd(_mm512_set1_pd(A[12]), _B0, _C0_12);
    _C0_13 = _mm512_fmadd_pd(_mm512_set1_pd(A[13]), _B0, _C0_13);
    _C0_14 = _mm512_fmadd_pd(_mm512_set1_pd(A[14]), _B0, _C0_14);
    _C0_15 = _mm512_fmadd_pd(_mm512_set1_pd(A[15]), _B0, _C0_15);
    _C0_16 = _mm512_fmadd_pd(_mm512_set1_pd(A[16]), _B0, _C0_16);
    A += MR;
    B += NR;
  }
  // store _C -> C
  _mm512_storeu_pd(&C[0*ncol+0], _C0_0);
  _mm512_storeu_pd(&C[1*ncol+0], _C0_1);
  _mm512_storeu_pd(&C[2*ncol+0], _C0_2);
  _mm512_storeu_pd(&C[3*ncol+0], _C0_3);
  _mm512_storeu_pd(&C[4*ncol+0], _C0_4);
  _mm512_storeu_pd(&C[5*ncol+0], _C0_5);
  _mm512_storeu_pd(&C[6*ncol+0], _C0_6);
  _mm512_storeu_pd(&C[7*ncol+0], _C0_7);
  _mm512_storeu_pd(&C[8*ncol+0], _C0_8);
  _mm512_storeu_pd(&C[9*ncol+0], _C0_9);
  _mm512_storeu_pd(&C[10*ncol+0], _C0_10);
  _mm512_storeu_pd(&C[11*ncol+0], _C0_11);
  _mm512_storeu_pd(&C[12*ncol+0], _C0_12);
  _mm512_storeu_pd(&C[13*ncol+0], _C0_13);
  _mm512_storeu_pd(&C[14*ncol+0], _C0_14);
  _mm512_storeu_pd(&C[15*ncol+0], _C0_15);
  _mm512_storeu_pd(&C[16*ncol+0], _C0_16);
}
// Packing A, r-major -> c-major
void packarc(int           row, // # of rows
             int           col, // # of cols
             const double *mt,
             int           inc, // distance
             double       *bk)
{
  int q = row / MR;
  int r = row % MR;
  int i;
  int j;
  for(i = 0; i < q; ++i)
  {
    for(j = 0; j < col; ++j)
    {
      bk[0:MR] = mt[0:MR:inc];
      bk += MR;
    //  bk += MR;
      mt += 1;
    }
    mt += (inc*MR-col);
  }
  if(r > 0)
  {
    for(j = 0; j < col; ++j)
    {
      bk[0:r] = mt[0:r:inc];
      bk[r:MR-r] = 0.0;
      bk += MR;
//      bk += MR;
      mt += 1;
    }
  }
}

// Packing B, r-major -> r-major
void packbrr(int           row, // # of rows
             int           col, // # of cols
             const double *mt,
             int           inc, // distance
             double       *bk)
{
  int q = col / NR;
  int r = col % NR;
  int i;
  int j;
  register __m512d temp;
  omp_set_nested(1);

  #pragma omp parallel for num_threads(NT) private(i) //schedule(dynamic)
  for(j = 0; j < q; ++j)
  {
    for(i = 0; i < row; ++i)
    {
      bk[i*NR+j*row*NR:NR] = mt[i*inc+j*NR:NR];
    }
//    mt += (NR - row*inc);
  }
  bk += q*row*NR;
  mt += q*NR;
  if(r > 0)
  {
    for(i = 0; i < row; ++i)
    {
      bk[0:r] = mt[0:r];
      bk[r:NR-r] = 0.0;
      bk += NR;
      mt += inc;
    }
  }
}

// Micro C = _C + C
void micro_dxpy(int                    m,
                int                    n,
                double       *restrict C,
                const double *restrict D,
                int                    ncol)
{
  int i;
  for(i = 0; i < m; ++i)
  {
    C[0:n] += D[i*NR:n];
    C += ncol;
  }
}
//
void ijrloop(const int              m,
             const int              n,
             const int              ni,
             const int              k,
             const int              ki,
             const double * restrict A,
             const int              la,
             const double * restrict B,
             double       * restrict C,
             const int              lc)
{
  int mq = (m+MB-1) / MB;
  int md = m % MB;
  int mc;
  int nq = (n+NR-1) / NR;
  int nd = n % NR;
  int nc;
  int pq;
  int pd;
  int pc;
  int i;
  int j;
  int p;
  int kt = MB/MR;
  //
  double _A[MB*KB] __attribute__((aligned(64)));
  double _C[MR*NR] __attribute__((aligned(64)));
  //
  {
  	omp_set_nested(1);
	//omp_set_dynamic(1);
  #pragma omp parallel num_threads(NT1) private(_A, mc, i)
{
    #pragma omp for schedule(dynamic)
    for(i = 0; i < mq; ++i)
    {
			//printf("i=%d,     , cpuid=%d, cpuid2=%d, thread=%d\n", i,sched_getcpu(),sched_getcpu()%68,omp_get_thread_num());
      mc = (i != mq-1 || md == 0) ? MB : md;
      packarc(mc,k,&A[ki*KB+i*MB*la],la,_A);
	#pragma omp parallel num_threads(NT2) private(j,nc,pq,p,pd,pc,_C) shared(_A, i,mc)
       {
       #pragma omp for //schedule(dynamic)  	
        for(j = 0; j < nq; ++j)
        {
					//printf("i=%d, j=%d, cpuid=%d, cpuid2=%d, thread=%d\n", i,j,sched_getcpu(),sched_getcpu()%68,omp_get_thread_num());
          nc = (j != nq-1 || nd == 0) ? NR : nd;
          pq = (mc+MR-1) / MR;
          pd = mc % MR;
          for(p = 0; p < pq; ++p)
          {
            pc = (p != pq-1 || pd == 0) ? MR : pd;
            if(pc == MR && nc == NR)
           {
			  micro_kernel0(k,&_A[p*(MR)*k],&B[j*NR*k],&C[i*MB*lc+p*MR*lc+ni*NB+j*NR],lc);
            }
            else
            {
              micro_kernel1(k,&_A[p*(MR)*k],&B[j*NR*k],_C,NR);
              micro_dxpy(pc,nc,&C[i*MB*lc+p*MR*lc+ni*NB+j*NR],_C,lc);
            }
          }
        
	}
//	#pragma omp barrier
      }
      }
	//  #pragma omp barrier
    }
 }
}

//
void user_dgemm(const int              m,
                const int              n,
                const int              k,
                const double *restrict A,
                const int              la, // distance
                const double *restrict B,
                const int              lb, // distance
                double       *restrict C,
                const int              lc)
{
  int nq = (n+NB-1) / NB;
  int nd = n % NB;
  int nc;
  int kq = (k+KB-1) / KB;
  int kd = k % KB;
  int kc;
  int j;
  int l;
	/****
	 * for Xeon Scalable processor
	 ****/
  static double _B[KB*NB] __attribute__((aligned(64)));

	/****
	 * for KNL's HBM
	 ****/
  // j-loop
	/* double *_B;
	_B = (double *) hbw_malloc(sizeof(double)*KB*NB);
	hbw_posix_memalign((void *) _B, 64, sizeof(double)*KB*NB); // */
  omp_set_nested(1);
  for(j = 0; j < nq; ++j)
  {
    nc = (j != nq-1 || nd == 0) ? NB : nd;
    for(l = 0; l < kq; ++l)
    {
      kc = (l != kq-1 || kd == 0) ? KB : kd;
      packbrr(kc,nc,&B[j*NB+l*KB*lb],lb,_B);
      //i-jr loop
      ijrloop(m,nc,j,kc,l,A,la,_B,C,lc);
    }
  }
}
