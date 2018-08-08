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
  //static double _B[KB*NB] __attribute__((aligned(64)));
	double *_B;
	_B = (double *) hbw_malloc(sizeof(double)*KB*NB);
	hbw_posix_memalign((void *) _B, 64, sizeof(double)*KB*NB);
  // j-loop
  //double *_B;
  omp_set_nested(1);
  //_B = (double *) hbw_malloc(sizeof(double)*KB*NB);
  //hbw_posix_memalign((void *) _B, 64, sizeof(double)*KB*NB); // */
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
