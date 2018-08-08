# how to use : ./set_affinity.sh ${NI} ${NJR} {program}
# then execute {program} with ${NI}x${NJR}core affinity

NI=$1
NJR=$2
shift
shift

export KMP_AFFINITY=$NULL
export OMP_NESTED=1
export KMP_HOT_TEAMS_MODE=1
export KMP_HOT_TEAMS_MAX_LEVEL=2
export OMP_PLACES="cores($(expr $NI \* $NJR))"
export OMP_PROC_BIND=spread,spread
export OMP_NUM_THREADS=${NI},${NJR}
echo omp_place = ${OMP_PLACES}  /  omp_num_threads = ${OMP_NUM_THREADS}

$*
