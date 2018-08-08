#include<stdio.h>
#include<assert.h>
#include"search_10.h"


typedef struct vectorStruct vector_t;
struct vectorStruct{
	int vector_stride;
	char vector_type[10];
	char vector_load[24];
	char vector_store[24];
	char vector_fmadd[24];
	char vector_set[24];
	char vector_prefetch[24];
};

vector_t vector = {8, "__m512d", "_mm512_loadu_pd", "_mm512_storeu_pd", 
	                 "_mm512_fmadd_pd", "_mm512_set1_pd", "_mm_prefetch"};
kernel_t genKernel;

void printCStore(FILE* fp){
	int a, b;
	fprintf(fp, "  // store _C -> C\n");
	for(a=0; a<genKernel.mu; a++){
		for(b=0; b<genKernel.nu/vector.vector_stride; b++){
			fprintf(fp, "  %s(&C[%d*ncol+%d], _C%d_%d);\n", vector.vector_store, a, 
					b*vector.vector_stride, b, a);
		}
	}
}

void printKLoop(FILE* fp){
	int a,b;
	fprintf(fp, "  // _C += A*B\n");
	if(genKernel.ku){
		fprintf(fp, "  #pragma unroll(%d)\n", genKernel.ku);
	}
	fprintf(fp, "  for(i=0; i<k ; i++)\n");
	fprintf(fp, "  {\n");

	/*
	 * set prefetch
	 */
	if(genKernel.prefetchA1 != 0){
		fprintf(fp, "    // A L1 prefetch\n");
		for(a=0; a<genKernel.mu; a+=vector.vector_stride){
			fprintf(fp, "    %s((const void*) &A[L1_DIST_A+%d],_MM_HINT_T0);\n", vector.vector_prefetch, a);
		}

	}
	if(genKernel.prefetchB1 != 0){
		fprintf(fp, "    // B L1 prefetch\n");
		for(b=0; b<genKernel.nu; b+=vector.vector_stride){
			fprintf(fp, "    %s((const void*) &B[L1_DIST_B+%d],_MM_HINT_T0);\n", vector.vector_prefetch, b);
		}

	}

	for(b=0; b<genKernel.nu/vector.vector_stride; b++){
		fprintf(fp, "    _B%d = %s(&B[%d]);\n", b, vector.vector_load, b*vector.vector_stride);
	}
	for(a=0; a<genKernel.mu; a++){
		for(b=0; b<genKernel.nu/vector.vector_stride; b++){
			fprintf(fp, "    _C%d_%d = %s(%s(A[%d]), _B%d, _C%d_%d);\n", b, a, vector.vector_fmadd, 
					vector.vector_set, a, b, b, a);
		}
	}

	fprintf(fp, "    A += MR;\n");
	fprintf(fp, "    B += NR;\n");
	fprintf(fp, "  }\n");
}

void printCLoad(FILE* fp){
	int a,b,offset;
 
	offset = 0;
	for(a=0; a<genKernel.mu; a++){
		for(b=0; b<genKernel.nu/vector.vector_stride; b++){
			fprintf(fp, "    _C%d_%d = %s(&C[%d*ncol+%d]);\n", b, a, vector.vector_load, a, b*vector.vector_stride);
		}
	}
}

/*
 * print header file & macro
 */ 
void printHeader(FILE* fp){
	fprintf(fp, "#include \"immintrin.h\"\n");
	fprintf(fp, "#include \"x86intrin.h\"\n");
	fprintf(fp, "#include \"zmmintrin.h\"\n");
	fprintf(fp, "//#include <hbwmalloc.h>\n");
	fprintf(fp, "\n");
	fprintf(fp, "#define MR %d\n", genKernel.mu);
	fprintf(fp, "#define NR %d\n", genKernel.nu);
	fprintf(fp, "#define MB %d\n", genKernel.mb);
	fprintf(fp, "#define NB %d\n", genKernel.nb);
	fprintf(fp, "#define KB %d\n", genKernel.kb);
	fprintf(fp, "\n");
	fprintf(fp, "#define L1_DIST_A %d\n", genKernel.prefetchA1);
	fprintf(fp, "#define L1_DIST_B %d\n", genKernel.prefetchB1);
	fprintf(fp, "\n");
}

