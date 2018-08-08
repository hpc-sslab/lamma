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
	kernel_t **bestKernel1, **bestKernel2;

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
	bestKernel2 = newKernelList(5);


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
	ku = 0;
	for(nu = 8; nu <= 8; nu+=8){
		int mu_half = (128/nu)-1; // mu*(nu/8) + nu/8 < 16;
		int mu_full = (256/nu)-1; // mu*(nu/8) + nu/8 < 32;
		nb=20000; // nb is maximum of N
		for(i = 0; i<2; i++){
			if(i == 0){ 
				mu = mu_half;
			}else{
				mu = mu_full;
			}
			for(mb = mu; mb < 2*mu; mb+=mu){
				//(mb*kb + kb*nr + mr*nr)*8byte*2core < 1MB L2cache
				int kb_standard = (data->L2size/type_byte - (mu*nu)) / (mb + nu); 
				for(kb = kb_standard+1; kb<kb_standard+10; kb+=1){
					testKernel=setKernel(testKernel,mu,nu,ku,mb,nb,kb,20*mu,0,20*nu,0,16,kb-kb_standard,4);
					if(prepareKernel(testKernel) == -1){ 
						//it means that it is failed to make kernel.
						//it is skipped.
					}else{
						printf("-----------------------1-----------------------------\n");
						printf("%c[32m", 27);
						printf("count = %d\n", *count);
						printf("mu = %d, nu = %d, ku = %d, mb = %d, nb = %d, kb = %d\n", 
								testKernel->mu, testKernel->nu, testKernel->ku,
								testKernel->mb, testKernel->nb, testKernel->kb);
						printf("L1_distanceA = %d, L1_distanceB = %d\n", 
								testKernel->prefetchA1, testKernel->prefetchB1);
						printf("%c[0m\n", 27);
						testKernel->mflop[0] = flopsTest2(testKernel, count, 5+kb-kb_standard);
						printf("mflop(check) = %f\n", testKernel->mflop[0]);
						/*
						 * bestKernel choose best 5 (nu,mu,mb,kb)set.
						 */
						putBestKernel(bestKernel1, testKernel, 20);
					}
					printf("-----------------------------------------------------\n");
				}
			}
		}
	}
	printf("-----------------------2-----------------------------\n");
	kernelListRepeatTest(bestKernel1, 3, 20, count);

	
						
						
	printf("%c[33m", 27);
	printf("count = %d", *count);
	printf("%c[0m", 27);

	killKernelList(bestKernel2,5);

	return bestKernel1;
}
