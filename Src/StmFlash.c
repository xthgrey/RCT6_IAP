#include "StmFlash.h"
#if STM32_FLASH_SIZE < 256
  #define STM_SECTOR_SIZE  1024 //�ֽ�
#else 
  #define STM_SECTOR_SIZE	 2048
#endif

/* ˽�б��� ------------------------------------------------------------------*/
#if STM32_FLASH_WREN	//���ʹ����д 
static uint16_t STMFLASH_BUF [ STM_SECTOR_SIZE / 2 ];//�����2K�ֽ�
static FLASH_EraseInitTypeDef EraseInitStruct;
#endif

/*
funName	:StmFlashReadHalfWord
input		:faddr
output	:u16
describe:��ȡָ����ַ�İ���(16λ����)
remark	:����ַ(�˵�ַ����Ϊ2�ı���!!)
*/
uint16_t StmFlashReadHalfWord(uint32_t faddr)
{
	return *(__IO uint16_t*)faddr; 
}
/*
funName	:StmFlashRead
input		:
output	:
describe:���ڲ�flash
remark	:
*/
void StmFlashRead( uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead )   	
{
	uint16_t i;
	
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=StmFlashReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}
#if STM32_FLASH_WREN	//���ʹ����д   
/*
funName	:StmFlashWriteNoCheck
input		:
output	:
describe:������д��
remark	:
*/
void StmFlashWriteNoCheck( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )   
{ 			 		 
	uint16_t i;	
	
	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,WriteAddr,pBuffer[i]);
	  WriteAddr+=2;                                    //��ַ����2.
	}  
}
/*
funName	:StmFlashWrite
input		:
output	:
describe:д��
remark	:д��֮ǰ��У��õ�ַ�Ƿ������û���������д��
*/
void StmFlashWrite( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )	
{
  uint32_t SECTORError = 0;
	uint16_t secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain; //������ʣ���ַ(16λ�ּ���)	   
 	uint16_t i;    
	uint32_t secpos;	   //������ַ
	uint32_t offaddr;   //ȥ��0X08000000��ĵ�ַ
	
	if(WriteAddr<FLASH_BASE||(WriteAddr>=(FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	
	HAL_FLASH_Unlock();						//����
	
	offaddr=WriteAddr-FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	
	while(1) 
	{	
		StmFlashRead(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
      //�����������
      /* Fill EraseInit structure*/
      EraseInitStruct.TypeErase     = FLASH_TYPEERASE_PAGES;
      EraseInitStruct.PageAddress   = secpos*STM_SECTOR_SIZE+FLASH_BASE;
      EraseInitStruct.NbPages       = 1;
      HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);

			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			StmFlashWriteNoCheck(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}
    else
    {
      StmFlashWriteNoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		}
    if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	HAL_FLASH_Lock();//����
}
#endif

void StmFlashTest(void){
	uint16_t Tx_Buffer[]={1,2,3,4,5};
	uint16_t Rx_Buffer[stmBufferSize]={0};
	uint16_t i;
	StmFlashWrite(STMFLASH_WriteAddress,Tx_Buffer,stmBufferSize);
	for(i = 0;i < stmBufferSize;i ++){
		printf("stmFlashWrite:%d\n",(int)Tx_Buffer[i]);
	}
	StmFlashRead(STMFLASH_ReadAddress,Rx_Buffer,stmBufferSize);
	for(i = 0;i < stmBufferSize;i ++){
		printf("stmFlashRead:%d\n",(int)Rx_Buffer[i]);
	}
//	printf("%d\n",bufferCmp16(Tx_Buffer, Rx_Buffer, stmBufferSize));
}
