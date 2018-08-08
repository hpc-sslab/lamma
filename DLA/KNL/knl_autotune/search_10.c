#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<omp.h>
#include"search_10.h"
#include"function.h"

data_t* data;
int nResult = 10;

int main(void)
{
	FILE *fin, *fout;
	kernel_t** bestKernels = NULL;
	double startTime=0, endTime=0;
	int count[1] = {0};

	if(fin = fopen("result/dMMRES.sum", "r") ){
		// if there is answer, then return.
		bestKernels = readkernel(fin);
		fclose(fin);
	}else{
		// if there is no answer, then search & make result file.
		
		startTime = omp_get_wtime();
    bestKernels = search(count); // search() is choosed when it is compiled.
		endTime = omp_get_wtime();

		assert(fout = fopen("result/dMMRES.sum", "w") );

		writeKernel(fout, bestKernels, nResult);
		fprintf(fout, "total time cost : %f\n", endTime-startTime);
		fprintf(fout, "total number of kernel : %d\n", *count);
		fclose(fout);
	}
	printf("---------------------------------------------\n");
	printf("|  search end. total time cost : %f |\n", endTime-startTime);
	printf("|             number of kernel : %d         |\n", count[0]);
	printf("---------------------------------------------\n");

	return 0;
}

void setDefaultKernel(kernel_t* kernel){
  kernel->mflop[0] = 0;
  kernel->mflop[1] = 0;
  kernel->mflop[2] = 0;
  kernel->mflop[3] = 0;
  kernel->mflop[4] = 0;
  kernel->mflop[5] = 0;
  kernel->mflop[6] = 0;
  kernel->mflop[7] = 0;

	kernel->mu = 1;
	kernel->nu = 1;
	kernel->ku = 1;

	kernel->mb = 1;
	kernel->nb = 1;
	kernel->kb = 1;

	kernel->prefetchA1 = 0;
	kernel->prefetchA2 = 0;

	kernel->prefetchB1 = 0;
	kernel->prefetchB2 = 0;

	kernel->nPack = 1;
	kernel->ni = 1;
	kernel->njr = 1;
}

kernel_t* copyKernel(kernel_t* src){
	kernel_t* dst = setKernel(src->mu, src->nu, src->ku,
			 src->mb, src->nb, src->kb, src->prefetchA1, src->prefetchA2, src->prefetchB1, src->prefetchB2,
			 src->nPack, src->ni, src->njr);
	return dst;
}

kernel_t* setKernel(int mu, int nu, int ku, 
		int mb, int nb, int kb, int pA1, int pA2, int pB1, int pB2,
		int nPack, int ni, int njr){
	kernel_t* kernel = newKernel();
	kernel->mu = mu;
	kernel->nu = nu;
	kernel->ku = ku;
	
	kernel->mb = mb;
	kernel->nb = nb;
	kernel->kb = kb;
	
	kernel->prefetchA1 = pA1;
	kernel->prefetchA2 = pA2;
	kernel->prefetchB1 = pB1;
	kernel->prefetchB2 = pB2;
	
	kernel->nPack = nPack;
	kernel->ni = ni;
	kernel->njr = njr;

	return kernel;
}

kernel_t* newKernel(void){
	kernel_t* kernel = malloc(sizeof(kernel_t));
	setDefaultKernel(kernel);
	return kernel;
}

kernel_t** newKernelList(int size){
	int i;
	kernel_t** kernels = (kernel_t**)malloc(sizeof(kernel_t*) * (size+1));
	for(i = 0; i<size+1; i++){
		kernels[i] = newKernel();
	}
	return kernels;
}

void killKernelList(kernel_t** kernels, int size){
	int i;
	for(i=0; i<size+1; i++){
		if(kernels[i] != NULL){
			free(kernels[i]);
		}
	}
}

/**
 * repeat : repeat number of tests
 * size : size of kernels. e.g. number of kernel.
 */
void kernelListRepeatTest(kernel_t** kernels, int repeat, int size, int* count){
	int i,j;
	double result;
	kernel_t* temp;
	for(i=0; i<size; i++){
		// repeat test phase
		result = 0;
		for(j=0; j<repeat; j++){
			result = flopsTest(kernels[i], count, 1);
			if(result > kernels[i]->mflop[0]){
				kernels[i]->mflop[0] = result;
				printf("mflop change : kernel[%d] = %f\n", i, result);
			}
		}

		// sort phase
		temp = kernels[i];
		for(j=i-1; j>=0; j--){
			if(temp->mflop[0] > kernels[j]->mflop[0]){
				kernels[j+1] = kernels[j];
			}else{
				break;
			}
		}
		kernels[j+1] = temp;
		printf("rank change : %d -> %d\n", i, j+1);
	}
}




int isContainKernel(kernel_t** kernels, kernel_t* new_kernel, int size){
	int i;
	for(i = 0; i<size; i++){
		if(kernels[i]->mu == new_kernel->mu &&
				kernels[i]->nu == new_kernel->nu &&
				kernels[i]->ku == new_kernel->ku &&
				kernels[i]->mb == new_kernel->mb &&
				kernels[i]->nb == new_kernel->nb &&
				kernels[i]->kb == new_kernel->kb &&
				kernels[i]->prefetchA1 == new_kernel->prefetchA1 &&
				kernels[i]->prefetchA2 == new_kernel->prefetchA2 &&
				kernels[i]->prefetchB1 == new_kernel->prefetchB1 &&
				kernels[i]->prefetchB2 == new_kernel->prefetchB2 &&
				kernels[i]->nPack == new_kernel->nPack &&
				kernels[i]->ni == new_kernel->ni &&
				kernels[i]->njr == new_kernel->njr 				
				)
		{
				return i;
		}
	}
	return 0;
}

void putBestKernel(kernel_t** kernels, kernel_t* new_kernel, int size){
	int i;
	printf("rank : ");
	if(isContainKernel(kernels, new_kernel, size)){
		printf(" it is contain\n");
		return;
	}
	for(i=size-1; i>-1; i--){
		if(kernels[i]->mflop[0] < new_kernel->mflop[0]){
			kernels[i+1] = kernels[i];
			printf(" %d ", i);
		}else{
			break;
		}
	}
	kernels[i+1] = new_kernel;
	if(kernels[size] != NULL){
		free(kernels[size]);
	}
	printf("\n");
}

void setDefaultData(data_t* data){
	data->L1size = 0;
	data->L2size = 0;
	data->L3size = 0;
	data->numberOfThreads = 0;
	data->affinity = NULL;
	data->numberOfRegister = 0;
}


data_t* newData(void){
	data_t* data = malloc(sizeof(data_t));
	setDefaultData(data);
	return data;
}

data_t* readData(FILE* fp){
	char variable[32];
	char value[32];
	int i, thread;
	data_t* data = newData();

	assert(fp);

	while(2 == fscanf(fp, "%s %s\n", variable, value))
	{
		if(!strcmp("L1size", variable))
			sscanf(value, "%d", &(data->L1size));
		if(!strcmp("L2size", variable))
			sscanf(value, "%d", &(data->L2size));
		if(!strcmp("L3size", variable))
			sscanf(value, "%d", &(data->L3size));
		if(!strcmp("numberOfThreads", variable))
			sscanf(value, "%d", &(data->numberOfThreads));
		data->affinity = (int*) malloc(sizeof(int) * data->numberOfThreads);
		if(!strcmp("affinity", variable)){
			if(2 == sscanf(value, "%d:%d:%d", &(data->startThreads), &(data->endThreads), &(data->increase))){
				for(i=0, thread=data->startThreads; thread<data->endThreads; thread++, i++){
					data->increase = 1;
					data->affinity[i]=thread;
				}
			}else{
				for(i=0, thread=data->startThreads; thread<data->endThreads; thread+=data->increase, i++){
					data->affinity[i]=thread;
				}
			}
		}
		if(!strcmp("numberOfRegister", variable))
			sscanf(value, "%d", &(data->numberOfRegister));
	}

	return data;
}

kernel_t** readkernel(FILE *fp){
	char variable[32];
	char value[32];
	int i;
	kernel_t** kernels = malloc(nResult*sizeof(kernel_t*));
	for(i=0; i<nResult; i++){
		kernels[i] = newKernel();
	}

	assert(fp);

	for(i = 0; i<nResult; i++){
		while(2 == fscanf(fp, "%s %s\n", variable, value))
		{
			if(!strcmp("mflop", variable))
				sscanf(value, "%f", &(kernels[i]->mflop[0]));

			if(!strcmp("mu", variable))
				sscanf(value, "%d", &(kernels[i]->mu));
			if(!strcmp("nu", variable))
				sscanf(value, "%d", &(kernels[i]->nu));
			if(!strcmp("ku", variable))
				sscanf(value, "%d", &(kernels[i]->ku));

			if(!strcmp("mb", variable))
				sscanf(value, "%d", &(kernels[i]->mb));
			if(!strcmp("nb", variable))
				sscanf(value, "%d", &(kernels[i]->nb));
			if(!strcmp("kb", variable))
				sscanf(value, "%d", &(kernels[i]->kb));

			if(!strcmp("prefetchA1", variable))
				sscanf(value, "%d", &(kernels[i]->prefetchA1));
			if(!strcmp("prefetchA2", variable))
				sscanf(value, "%d", &(kernels[i]->prefetchA2));
			if(!strcmp("prefetchB1", variable))
				sscanf(value, "%d", &(kernels[i]->prefetchB1));
			if(!strcmp("prefetchB2", variable))
				sscanf(value, "%d", &(kernels[i]->prefetchB2));

			if(!strcmp("nPack", variable))
				sscanf(value, "%d", &(kernels[i]->nPack));
			if(!strcmp("ni", variable))
				sscanf(value, "%d", &(kernels[i]->ni));
			if(!strcmp("njr", variable))
				sscanf(value, "%d", &(kernels[i]->njr));
		}
	}
	return kernels;
}

void writeKernel(FILE *fp, kernel_t** kernels, int nResult){
	char variable[32];
	int i =0;
	int temp_count=0;

	assert(fp);

	for(i = 0; i<nResult; i++){
		fprintf(fp, "id %d\n", i);
		fprintf(fp, "%d.%dx%dkernel%dx%dx%d_%dx%dx%d_%d+%d\n", 
				kernels[i]->nPack, kernels[i]->ni, kernels[i]->njr, kernels[i]->mu, kernels[i]->nu, kernels[i]->ku,
				kernels[i]->mb, kernels[i]->nb, kernels[i]->kb, kernels[i]->prefetchA1, kernels[i]->prefetchB1);
		fprintf(fp, "mflop %f\n", kernels[i]->mflop[0]);

		fprintf(fp, "mu %d\n", kernels[i]->mu);
		fprintf(fp, "nu %d\n", kernels[i]->nu);
		fprintf(fp, "ku %d\n", kernels[i]->ku);

		fprintf(fp, "mb %d\n", kernels[i]->mb);
		fprintf(fp, "nb %d\n", kernels[i]->nb);
		fprintf(fp, "kb %d\n", kernels[i]->kb);

		fprintf(fp, "prefetchA1 %d\n", kernels[i]->prefetchA1);
		fprintf(fp, "prefetchA2 %d\n", kernels[i]->prefetchA2);
		fprintf(fp, "prefetchB1 %d\n", kernels[i]->prefetchB1);
		fprintf(fp, "prefetchB2 %d\n", kernels[i]->prefetchB2);

		fprintf(fp, "nPack %d\n", kernels[i]->nPack);
		fprintf(fp, "ni %d\n", kernels[i]->ni);
		fprintf(fp, "njr %d\n", kernels[i]->njr);
		//fprintf(fp, "valid = %f\n", validTest(&(*kernels[i]), &temp_count));
		fprintf(fp, "-------------------------------------\n");
	}
}
