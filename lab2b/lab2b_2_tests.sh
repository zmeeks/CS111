./lab2b_list_mutex_wait --threads=1  --iterations=1000 --sync=m >> lab2b_list_mutex.csv
./lab2b_list_mutex_wait --threads=2  --iterations=1000 --sync=m >> lab2b_list_mutex.csv
./lab2b_list_mutex_wait --threads=4  --iterations=1000 --sync=m >> lab2b_list_mutex.csv
./lab2b_list_mutex_wait --threads=8  --iterations=1000 --sync=m >> lab2b_list_mutex.csv
./lab2b_list_mutex_wait --threads=12 --iterations=1000 --sync=m >> lab2b_list_mutex.csv
./lab2b_list_mutex_wait --threads=16 --iterations=1000 --sync=m >> lab2b_list_mutex.csv
./lab2b_list_mutex_wait --threads=24 --iterations=1000 --sync=m >> lab2b_list_mutex.csv
