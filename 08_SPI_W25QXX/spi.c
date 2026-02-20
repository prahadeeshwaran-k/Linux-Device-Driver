#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>    
#include <asm/types.h>

#define BUS_NUM 0
static struct spi_device *w25qxx_dev;




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
