#ifndef	_REPLACE_ARITHMETIC_H_
#define _REPLACE_ARITHMETIC_H_

#include <stdio.h>
#include "main_memory.h"
#include "cache_memory.h"
#include "address_transform.h"

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
	int	cache_base_add, main_base_add, t, i;

	cache_base_add = ((rand() % CACHE_BLOCK_COUNT) << A_BLOCK_BIT);	//随机找一个 cache 基址
	main_base_add = main_add - (main_add & (A_BLOCK_COUNT - 1));	//主存基址

	if( (t=find_catalog_list_by_0()) != NOT_FIND )
		cache_base_add = (t << A_BLOCK_BIT);

	for(i = 0; i < A_BLOCK_COUNT; i++)		//数据从主存替换到 cache (块) 
		cache_memory[cache_base_add + i] = main_memory[main_base_add + i];

	return cache_base_add + (main_add & (A_BLOCK_COUNT - 1));	//cache 地址
}

void modefy_catalog_list(int main_add, int cache_add)
{
	int	main_block_num, cache_block_num;

	main_block_num = (main_add >> A_BLOCK_BIT);	//主存块号
	cache_block_num = (cache_add >> A_BLOCK_BIT);	//cache 块号  等于目录表下标

	catalog_list[cache_block_num].main_block_num = main_block_num;	//填写目录表
	catalog_list[cache_block_num].cache_block_num = cache_block_num;
	catalog_list[cache_block_num].flag = 1;
}

void modefy_district_list(int main_add)
{
	int	main_district_num, main_block_num;

	main_block_num = (main_add & (CACHE_COUNT - 1)) >> A_BLOCK_BIT;
	main_district_num = main_add >> CACHE_BIT;

	district_list[main_block_num].district_num = main_district_num;
	district_list[main_block_num].flag = 1;
}

void modefy_block_list(int main_add)
{
	int	group_num, district_num, main_group_block_num, cache_group_block_num;

	group_num = (main_add >> (GROUP_BLOCK_BIT + A_BLOCK_BIT)) & (GROUP_BLOCK_COUNT - 1);;
	district_num = main_add >> CACHE_BIT;
        main_group_block_num = (main_add >> A_BLOCK_BIT) & (GROUP_BLOCK_COUNT - 1);
	
	block_list[group_num].district_num = district_num;
	block_list[group_num].main_group_block_num = main_group_block_num;
	block_list[group_num].cache_group_block_num = main_group_block_num;
	block_list[group_num].flag = 1;
}

int FIFO_replace(int main_add)
{
	int		main_base_add, cache_base_add, block_num, i;

	if( !FIFO_QUEUE_list.FIFO_flag || FIFO_QUEUE_list.FIFO_num ){

		block_num = (main_add & (CACHE_COUNT - 1)) >> A_BLOCK_BIT;
		FIFO_QUEUE_list.FIFO_QUEUE[FIFO_QUEUE_list.FIFO_num] = block_num;
		FIFO_QUEUE_list.FIFO_num = (FIFO_QUEUE_list.FIFO_num + 1) % CACHE_BLOCK_COUNT;
		FIFO_QUEUE_list.FIFO_flag = 1;
	}
	else{

		block_num = FIFO_QUEUE_list.FIFO_QUEUE[FIFO_QUEUE_list.FIFO_num];
		FIFO_QUEUE_list.FIFO_num = (FIFO_QUEUE_list.FIFO_num + 1) % CACHE_BLOCK_COUNT;
	}

	main_base_add = main_add & (~(A_BLOCK_COUNT - 1));	//主存基址
	cache_base_add = block_num << A_BLOCK_BIT;	//cache 基址
	for(i = 0; i < A_BLOCK_COUNT; i++)		//数据从主存替换到 cache (块)
		cache_memory[cache_base_add + i] = main_memory[main_base_add + i];

	return cache_base_add + (main_add & (A_BLOCK_COUNT - 1));	//cache 地址
}

int LRU_replace(int main_add)
{
	int		main_base_add, cache_base_add, block_num, i, flag = 0, j = 0;
	
	for(i = LRU_list.station + 1; i == LRU_list.station; i = (i+1) % CACHE_BLOCK_COUNT)
		if( !LRU_list.LRU_list_array[i].flag ){

			j = i;
			break ;
		}
		else if( flag < LRU_list.LRU_list_array[i].flag ){

			flag = LRU_list.LRU_list_array[i].flag;
			j = i;
		}

	block_num = (main_add & (CACHE_COUNT - 1)) >> A_BLOCK_BIT;
	LRU_list.LRU_list_array[j].block_num = block_num;
	LRU_list.LRU_list_array[j].flag = 1;
	LRU_list.station = j;

	main_base_add = main_add & (~(A_BLOCK_COUNT - 1));	//主存基址
	cache_base_add = block_num << A_BLOCK_BIT;	//cache 基址
	for(i = 0; i < A_BLOCK_COUNT; i++)		//数据从主存替换到 cache (块)
		cache_memory[cache_base_add + i] = main_memory[main_base_add + i];

	return cache_base_add + (main_add & (A_BLOCK_COUNT - 1));	//cache 地址
}

#endif
