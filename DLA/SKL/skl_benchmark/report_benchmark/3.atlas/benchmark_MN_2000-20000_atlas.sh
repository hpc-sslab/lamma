id=atlas
kernel=11.11x4kernel14x16x0_42x2224x1000_196+384
#mflop0.000000

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> atlas_flops.txt
	make flopsKernel_atlas KERNEL=${kernel} M=${i} N=${i} K=5000  
	make executeFlops_atlas KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
	make executeFlops_atlas KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
	make executeFlops_atlas KERNEL=${kernel} M=${i} N=${i} K=5000  
done;
sleep 2
mv atlas_flops.txt atlas_MN_2000-20000.txt

id=mkl
for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mkl_flops.txt
	make flopsKernel_mkl KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
	make flopsKernel_mkl KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
	make flopsKernel_mkl KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
done;
sleep 2;
mv mkl_flops.txt mkl_MN_2000-20000.txt
