#include "stm32f4xx_hal.h"
#include  <errno.h>
#include  <stdio.h>
#include  <sys/stat.h>
#include  <sys/unistd.h>


#define PRINTF_UART		huart2

extern UART_HandleTypeDef PRINTF_UART;

//send chr via UART (platform dependent)
static void print_chr(char chr)
{
	HAL_UART_Transmit(&PRINTF_UART,
	(uint8_t*)&chr, 1, 1000);
}

static char wait_for_char(void)
{
	uint8_t rxed;
	HAL_StatusTypeDef res;
	
	do
	{
		res = HAL_UART_Receive(&PRINTF_UART,&rxed,1,HAL_MAX_DELAY);
	}
	while(res != HAL_OK);
	return rxed;
}



//nie sprawdzane, moze nie dzialac :P
ssize_t _read_r(struct _reent *r, int fd, void *ptr, size_t len)
{
	char *pTemp = (char*)ptr;
	for(int i=0;i<len;i++)
	{
		*pTemp = wait_for_char();
		pTemp++;
	}
    return len;
}

ssize_t _write_r(struct _reent *r, int fd, const void *ptr, size_t len)
{
	int cntr = len;
	char *pTemp = (char*)ptr;
	while(cntr--)
		print_chr(*pTemp++);
    return len;
}

