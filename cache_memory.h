#ifndef _CACHE_MEMORY_H_
#define	_CACHE_MEMORY_H_

#include <stdio.h>

/* cache 里的数据信息 */ 
#define	CACHE_COUNT		256	//2^8个存储单元		分为2^5个块
#define CACHE_BIT		8
#define CACHE_BLOCK_COUNT	32
#define	A_BLOCK_BIT		3	//一个块 3 位
#define A_BLOCK_COUNT		8
#define	GROUP_BLOCK_BIT		2
#define GROUP_BLOCK_COUNT	4
#define GROUP_BIT		3
#define GROUP_COUNT		8

/* cache 的元素 */
typedef int     CACHE_TYPE;

/* 数组模拟cache */
CACHE_TYPE       cache_memory[CACHE_COUNT];

/* cache 的初始化操作 */
void creat_cache_memory(void);
void reset_caceh_memory(void);
void show_cache_memory_info(void);

void creat_cache_memory(void)
{
    int i;

    for(i = 0; i < CACHE_COUNT; i++)
        cache_memory[i] = rand() % 100;		//代表上次残留的垃圾数据
}

void reset_caceh_memory(void)
{
	int	i;

	for(i = 0; i < CACHE_COUNT; i++)
		cache_memory[i] = 0;
}

void show_cache_memory_info(void)
{
    int     i;

    for(i = 0; i < CACHE_COUNT; i++)
        printf("%d\t", cache_memory[i]);
	printf("\n");
}

#endif

