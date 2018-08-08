srcs="$(ls *_3.big_nb.h)"

for sub in $srcs
do
src=$sub
dst=${sub/%_3.big_nb.h/.h}
	echo "src = $src   dst = $dst"
	cp $src $dst
done

