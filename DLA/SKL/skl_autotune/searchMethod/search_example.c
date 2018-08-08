#include<stdio.h>
#include<assert.h>
#include<malloc.h>
#include<omp.h>
#include"search_10.h"
#include"function.h"

extern data_t* data;

kernel_t** search(int* count){ 
	FILE* fin;
	int i, j;
	int mu, nu, ku, mb, nb, kb;
	int L1_A, L1_B;
	int type_byte = 8;
	int ni = 11, njr = 4;
	kernel_t **bestKernel1 = newKernelList(20); // list for sorting the best cases

	fin = fopen("data/data", "r"); // get hardware information
	data = readData(fin);

	/* search part contain these
	 *  1. call setKernel() for new test case.
	 *  2. call prepareKernel(kernel_t* kernel). 
	 *     if it return -1, it means kernel compile is failed.
	 *  3. call validTest(kernel_t* kernel). 
	 *     if it return -1, it means kernel result is wrong.
	 *     it can be skipped.
	 *  4. call testKernel->mflop[0] = flopsTest(kernel_t* kernel, int thread)
	 *     it records the flops.
	 *  5. *count ++;
	 *  6. put the kernel to kernelList. it aligns the kernels in order of speed.
	 */
	kernel_t *testKernel;
	ku = 0;
	for(nu = 8; nu <= data->numberOfRegister; nu+=8){
		for(mu = (256/nu)-1; mu>7; mu--){
			nb=20000; // nb is maximum of N
			for(i = 30; i<160; i+=20){
				for(mb = mu; mb<i ; mb+=mu);
				//(mb*kb + kb*nr + mr*nr)*8byte*2core < 1MB L2cache
				int range_byte = 65536; //64KB
				int kb_min = (((data->L2size/2)-range_byte)/type_byte - (mu*nu)) / (mb + nu); 
				int kb_max = (((data->L2size/2))/type_byte - (mu*nu)) / (mb + nu); 
				for(kb = kb_min; kb<kb_max; kb+=16){
					int range_byte = 2097152; //2MB
					int nb_min = ((data->L3size/2)-range_byte)/type_byte/kb;
					int nb_max = ((data->L3size/2)+range_byte)/type_byte/kb;
					for(nb = nb_min; nb<nb_max; nb+=512){
						while(nb % nu != 0){
							nb++;
						}
						testKernel=setKernel(mu,nu,ku,mb,nb,kb,20*mu,0,20*nu,0,ni,ni,njr);
						if(prepareKernel(testKernel) != -1){ 
							printf("mflop(check) = %f\n", testKernel->mflop[0]);
							++(*count);
							putBestKernel(bestKernel1, testKernel, 20);
						}
					}
				}
			}
		}
	}
	//additional testing for accuracy (top 20. 3times)
	kernelListRepeatTest(bestKernel1, 3, 20, count);
	printf("count = %d", *count);
	return bestKernel1;
}
