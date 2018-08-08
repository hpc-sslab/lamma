srcs="$(ls *_4.nb_search_first.h)"

for sub in $srcs
do
src=$sub
dst=${sub/%_4.nb_search_first.h/.h}
	echo "src = $src   dst = $dst"
	cp $src $dst
done

