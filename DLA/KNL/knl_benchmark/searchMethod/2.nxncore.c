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
	ku = 0;
	for(nu = 8; nu <= data->numberOfRegister; nu+=8){
		mu = (256/nu)-1; // mu*(nu/8) + nu/8 < 32;
		nb=20000; // nb is maximum of N
		for(i = 30; i<160; i+=20){
			for(mb = mu; mb<i ; mb+=mu);
			//(mb*kb + kb*nr + mr*nr)*8byte*2core < 1MB L2cache
			int range_byte = 65536; //64KB
			int kb_min = (((data->L2size/2)-range_byte)/type_byte - (2*mu*nu)) / (mb + 2*nu); 
			int kb_max = (((data->L2size/2))/type_byte - (2*mu*nu)) / (mb + 2*nu); 
			for(kb = kb_min; kb<kb_max; kb+=16){
				int range_byte = 2097152; //2MB
				int nb_min = ((data->L3size/2)-range_byte)/type_byte/kb;
				int nb_max = ((data->L3size/2)+range_byte)/type_byte/kb;
				for(nb = nb_min; nb<nb_max; nb+=512){
					while(nb % nu != 0){
						nb++;
					}
					testKernel=setKernel(mu,nu,ku,mb,nb,kb,20*mu,0,20*nu,0,npack,ni,njr);
					printf("-----------------------1-----------------------------\n");
					if(prepareKernel(testKernel) == -1){ 
						//it means that it is failed to make kernel.
						//it is skipped.
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
						printf("%c[32m", 27);
						printf("count = %d\n", *count);
						printf("mu = %d, nu = %d, ku = %d, mb = %d, nb = %d, kb = %d\n", 
								testKernel->mu, testKernel->nu, testKernel->ku,
								testKernel->mb, testKernel->nb, testKernel->kb);
						printf("L1_distanceA = %d, L1_distanceB = %d\n", 
								testKernel->prefetchA1, testKernel->prefetchB1);
					printf("npack = %d, ni = %d, njr = %d\n", 
							testKernel->nPack, testKernel->ni, testKernel->njr);
						printf("%c[0m\n", 27);
						testKernel->mflop[0] = flopsTest(testKernel, count, 0);
						printf("mflop(check) = %f\n", testKernel->mflop[0]);
						/*
						 * bestKernel choose best 5 (nu,mu,mb,kb)set.
						 */
						printf(" bestKernel1 - ");
						putBestKernel(bestKernel1, testKernel, 20);
					}
					printf("-----------------------------------------------------\n");
				}
			}
		}
	}
	printf("-----------------------1_refine-----------------------------\n");
	kernelListRepeatTest(bestKernel1, 3, 20, count);
	printf("---------------------phase 1 best ---------------------------\n");
	for(i = 0; i < 5; i++){
		printf("%c[32m\n", 27);
		printf("%d : mu = %d, nu = %d, ku = %d, mb = %d, nb = %d, kb = %d\n",
				i, bestKernel1[i]->mu, bestKernel1[i]->nu, bestKernel1[i]->ku,
				bestKernel1[i]->mb, bestKernel1[i]->nb, bestKernel1[i]->kb);
		printf("L1_distanceA = %d, L1_distanceB = %d\n",
				bestKernel1[i]->prefetchA1, bestKernel1[i]->prefetchB1);
			printf("npack = %d, ni = %d, njr = %d\n", 
					bestKernel1[i]->nPack, bestKernel1[i]->ni, bestKernel1[i]->njr);
		printf("%c[0m\n", 27);
	}
	
	printf("/****************** phase 1-2 ************************/\n");
	for(i = 0; i < 5; i++){
		int test_ni=1;
		int test_njr=1;
		for(test_ni = 1; test_ni < 45; test_ni+=1){
			test_njr = 44/test_ni;
			if(test_ni * test_njr !=44){
				continue;
			}
				testKernel = copyKernel(bestKernel1[i]);
				testKernel->ni = test_ni;
				testKernel->njr = test_njr;
				printf("-----------------------1-2-----------------------------\n");
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
					printf(" bestKernel1_2 - ");
					putBestKernel(bestKernel1_2, testKernel, 20);

				}
				printf("-----------------------------------------------------\n");
			}
		}
		printf("-----------------------1-2_refine-----------------------------\n");
		kernelListRepeatTest(bestKernel1_2, 3, 20, count);
		printf("--------------------phase 1_2 best ------------------------\n");
		for(i = 0; i < 5; i++){
			printf("%c[32m\n", 27);
			printf("%d : mu = %d, nu = %d, ku = %d, mb = %d, nb = %d, kb = %d\n", 
					i, bestKernel1_2[i]->mu, bestKernel1_2[i]->nu, bestKernel1_2[i]->ku,
					bestKernel1_2[i]->mb, bestKernel1_2[i]->nb, bestKernel1_2[i]->kb);
			printf("L1_distanceA = %d, L1_distanceB = %d\n", 
					bestKernel1_2[i]->prefetchA1, bestKernel1_2[i]->prefetchB1);
			printf("npack = %d, ni = %d, njr = %d\n", 
					bestKernel1_2[i]->nPack, bestKernel1_2[i]->ni, bestKernel1_2[i]->njr);
			printf("%c[0m\n", 27);
		}


	printf("/****************** phase 2 ************************/\n");
	for(i = 0; i < 5; i++){
		for(L1_A = 10; L1_A < 21; L1_A+=2){
			for(L1_B = 20; L1_B < 41; L1_B+=2){
				testKernel = copyKernel(bestKernel1_2[i]);
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
					printf(" bestKernel2 - ");
					putBestKernel(bestKernel2, testKernel, 20);

				}
				printf("-----------------------------------------------------\n");
				}
			}
		}
		printf("-----------------------2_refine-----------------------------\n");
		kernelListRepeatTest(bestKernel2, 3, 20, count);
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

		printf("/****************** phase 3 ************************/\n");
		for(i = 0; i < 5; i++){
			// in the for(i), check best ku
			for(ku = 0; ku<6 ; ku++){
				testKernel = copyKernel(bestKernel2[i]);
				testKernel->ku = ku;
				printf("------------------------3----------------------------\n");
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
					printf(" bestKernel3 - ");
					putBestKernel(bestKernel3, testKernel, 20);
					printf("-----------------------------------------------------\n");
				}
				}
			}
			printf("-----------------------3_refine-----------------------------\n");
			kernelListRepeatTest(bestKernel3, 3, 20, count);
			printf("-------------------phase 3 best ---------------------------\n");
			for(i = 0; i < 10; i++){
				printf("%c[32m\n", 27);
				printf("%d : mu = %d, nu = %d, ku = %d, mb = %d, nb = %d, kb = %d\n", 
						i, bestKernel3[i]->mu, bestKernel3[i]->nu, bestKernel3[i]->ku,
						bestKernel3[i]->mb, bestKernel3[i]->nb, bestKernel3[i]->kb);
				printf("L1_distanceA = %d, L1_distanceB = %d\n", 
						bestKernel3[i]->prefetchA1, bestKernel3[i]->prefetchB1);
			printf("npack = %d, ni = %d, njr = %d\n", 
					bestKernel3[i]->nPack, bestKernel3[i]->ni, bestKernel3[i]->njr);
				printf("%c[0m\n", 27);
			}



			printf("%c[33m", 27);
			printf("count = %d", *count);
			printf("%c[0m", 27);

			killKernelList(bestKernel1,20);
			killKernelList(bestKernel2,20);

			return bestKernel3;
		}
