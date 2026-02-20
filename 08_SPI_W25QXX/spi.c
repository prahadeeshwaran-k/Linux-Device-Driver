#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <asm/types.h>

#define BUS_NUM 0
static struct spi_device *w25qxx_dev;

/*
 * 8.2.29 Read JEDEC ID (0x9F)
 *
 * For compatibility, the W25Q32FV supports multiple identification
 * instructions. Read JEDEC ID follows the JEDEC SPI serial memory standard
 * (adopted in 2003).
 *
 * Sequence:
 * 1) Drive /CS low.
 * 2) Shift instruction code 0x9F.
 * 3) Read 3 bytes (MSB first, shifted out on CLK falling edge):
 *    - Manufacturer ID (Winbond: 0xEF)
 *    - Memory Type (ID15-ID8)
 *    - Capacity (ID7-ID0)
 *
 * Refer to Figure 43a/43b and the Manufacturer/Device Identification table
 * for memory type and capacity values.
 */
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

int w25q_send_cmd(struct_device *spi, u8 opcode)
{
	struct spi_transfer spi_txInfo = {
		.tx_buf = &opcode,
		.len = 1,
	}

	struct spi_message spi_DataBuffer;

	spi_message_init(&spi_DataBuffer);
	spi_message_add_tail(&spi_txInfo, &spi_DataBuffer);

	return spi_sync(spi, &spi_DataBuffer);
}

/*
 * 8.2.43 Enable Reset (0x66) and Reset Device (0x99)
 *
 * W25Q32FV uses software reset (no dedicated RESET pin).
 *
 * Sequence requirement (SPI or QPI mode):
 * 1) Send Enable Reset (0x66)
 * 2) Send Reset Device (0x99)
 *
 * Notes:
 * - Both commands must be sent in order; this prevents accidental reset.
 * - Any command other than 0x99 after 0x66 clears reset-enable state.
 * - If cleared, a new 0x66 -> 0x99 sequence is required.
 * - After reset is accepted, device needs tRST ~= 30 us.
 * - No command is accepted during reset time.
 * - Reset returns device to power-on defaults and clears volatile states.
 *
 * Caution:
 * - If erase/program is active or suspended, reset may cause data
 *   corruption. Check BUSY and SUS bits before issuing reset sequence.
 */
int w25q_reset(struct spi_device *spi)
{
	int ret;
	ret = wq25_send_cmd(spi, 0x066)// Enable reset
	if(ret)
		return ret;
	
	ret = wq25_send_cmd(spi, 0x099)// Reset
	if(ret)
		return ret;

}

/*
 * Pull CS LOW
 * Send READ command (0x03)
 * Send 24-bit address
 * Read data bytes
 * Pull CS HIGH
 * w25q_read_id()
 */
w25q_read(addr, buffer, len){
    spi_w8r8(w25qxx_dev, READ_DATA); // send read command
}

static int __init my_init(void)
{
    struct spi_master *master; // information about spi
    struct spi_board_info spi_device_info = {
        .modalias = "wq25xx",
        .max_speed_hz = 100000,
        .bus_num = BUS_NUM,
        .chip_select = 0,
        .mode = SPI_MODE_3,
    };

    /*get access to bus*/
    master = spi_busnum_to_master(BUS_NUM);
    if (!master) {
        printk(KERN_ERR "SPI Master not found ther is no SPI NO 3.\n");
        return -ENODEV;
    }

    /*create a new device on the bus*/
    w25qxx_dev = spi_new_device(master, &spi_device_info);
    if (!w25qxx_dev) {
        printk(KERN_ERR "Failed to create SPI device.\n");
        return -ENODEV;
    }
    w25qxx_dev->bits_per_word = 8;// set the bits per word
    if (spi_setup(w25qxx_dev)) {
        printk(KERN_ERR "Failed to setup SPI device.\n");
        spi_unregister_device(w25qxx_dev);
        return -ENODEV;
    }

    return 0;
}

static void __exit my_exit(void)
{
    printk(KERN_INFO "Goodbye Kernel!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prahadeeshwaran");
MODULE_DESCRIPTION("Spi");
