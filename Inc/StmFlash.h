#ifndef _STMFLASH_H_
#define _STMFLASH_H_
#include "stm32f1xx_hal.h"
/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/************************** STM32 内部 FLASH 配置 *****************************/
#define STM32_FLASH_SIZE        512  // 所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN        1    // stm32芯片内容FLASH 写入使能(0，禁用;1，使能)

#define  STMFLASH_WriteAddress     0x8070000            // 写在靠后位置，防止破坏程序
#define  STMFLASH_ReadAddress      STMFLASH_WriteAddress
#define  STMFLASH_TESTSIZE         512                 //实际是512*2=1024字节

#define stmCountOf(a)      (sizeof(a) / sizeof(*(a)))
#define stmBufferSize      (stmCountOf(Tx_Buffer))

void StmFlashRead( uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead );
void StmFlashWrite( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );
void StmFlashTest(void);
#endif
