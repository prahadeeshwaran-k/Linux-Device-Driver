#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>

#include "W25QXX_Instruction.h"

int w25q_send_cmd(struct spi_device *spi, u8 opcode)
{
    struct spi_transfer t = {
        .tx_buf = &opcode,
        .len = 1,
    };

    struct spi_message m;

    spi_message_init(&m);
    spi_message_add_tail(&t, &m);

    return spi_sync(spi, &m);
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
 * - Reset returns device to power-on defaults and clears volatile states
 *   (e.g., WEL, suspend status, read parameters, continuous read mode,
 *   wrap settings, volatile status bits).
 *
 * Caution:
 * - If erase/program is active or suspended, reset may cause data
 *   corruption. Check BUSY and SUS bits before issuing reset sequence.
 */

int w25q_reset(struct spi_device *spi)
{
    int ret;

    /* Enable reset (0x66) */
    ret = w25q_send_cmd(spi, 0x66);
    if (ret)
        return ret;

    /* Reset device (0x99) */
    ret = w25q_send_cmd(spi, 0x99);
    if (ret)
        return ret;

    usleep_range(30, 40);

    return 0;
}


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

int w25q_read_id(struct spi_device *spi)
{
    u8 cmd = READ_ID;
    u8 id[3] = {0};

    struct spi_transfer xfers[] = {
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
    spi_message_add_tail(&xfers[0], &m);
    spi_message_add_tail(&xfers[1], &m);

    spi_sync(spi, &m);

    return (id[0] << 16) | (id[1] << 8) | id[2];
}

int w25q_init(struct spi_device *spi)
{
    int id;

    w25q_reset(spi);

    id = w25q_read_id(spi);

    pr_info("W25Q ID       : 0x%02X\n", id & 0xFF);
    pr_info("W25Q Type     : 0x%02X\n", (id >> 8) & 0xFF);
    pr_info("W25Q Capacity : 0x%02X\n", (id >> 16) & 0xFF);

    return 0;
}

/*
 * Pull CS LOW
 * Send READ command (0x03)
 * Send 24-bit address
 * Read data bytes
 * Pull CS HIGH
 * w25q_read_id()
 */