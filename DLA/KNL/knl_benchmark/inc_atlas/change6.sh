srcs="$(ls *_6.long_kb.h)"

for sub in $srcs
do
src=$sub
dst=${sub/%_6.long_kb.h/.h}
	echo "src = $src   dst = $dst"
	cp $src $dst
done

