#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <assert.h>
#include <string.h>

#define SBO 1024 	/* SBO = Super Block Offset */

unsigned short magic_num; 
unsigned int inodes_total;
unsigned int blocks_total;
unsigned int block_size;
unsigned int frag_size;
unsigned int blocks_per_g; //*
unsigned int inodes_per_g; //  g = group
unsigned int frags_per_g;  //*
unsigned int first_data_block;
unsigned int groups_total;
unsigned int last_data_block;

int fsi_fd;		/* fsi = file system image */
int super_fd;
int group_fd;
int bitmap_fd;
int inode_fd;
int directory_fd;
int indirect_fd;
int last_inode = -1;
int last_entry;
int indirect_flag = 0;

void bitmap_parser(unsigned int inode_bitmap_id, unsigned int block_bitmap_id, int k, int inode_table_id);
void inode_parser(int inode_num, int inode_table_id);
int directory_parser(int parent_inode, int block_num);
int directory_indirect_1(int parent_inode, int block_num);
int directory_indirect_2(int parent_inode, int block_num);
void directory_indirect_3(int parent_inode, int block_num);
int indirect_parser_1(int block_num);
int indirect_parser_2(int block_num);
void indirect_parser_3(int block_num);

void fs_to_fd(int argc, char** argv){
	/* fs_to_fd = file system to file descriptor */
	if(argc != 2) {
		fprintf(stderr, "ERROR: argument count is invalid for this program\n");
		exit(1);
	}
	fsi_fd = open(argv[1], O_RDONLY);
	assert(fsi_fd != -1);
}

void csv_init(){
	super_fd = 		open("super.csv", O_WRONLY | O_APPEND | O_CREAT);
	group_fd = 		open("group.csv", O_WRONLY | O_APPEND | O_CREAT);
	bitmap_fd = 	open("bitmap.csv", O_WRONLY | O_APPEND | O_CREAT);
	inode_fd =  	open("inode.csv", O_WRONLY | O_APPEND | O_CREAT);
	directory_fd = 	open("directory.csv", O_WRONLY | O_APPEND | O_CREAT);
	indirect_fd = 	open("indirect.csv", O_WRONLY | O_APPEND | O_CREAT);	
	assert(super_fd != -1);
	assert(group_fd != -1);
	assert(bitmap_fd != -1);
	assert(inode_fd != -1);
	assert(directory_fd != -1);
	assert(indirect_fd != -1);
}

void super_block_parser(){
	unsigned int ubuf_4;
	int sbuf_4;
	pread(fsi_fd, &magic_num, 2, SBO + 56);
	pread(fsi_fd, &inodes_total, 4, SBO + 0);
	pread(fsi_fd, &blocks_total, 4, SBO + 4);
	pread(fsi_fd, &ubuf_4, 4, SBO + 24);
	block_size = 1024 << ubuf_4;
	pread(fsi_fd, &blocks_per_g, 4, SBO + 32);
	pread(fsi_fd, &inodes_per_g, 4, SBO + 40);
	pread(fsi_fd, &frags_per_g, 4, SBO + 36);
	pread(fsi_fd, &first_data_block, 4, SBO + 20);
	pread(fsi_fd, &sbuf_4, 4, SBO + 28);
	if(sbuf_4 >= 0)
		frag_size = 1024 << sbuf_4;
	else
		frag_size = 1024 >> (-1)*sbuf_4;
	groups_total = blocks_total / blocks_per_g;
	last_data_block = first_data_block + blocks_total - 1; //bc off by one 
		
	/* sanity checks below */
	if(magic_num != 61267){
		fprintf(stderr, "ERROR: magic number != ef53 \n");
		exit(1);
	}
	if(block_size < 1024 || block_size > 64000){
		fprintf(stderr, "ERROR: block_size fails sanity check \n");
		exit(1);
	}
	struct stat st;
	int fsi_size;
	fstat(fsi_fd, &st);
	fsi_size = st.st_size;
	if(blocks_total > fsi_size){
		fprintf(stderr, "ERROR: block_total fails sanity check \n");
		exit(1);
	}
	if(first_data_block != 0 && first_data_block != 1){
		fprintf(stderr, "ERROR: first_data_block = %d --> fails sanity check \n", first_data_block);
		exit(1);
	}
	if(blocks_total % blocks_per_g != 0){
		fprintf(stderr, "ERROR: block_total MOD blocks_per_g fails sanity check \n");
		exit(1);
	}
	if(inodes_total % inodes_per_g != 0){
		fprintf(stderr, "ERROR: inodes_total MOD blocks_per_g fails sanity check \n");
		exit(1);
	}
	dprintf(super_fd, "%x,%d,%d,%d,%d,%d,%d,%d,%d\n", magic_num, inodes_total, blocks_total, block_size, frag_size, blocks_per_g, inodes_per_g, frags_per_g, first_data_block);			 
}

