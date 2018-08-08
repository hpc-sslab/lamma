CC=icc
CFLAG=-O3 -g3 -qopenmp -xMIC-AVX512 -restrict -lmemkind
INCLUDE_MINE=-I./  -mkl
#ATLAS_MULTI_CORE
INCLUDE_ATLAS=-I./ -I./inc_atlas -lptcblas -latlas -L./lib_atlas
#ATLAS_SINGLE_CORE
#INCLUDE_ATLAS=-I./ -I./inc_atlas -lcblas -latlas -L./lib_atlas
INCLUDE_BLIS=-I./ -std=c99 -I./inc_blis ./lib_blis/libblis.a
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

#STACK : size of OMP_STACKSIZE
STACK=

SRC=1.bruteforce

#folder
SEARCHDIR=searchMethod
KERNELDIR=kernel
RECORDDIR=speedRecord
RESULTDIR=result
FUNCTNDIR=function

10 :
	#######################################################
	# you must set SRC=(search_type file)          #
	# without ".c", it have to in the searchMethod folder #
	#######################################################
	$(CC) search_10.c $(SEARCHDIR)/$(SRC).c generate.c execute.c function/*.c \
  $(INCLUDE_MINE) $(CFLAG) -o $(SRC)_out



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
	sh ./set_affinity_stacksize.sh ${NT1} ${NT2} ${STACK} ./mine.$(KERNEL).valid >> valid.txt

executeFlops:
	echo "--------- mine ----------" >> flops.txt
	sh ./set_affinity_stacksize.sh ${NT1} ${NT2} ${STACK} ./mine.$(KERNEL).flops >> flops.txt

#################### ATLAS ##################################

compileKernel_atlas:
	$(CC) $(KERNELDIR)/$(KERNEL).c $(CFLAG) -c \
		-DNT=${NT} -DNT1=${NT1} -DNT2=${NT2} \
		-o $(KERNELDIR)/$(KERNEL).o $(INCLUDE_ATLAS)

validKernel_atlas: compileKernel_atlas
	$(CC) valid_test_atlas.c $(KERNELDIR)/$(KERNEL).o $(CFLAG) \
		-o atlas.$(KERNEL).valid $(INCLUDE_ATLAS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}

flopsKernel_atlas:
	$(CC) flops_test_atlas.c $(CFLAG) \
		-o atlas.flops $(INCLUDE_ATLAS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}

executeValid_atlas:
	echo "--------- atlas ----------" >> valid.txt
	./atlas.$(KERNEL).valid >> valid.txt
	
executeFlops_atlas:
	echo "--------- atlas ----------" >> flops.txt
	./atlas.flops >> flops.txt

#################### BLIS ##################################

compileKernel_blis:
	$(CC) $(KERNELDIR)/$(KERNEL).c $(CFLAG) -c \
		-DNT=${NT} -DNT1=${NT1} -DNT2=${NT2} \
		-o $(KERNELDIR)/$(KERNEL).o $(INCLUDE_BLIS)

validKernel_blis: compileKernel_blis
	$(CC) valid_test_blis.c $(KERNELDIR)/$(KERNEL).o $(CFLAG) \
		-o blis.$(KERNEL).valid $(INCLUDE_BLIS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}

flopsKernel_blis: 
	$(CC) flops_test_blis.c $(CFLAG) \
		-o blis.flops $(INCLUDE_BLIS) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}

executeValid_blis:
	echo "--------- blis ----------" >> valid.txt
	BLIS_NUM_THREADS=68 ./blis.$(KERNEL).valid >> valid.txt

executeFlops_blis:
	echo "--------- blis ----------" >> flops.txt
	BLIS_NUM_THREADS=68 ./blis.flops >> flops.txt

###################### MKL #################################

flopsKernel_mkl: 
	$(CC) flops_test_mkl.c $(CFLAG) \
		-o mkl.flops $(INCLUDE_MINE) -DMSIZE=$M -DNSIZE=$N -DKSIZE=$K 
	./mkl.flops >> mkl_flops.txt

##########################################################


clear :
	rm $(KERNELDIR)/* -f
	rm $(RECORDDIR)/* -f
	cp $(RESULTDIR)/dMMRES.sum $(RESULTDIR)/dMMRES.backup
	rm $(RESULTDIR)/dMMRES.sum -f
				