/*
 * print function
 */
void printFunction(FILE* fp){
	int a, b;
	//print function name
	fprintf(fp, "void micro_kernel0(int k, const double * A, const double * B, double * C, int ncol)\n");
	fprintf(fp, "{\n");

	fprintf(fp, "  int i;\n");

	//set vector register variable
	fprintf(fp, "  register %s _B0", vector.vector_type);
	for(b=1; b<genKernel.nu/vector.vector_stride; b++){
		fprintf(fp,", _B%d", b);
	}
	fprintf(fp, ";\n");

	for(b=0; b<genKernel.nu/vector.vector_stride; b++){
		fprintf(fp, "  register %s _C%d_%d", vector.vector_type, b, 0);
		for(a=1; a<genKernel.mu; a++){
			fprintf(fp, ", _C%d_%d", b, a);
		}
		fprintf(fp, ";\n");
	}

	printCLoad(fp);
	printKLoop(fp);
	printCStore(fp);

	fprintf(fp, "}\n");

}

void	generateKernelCode(FILE* fp, kernel_t* kernel){

	genKernel.mu = kernel->mu;
	genKernel.nu = kernel->nu;
	genKernel.ku = kernel->ku;
  
	genKernel.mb = kernel->mb;
	genKernel.nb = kernel->nb;
	genKernel.kb = kernel->kb;

	genKernel.prefetchA1 = kernel->prefetchA1;
	genKernel.prefetchB1 = kernel->prefetchB1;

	printHeader(fp);
	printFunction(fp);

}

void attachOuterLoop(const char* kernelName){
	char command[1024];
	sprintf(command, "cat outerLoop >> %s", kernelName);
	assert(!system(command));
}


void generateKernel(kernel_t* kernel){
	FILE* fp;
	char kernelName[512];
	char path[256] = "kernel";

	sprintf(kernelName, "%s/%d.%dx%dkernel%dx%dx%d_%dx%dx%d_%d+%d.c", path, 
			kernel->nPack, kernel->ni, kernel->njr, kernel->mu, kernel->nu, kernel->ku,
			kernel->mb, kernel->nb, kernel->kb, kernel->prefetchA1, kernel->prefetchB1);
	printf("kernelName = %s\n", kernelName);

	fp = fopen(kernelName, "r");
	if(fp){
		printf("%s generating is skipped.\n", kernelName);
		fclose(fp);
	}else{
		fp = fopen(kernelName, "w");
		assert(fp);
		generateKernelCode(fp, kernel);
	 	fclose(fp);
		printf("%s is generated.\n", kernelName);
		attachOuterLoop(kernelName);
		printf("outerLoop is attached.\n");
	}
}

void compileKernel(kernel_t* kernel){
	char kernelName[512];
	char makeCommand[1024];
	char action[256] = "compileKernel";

	sprintf(makeCommand, "make %s KERNEL=%d.%dx%dkernel%dx%dx%d_%dx%dx%d_%d+%d NT=%d NT1=%d NT2=%d", 
			action, kernel->nPack, kernel->ni, kernel->njr, 
			kernel->mu, kernel->nu, kernel->ku,	kernel->mb, kernel->nb, kernel->kb, 
			kernel->prefetchA1, kernel->prefetchB1, kernel->nPack, kernel->ni, kernel->njr);

	assert(!system(makeCommand));

	printf("compile is finished\n");
	return;

}

int constraint(kernel_t* kernel){
	if(kernel->mu <= 0) return -1;
	if(kernel->nu <= 0 || kernel->nu % vector.vector_stride != 0) return -1;

	if(kernel->mb % kernel->mu != 0) return -1;
	//if(kernel->nb % kernel->nu != 0) return -1;

	if(kernel->mb <= 0) return -1;
	if(kernel->nb <= 0) return -1;
	if(kernel->kb <= 0) return -1;

	if(kernel->prefetchA1 < 0) return -1;
	if(kernel->prefetchB1 < 0) return -1;
  return 0;
}

int prepareKernel(kernel_t* kernel){
  if(constraint(kernel) == -1) return -1;
	generateKernel(kernel);
	compileKernel(kernel);
	return 0;
}



