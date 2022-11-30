#include <stdio.h>
#include <generated/autoconf.h>

int func(void)
{
#ifdef CONFIG_TS
    printf("Hello I am func!\n");
#else
    printf("Hello I am func!!!!!!!!!\n");
#endif
    return 0;
}
