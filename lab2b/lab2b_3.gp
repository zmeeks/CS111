set terminal png
set datafile separator ","

set title "Iterations that run without failure"
set logscale x 2
set xlabel "threads"
set ylabel "successful iterations"
set logscale y 10
set output 'lab2b_3.png'
plot \
    "< grep 'list-id-m,' lab2b_list_3.csv" using ($2):($3) \
	with points lc rgb "red" title "Mutex, lists=4", \
    "< grep 'list-id-s,' lab2b_list_3.csv" using ($2):($3) \
	with points lc rgb "blue" title "Spin-Lock, lists=4", \
	 "< grep 'list-id-none,' lab2b_list_3.csv" using ($2):($3) \
	with points lc rgb "green" title "unprotected, lists=4"
  