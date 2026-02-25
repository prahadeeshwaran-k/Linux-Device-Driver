import spidev
import time

# Open SPI bus
spi = spidev.SpiDev()
spi.open(0, 0)        # bus 0, chip select 0

# SPI configuration (W25Q uses MODE 3)
spi.mode = 0b11
spi.max_speed_hz = 1_000_000
spi.bits_per_word = 8

print("SPI opened")

# ---------- Read JEDEC ID ----------
# Command 0x9F + 3 dummy bytes
resp = spi.xfer2([0x9F, 0x00, 0x00, 0x00])

manufacturer = resp[1]
memory_type = resp[2]
capacity = resp[3]

print("JEDEC ID:")
print(f" Manufacturer : 0x{manufacturer:02X}")
print(f" Memory Type  : 0x{memory_type:02X}")
print(f" Capacity     : 0x{capacity:02X}")

# ---------- Write Enable ----------
spi.xfer2([0x06])
print("Write Enable sent")

# ---------- Read Status Register ----------
status = spi.xfer2([0x05, 0x00])

print(f"Status Register: 0x{status[1]:02X}")

spi.close()
print("SPI closed")