void group_parser(int i){
	// note: "contained blocks" = blocks_per_g
	int GO = ((first_data_block + 1) * block_size) + (32*i);
	unsigned short free_blocks_count;
	unsigned short free_inodes_count;
	unsigned short directories_count;
	unsigned int inode_bitmap_id;
	unsigned int block_bitmap_id;
	unsigned int inode_table_id;		
	pread(fsi_fd, &free_blocks_count, 2, GO + 12);
	pread(fsi_fd, &free_inodes_count, 2, GO + 14);
	pread(fsi_fd, &directories_count, 2, GO + 16);
	pread(fsi_fd, &inode_bitmap_id, 4, GO + 4);
	pread(fsi_fd, &block_bitmap_id, 4, GO + 0);
	pread(fsi_fd, &inode_table_id, 4, GO + 8);
	/* sanity checks below */
	if(inode_bitmap_id > blocks_per_g * block_size * (i+1)){
		fprintf(stderr, "ERROR: inode_bitmap_id fails sanity check! \n");
		exit(1);
	}
	if(block_bitmap_id > blocks_per_g * block_size * (i+1)){
		fprintf(stderr, "ERROR: block_bitmap_id fails sanity check! \n");
		exit(1);
	}
	if(inode_table_id > blocks_per_g * block_size * (i+1)){
		fprintf(stderr, "ERROR: inode_table_id fails sanity check! \n");
		exit(1);
	}
	dprintf(group_fd, "%d,%d,%d,%d,%x,%x,%x\n", blocks_per_g, free_blocks_count, free_inodes_count, directories_count, inode_bitmap_id, block_bitmap_id, inode_table_id);
	bitmap_parser(inode_bitmap_id, block_bitmap_id, i, inode_table_id);
}

void bitmap_parser(unsigned int inode_bitmap_id, unsigned int block_bitmap_id, int k, int inode_table_id){
	int i, j;
	unsigned int bool;
	unsigned int i_block = inode_bitmap_id;
	unsigned int b_block = block_bitmap_id;
	unsigned int mask = 1;	//power of 2 mask
	unsigned char byte;
	int BPIO = inodes_per_g*k; //Bitmap Parser Inodes Offset
	int BPBO = blocks_per_g*k; //Bitmap Parser Blocks Offset
	int IBO = block_size * inode_bitmap_id; // IBO = Inode Bitmap Offset 
	int BBO = block_size * block_bitmap_id; // BBO = Block Bitmap Offset 
	for(i=0; i*8<blocks_per_g; i++){
		pread(fsi_fd, &byte, 1, BBO + i);
		for(j=0; j<8; j++){
			bool = byte & mask;
			mask = mask << 1;
			if(bool)
				continue;
			else
				dprintf(bitmap_fd, "%x,%d\n", b_block, BPBO + i*8 + j + 1); //off by one bc first block should be 1 (not 0)		 
		}
		mask = 1;
	} 
	for(i=0; i*8<inodes_per_g; i++){
		pread(fsi_fd, &byte, 1, IBO + i);
		for(j=0; j<8; j++){
			bool = byte & mask;
			mask = mask << 1;
			if(bool)
				inode_parser(BPIO + i*8 + j + 1, inode_table_id);			
			else
				dprintf(bitmap_fd, "%x,%d\n", i_block, BPIO + i*8 + j + 1);			
		}
		mask = 1;
	}
}

