# generate time per operation vs number of iterations
./lab2_list --threads=1  --iterations=10	      >> lab_2b_list.csv
./lab2_list --threads=1  --iterations=100	      >> lab_2b_list.csv
./lab2_list --threads=1  --iterations=1000	      >> lab_2b_list.csv
./lab2_list --threads=1  --iterations=10000	      >> lab_2b_list.csv
./lab2_list --threads=1  --iterations=20000	      >> lab_2b_list.csv
# conflicts w/o yielding
./lab2_list --threads=2  --iterations=10             >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=100            >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=1000           >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=10             >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=100            >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=1000           >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=10             >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=100            >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=1000           >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=10             >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=100            >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=1000           >> lab_2b_list.csv
# generate insert conflicts
./lab2_list --threads=2  --iterations=2   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=4   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=8   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=16  --yield=i  >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=32  --yield=i  >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=2   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=4   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=8   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=16  --yield=i  >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=2   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=4   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=8   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=16  --yield=i  >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=2   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=4   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=8   --yield=i  >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=16  --yield=i  >> lab_2b_list.csv
# generate delete conflicts
./lab2_list --threads=2  --iterations=2   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=4   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=8   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=16  --yield=d  >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=32  --yield=d  >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=2   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=4   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=8   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=16  --yield=d  >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=2   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=4   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=8   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=16  --yield=d  >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=2   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=4   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=8   --yield=d  >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=16  --yield=d  >> lab_2b_list.csv
# generate insert/lookup conflicts
./lab2_list --threads=2  --iterations=2   --yield=il >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=4   --yield=il >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=8   --yield=il >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=16  --yield=il >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=32  --yield=il >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=2   --yield=il >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=4   --yield=il >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=8   --yield=il >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=16  --yield=il >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=2   --yield=il >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=4   --yield=il >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=8   --yield=il >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=16  --yield=il >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=2   --yield=il >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=4   --yield=il >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=8   --yield=il >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=16  --yield=il >> lab_2b_list.csv
# generate delete/lookup conflicts
./lab2_list --threads=2  --iterations=2   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=4   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=8   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=16  --yield=dl >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=32  --yield=dl >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=2   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=4   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=8   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=16  --yield=dl >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=2   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=4   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=8   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=16  --yield=dl >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=2   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=4   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=8   --yield=dl >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=16  --yield=dl >> lab_2b_list.csv
# demonstrate the efficacy of mutex and spin-lock synchronization
./lab2_list --threads=12 --iterations=32 --yield=i  --sync=m >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=32 --yield=d  --sync=m >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=32 --yield=i  --sync=s >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=32 --yield=d  --sync=s >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab_2b_list.csv
# measure the performance
./lab2_list --threads=1  --iterations=1000          >> lab_2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=m >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=m >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=m >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=m >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m >> lab_2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=m >> lab_2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=m >> lab_2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=s >> lab_2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=s >> lab_2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=s >> lab_2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=s >> lab_2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s >> lab_2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=s >> lab_2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=s >> lab_2b_list.csv
