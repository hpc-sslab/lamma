kernel=16.4x11kernel9x24x0_36x2832x741_180+480
mu=9
nu=24
ku=0
mb=36
nb=2832
kb=741
prefetchA1=180
prefetchA2=0
prefetchB1=480
prefetchB2=0
nPack=1
ni=1
njr=1

#make validKernel KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
#make flopsKernel KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
#make executeValid KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
#make executeFlops KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

make validKernel_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

make validKernel_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

nPack=16
ni=22
njr=2

make validKernel_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

make validKernel_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

nPack=16
ni=11
njr=4

make validKernel_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

make validKernel_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

nPack=16
ni=4
njr=11

make validKernel_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

make validKernel_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

nPack=16
ni=2
njr=22

make validKernel_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_blis KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_blis M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

make validKernel_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make flopsKernel_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
make executeValid_atlas KERNEL=${kernel} M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 
sleep 2
make executeFlops_atlas M=$mb N=$nb K=1 NT=${nPack} NT1=${ni} NT2=${njr} 

