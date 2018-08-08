#ifndef SEARCH_H
  #define SEARCH_H
#include <stdlib.h>

typedef struct kernelStruct kernel_t;
struct kernelStruct
{
	double mflop[8]; // 0: no remain part
	int mu, nu, ku;
	int mb, nb, kb;
	int prefetchA1, prefetchA2; //L1, L2
	int prefetchB1, prefetchB2; //L1, L2

	int nPack;
	int ni;
	int njr;

};

typedef struct dataStruct data_t;
struct dataStruct
{
	int L1size, L2size, L3size; //per core. unit is byte.
	int numberOfThreads;
	int* affinity; // not used. will be deleted
	int startThreads, endThreads, increase; //not used. will be deleted
	int numberOfRegister;
};

//search.c
  //search.c - about kernel
void setDefaultKernel(kernel_t* kernel);
kernel_t* copyKernel(kernel_t* src);
kernel_t* setKernel(int mu, int nu, int ku,
		    int mb, int nb, int kb, int pA1, int pA2, int pB1, int pB2,
				int nPack, int ni, int njr);
kernel_t* newKernel(void);
kernel_t** readkernel(FILE *fp);
void writeKernel(FILE *fp, kernel_t** kernels, int nResult);

  //search.c - about kernelList
kernel_t** newKernelList(int size);
void killKernelList(kernel_t** kernels, int size);
void kernelListRepeatTest(kernel_t** kernels, int repeat, int size, int* count);
int isContainKernel(kernel_t** kernels, kernel_t* new_kernel, int size);
void putBestKernel(kernel_t** kernels, kernel_t* new_kernel, int size);

  //search.c - about data
void setDefaultData(data_t* data);
data_t* newData(void);
data_t* readData(FILE* fp);

//generate.c
void generateKernelCode(FILE* fp, kernel_t* kernel);
void generateKernel(kernel_t* kernel);
void compileKernel(kernel_t* kernel);
int prepareKernel(kernel_t* kernel);

//execute.c
double validTest(kernel_t *kernel, int* count);
double flopsTest(kernel_t *kernel, int* count, int isNeedRetest);
double flopsTest2(kernel_t *kernel, int* count, int timeout, int isNeedRetest);



// searchMethod files are must have these functions
kernel_t** search(int* count);

#endif
