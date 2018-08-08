id=0
kernel=17.17x4kernel31x8x3_124x20000x438_620+160
#mflop1733.881226
mu=31
nu=8
ku=3
mb=124
nb=20000
kb=438
prefetchA1=620
prefetchA2=0
prefetchB1=160
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=4;i<=130;i+=12));do
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	rm mine*.flops -f
	make flopsKernel KERNEL=${kernel} M=20000 N=20000 K=5000 NT=${nPack} NT1=${ni} NT2=${njr}
	make executeFlops KERNEL=${kernel} M=20000 N=20000 K=5000 STACK=${i} NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=20000 N=20000 K=5000 STACK=${i} NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=20000 N=20000 K=5000 STACK=${i} NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
done;
mv flops.txt omp_Stackflops.txt
sleep 2
