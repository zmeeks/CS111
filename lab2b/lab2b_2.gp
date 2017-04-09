#! /usr/bin/gnuplot
#
#
#
# input: 
#		lab2b_list.csv :
#
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. average op time (ns)
#	8. average mutex-lock wait time (ns)
#
# output:
#	lab2b_2.png ... time vs number of threads from lab2b_mutex data
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#

# general plot parameters
set terminal png
set datafile separator ","


set title "Time vs Number of Threads"
set xlabel "threads"
set logscale x 2
set xrange [0.75:]
set ylabel "time (ns)"
set logscale y 10
set output 'lab2b_2.png'
set key left top
# grep out only successful (sum=0) yield runs
plot \
     "< grep list lab2b_list_mutex.csv" using ($2):($8) \
	title 'wait-for-lock time' with linespoints lc rgb 'blue', \
     "< grep list lab2b_list_mutex.csv" using ($2):($7) \
	title 'average time per operation' with linespoints lc rgb 'orange'