void inode_parser(int inode_num, int inode_table_id){
	int local_index = (inode_num - 1)%inodes_per_g;
	int TIO = (inode_table_id * block_size) + (local_index * 128); //128 = inode_size //TIO = This Inode Offset
	char file_type;
	int mode;
	int i;
	unsigned short owner_id;
	unsigned short group_id;
	unsigned short link_count;
	unsigned int cre_time;
	unsigned int mod_time;
	unsigned int acc_time;
	unsigned int file_size;
	unsigned int iblocks_total;
	unsigned int iblock_ptr_array[15];
	unsigned short ubuf_2;
	unsigned int ubuf_4;
	pread(fsi_fd, &ubuf_2, 2, TIO + 0);
	mode = ubuf_2; 
	ubuf_2 = ubuf_2 >> 12;
	switch(ubuf_2){
		case 4:
			file_type = 'd';
			break;
		case 8:
			file_type = 'f';
			break;
		case 10:
			file_type = 's';
			break;
		default:
			file_type = '?';
			break;
	}
	pread(fsi_fd, &owner_id, 2, TIO + 2);
	pread(fsi_fd, &group_id, 2, TIO + 24);
	pread(fsi_fd, &link_count, 2, TIO + 26);
	pread(fsi_fd, &cre_time, 4, TIO + 12);
	pread(fsi_fd, &mod_time, 4, TIO + 16);
	pread(fsi_fd, &acc_time, 4, TIO + 8);
	pread(fsi_fd, &file_size, 4, TIO + 4);
	pread(fsi_fd, &ubuf_4, 4, TIO + 28);
	iblocks_total = ubuf_4*512/block_size; 	/* see 3.5.10 in nongnu webpg */
	for(i=0; i<15; i++){
		pread(fsi_fd, &ubuf_4, 4, TIO + 40 + 4*i);
		iblock_ptr_array[i] = ubuf_4;
	}
	dprintf(inode_fd, "%d,%c,%o,%d,%d,%d,%x,%x,%x,%d,%d,", inode_num, file_type, mode, owner_id, group_id, link_count, cre_time, mod_time, acc_time, file_size, iblocks_total);
	for(i=0; i<14; i++)
		dprintf(inode_fd, "%x,", iblock_ptr_array[i]);
	dprintf(inode_fd, "%x\n", iblock_ptr_array[14]);
	if(file_type == 'd')
		for(i=0; i< 12; i++)
			directory_parser(inode_num, iblock_ptr_array[i]);
		directory_indirect_1(inode_num, iblock_ptr_array[12]); 
		directory_indirect_2(inode_num, iblock_ptr_array[13]); 
		directory_indirect_3(inode_num, iblock_ptr_array[14]);
	indirect_parser_1(iblock_ptr_array[12]);
	indirect_parser_2(iblock_ptr_array[13]);
	indirect_parser_3(iblock_ptr_array[14]);
}

int directory_parser(int parent_inode, int block_num){
	int CO = block_num * block_size; 	// current offset
	int EO = CO + block_size; 			// ending offset
	unsigned int inode_num;
	unsigned short rec_len;
	unsigned char name_len;
	char name_buf;
	int i;
	//char *name = NULL;
	int rec_num = 0;
	if(block_num == 0)
		return 0; 
	if(block_num < first_data_block || block_num > last_data_block){
			fprintf(stderr, "ERROR: directory data block out of range = sanity check failure! \n");
			fprintf(stderr, "block_num = %d, first_data_block = %d, last_data_block = %d \n", block_num, first_data_block, last_data_block);
			exit(1);
		}	
	if(parent_inode == last_inode)
		rec_num = last_entry;	
	while(CO < EO){
		pread(fsi_fd, &rec_len , 2, CO + 4);
		pread(fsi_fd, &inode_num, 4, CO + 0);		
		if(inode_num == 0){
			rec_num++;
			CO = CO + rec_len;
			if(rec_len == 0)
				return 0;
			else
				continue;
		}
		pread(fsi_fd, &name_len, 1, CO + 6);	
		if(rec_len > 1024 || rec_len < 8){
			fprintf(stderr, "ERROR: entry length fails sanity check \n");
			exit(1);
		}
		dprintf(directory_fd, "%d,%d,%d,%d,%d,\"", parent_inode, rec_num, rec_len, name_len, inode_num);
		for(i=0; i<name_len; i++){
			pread(fsi_fd, &name_buf, 1, CO + 8 + i);
			dprintf(directory_fd, "%c", name_buf);
		}
		dprintf(directory_fd, "\"\n");
		rec_num++;
		CO = CO + rec_len;
	}	
	last_inode = parent_inode;
	last_entry = rec_num;
	return 1;
}

int directory_indirect_1(int parent_inode, int block_num){
	int cont = 1;
	int i;
	int CO = block_num * block_size; 	// current offset
	int EO = CO + block_size;			// ending offset
	int block_num_1;	
	if(block_num == 0)
		return 0; 
	if(block_num < first_data_block || block_num > last_data_block){
		fprintf(stderr, "ERROR: directory1 data block out of range = sanity check failure! \n");
		exit(1);
	}	
		
	while(CO < EO && cont){
		pread(fsi_fd, &block_num_1, 4, CO);
		cont = directory_parser(parent_inode, block_num_1);
		CO = CO + 4;	
	} 
	if(cont == 0)
		return 0;
	else
		return 1;

}

