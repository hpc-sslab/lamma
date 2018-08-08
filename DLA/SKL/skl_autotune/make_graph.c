#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<omp.h>
#include<malloc.h>
#include<dirent.h>
#include<sys/types.h>
#include"search_10.h"
#include"function.h"

data_t* data;
int nResult = 10;
#define MAX_SIZE 512

typedef struct graph_table graph_t;
struct graph_table
{
	char arg1[20], arg2[20];
	int size1, size2;
	int arg1Value[MAX_SIZE];
	int arg2Value[MAX_SIZE];
};

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
void putValue(int value, int array[][MAX_SIZE], int *size){
	int i;
	//if there is same value; then skip the adding
	for(i = 0 ; i<*size; i++){
		if(value == (*array)[i]){
			return;
		}
	}

	//sort case : size = 0;
	if(*size == 0){
		(*array)[0] = value;
		(*size)++;
		return;
	}
	//sort case : else case
	for(i = *size ; i>=0 ; i--){
		if((*array)[i] > value){
			(*array)[i+1] = (*array)[i];
		}else{
			break;
		}
	}
	(*array)[i+1] = value;
	(*size)++;
}

double getValue(kernel_t *rule, graph_t *graph, int arg1Index, int arg2Index){
	int *count=0;
	kernel_t *kernel = copyKernel(rule);

	if(!strcmp(graph->arg1,"mu")){
		kernel->mu = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"nu")){
		kernel->nu = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"ku")){
		kernel->ku = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"mb")){
		kernel->mb = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"nb")){
		kernel->nb = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"kb")){
		kernel->kb = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"prefetchA1")){
		kernel->prefetchA1 = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"prefetchA2")){
		kernel->prefetchA2 = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"prefetchB1")){
		kernel->prefetchB1 = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"prefetchB2")){
		kernel->prefetchB2 = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"nPack")){
		kernel->nPack = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"ni")){
		kernel->ni = graph->arg1Value[arg1Index];
	}else if(!strcmp(graph->arg1,"njr")){
		kernel->njr = graph->arg1Value[arg1Index];
	}

	if(!strcmp(graph->arg2,"mu")){
		kernel->mu = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"nu")){
		kernel->nu = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"ku")){
		kernel->ku = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"mb")){
		kernel->mb = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"nb")){
		kernel->nb = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"kb")){
		kernel->kb = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"prefetchA1")){
		kernel->prefetchA1 = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"prefetchA2")){
		kernel->prefetchA2 = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"prefetchB1")){
		kernel->prefetchB1 = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"prefetchB2")){
		kernel->prefetchB2 = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"nPack")){
		kernel->nPack = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"ni")){
		kernel->ni = graph->arg2Value[arg2Index];
	}else if(!strcmp(graph->arg2,"njr")){
		kernel->njr = graph->arg2Value[arg2Index];
	}
	
	return flopsTest(kernel, count, 0);
}

int graph_search(kernel_t rule, kernel_t kernel, graph_t* graph_table){
	if(rule.mu != -1 && rule.mu != kernel.mu) { return 0; }
	if(rule.nu != -1 && rule.nu != kernel.nu) { return 0; }
	if(rule.ku != -1 && rule.ku != kernel.ku) { return 0; }
	if(rule.mb != -1 && rule.mb != kernel.mb) { return 0; }
	if(rule.nb != -1 && rule.nb != kernel.nb) { return 0; }
	if(rule.kb != -1 && rule.kb != kernel.kb) { return 0; }
	if(rule.prefetchA1 != -1 && rule.prefetchA1 != kernel.prefetchA1) { return 0; }
	if(rule.prefetchB1 != -1 && rule.prefetchB1 != kernel.prefetchB1) { return 0; }
	if(rule.prefetchA2 != -1 && rule.prefetchA2 != kernel.prefetchA2) { return 0; }
	if(rule.prefetchB2 != -1 && rule.prefetchB2 != kernel.prefetchB2) { return 0; }
	if(rule.nPack != -1 && rule.nPack != kernel.nPack) { return 0; }
	if(rule.ni != -1 && rule.ni != kernel.ni) { return 0; }
	if(rule.njr != -1 && rule.njr != kernel.njr) { return 0; }

	if(!strcmp(graph_table->arg1,"mu")){
		putValue(kernel.mu, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"nu")){
		putValue(kernel.nu, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"ku")){
		putValue(kernel.ku, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"mb")){
		putValue(kernel.mb, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"nb")){
		putValue(kernel.nb, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"kb")){
		putValue(kernel.kb, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"prefetchA1")){
		putValue(kernel.prefetchA1, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"prefetchA2")){
		putValue(kernel.prefetchA2, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"prefetchB1")){
		putValue(kernel.prefetchB1, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"prefetchB2")){
		putValue(kernel.prefetchB2, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"nPack")){
		putValue(kernel.nPack, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"ni")){
		putValue(kernel.ni, &graph_table->arg1Value, &graph_table->size1);
	}else if(!strcmp(graph_table->arg1,"njr")){
		putValue(kernel.njr, &graph_table->arg1Value, &graph_table->size1);
	}

	if(!strcmp(graph_table->arg2,"mu")){
		putValue(kernel.mu, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"nu")){
		putValue(kernel.nu, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"ku")){
		putValue(kernel.ku, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"mb")){
		putValue(kernel.mb, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"nb")){
		putValue(kernel.nb, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"kb")){
		putValue(kernel.kb, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"prefetchA1")){
		putValue(kernel.prefetchA1, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"prefetchA2")){
		putValue(kernel.prefetchA2, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"prefetchB1")){
		putValue(kernel.prefetchB1, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"prefetchB2")){
		putValue(kernel.prefetchB2, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"nPack")){
		putValue(kernel.nPack, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"ni")){
		putValue(kernel.ni, &graph_table->arg2Value, &graph_table->size2);
	}else if(!strcmp(graph_table->arg2,"njr")){
		putValue(kernel.njr, &graph_table->arg2Value, &graph_table->size2);
	}

	return 1;
}


