#include<assert.h>
#include<malloc.h>
#include<string.h>
#include"search_10.h"
#include"function.h"


kernel_t** climbinghill_2D(kernel_t *bestKernel, char* arg1, int arg1_range, int arg1_inc,
		char* arg2, int arg2_range, int arg2_inc, double buffer, int* count){
	int i,j,k;
	int index=0;
	int isAscending=1;
	double previousResult = 0.0;
	int arg1_standard = readArgument(bestKernel, arg1);
	int arg2_standard = readArgument(bestKernel, arg2);
	kernel_t* testKernel;
	kernel_t** bestKernels = newKernelList((2*arg1_range/arg1_inc)+1);

	for(i=arg1_standard-arg1_range; i<arg1_standard+arg1_range+1; i+=arg1_inc){
		j=arg2_standard-arg2_range;
		while(isAscending){

			testKernel= copyKernel(bestKernel);
			writeArgument(testKernel, arg1, i);
			writeArgument(testKernel, arg2, j);
			if(prepareKernel(testKernel) == -1){
				printf("falid to make kernel, skip\n");
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
				testKernel->mflop[0] = flopsTest(testKernel, count, 0);
				printf("mflop(check) = %f\n", testKernel->mflop[0]);
				if(testKernel->mflop[0] > buffer*previousResult){
					j += arg2_inc;
					if(testKernel->mflop[0] > previousResult){
						bestKernels[index]=testKernel;
					}
				}else{
					double kernelTestResult;
					for(k = 0; k<2; k++){
						kernelTestResult = flopsTest(testKernel, count, 1);
						if(testKernel->mflop[0] < kernelTestResult){
							testKernel->mflop[0] = kernelTestResult;
						}
					}
					if( testKernel->mflop[0] > buffer*previousResult){
						j += arg2_inc;
						if(testKernel->mflop[0] > previousResult){
							bestKernels[index]=testKernel;
						}
					}else{
						isAscending=0;
					}
				}
			printf("-----------------------------------------------------\n");
			}
		}
		index++;
	}

	// sorting
	kernel_t* temp;
	for(i = 0; i<index-1; i++){
		for(j = 0; j<index-i; j++){
			if(bestKernels[j]->mflop[0] < bestKernels[j+1]->mflop[0]){
				temp = bestKernels[j];
				bestKernels[j] = bestKernels[j+1];
				bestKernels[j+1] = temp;
			}
		}
	}
	return bestKernels;
}
