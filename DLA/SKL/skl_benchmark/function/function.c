#include<assert.h>
#include<malloc.h>
#include<string.h>
#include"search_10.h"
#include"function.h"

int readArgument(kernel_t* bestKernel, char* arg){
	if(!strcmp("mu", arg))
		return bestKernel->mu;
	if(!strcmp("nu", arg))
		return bestKernel->nu;
	if(!strcmp("ku", arg))
		return bestKernel->ku;
	if(!strcmp("mb", arg))
		return bestKernel->mb;
	if(!strcmp("nb", arg))
		return bestKernel->nb;
	if(!strcmp("kb", arg))
		return bestKernel->nb;
	if(!strcmp("prefetchA1", arg))
		return bestKernel->prefetchA1;
	if(!strcmp("prefetchA2", arg))
		return bestKernel->prefetchA2;
	if(!strcmp("prefetchB1", arg))
		return bestKernel->prefetchB1;
	if(!strcmp("prefetchB2", arg))
		return bestKernel->prefetchB2;
}

int writeArgument(kernel_t* kernel, char* arg, int argValue){
	if(!strcmp("mu", arg))
		return kernel->mu=argValue;
	if(!strcmp("nu", arg))
		return kernel->nu=argValue;
	if(!strcmp("ku", arg))
		return kernel->ku=argValue;
	if(!strcmp("mb", arg))
		return kernel->mb=argValue;
	if(!strcmp("nb", arg))
		return kernel->nb=argValue;
	if(!strcmp("kb", arg))
		return kernel->nb=argValue;
	if(!strcmp("prefetchA1", arg))
		return kernel->prefetchA1=argValue;
	if(!strcmp("prefetchA2", arg))
		return kernel->prefetchA2=argValue;
	if(!strcmp("prefetchB1", arg))
		return kernel->prefetchB1=argValue;
	if(!strcmp("prefetchB2", arg))
		return kernel->prefetchB2=argValue;
}

