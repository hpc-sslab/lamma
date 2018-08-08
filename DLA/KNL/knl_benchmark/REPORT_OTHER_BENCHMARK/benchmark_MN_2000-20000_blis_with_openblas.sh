id=blis
#mflop1733.881226
mu=31
nu=8
ku=3
mb=124
nb=20000
kb=432
prefetchA1=620
prefetchA2=0
prefetchB1=160
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> blis_flops.txt
	make flopsKernel_blis M=$i N=$i K=5000  
	make executeFlops_blis  
	sleep 2
	make executeFlops_blis 
	sleep 2
	make executeFlops_blis  
done;
sleep 2

id=openBlas
for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> openBlas_flops.txt
	make flopsKernel_openblas M=$i N=$i K=5000  
	make executeFlops_openblas  
	sleep 2
	make executeFlops_openblas 
	sleep 2
	make executeFlops_openblas  
done;
sleep 2
