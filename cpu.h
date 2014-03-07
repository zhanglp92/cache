#ifndef	_CPU_H_
#define	_CPU_H_


#include <stdio.h>
#include "main_memory.h"
#include "cache_memory.h"

/* 全相联 区表项的类型 */
typedef struct LRU_ELEMENT_TYPE{

	int	block_num;  // 区号
	int	flag;       // 有效位
}LRU_ELEMENT_TYPE; 

/* 区表类型 */
typedef struct LRU_LIST_TYPE{

	LRU_ELEMENT_TYPE	LRU_list_array[CACHE_BLOCK_COUNT];
	int			station;
}LRU_LIST_TYPE;

LRU_LIST_TYPE	LRU_list;		//模拟 LRU 表的寄存器 

typedef struct FIFO_QUEUE_TYPE{

	int     FIFO_QUEUE[CACHE_BLOCK_COUNT];
	int	FIFO_num, FIFO_flag;

}FIFO_QUEUE_TYPE;

FIFO_QUEUE_TYPE	FIFO_QUEUE_list;	//模拟 FIFO 表的寄存器


#include"replace_arithmetic.h"
#include"address_transform.h"

#define	INSTRUCTION_COUNT	100000
/* 模拟 cpu 执行指令 */
int		cpu_instruction[INSTRUCTION_COUNT];

void show_cpu_instruction(void); 
void fill_instruction_for_cpu(void); 
void cpu_run_quan(void); 
void cpu_run_direct(void); 
void cpu_run_group(void); 
void reset_LRU_list(void); 
void reset_FIFO_list(void); 

void reset_all(void)
{
	reset_caceh_memory();
	reset_LRU_list();
	reset_FIFO_list();
}


void cpu_run(void)
{
	reset_all();
	printf("quan address transform:\n");
	cpu_run_quan();
	reset_all();
	printf("direct address transform:\n");
	cpu_run_direct();
	reset_all();
	printf("group address transform:\n");
	cpu_run_group();
}

void cpu_run_group(void)
{
	static int	k, cache_add;

	reset_hit();
	for(k = 0; k < INSTRUCTION_COUNT; k++){

		if( (cache_add = group_address_transform(cpu_instruction[k])) == NOT_FIND ){
	
	//		cache_add = random_replace(cpu_instruction[k]);		//随机替换
	//		cache_add = FIFO_replace(cpu_instruction[k]);		//FIFO
			cache_add = LRU_replace(cpu_instruction[k]);		//LRU
			modefy_block_list(cpu_instruction[k]);
		}
	}
	printf("\ngroup hit probalilty : %.4lf\n", hit_probalilty());
}

void cpu_run_direct(void)
{
	static int	k, cache_add;

	reset_hit();
	for(k = 0; k < INSTRUCTION_COUNT; k++){

		if( (cache_add = direct_address_transform(cpu_instruction[k])) == NOT_FIND ){

	//		cache_add = random_replace(cpu_instruction[k]);		//随机替换
	//		cache_add = FIFO_replace(cpu_instruction[k]);		//FIFO
			cache_add = LRU_replace(cpu_instruction[k]);		//LRU
			modefy_district_list(cpu_instruction[k]);
		}
	//	printf("instruction : %d, data = %d\n", main_memory[cpu_instruction[k]], cache_memory[cache_add]);	
	}
	printf("\ndirect hit probalilty : %.4lf\n", hit_probalilty());
}

void cpu_run_quan(void)
{
	static int	k, cache_add;

	reset_hit();
	for(k = 0; k < INSTRUCTION_COUNT; k++){

		if((cache_add = quan_address_transform(cpu_instruction[k])) == NOT_FIND){

	//		cache_add = random_replace(cpu_instruction[k]);		//随机替换
	//		cache_add = FIFO_replace(cpu_instruction[k]);		//FIFO
			cache_add = LRU_replace(cpu_instruction[k]);		//LRU
			modefy_catalog_list(cpu_instruction[k], cache_add);
		}
		if( rand() % 2 ){	//改写 cache 的值

			cache_memory[cache_add] = rand() % 1000;
			catalog_list[cache_add >> A_BLOCK_BIT].flag = 0;
		}
	//	printf("instruction : %d, data = %d\n", main_memory[cpu_instruction[k]], cache_memory[cache_add]);
	}
	printf("\nquan hit probalilty : %.4lf\n", hit_probalilty());
}

void fill_instruction_for_cpu(void)
{
	int	i;

	for(i = 0; i < INSTRUCTION_COUNT; i++)
		cpu_instruction[i] = rand() % MAIN_COUNT;    //cpu 指令位数不超过主存容量
}

void show_cpu_instruction(void)
{
	int	i;

	for(i = 0; i < INSTRUCTION_COUNT; i++)
		printf("%d\t", cpu_instruction[i]);
	printf("\n");
}

void reset_LRU_list(void)
{
	int	i;

	for(i = 0; i < CACHE_BLOCK_COUNT; i++)
		LRU_list.LRU_list_array[i].block_num = LRU_list.LRU_list_array[i].flag = 0;
	LRU_list.station = 0;
}
void reset_FIFO_list(void)
{
	int	i;

	for(i = 0; i < CACHE_BLOCK_COUNT; i++)
		FIFO_QUEUE_list.FIFO_QUEUE[i] = 0;
	FIFO_QUEUE_list.FIFO_num = FIFO_QUEUE_list.FIFO_flag = 0;
}

#endif
