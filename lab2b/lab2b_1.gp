#! /usr/bin/gnuplot
#
#
#
# input: 
#	lines from lab2b_add.csv :
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # add operations
#	5. run time (ns)
#	6. throughput (ns)
#	7. total sum at end of run (should be zero)
#
#   lines from lab2b_list.csv :
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. throughput (ns)
#
# output:
#	lab2b_1.png ... throughput vs number of threads from lab2a data
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#

# general plot parameters
set terminal png
set datafile separator ","


set title "Throughput vs Number of Threads"
set xlabel "threads"
set logscale x 2
set xrange [0.75:]
set ylabel "throughput (ns)"
set logscale y 10
set output 'lab2b_1.png'
set key left top
# grep out only successful (sum=0) yield runs
plot \
     "< grep add-m lab2a_add.csv" using ($2):(1000000000/($6)) \
	title 'add mutex' with linespoints lc rgb 'blue', \
     "< grep add-s lab2a_add.csv" using ($2):(1000000000/($6)) \
	title 'add spin-lock' with linespoints lc rgb 'orange', \
	 "< grep list-none-m lab2a_list.csv" using ($2):(1000000000/($7)) \
	title 'list mutex' with linespoints lc rgb 'grey', \
     "< grep list-none-s lab2a_list.csv" using ($2):(1000000000/($7)) \
	title 'list spin-lock' with linespoints lc rgb 'violet'
	