void printGraph(FILE* fout, kernel_t *rule, graph_t *graph){
	int i, j;
	// x axis : arg2, y axis : arg1

	//make first line head
	fprintf(fout, "%12s>> ",graph->arg2);
	for(j = 0; j<graph->size2; j++){
		fprintf(fout, "%10d ",graph->arg2Value[j]);
	}
	fprintf(fout, "\n");

	//make second line head
	fprintf(fout, "%12svv ", graph->arg1);
	for(j = 0; j<graph->size2; j++){
		fprintf(fout, "%10s ","----------");
	}
	fprintf(fout, "\n");

	//put the value
	for(i = 0; i<graph->size1; i++){
		//make row head
		fprintf(fout, "%10d     ",graph->arg1Value[i]);
		for(j = 0; j<graph->size2; j++){
			fprintf(fout, "%10f ", getValue(rule, graph, i, j));
		}
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");
}
			

int main(int argc, char* argv[])
{
	//input : arg1 = graph X axis | arg2 = graph Y axis
	
	DIR *dir;
	struct dirent *ent;
	int mu, nu, ku;
	int mb, nb, kb;
	int prefetchA1, prefetchB1;
	int nPack, ni, njr;

	kernel_t rule, kernel;
	graph_t graph;
	int i;

	/*set default setting*/
	graph.size1 = 0;
	graph.size2 = 0;
	for(i = 0; i<MAX_SIZE; i++){
		graph.arg1Value[i] = 9999999;
		graph.arg2Value[i] = 9999999;
	}

	/*set default setting*/
	rule.mu = kernel.mu = 9;
	rule.nu = kernel.nu = 24;
	rule.ku = kernel.ku = 0;
	rule.mb = kernel.mb = 36;
	rule.nb = kernel.nb = 2832;
	rule.kb = kernel.kb = 741;
	rule.prefetchA1 = kernel.prefetchA1 = 450;
	rule.prefetchB1 = kernel.prefetchB1 = 840;
	rule.prefetchA2 = kernel.prefetchA2 = 0;
	rule.prefetchB2 = kernel.prefetchB2 = 0;
	rule.nPack = kernel.nPack = 16;
	rule.ni = kernel.ni = 22;
	rule.njr = kernel.njr = 2;


	/*set search rule*/
	strcpy(graph.arg1, "prefetchA1");
	strcpy(graph.arg2, "prefetchB1");
	rule.prefetchA1 = -1;
	rule.prefetchB1 = -1;

	dir = opendir("./speedRecord/");
	if(dir != NULL){
		while((ent = readdir(dir)) != NULL){
			//printf("%s\n", ent->d_name);
			sscanf(ent->d_name, "%d.%dx%dkernel%dx%dx%d_%dx%dx%d_%d+%d.res", 
					&kernel.nPack, &kernel.ni, &kernel.njr,
					&kernel.mu, &kernel.nu, &kernel.ku,
					&kernel.mb, &kernel.nb, &kernel.kb, &kernel.prefetchA1, &kernel.prefetchB1);
			if(graph_search(rule, kernel, &graph)){
				printf("%s\n", ent->d_name);
			}
		}
		printf("graph size 1 = %d, size 2 = %d \n", graph.size1, graph.size2);
	closedir(dir);
	}

	FILE* fout;
	if(fout = fopen("graph.txt","w")){
		printGraph(fout, &rule, &graph);
		fclose(fout);
	}

	return 0;
}
