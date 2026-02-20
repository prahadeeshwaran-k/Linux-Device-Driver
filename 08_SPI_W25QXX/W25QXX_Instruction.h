#ifndef W25QXX_INSTRUCTION_H
#define W25QXX_INSTRUCTION_H

#define READ_ID               0x9F    
#define READ_DATA             0x03    
#define PAGE_PROGRAM          0x02    
#define SECTOR_ERASE          0x20    
#define WRITE_ENABLE          0x06    
#define READ_STATUS_REGISTER  0x05

w25q_init();
w25q_read_id();
w25q_read(addr, buffer, len);
w25q_write_page(addr, buffer, len);
w25q_sector_erase(addr);
w25q_is_busy();

#endif