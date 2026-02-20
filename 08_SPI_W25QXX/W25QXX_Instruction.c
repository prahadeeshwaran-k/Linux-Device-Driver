#include "W25QXX_Instruction.h"

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
	ret = wq25_send_cmd(spi, 0x066)// Enable reset
	if(ret)
		return ret;
	
	ret = wq25_send_cmd(spi, 0x099)// Reset
    usleep_range(30, 40); /* tRST = 30 us minimum */
	if(ret)
		return ret;
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

u32 w25q_read_id(struct spi_device *spi)
{
    u8 cmd = READ_ID;
    u8 id[3];
    struct spi_transfer spi_txInfo = {
        .tx_buf = &cmd,
        .len = 1,
    }

    struct spi_transfer spi_rxInfo = {
        .rx_buf = id,
        .len = 3,
    }

    struct spi_message spi_DataBuffer;

    spi_message_init(&spi_DataBuffer);
    spi_message_add_tail(&spi_txInfo, &spi_DataBuffer);
    spi_message_add_tail(&spi_rxInfo, &spi_DataBuffer);

    spi_sync(spi, &spi_DataBuffer);

    return (id[0] << 16) | (id[1] << 8) | id[2];
}

u8 w25q_init(struct spi_device *spi)
{
    w25q_reset();
    w25q_read_id(spi);
    return 0;
}