srcs="$(ls *_1and2.h)"
srcs2="$(ls *_1.minimal_build.h)"

for sub in $srcs
do
src=$sub
dst=${sub/%_1and2.h/.h}
	echo "src = $src   dst = $dst"
	cp $src $dst
done

for sub in $srcs2
do
src=$sub
dst=${sub/%_1.minimal_build.h/.h}
	echo "src = $src   dst = $dst"
	cp $src $dst
done

