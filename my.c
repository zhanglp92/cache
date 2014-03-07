#include <stdio.h>
#include "cpu.h"

int main(void)
{
	creat_main_memory();
    creat_cache_memory();

	fill_instruction_for_cpu();
	cpu_run();

    return 0;
}