int directory_indirect_2(int parent_inode, int block_num){
	int cont = 1;
	int i;
	int CO = block_num * block_size; 	// current offset
	int EO = CO + block_size;			// ending offset
	int block_num_2;
	if(block_num == 0)
		return 0; 
	if(block_num < first_data_block || block_num > last_data_block){
		fprintf(stderr, "ERROR: directory2 data block out of range = sanity check failure! \n");
		exit(1);
	}	
	while(CO < EO && cont){
		pread(fsi_fd, &block_num_2, 4, CO);
		cont = directory_indirect_1(parent_inode, block_num_2);
		CO = CO + 4;		
	} 
	if(cont == 0)
		return 0;
	else
		return 1;
}

void directory_indirect_3(int parent_inode, int block_num){
	int cont = 1;
	int i;
	int CO = block_num * block_size; 	// current offset
	int EO = CO + block_size;			// ending offset
	int block_num_3;
	if(block_num == 0)
		return; 
	else if(block_num < first_data_block || block_num > last_data_block){
		fprintf(stderr, "ERROR: directory3 data block out of range = sanity check failure! \n");
		exit(1);
	}	
	while(CO < EO && cont){
		pread(fsi_fd, &block_num_3, 4, CO);
		cont = directory_indirect_2(parent_inode, block_num_3);
		CO = CO + 4;		
	} 
}

int indirect_parser_1(int block_num){
	int CO = block_num * block_size; 	// current offset
	int EO = CO + block_size; 			// ending offset
	unsigned int new_block_num;
	int rec_num = 0;
	if(block_num == 0)
		return 0;
	if(block_num < first_data_block || block_num > last_data_block){
		fprintf(stderr, "ERROR: indirect1 data block out of range = sanity check failure! \n");
		fprintf(stderr, "block_num = %d, first_data_block = %d, last_data_block = %d \n", block_num, first_data_block, last_data_block);
		exit(1);
	}				
	while(CO < EO){
		pread(fsi_fd, &new_block_num, 4, CO);		
		if(new_block_num == 0)
			return 0;
		if(new_block_num < first_data_block || new_block_num > last_data_block){
			fprintf(stderr, "ERROR: indirect1 data block out of range = sanity check failure! \n");
			fprintf(stderr, "new_block_num = %d, first_data_block = %d, last_data_block = %d \n", new_block_num, first_data_block, last_data_block);
			exit(1);
		}
		dprintf(indirect_fd, "%x,%d,%x\n", block_num, rec_num, new_block_num);
		rec_num++;
		CO = CO + 4;
	}	
	return 1;
}

int indirect_parser_2(int block_num){
	int cont = 1;
	int i;
	int CO = block_num * block_size; 	// CO = current offset
	int EO = CO + block_size;			// EO = ending offset
	unsigned int block_num_2;
	if(block_num == 0)
		return 0;
	if(block_num < first_data_block || block_num > last_data_block){
		fprintf(stderr, "ERROR: indirect2 data block out of range = sanity check failure! \n");
		exit(1);
	}				
	while(CO < EO && cont){
		pread(fsi_fd, &block_num_2, 4, CO);
		cont = indirect_parser_1(block_num_2);
		CO = CO + 4;		
	} 
	if(cont == 0)
		return 0;
	else
		return 1;
}

void indirect_parser_3(int block_num){
	int cont = 1;
	int i;
	int CO = block_num * block_size; 	// CO = current offset
	int EO = CO + block_size;			// EO = ending offset
	unsigned int block_num_3;
	if(block_num == 0)
		return;
	if(block_num < first_data_block || block_num > last_data_block){
		fprintf(stderr, "ERROR: indirect3 data block out of range = sanity check failure! \n");
		exit(1);
	}				
	while(CO < EO && cont){
		pread(fsi_fd, &block_num_3, 4, CO);
		cont = indirect_parser_2(block_num_3);
		CO = CO + 4;		
	} 
}

int main(int argc, char** argv) {
	int i;
	fs_to_fd(argc, argv);
	csv_init();
	super_block_parser();
	for(i=0;i<groups_total;i++)
		group_parser(i);
	close(directory_fd);
return 0;
}