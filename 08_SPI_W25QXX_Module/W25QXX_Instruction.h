#ifndef W25QXX_INSTRUCTION_H
#define W25QXX_INSTRUCTION_H

#include <linux/spi/spi.h>
#include <linux/types.h>

#define READ_ID               0x9F
#define READ_DATA             0x03
#define PAGE_PROGRAM          0x02
#define SECTOR_ERASE          0x20
#define WRITE_ENABLE          0x06
#define READ_STATUS_REGISTER  0x05

int w25q_send_cmd(struct spi_device *spi, u8 opcode);
int w25q_reset(struct spi_device *spi);
int w25q_read_id(struct spi_device *spi);
int w25q_init(struct spi_device *spi);

#endif