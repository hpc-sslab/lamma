id=ATLAS
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
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make flopsKernel_atlas KERNEL=${kernel} M=${i} N=${i} K=5000  
	make executeFlops_atlas KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
	make executeFlops_atlas KERNEL=${kernel} M=${i} N=${i} K=5000  
	sleep 2
	make executeFlops_atlas KERNEL=${kernel} M=${i} N=${i} K=5000  
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt
