CC=icc
CFLAG=-O3 -g3 -qopenmp -xCORE-AVX512 -restrict
INCLUDE_MINE=-I./  -mkl
#ATLAS_MULTI_CORE
INCLUDE_ATLAS=-I./ -I./inc_atlas -lptcblas -latlas -L./lib_atlas
#ATLAS_SINGLE_CORE
#INCLUDE_ATLAS=-I./ -I./inc_atlas -lcblas -latlas -L./lib_atlas
INCLUDE_BLIS=-I./ -std=c99 -I./inc_blis -lblis -L./lib_blis
INCLUDE_OPENBLAS=-I./ -std=c99 -I./inc_openBlas -L./lib_openBlas -lopenblas -lpthread -I /opt/intel/lib/intel64 -lifcore
KERNEL=
M=
N=
K=

# NT  : packing parallelism
# NT1 : I loop packing
# NT2 : JR loop packing
NT=
NT1=
NT2=

SRC=1.bruteforce

#folder
SEARCHDIR=searchMethod
KERNELDIR=kernel
RECORDDIR=speedRecord
RESULTDIR=result
FUNCTNDIR=function

#################### MINE ##################################

compileKernel:
	$(CC) $(KERNELDIR)/$(KERNEL).c $(CFLAG) -c \
		-DNT=${NT} -DNT1=${NT1} -DNT2=${NT2} \
		-o $(KERNELDIR)/$(KERNEL).o $(INCLUDE_MINE)

validKernel:
	$(CC) valid_test.c $(KERNELDIR)/$(KERNEL).c $(CFLAG) \
		-o mine.$(KERNEL).valid $(INCLUDE_MINE) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}

flopsKernel:
	$(CC) flops_test.c $(KERNELDIR)/$(KERNEL).c $(CFLAG) \
		-o mine.$(KERNEL).flops $(INCLUDE_MINE) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}

executeValid:
	echo "--------- mine ----------" >> valid.txt
	sh ./set_affinity.sh ${NT1} ${NT2} ./mine.$(KERNEL).valid >> valid.txt

executeFlops:
	echo "--------- mine ----------" >> mine_flops.txt
	sh ./set_affinity.sh ${NT1} ${NT2} ./mine.$(KERNEL).flops >> mine_flops.txt

#################### ATLAS ##################################

compileKernel_atlas:
	$(CC) $(KERNELDIR)/$(KERNEL).c $(CFLAG) -c \
		-DNT=${NT} -DNT1=${NT1} -DNT2=${NT2} \
		-o $(KERNELDIR)/$(KERNEL).o $(INCLUDE_ATLAS)

validKernel_atlas: compileKernel_atlas
	$(CC) valid_test_atlas.c $(CFLAG) \
		-o atlas.valid $(INCLUDE_ATLAS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K 

flopsKernel_atlas:
	$(CC) flops_test_atlas.c $(CFLAG) \
		-o atlas.flops $(INCLUDE_ATLAS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K

executeValid_atlas:
	echo "--------- atlas ----------" >> valid.txt
	./atlas.$(KERNEL).valid >> valid.txt
	
executeFlops_atlas:
	echo "--------- atlas ----------" >> atlas_flops.txt
	./atlas.flops >> atlas_flops.txt

#################### BLIS ##################################

compileKernel_blis:
	$(CC) $(KERNELDIR)/$(KERNEL).c $(CFLAG) -c \
		-DNT=${NT} -DNT1=${NT1} -DNT2=${NT2} \
		-o $(KERNELDIR)/$(KERNEL).o $(INCLUDE_BLIS)

validKernel_blis: compileKernel_blis
	$(CC) valid_test_blis.c $(KERNELDIR)/$(KERNEL).c $(CFLAG) \
		-o blis.$(KERNEL).valid $(INCLUDE_BLIS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}

flopsKernel_blis: 
	$(CC) flops_test_blis.c $(CFLAG) \
		-o blis.flops $(INCLUDE_BLIS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K

executeValid_blis:
	echo "--------- blis ----------" >> valid.txt
	BLIS_NUM_THREADS=44 ./blis.$(KERNEL).valid >> valid.txt

executeFlops_blis:
	echo "--------- blis ----------" >> blis_flops.txt
	BLIS_NUM_THREADS=44 ./blis.flops >> blis_flops.txt

#################### OPENBLAS ##################################

compileKernel_openblas:
	$(CC) $(KERNELDIR)/$(KERNEL).c $(CFLAG) -c \
		-DNT=${NT} -DNT1=${NT1} -DNT2=${NT2} \
		-o $(KERNELDIR)/$(KERNEL).o $(INCLUDE_OPENBLAS)

validKernel_openblas: compileKernel_openblas
	$(CC) valid_test_openBlas.c $(KERNELDIR)/$(KERNEL).c $(CFLAG) \
		-o openBlas.$(KERNEL).valid $(INCLUDE_OPENBLAS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}

flopsKernel_openblas: 
	$(CC) flops_test_openBlas.c $(CFLAG) \
		-o openBlas.flops $(INCLUDE_OPENBLAS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K 

executeValid_openblas:
	echo "--------- openBlas ----------" >> valid.txt
	./openBlas.$(KERNEL).valid >> valid.txt

executeFlops_openblas:
	echo "--------- openBlas ----------" >> openBlas_flops.txt
	./openBlas.flops >> openBlas_flops.txt

###################### MKL #################################

flopsKernel_mkl:
	$(CC) flops_test_mkl.c $(CFLAG) \
	  -o mkl.flops $(INCLUDE_MINE) -DMSIZE=$M -DNSIZE=$N -DKSIZE=$K
	./mkl.flops >> mkl_flops.txt

###########################################################

clear :
	rm $(KERNELDIR)/* -f
	rm $(RECORDDIR)/* -f
	cp $(RESULTDIR)/dMMRES.sum $(RESULTDIR)/dMMRES.backup
	rm $(RESULTDIR)/dMMRES.sum -f
				
