#ifndef	_REPLACE_ARITHMETIC_H_
#define _REPLACE_ARITHMETIC_H_

#include<stdio.h>
#include"main_memory.h"
#include"cache_memory.h"
#include"address_transform.h"

int random_replace(int main_add);
int find_catalog_list_by_0(void);


int find_catalog_list_by_0(void)
{
	int	i;

	for(i = 0; i < LIST_LINE; i++)
		if( !catalog_list[i].main_block_num &&
				 !catalog_list[i].cache_block_num && !catalog_list[i].flag )
			return i;

	return NOT_FIND;
}

int random_replace(int main_add)
{
	int	cache_base_add, main_base_add, t, i, num;

	num = rand() % CACHE_BLOCK_COUNT;		//在目录表中随机找一行

	if( (t=find_catalog_list_by_0()) != NOT_FIND )
		num = t;
	main_base_add = (main_add >> A_BLOCK_BIT);	//主存块号

	catalog_list[cache_base_add].main_block_num = main_base_add;	//填写目录表
	catalog_list[cache_base_add].cache_block_num = cache_base_add;
	catalog_list[cache_base_add].flag = 1;

	cache_base_add = num << A_BLOCK_BIT;		//cache 基址
	main_base_add = main_base_add << A_BLOCK_BIT;	//主存基址

	for(i = 0; i < A_BLOCK_COUNT; i++)		//数据从主存替换到 cache
		cache_memory[cache_base_add + i] = main_memory[main_base_add + i];

	return cache_base_add + (main_add - main_base_add);	//拼接 cache 地址
}


/*
int FIFO_replace(int main_add)
{
	int		district_num, block_num, t;	//区号和块号
	static int	FIFO_QUEUE[CACHE_BLOCK_COUNT], pre, last;

	district_num = main_add >> CACHE_BIT;
	block_num = ((t = main_add - (district_num << CACHE_BIT)) >> A_BLOCK_BIT);

	if( (pre == last) || 
		(!district_list[block_num].district_num && !district_list[block_num].flag) ){

		district_list[block_num].district_num = district_num;
		district_list[block_num].flag = 1;
		FIFO_QUEUE[last] = block_num;
		last = (last + 1) % CACHE_BLOCK_COUNT;
	}
	else{

		district_list[FIFO_QUEUE[pre]].district_num = district_num;
		district_list[FIFO_QUEUE[pre]].flag = 1;
		pre = (pre + 1) % CACHE_BLOCK_COUNT;
	}

	return t;	//cache 地址
}
*/

#endif
