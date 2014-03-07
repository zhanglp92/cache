#ifndef _MAIN_MEMORY_H_
#define	_MAIN_MEMORY_H_

#include <stdio.h>

/* 宏定义一些数据信息 */
#define MAIN_COUNT		4096	// 2^12	个存储单元	分为2^9个块 
#define MAIN_BIT		12 
#define MAIN_BLOCK_COUNT	512 
#define A_BLOCK_BIT    		3       //一个块 3 位 
#define A_BLOCK_COUNT		8
#define GROUP_BLOCK_BIT         2
#define GROUP_BLOCK_COUNT       4
#define GROUP_BIT               3
#define GROUP_COUNT             8


/* 主存单元 */
typedef int	MAIN_TYPE;

/* 数组模拟主存 */
MAIN_TYPE	main_memory[MAIN_COUNT];

/* 主存单元的处理 */
void creat_main_memory(void);
void reset_main_memory(void);
void show_main_memory_info(void);


/* 模拟主存，里边的随机值 */
void creat_main_memory(void)
{
	int		i;

	for(i = 0; i < MAIN_COUNT; i++)
		main_memory[i] = rand() % 100;
}

/* 清空主存 */
void reset_main_memory(void)
{
        int     i;

        for(i = 0; i < MAIN_COUNT; i++)
                main_memory[i] = 0;
}

/* 打印主存 */
void show_main_memory_info(void)
{
	int	i;

	for(i = 0; i < MAIN_COUNT; i++)
		printf("%d\t", main_memory[i]);
	printf("\n");
}

#endif

