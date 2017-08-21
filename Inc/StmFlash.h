#ifndef _STMFLASH_H_
#define _STMFLASH_H_
#include "stm32f1xx_hal.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/************************** STM32 �ڲ� FLASH ���� *****************************/
#define STM32_FLASH_SIZE        512  // ��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN        1    // stm32оƬ����FLASH д��ʹ��(0������;1��ʹ��)

#define  STMFLASH_WriteAddress     0x8070000            // д�ڿ���λ�ã���ֹ�ƻ�����
#define  STMFLASH_ReadAddress      STMFLASH_WriteAddress
#define  STMFLASH_TESTSIZE         512                 //ʵ����512*2=1024�ֽ�

#define stmCountOf(a)      (sizeof(a) / sizeof(*(a)))
#define stmBufferSize      (stmCountOf(Tx_Buffer))

void StmFlashRead( uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead );
void StmFlashWrite( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );
void StmFlashTest(void);
#endif
