#include<stdio.h>
#include<assert.h>
#include<malloc.h>
#include<omp.h>
#include"search_10.h"
#include"function.h"

extern data_t* data;

kernel_t** search(int* count){ // count : output parameter. it returns how many kernels are tested
	FILE* fin;
	int i, j;
	int mu, nu, ku;
	int mb, nb, kb;
	int L1_A, L1_B;
	int type_byte = 8;
	int npack = 16;
	int ni = 4;
	int njr= 11;
	kernel_t **bestKernel1, **bestKernel1_2, **bestKernel2, **bestKernel3;

	/*
	 * get hardware information. fixed part
	 */
	fin = fopen("data/data", "r");
	assert(fin);
	data = readData(fin);

	/*
	 * bestKernel for best 5 (mu,nu,mb,kb) sets.
	 * because of 6 space : if there is 5 space, then have to make exception case for final case.
	 *                      so i added 1 more space at last space.
	 */
	bestKernel1 = newKernelList(20);
	bestKernel1_2 = newKernelList(20);
	bestKernel2 = newKernelList(20);
	bestKernel3 = newKernelList(20);


	/*
	 * search part. you have to implement only here
	 *
	 * search part contain these
	 *  1. set variable for the making kernel.
	 *  2. call prepareKernel(kernel_t* kernel). if it return -1, it means kernel compile is failed.
	 *  3. call validTest(kernel_t* kernel). if it return -1, it means kernel result is wrong.
	 *     it can be skipped.
	 *  4. call testKernel->mflop[0] = flopsTest(kernel_t* kernel, int thread). 
	 *  5. *count ++;
	 *  6. if testKernel is faster then bestKernel, change bestKernel
	 */
	kernel_t *testKernel;
	testKernel = setKernel(31,8,3, 124,20000,432, 558,0,160,0, 16,17,4);
  prepareKernel(testKernel);
	testKernel->mflop[0] = flopsTest(testKernel, count, 0);
	putBestKernel(bestKernel1, testKernel,20);


			printf("%c[33m", 27);
			printf("count = %d", *count);
			printf("%c[0m", 27);
			printf("\n");


	return bestKernel1;
}
