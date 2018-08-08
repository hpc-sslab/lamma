id=0
kernel=11.11x4kernel14x16x0_42x2224x1000_196+384
#mflop0.000000
mu=14
nu=16
ku=0
mb=42
nb=2224
kb=1000
prefetchA1=196
prefetchA2=0
prefetchB1=384
prefetchB2=0
nPack=11
ni=11
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
mv mkl_flops.txt mkl_MNflops_fixed_hz.txt
sleep 2
