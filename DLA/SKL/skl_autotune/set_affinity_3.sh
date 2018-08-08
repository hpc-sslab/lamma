# how to use : ./set_affinity.sh ${NI} ${NJR} {program}
# then execute {program} with ${NI}x${NJR}core affinity

NCASE=$1
NI=$2
NJR=$3
shift
shift
shift

export KMP_AFFINITY=$NULL
export OMP_NESTED=1
export KMP_HOT_TEAMS_MODE=1
export KMP_HOT_TEAMS_MAX_LEVEL=3
export OMP_PLACES="cores($(expr $NCASE \* $NI \* $NJR))"
export OMP_PROC_BIND=spread,spread,close
export OMP_NUM_THREADS=${NCASE},${NI},${NJR}
echo omp_place = ${OMP_PLACES}  /  omp_num_threads = ${OMP_NUM_THREADS}

$*
