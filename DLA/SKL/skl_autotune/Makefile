CC=icc
CFLAG=-O3 -g3 -qopenmp -mkl -xCORE-AVX512 -restrict
MODULECFLAG=-O3 -g3 -qopenmp -mkl -xCORE-AVX512 -restrict
INCLUDE=-I./ 
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
TIMEOUT=33554432

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
	$(INCLUDE) $(CFLAG) -o $(SRC)_out


########## not used
module:
	$(CC) generate_68core.c -c $(INCLUDE) $(MODULECFLAG)
	$(CC) execute.c -c $(INCLUDE) $(MODULECFLAG)
###################

graph:
	$(CC) make_graph.c execute.c \
	$(INCLUDE) $(CFLAG) -o make_graph

compileKernel:
	$(CC) $(KERNELDIR)/$(KERNEL).c $(CFLAG) -c \
		-DNT=${NT} -DNT1=${NT1} -DNT2=${NT2} \
		-o $(KERNELDIR)/$(KERNEL).o $(INCLUDE)

validKernel:
	$(CC) valid_test.c $(KERNELDIR)/$(KERNEL).o $(CFLAG) \
		-o $(KERNELDIR)/$(KERNEL).valid $(INCLUDE) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}
	sh ./set_affinity.sh ${NT1} ${NT2} ./$(KERNELDIR)/$(KERNEL).valid > $(RECORDDIR)/$(KERNEL).out

flopsKernel:
	$(CC) flops_test.c $(KERNELDIR)/$(KERNEL).o $(CFLAG) \
		-o $(KERNELDIR)/$(KERNEL).flops $(INCLUDE) \
		-DMSIZE=$M -DNSIZE=$N -DKSIZE=$K -DNT=${NT} -DNT1=${NT1} -DNT2=${NT2}

executeKernel:
	sh ./set_affinity.sh ${NT1} ${NT2} ./$(KERNELDIR)/$(KERNEL).flops >> $(RECORDDIR)/$(KERNEL).res
#timeout ${TIMEOUT} sh ./set_affinity.sh ${NT1} ${NT2} ./$(KERNELDIR)/$(KERNEL).flops >> $(RECORDDIR)/$(KERNEL).res

with_log:
	 ./$(SRC)_out 2>&1 | tee $(SRC)_log

clear :
	rm $(KERNELDIR)/*.o -f
	rm $(KERNELDIR)/*.flops -f
	rm $(RECORDDIR)/* -f
	cp $(RESULTDIR)/dMMRES.sum $(RESULTDIR)/dMMRES.backup
	rm $(RESULTDIR)/dMMRES.sum -f
				
