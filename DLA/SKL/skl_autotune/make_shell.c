#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<omp.h>
#include<malloc.h>
#include"search_10.h"

data_t* data;
int nResult = 10;

int main(int argc, char* argv[])
{
	FILE *fin, *fout;
	kernel_t** bestKernels = NULL;
	double startTime=0, endTime=0;
	int count[1] = {0};

	printf("file name : %s\n", argv[1]);
	if(fin = fopen(argv[1], "r") ){
		// if there is answer, then return.
		bestKernels = readkernel(fin);
		fclose(fin);
	}else{
		printf("there is no file \n");
	}

	if(fout = fopen("shell.out", "w")){
		writeKernel(fout, bestKernels, 10);
		fclose(fout);
	}
	

	return 0;
}


void setDefaultKernel(kernel_t* kernel){
  kernel->mflop[0] = 0;

	kernel->mu = 0;
	kernel->nu = 0;
	kernel->ku = 0;

	kernel->mb = 0;
	kernel->nb = 0;
	kernel->kb = 0;

	kernel->prefetchA1 = 0;
	kernel->prefetchA2 = 0;

	kernel->prefetchB1 = 0;
	kernel->prefetchB2 = 0;
}

kernel_t* newKernel(void){
	kernel_t* kernel = malloc(sizeof(kernel_t));
	setDefaultKernel(kernel);
	return kernel;
}

kernel_t** readkernel(FILE *fp){
	char variable[64];
	char value[64];
	int i;
	kernel_t** kernels = malloc(nResult*sizeof(kernel_t*));
	for(i=0; i<nResult; i++){
		kernels[i] = newKernel();
	}

	assert(fp);

	for(i = 0; i<nResult; i++){
		//printf("i = %d\n", i);
		while(1 == fscanf(fp, "%s", variable)){
			//printf("variable = %s\n", variable);
			if(variable[0] == '-'){
				//printf("i++\n");
				break;
			}else{
				fscanf(fp, "%s\n", value);
				//printf("value = %s\n", value);
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
			}
		}
	}
	return kernels;
}

void writeKernel(FILE *fp, kernel_t** kernels, int nResult){
	char variable[32];
	int i =0;

	assert(fp);

	for(i = 0; i<nResult; i++){
		//fprintf(fp, "mflop %f\n", kernels[i]->mflop[0]);

		fprintf(fp, "id=%d\n", i);
		fprintf(fp, "mr=%d\n", kernels[i]->mu);
		fprintf(fp, "nr=%d\n", kernels[i]->nu);
		fprintf(fp, "mu=%d\n", kernels[i]->mu);
		fprintf(fp, "nu=%d\n", kernels[i]->nu);
		fprintf(fp, "ku=%d\n", kernels[i]->ku);
		fprintf(fp, "mb=%d\n", kernels[i]->mb);
		fprintf(fp, "nb=%d\n", kernels[i]->nb);
		fprintf(fp, "kb=%d\n", kernels[i]->kb);
		fprintf(fp, "prefetchA1=%d\n", kernels[i]->prefetchA1);
		fprintf(fp, "prefetchA2=%d\n", kernels[i]->prefetchA2);
		fprintf(fp, "prefetchB1=%d\n", kernels[i]->prefetchB1);
		fprintf(fp, "prefetchB2=%d\n", kernels[i]->prefetchB2);

		fprintf(fp, "kernel%dx%d%d_%dx%dx%d_%d+%d.c\n", kernels[i]->mu, kernels[i]->nu, kernels[i]->ku,
				kernels[i]->mb, kernels[i]->nb, kernels[i]->kb, kernels[i]->prefetchA1, kernels[i]->prefetchB1);
		fprintf(fp, "-------------------------------------\n");
	}
}

void setDefaultData(data_t* data){
	data->L1size = 0;
	data->L2size = 0;
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
