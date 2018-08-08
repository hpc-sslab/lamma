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

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mkl_flops.txt
	make flopsKernel_mkl KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
	make flopsKernel_mkl KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
	make flopsKernel_mkl KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
done;
mv mkl_flops.txt mkl_MNflops2.txt
sleep 2
