id=Manual
kernel=17.17x4kernel31x8x3_124x20000x432_620+160
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
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt

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
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt

id=1
kernel=17.17x4kernel31x8x3_124x20000x422_558+160
#mflop1732.679565
mu=31
nu=8
ku=3
mb=124
nb=20000
kb=422
prefetchA1=558
prefetchA2=0
prefetchB1=160
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt

id=2
kernel=17.17x4kernel31x8x3_124x20000x406_620+160
#mflop1732.575439
mu=31
nu=8
ku=3
mb=124
nb=20000
kb=406
prefetchA1=620
prefetchA2=0
prefetchB1=160
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt

id=3
kernel=17.17x4kernel31x8x2_124x20000x406_620+160
#mflop1722.604004
mu=31
nu=8
ku=2
mb=124
nb=20000
kb=406
prefetchA1=620
prefetchA2=0
prefetchB1=160
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt

id=4
kernel=17.17x4kernel31x8x3_124x20000x422_620+192
#mflop1719.583008
mu=31
nu=8
ku=3
mb=124
nb=20000
kb=422
prefetchA1=620
prefetchA2=0
prefetchB1=192
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt

id=5
kernel=17.17x4kernel31x8x2_124x20000x438_620+160
#mflop1719.182983
mu=31
nu=8
ku=2
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
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=6
kernel=17.17x4kernel31x8x3_124x20000x438_558+176
#mflop1718.781006
mu=31
nu=8
ku=3
mb=124
nb=20000
kb=438
prefetchA1=558
prefetchA2=0
prefetchB1=176
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt

id=7
kernel=17.17x4kernel31x8x2_124x20000x422_620+192
#mflop1717.725952
mu=31
nu=8
ku=2
mb=124
nb=20000
kb=422
prefetchA1=620
prefetchA2=0
prefetchB1=192
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt

id=8
kernel=17.17x4kernel31x8x2_124x20000x422_620+160
#mflop1715.163696
mu=31
nu=8
ku=2
mb=124
nb=20000
kb=422
prefetchA1=620
prefetchA2=0
prefetchB1=160
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt

id=9
kernel=17.17x4kernel31x8x2_124x20000x438_558+176
#mflop1713.504883
mu=31
nu=8
ku=2
mb=124
nb=20000
kb=438
prefetchA1=558
prefetchA2=0
prefetchB1=176
prefetchB2=0
nPack=17
ni=17
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=112 NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt
