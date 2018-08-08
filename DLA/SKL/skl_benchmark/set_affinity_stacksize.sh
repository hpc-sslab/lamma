# how to use : ./set_affinity.sh ${NI} ${NJR} {program}
# then execute {program} with ${NI}x${NJR}core affinity

NI=$1
NJR=$2
stack=$3
shift
shift
shift

export KMP_AFFINITY=$NULL
export OMP_NESTED=1
export OMP_STACKSIZE="${stack}M"
export KMP_HOT_TEAMS_MODE=1
export KMP_HOT_TEAMS_MAX_LEVEL=2
export OMP_PLACES="cores($(expr $NI \* $NJR))"
export OMP_PROC_BIND=spread,close
export OMP_NUM_THREADS=${NI},${NJR}
echo omp_place = ${OMP_PLACES}  /  omp_num_threads = ${OMP_NUM_THREADS}
echo omp_stacksize = ${OMP_STACKSIZE}

$*


##########backup#################
#export KMP_SETTINGS=true
#export KMP_HOT_TEAMS_MODE=1
#export KMP_HOT_TEAMS_MAX_LEVEL=2
#
#./mine.11.11x4kernel14x16x0_42x2224x1000_196+384.flops
#$*
#################################
