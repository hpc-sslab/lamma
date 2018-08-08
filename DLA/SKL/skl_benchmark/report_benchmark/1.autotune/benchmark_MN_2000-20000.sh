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
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=1
kernel=11.11x4kernel14x16x0_42x2224x1000_280+416
#mflop1585.983276
mu=14
nu=16
ku=0
mb=42
nb=2224
kb=1000
prefetchA1=280
prefetchA2=0
prefetchB1=416
prefetchB2=0
nPack=11
ni=11
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=2
kernel=11.11x4kernel14x16x0_42x2224x1000_196+576
#mflop1584.510132
mu=14
nu=16
ku=0
mb=42
nb=2224
kb=1000
prefetchA1=196
prefetchA2=0
prefetchB1=576
prefetchB2=0
nPack=11
ni=11
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=3
kernel=11.11x4kernel14x16x0_42x2224x1000_280+640
#mflop1584.232788
mu=14
nu=16
ku=0
mb=42
nb=2224
kb=1000
prefetchA1=280
prefetchA2=0
prefetchB1=640
prefetchB2=0
nPack=11
ni=11
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=4
kernel=11.11x4kernel14x16x0_42x2224x1000_196+416
#mflop1582.689453
mu=14
nu=16
ku=0
mb=42
nb=2224
kb=1000
prefetchA1=196
prefetchA2=0
prefetchB1=416
prefetchB2=0
nPack=11
ni=11
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=5
kernel=11.11x4kernel14x16x1_42x2224x1000_196+416
#mflop1548.089478
mu=14
nu=16
ku=1
mb=42
nb=2224
kb=1000
prefetchA1=196
prefetchA2=0
prefetchB1=416
prefetchB2=0
nPack=11
ni=11
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=6
kernel=11.11x4kernel14x16x1_42x2224x1000_196+576
#mflop1544.205688
mu=14
nu=16
ku=1
mb=42
nb=2224
kb=1000
prefetchA1=196
prefetchA2=0
prefetchB1=576
prefetchB2=0
nPack=11
ni=11
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=7
kernel=11.11x4kernel14x16x1_42x2224x1000_280+416
#mflop1542.585693
mu=14
nu=16
ku=1
mb=42
nb=2224
kb=1000
prefetchA1=280
prefetchA2=0
prefetchB1=416
prefetchB2=0
nPack=11
ni=11
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=8
kernel=11.11x4kernel14x16x1_42x2224x1000_196+384
#mflop1535.763916
mu=14
nu=16
ku=1
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
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


id=9
kernel=11.11x4kernel14x16x1_42x2224x1000_280+640
#mflop1531.078735
mu=14
nu=16
ku=1
mb=42
nb=2224
kb=1000
prefetchA1=280
prefetchA2=0
prefetchB1=640
prefetchB2=0
nPack=11
ni=11
njr=4

for((i=2000;i<=20000;i+=2000));do
	echo "------ id= ${id} size= ${i} ----------" >> mine_flops.txt
	make validKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make flopsKernel KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeValid KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
	sleep 2
	make executeFlops KERNEL=${kernel} M=${i} N=${i} K=5000 STACK=  NT=${nPack} NT1=${ni} NT2=${njr} 
done;
sleep 2
mv mine_flops.txt id${id}.MNflops.txt
mv valid.txt id${id}.MNvalid.txt


