import board
import busio


def i2c_read_reg(i2cbus, addr, reg, result):
    while not i2cbus.try_lock():
        pass
    try:
        i2cbus.writeto_then_readfrom(addr, bytes([reg]), result)
        return result
    finally:
        i2cbus.unlock()
        return None


def i2c_write_reg(i2cbus, addr, reg, data):
    while not i2cbus.try_lock():
        pass
    try:
        buf = bytearray(1)
        buf[0] = reg
        buf.extend(data)
        i2cbus.writeto(addr, buf)
    finally:
        i2cbus.unlock()


i2c = busio.I2C(board.SCL, board.SDA)


while not i2c.try_lock():
    pass
a = [hex(x) for x in i2c.scan()]
print(a)
i2c.unlock()

mc1_address = 54
mc1_reg = 63
enable = 1
frequency = 2
wavetype = 1

frequency_opt = ["00", "01", "10", "11"]
binary = [
    "0000",
    "0001",
    "0010",
    "0011",
    "0100",
    "0101",
    "0110",
    "0111",
    "1000",
    "1001",
    "1010",
    "1011",
    "1100",
    "1101",
    "1110",
    "1111",
]

mc_cmd = binary.index((str(enable) + frequency_opt[frequency] + str(wavetype)))


i2c_write_reg(i2c, mc1_address, mc1_reg, bytes([0, mc_cmd]))
