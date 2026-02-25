#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/of.h>
#include <linux/delay.h>

/* ================= META ================= */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prahadeeshwaran");
MODULE_DESCRIPTION("W25QXX SPI Flash Driver (RPi Kernel 6.12)");

/* ================= COMMANDS ================= */

#define W25Q_CMD_JEDEC_ID   0x9F

/* ================= JEDEC READ ================= */

static int w25q_read_jedec_id(struct spi_device *spi, u8 *id)
{
    u8 cmd = W25Q_CMD_JEDEC_ID;

    struct spi_transfer t[] = {
        {
            .tx_buf = &cmd,
            .len = 1,
        },
        {
            .rx_buf = id,
            .len = 3,
        },
    };

    struct spi_message m;

    spi_message_init(&m);
    spi_message_add_tail(&t[0], &m);
    spi_message_add_tail(&t[1], &m);

    return spi_sync(spi, &m);
}

/* ================= PROBE ================= */

static int w25q_probe(struct spi_device *spi)
{
    u8 id[3];

    dev_info(&spi->dev, "W25Q probe started\n");

    /* Configure SPI */
    spi->mode = SPI_MODE_3;
    spi->bits_per_word = 8;

    if (spi_setup(spi)) {
        dev_err(&spi->dev, "spi_setup failed\n");
        return -EINVAL;
    }

    /* Read JEDEC ID */
    if (w25q_read_jedec_id(spi, id) == 0) {
        dev_info(&spi->dev,
                 "JEDEC ID: %02X %02X %02X\n",
                 id[0], id[1], id[2]);
    } else {
        dev_err(&spi->dev, "JEDEC read failed\n");
    }

    dev_info(&spi->dev, "W25Q probe successful\n");
    return 0;
}

/* ================= REMOVE ================= */

static void w25q_remove(struct spi_device *spi)
{
    dev_info(&spi->dev, "W25Q device removed\n");
}

/* ================= DEVICE TREE MATCH ================= */

static const struct of_device_id w25q_of_match[] = {
    { .compatible = "winbond,w25q32" },
    { }
};
MODULE_DEVICE_TABLE(of, w25q_of_match);

/* ================= SPI DEVICE ID TABLE (IMPORTANT FIX) ================= */

static const struct spi_device_id w25q_id[] = {
    { "w25qxx", 0 },
    { }
};
MODULE_DEVICE_TABLE(spi, w25q_id);

/* ================= SPI DRIVER ================= */

static struct spi_driver w25q_driver = {
    .driver = {
        .name = "w25qxx",
        .of_match_table = w25q_of_match,
    },
    .probe    = w25q_probe,
    .remove   = w25q_remove,
    .id_table = w25q_id,
};

/* Auto creates init & exit */
module_spi_driver(w25q_driver);