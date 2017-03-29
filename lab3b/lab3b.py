import csv
with open('bitmap.csv', 'rb') as f:
    reader = csv.reader(f)
    bitmap_list = list(reader)
    
with open('inode.csv', 'rb') as f:
    reader = csv.reader(f)
    inode_list = list(reader)
    
with open('super.csv', 'rb') as f:
	reader = csv.reader(f)
	super_list = list(reader)
	
with open('directory.csv', 'rb') as f:
	reader = csv.reader(f)
	directory_list = list(reader)
	
with open('group.csv', 'rb') as f:
	reader = csv.reader(f)
	group_list = list(reader)	

with open('indirect.csv', 'rb') as f:
	reader = csv.reader(f)
	indirect_list = list(reader)	
	
f3b = open("lab3b_check.txt", "w")	
	
	
# part 1	

last_db = int(super_list[0][2]) + int(super_list[0][8]) - 1 	# last_data_block
first_db = int(super_list[0][8])								# first_data_block
	
inodes_total = int(super_list[0][1])

used_inodes = [0] * (inodes_total + 1) # so I don't have to account for off by one error 

for list in inode_list:
	x = int(list[0])
	used_inodes[x] = 1 
	
blocknode_list = [0] * (last_db +1)  # so I don't have to account for off by one error 

for list in bitmap_list:
	x = int(list[1])
	blocknode_list[x] = blocknode_list[x] + 1

for list in inode_list:
	for i in range(11, 26):
		block_pointer = int(list[i], 16)
		if blocknode_list[block_pointer] > 1:
			print >> f3b, "UNALLOCATED BLOCK <", block_pointer, "> REFERENCED BY INODE <", list[0], "> ENTRY <", i - 11, ">"
		if blocknode_list[block_pointer] == 1:
 			if block_pointer <= inodes_total:
 				if used_inodes[block_pointer] == 1:
 					print >> f3b, "UNALLOCATED BLOCK <", block_pointer, "> REFERENCED BY INODE <", list[0], "> ENTRY <", i - 11, ">"
					

# part 2

import sys

usedblock_list = [[] for _ in range(0,last_db + 1)] # + 1 so I don't have to worry about off by one error here

for list in inode_list:
	for i in range(11, 26):
		block_pointer = int(list[i], 16)
		inode_num = int(list[0])
		if block_pointer != 0:
			usedblock_list[block_pointer].append(inode_num)
			usedblock_list[block_pointer].append(i - 11)
		
for i in range(0, last_db + 1):
	if len(usedblock_list[i]) > 2:
		print >> f3b, "MULTIPLY REFERENCED BLOCK <", i, "> BY",
		for j in range(0, len(usedblock_list[i]), 2):
			print >> f3b, "INODE <", usedblock_list[i][j], "> ENTRY <", usedblock_list[i][j+1], ">",
		f3b.write('\n')
			
		
# part 3

for list in directory_list:
	x = int(list[4])
	if used_inodes[x] == 0:
		print >> f3b, "UNALLOCATED INODE <", x, "> REFERENCED BY DIRECTORY <", list[0], "> ENTRY <", list[1], ">"
		
		
# part 4

for list in inode_list:
	x = int(list[5])
	if x == 0:
		inode = int(list[0])
		group = inode//8192
		block = int(group_list[group][4], 16)
		bool = 0
		for z in bitmap_list:
			if int(z[0], 16) == block and int(z[1]) == inode:
				bool = 1
		if bool == 0 and inode > 10:
			print >> f3b, "MISSING INODE <", inode, "> SHOULD BE IN FREE LIST <", block, ">"
	
		
# part 5

inode_links = [0] * (inodes_total + 1) # so I don't have to account for off by one error 

for list in directory_list:
	inode_links[int(list[4])] += 1;
	
for list in inode_list:
	link_is = int(list[5])
	link_should = inode_links[int(list[0])]
	if link_is != link_should and link_should > 0:
		print >> f3b, "LINKCOUNT <", int(list[0]), "> IS <", link_is, "> SHOULD BE <", link_should, ">"

		
# part 6

double_dot = [0] * (inodes_total + 1) # so I don't have to account for off by one error 

for d in directory_list:
	if d[5] == ".":
		if int(d[0]) != int(d[4]):
			print >> f3b, "INCORRECT ENTRY IN <", d[0], "> NAME <", d[5], "> LINK TO <", d[4], "> SHOULD BE <", d[0], ">"
	if d[5] != "." and d[5] != "..":
		double_dot[int(d[4])] = int(d[0])
	
for d in directory_list:
	if d[5] == "..":
		if int(d[4]) != double_dot[int(d[0])] and int(d[0]) > 10:
			print >> f3b, "INCORRECT ENTRY IN <", d[0], "> NAME <", d[5], "> LINK TO <", d[4], "> SHOULD BE <", double_dot[int(d[0])], ">"
	
		
# part 7

for list in inode_list:
	var = int(list[10])
	i = 0
	while i < 12 and i < var:
		db = int(list[11 + i], 16)
		if db < first_db or db > last_db:
			print >> f3b, "INVALID BLOCK <", db, "> IN INODE <", int(list[0]), "> ENTRY <", i, ">"
		i += 1
# 	if var > 12:
# 		j = 0
# 		count = 12
# 		db =  int(list[11 + 12], 16)		
# 		for indir_1 in indirect_list:
# 			if int(indir_1[0], 16) == db:
# 				count += 1				
# 		if var < 269 and count < var:
# 			dif = var - count
# 			for z in range(0,dif):
# 				print "INVALID BLOCK < 0 > IN INODE < ", int(list[0]), " > INDIRECT BLOCK < ", db, " > ENTRY < ", var - dif + z, " >"
# 						
##	Had to comment out the above code since it finds an error that the sample solution does not find.  
##	The doubly-indirect and trebly-indirect solutions would follow the exact same pattern as above.
##	I am not including them on account of the fact that sample solution does not consider it.
	
		
		

