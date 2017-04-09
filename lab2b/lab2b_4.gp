set terminal png
set datafile separator ","


set title "Mutex Throughput vs Number of Threads"
set xlabel "threads"
set logscale x 2
set ylabel "throughput (ns)"
set logscale y 10
set output 'lab2b_4.png'
set key left top

plot \
     "< egrep list-none-m,[1248]+,1000,1, lab2b_list_4.csv" using ($2):(1000000000/($7)) \
	title 'mutex, lists=1' with linespoints lc rgb 'blue', \
     "< egrep list-none-m,[1248]+,1000,4, lab2b_list_4.csv" using ($2):(1000000000/($7)) \
	title 'mutex, lists=4' with linespoints lc rgb 'orange', \
	 "< egrep list-none-m,[1248]+,1000,8, lab2b_list_4.csv" using ($2):(1000000000/($7)) \
	title 'mutex, lists=8' with linespoints lc rgb 'grey', \
     "< egrep list-none-m,[1248]+,1000,16, lab2b_list_4.csv" using ($2):(1000000000/($7)) \
	title 'mutex, lists=16' with linespoints lc rgb 'violet'