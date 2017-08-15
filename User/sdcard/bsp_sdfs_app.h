#ifndef __BSP_SDFS_APP_H
#define	__BSP_SDFS_APP_H

#include "stm32f10x.h"
#include "diskio.h"

void Sd_fs_test(void);
void Sd_fs_init(void);
void SDIO_NVIC_Configuration(void);
int Sdfs_new(BYTE *new_file_name, BYTE *write_buffer, BYTE buffer_size);
int Sdfs_write(BYTE *write_file_name, BYTE *write_buffer, BYTE buffer_size);
int Sdfs_read(BYTE *read_file_name, BYTE *save_buffer);
int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c);

#endif /* __BSP_SDFS_APP_H */
