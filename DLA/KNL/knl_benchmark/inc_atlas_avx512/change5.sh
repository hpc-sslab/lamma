srcs="$(ls *_5.cleanup.h)"

for sub in $srcs
do
src=$sub
dst=${sub/%_5.cleanup.h/.h}
	echo "src = $src   dst = $dst"
	cp $src $dst
done

