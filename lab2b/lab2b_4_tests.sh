#mutex
./lab2_list --threads=1  --iterations=1000  --lists=1  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=1  --iterations=1000  --lists=4  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=1  --iterations=1000  --lists=8  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=1  --iterations=1000  --lists=16  --sync=m  >> lab2b_list_4.csv

./lab2_list --threads=2  --iterations=1000  --lists=1  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=2  --iterations=1000  --lists=4  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=2  --iterations=1000  --lists=8  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=2  --iterations=1000  --lists=16  --sync=m  >> lab2b_list_4.csv

./lab2_list --threads=4  --iterations=1000  --lists=1  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=4  --iterations=1000  --lists=4  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=4  --iterations=1000  --lists=8  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=4  --iterations=1000  --lists=16  --sync=m  >> lab2b_list_4.csv

./lab2_list --threads=8  --iterations=1000  --lists=1  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=8  --iterations=1000  --lists=4  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=8  --iterations=1000  --lists=8  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=8  --iterations=1000  --lists=16  --sync=m  >> lab2b_list_4.csv

./lab2_list --threads=12  --iterations=1000  --lists=1  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=12  --iterations=1000  --lists=4  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=12  --iterations=1000  --lists=8  --sync=m  >> lab2b_list_4.csv
./lab2_list --threads=12  --iterations=1000  --lists=16  --sync=m  >> lab2b_list_4.csv

#spin
./lab2_list --threads=1  --iterations=1000  --lists=1  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=1  --iterations=1000  --lists=4  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=1  --iterations=1000  --lists=8  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=1  --iterations=1000  --lists=16  --sync=s  >> lab2b_list_4.csv

./lab2_list --threads=2  --iterations=1000  --lists=1  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=2  --iterations=1000  --lists=4  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=2  --iterations=1000  --lists=8  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=2  --iterations=1000  --lists=16  --sync=s  >> lab2b_list_4.csv

./lab2_list --threads=4  --iterations=1000  --lists=1  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=4  --iterations=1000  --lists=4  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=4  --iterations=1000  --lists=8  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=4  --iterations=1000  --lists=16  --sync=s  >> lab2b_list_4.csv

./lab2_list --threads=8  --iterations=1000  --lists=1  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=8  --iterations=1000  --lists=4  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=8  --iterations=1000  --lists=8  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=8  --iterations=1000  --lists=16  --sync=s  >> lab2b_list_4.csv

./lab2_list --threads=12  --iterations=1000  --lists=1  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=12  --iterations=1000  --lists=4  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=12  --iterations=1000  --lists=8  --sync=s  >> lab2b_list_4.csv
./lab2_list --threads=12  --iterations=1000  --lists=16  --sync=s  >> lab2b_list_4.csv
