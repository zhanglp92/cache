#ifndef	_ADDRESS_TRANSFORM_H_
#define _ADDRESS_TRANSFORM_H_

#include<stdio.h>
#include"main_memory.h"
#include"cache_memory.h"

#define	LIST_LINE		CACHE_BLOCK_COUNT	//2^5行 cache 块数
#define DISTRICT_LINE		16      //MAIN_BLOCK_COUNT/CACHE_BLOCK_NUM
#define	BLOCK_LINE		8	//CACHE_BLOCK_COUNT/(GROUP_BLOCK_COUNT + A_BLOCK_COUNT)
#define NOT_FIND	-1

typedef struct CATALOG_TYPE{

	int	main_block_num;
	int	cache_block_num;
	char	flag;
}catalog_type;		//目录表的类型

catalog_type	catalog_list[LIST_LINE];
static int	hit, count;

void reset_hit(void);
double hit_probalilty(void);


int quan_address_transform(int cpu_struction);

int quan_address_transform(int cpu_struction)
{

	int	main_block_num, cache_add, main_add, i;

	main_block_num = cpu_struction >> A_BLOCK_BIT;
	for(i = 0; i < LIST_LINE; i++)
		if(catalog_list[i].main_block_num == main_block_num){

			cache_add = (catalog_list[i].cache_block_num << A_BLOCK_BIT) + cpu_struction - (main_block_num << A_BLOCK_BIT);
			if( !catalog_list[i].flag ){	//拼接 cache 地址

				main_memory[cpu_struction] = cache_memory[cache_add];
				catalog_list[i].flag = 1;
			}
			else
				hit++;

			return cache_add;
		}
	count++;

	return NOT_FIND;
}

typedef struct DISTRICT_TYPE{

	int	district_num;
	int	flag;

}district_type;		//区表的类型

district_type	district_list[DISTRICT_LINE];

int direct_address_transform(int cpu_struction);

int direct_address_transform(int cpu_struction)
{
	int	main_district_num, main_block_num, main_block_add, t;
	int	cache_add;

	main_district_num = cpu_struction >> CACHE_BIT;
	main_block_num = (t = cpu_struction - (main_district_num << CACHE_BIT)) >> A_BLOCK_BIT;
	main_block_add = t - (main_block_num << A_BLOCK_BIT);
	cache_add = t;		//拼接的cache地址

	if( district_list[main_block_num].district_num == main_district_num ){

		if( district_list[main_block_num].flag == 1 ){	//命中

			hit++;
			return cache_add;
		}
		else{		//块失效 需 主存-(写)->cache; (主存已被改写)

			cache_memory[cache_add] = main_memory[cpu_struction];
			district_list[main_block_num].flag = 1;

			return cache_add;
		}
	}
	else
		if( district_list[main_block_num].flag == 1 ){	//块失效 需 cache-(写)->主存； (cache已被改写)

			main_memory[district_list[main_block_num].district_num + cache_add] = cache_memory[cache_add];
			cache_memory[cache_add] = main_memory[cpu_struction];	//重写cache;

			return cache_add;
		}
	count++;

	return NOT_FIND;
}


typedef struct BLOCK_TYPE{

	int	district_num;
	int	main_group_block_num;
	int	cache_group_block_num;
	int	flag;
	
}block_type;	//块表

block_type	block_list[BLOCK_LINE];

int group_address_transform(int cpu_struction);

int group_address_transform(int cpu_struction)
{
	int	group_num;
	int	main_district_num, main_group_block_num, main_block_add;
	int	cache_add;

	group_num = (cpu_struction >> (GROUP_BLOCK_BIT + A_BLOCK_BIT)) & (GROUP_BLOCK_COUNT - 1);
	main_district_num = cpu_struction >> CACHE_BIT;
	main_group_block_num = (cpu_struction >> A_BLOCK_BIT) & (GROUP_BLOCK_COUNT - 1);
	main_block_add = cpu_struction & (A_BLOCK_COUNT - 1);
	cache_add = (((group_num << GROUP_BLOCK_BIT) + block_list[group_num].main_group_block_num) << A_BLOCK_BIT) + main_block_add;

	if( block_list[group_num].district_num == main_district_num ){

		if( block_list[group_num].main_group_block_num == main_group_block_num ){

			if( block_list[group_num].flag == 1 ){

				hit++;
				return cache_add;
			}
			else{		//主存 -> cache

				cache_memory[cache_add] = main_memory[cpu_struction];
				block_list[group_num].flag = 1;

				return cache_add;
			}
		}
	}
	count++;

	return cache_add;
}



void reset_hit(void)
{
	hit = 0;
	count = 0;
}
double hit_probalilty(void)
{
	if( !count )
		return -1;

	return ((double)(hit))/count;
}


#endif
