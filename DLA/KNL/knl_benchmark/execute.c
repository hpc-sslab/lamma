#include<stdio.h>
#include<assert.h>
#include"search_10.h"

/*
 * get source from kernel folder & measure that kernel's valid value.
 * output is saved at speedRecord folder
 */
double validTest(kernel_t *kernel, int* count){
	FILE* fp;
	char kernelName[256];
	char makeCommand[512];
	char action[256] = "validKernel";
	float valid = 9999.0;
	char getString[256];
	
	sprintf(kernelName, "%d.%dx%dkernel%dx%dx%d_%dx%dx%d_%d+%d", 
			kernel->nPack, kernel->ni, kernel->njr, kernel->mu, kernel->nu, kernel->ku, 
			kernel->mb, kernel->nb, kernel->kb, kernel->prefetchA1, kernel->prefetchB1);
	sprintf(makeCommand, "make %s KERNEL=%s M=%d N=%d K=%d NT=%d NT1=%d NT2=%d", action, kernelName,
			kernel->mb, kernel->nu, 1, kernel->nPack, kernel->ni, kernel->njr );

	if(system(makeCommand)){
		return -1.0;
	}

	sprintf(makeCommand, "speedRecord/%s.out", kernelName);
	(*count)++;

	sleep(1);
	fp = fopen(makeCommand, "r");
	while(fgets(getString, 256, fp)){
		int p;
		if(p=sscanf(getString, "|| C-C_mkl || = %f", &valid))
			break;
	}
	printf("%c[31m", 27);
	printf("valid = %f", valid);
	printf("%c[0m\n", 27);
	fclose(fp);
	return valid;
}

/*
 * get source from kernel folder & measure that kernel's flops
 * output is saved at speedRecord folder
 */
double flopsTest(kernel_t *kernel, int* count, int isNeedRetest){
	return flopsTest2(kernel, count, 33554432, isNeedRetest); //33554432
}

double flopsTest2(kernel_t *kernel, int* count, int timeout, int isNeedRetest){
	FILE* fp;
	char kernelName[256];
	char openCommand[512];
	char makeCommand[512];
	char action[256] = "flopsKernel";
	char action2[256] = "executeKernel";
	float flops = -100.0;
	float maxflops = -100.0;
	char getString[256];
	int flag=0;


	//check that is there flops file already and if there is no flops file, then make that.
	sprintf(kernelName, "%d.%dx%dkernel%dx%dx%d_%dx%dx%d_%d+%d", 
			kernel->nPack, kernel->ni, kernel->njr, kernel->mu, kernel->nu, kernel->ku, 
			kernel->mb, kernel->nb, kernel->kb, kernel->prefetchA1, kernel->prefetchB1);
	sprintf(openCommand, "kernel/%s.flops", kernelName);
	fp = fopen(openCommand, "r");
	if(!fp){
		sprintf(makeCommand, "make %s KERNEL=%s M=%d N=%d K=%d NT=%d NT1=%d NT2=%d", action, kernelName,
				kernel->mb, kernel->nu, 1, kernel->nPack, kernel->ni, kernel->njr);
		printf("make : %s\n", makeCommand);
		system(makeCommand);
	}else{
		fclose(fp);
	}

	// check is there result file already
	sprintf(openCommand, "speedRecord/%s.res", kernelName);
	fp = fopen(openCommand, "r");
	if(!fp){
		isNeedRetest = 1;
	}else{
		fclose(fp);
	}

	// if there is no result || need to retest -> execute "executeKernel"
	if(isNeedRetest){
		sprintf(makeCommand, "make %s KERNEL=%s NT=%d NT1=%d NT2=%d TIMEOUT=%d", action2, kernelName,
				kernel->nPack, kernel->ni, kernel->njr, timeout);
		if(flag = system(makeCommand)){
			if(0){ // have to check why it is 512. manual explain it will be return 124.
			//if(flag == 512){ // have to check why it is 512. manual explain it will be return 124.
				printf("flopsTest time is too long. timeout=%d\n", timeout);
				sleep(1);
				return -75.0;
			}else{
				printf("flopsTest execute is failed\n");
				sleep(1);
				return -50.0;
			}
		}
		sleep(1);
		(*count)++;
	}

	// get a result
	fp = fopen(openCommand, "r");
	while(fgets(getString, 256, fp)){
		/***************************************************************
		 * if you want get first record as soon as fast, use this code.
		 *if(0<sscanf(getString, "average Gflops : %f", &flops))
		 *	break;
		 ***************************************************************/
		sscanf(getString, "average Gflops : %f", &flops);
		if(flops > maxflops){
			maxflops=flops;
		}
	}
	printf("%c[31m", 27);
	printf("average Gflops = %f", maxflops);
	printf("%c[0m\n", 27);
	fclose(fp);

	return (double)maxflops;
}


