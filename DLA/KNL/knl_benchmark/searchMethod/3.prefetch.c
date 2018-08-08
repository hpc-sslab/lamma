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
	printf("/****************** phase 2 ************************/\n");
	for(i = 0; i < 1; i++){
		for(L1_A = 10; L1_A < 31; L1_A+=2){
			for(L1_B = 20; L1_B < 41; L1_B+=2){
				testKernel = setKernel(9,24,0, 36,2832,741, 0,0,0,0, 16,22,2);
				testKernel->prefetchA1 = L1_A*(testKernel->mu);
				testKernel->prefetchB1 = L1_B*(testKernel->nu);
				printf("-----------------------2-----------------------------\n");
				if(prepareKernel(testKernel) == -1){
					//if(0){
					printf("%c[32m\n", 27);
					printf("skip!!!!!!!!\n");
					printf("mu = %d, nu = %d, ku = %d, mb = %d, nb = %d, kb = %d\n", 
							testKernel->mu, testKernel->nu, testKernel->ku,
							testKernel->mb, testKernel->nb, testKernel->kb);
					printf("L1_distanceA = %d, L1_distanceB = %d\n", 
							testKernel->prefetchA1, testKernel->prefetchB1);
					printf("npack = %d, ni = %d, njr = %d\n", 
							testKernel->nPack, testKernel->ni, testKernel->njr);
					printf("%c[0m\n", 27);
				}else{
					printf("%c[32m\n", 27);
					printf("count = %d\n", ++(*count));
					printf("mu = %d, nu = %d, ku = %d, mb = %d, nb = %d, kb = %d\n", 
							testKernel->mu, testKernel->nu, testKernel->ku,
							testKernel->mb, testKernel->nb, testKernel->kb);
					printf("L1_distanceA = %d, L1_distanceB = %d\n", 
							testKernel->prefetchA1, testKernel->prefetchB1);
					printf("npack = %d, ni = %d, njr = %d\n", 
							testKernel->nPack, testKernel->ni, testKernel->njr);
					printf("%c[0m\n", 27);
					//testKernel->mflop[0] = (float)*count;
					testKernel->mflop[0] = flopsTest(testKernel, count, 0);
					testKernel->mflop[0] = flopsTest(testKernel, count, 1);
					testKernel->mflop[0] = flopsTest(testKernel, count, 1);
					printf(" bestKernel2 - ");
					putBestKernel(bestKernel2, testKernel, 20);

				}
				printf("-----------------------------------------------------\n");
				}
			}
		}
		printf("--------------------phase 2 best ------------------------\n");
		for(i = 0; i < 5; i++){
			printf("%c[32m\n", 27);
			printf("%d : mu = %d, nu = %d, ku = %d, mb = %d, nb = %d, kb = %d\n", 
					i, bestKernel2[i]->mu, bestKernel2[i]->nu, bestKernel2[i]->ku,
					bestKernel2[i]->mb, bestKernel2[i]->nb, bestKernel2[i]->kb);
			printf("L1_distanceA = %d, L1_distanceB = %d\n", 
					bestKernel2[i]->prefetchA1, bestKernel2[i]->prefetchB1);
			printf("npack = %d, ni = %d, njr = %d\n", 
					bestKernel2[i]->nPack, bestKernel2[i]->ni, bestKernel2[i]->njr);
			printf("%c[0m\n", 27);
		}



			printf("%c[33m", 27);
			printf("count = %d", *count);
			printf("%c[0m", 27);
			printf("\n");


			return bestKernel2;
		}
