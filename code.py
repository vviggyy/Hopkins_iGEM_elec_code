import board
import displayio
import terminalio
import time
import binascii
from digitalio import DigitalInOut, Direction, Pull

# can try import bitmap_label below for alternative
from adafruit_display_text import label
import adafruit_displayio_sh1107

displayio.release_displays()
# oled_reset = board.D9

# addresses
adc_addr = 84
rheostat_addr = 44
mc1_address = 54

# Define i2c commands
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

def read_current(i2c, addr, n):
    while not i2c.try_lock():
        pass
    x = 0
    voltages = []
    while x < n:
        result = bytearray(2)
        i2c.readfrom_into(addr, result)
        bitstring = str(bin(int(binascii.hexlify(result).decode(),16)))
        if len(bitstring) < 14:
            adc_reading = int(bitstring[2:-2],2)
        else:
            n = len(bitstring) - 12
            adc_reading = int(bitstring[n:-2],2)
        voltages.append(5*adc_reading/1023)
        x+=1
    i2c.unlock()
    Vavg = sum(voltages)/len(voltages)
    Iavg = 10*Vavg - 25
    return Iavg

# OLED buttons
A = DigitalInOut(board.D9)
A.direction = Direction.INPUT
A.pull = Pull.UP
B = DigitalInOut(board.D6)
B.direction = Direction.INPUT
B.pull = Pull.UP
C = DigitalInOut(board.D5)
C.direction = Direction.INPUT
C.pull = Pull.UP


# Use for I2C
i2c = board.I2C()
display_bus = displayio.I2CDisplay(i2c, device_address=0x3C)

# SH1107 is vertically oriented 64x128
WIDTH = 128
HEIGHT = 64
BORDER = 2

display = adafruit_displayio_sh1107.SH1107(
    display_bus, width=WIDTH, height=HEIGHT, rotation=0
)

# Make the display context
splash = displayio.Group()
display.show(splash)

color_bitmap = displayio.Bitmap(WIDTH, HEIGHT, 1)
color_palette = displayio.Palette(1)
color_palette[0] = 0xFFFFFF  # White

bg_sprite = displayio.TileGrid(color_bitmap, pixel_shader=color_palette, x=0, y=0)
splash.append(bg_sprite)

# Draw a smaller inner rectangle in black
inner_bitmap = displayio.Bitmap(WIDTH - BORDER * 2, HEIGHT - BORDER * 2, 1)
inner_palette = displayio.Palette(1)
inner_palette[0] = 0x000000  # Black
inner_sprite = displayio.TileGrid(
    inner_bitmap, pixel_shader=inner_palette, x=BORDER, y=BORDER
)
splash.append(inner_sprite)

# button test
stop = False
while not stop:
    text1 = "Press A to enable"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=16)
    splash.append(text_area)
    text1 = "Press B to stop"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=32)
    splash.append(text_area)
    enable = ""
    while len(enable) == 0:
        A.pull = Pull.UP
        B.pull = Pull.UP
        if not A.value:
            enable = "1"
        elif not B.value:
            enable = "0"
        pass

    #splash.append(bg_sprite)
    time.sleep(0.1)
    splash.pop(-1)
    splash.pop(-1)
    text1 = "Choose wave type:"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=8)
    splash.append(text_area)
    text1 = "A) DC"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=32)
    splash.append(text_area)
    text1 = "B) Switching"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=44)
    splash.append(text_area)
    wave_type = ""
    while len(wave_type) == 0:
        A.pull = Pull.UP
        B.pull = Pull.UP
        if not A.value:
            wave_type = "1"
        elif not B.value:
            wave_type = "0"
        pass
    time.sleep(0.1)
    splash.pop(-1)
    splash.pop(-1)
    splash.pop(-1)
    text1 = "Choose frequency:"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=8)
    splash.append(text_area)
    text1 = "A)0 kHz   B)167 kHz"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=32)
    splash.append(text_area)
    text1 = "C)More options"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=44)
    splash.append(text_area)
    frequency = ""
    while len(frequency) == 0:
        A.pull = Pull.UP
        B.pull = Pull.UP
        C.pull = Pull.UP
        if not A.value:
            frequency = "00"
        elif not B.value:
            frequency = "01"
        elif not C.value:
            splash.pop(-1)
            splash.pop(-1)
            text1 = "A)333 kHz B)500 kHz"  # overly long to see where it clips
            text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=32)
            splash.append(text_area)
            text1 = "C)Back"  # overly long to see where it clips
            text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=44)
            splash.append(text_area)
            a = ""
            while len(a) == 0:
                A.pull = Pull.UP
                B.pull = Pull.UP
                C.pull = Pull.UP
                if not A.value:
                    frequency = "10"
                    a+="a"
                if not B.value:
                    frequency = "11"
                    a+="b"
                if not C.value:
                    splash.pop(-1)
                    splash.pop(-1)
                    text1 = "A)0 kHz   B)167 kHz"  # overly long to see where it clips
                    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=32)
                    splash.append(text_area)
                    text1 = "C)More options"  # overly long to see where it clips
                    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=44)
                    splash.append(text_area)
                    a+="c"
                pass
            pass

    splash.pop(-1)
    splash.pop(-1)
    splash.pop(-1)
    print(enable+frequency+wave_type)

    text2 = "Command submitted:"
    text_area2 = label.Label(
        terminalio.FONT, text=text2, scale=1, color=0xFFFFFF, x=12, y=28
    )
    splash.append(text_area2)
    text2 = enable+frequency+wave_type
    text_area2 = label.Label(
        terminalio.FONT, text=text2, scale=1, color=0xFFFFFF, x=12, y=40
    )
    splash.append(text_area2)

    time.sleep(2)
    splash.pop(-1)
    splash.pop(-1)
    text1 = "Code running..."  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=8)
    splash.append(text_area)
    text1 = "Current: "+enable+frequency+wave_type  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=20)
    splash.append(text_area)

    # Scan for devices
    while not i2c.try_lock():
        pass

    a = []

    while len(a) == 0:
        time.sleep(3)
        a = [hex(x) for x in i2c.scan()]

    print(a)
    i2c.unlock()

    # Send command to microcontroller
    mc_cmd = int(enable + frequency + wave_type,2)
    print(mc_cmd)

    if 0x36 in a:
        while not i2c.try_lock():
            pass
        i2c.writeto(mc1_address,bytes([mc_cmd]))
        i2c.unlock()

    time.sleep(10)
    current = read_current(i2c, adc_addr, 10)

    text1 = "Press A to edit"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=38)
    splash.append(text_area)
    text1 = "Press B to stop"  # overly long to see where it clips
    text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=50)
    splash.append(text_area)
    option = ""

    while len(option) == 0:
        A.pull = Pull.UP
        B.pull = Pull.UP
        if not A.value:
            splash.pop(-1)
            splash.pop(-1)
            splash.pop(-1)
            splash.pop(-1)
            option = "a"
        elif not B.value:
            stop = True
            option = "b"

        # Adjust rheostat resistance

        if current < 0.3:
            print(current)
            print("Coils not started")
            option = "a"
            splash.pop(-1)
            splash.pop(-1)
            splash.pop(-1)
            splash.pop(-1)
            text1 = "Coils not active"  # overly long to see where it clips
            text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=20)
            splash.append(text_area)
            text1 = "Try again"  # overly long to see where it clips
            text_area = label.Label(terminalio.FONT, text=text1, color=0xFFFFFF, x=8, y=32)
            splash.append(text_area)
            time.sleep(3)
            splash.pop(-1)
            splash.pop(-1)

        if current > 15:
            #stop if current too high
            while not i2c.try_lock():
                pass
            i2c.writeto(mc1_address,bytes([0]))
            i2c.unlock()

        while current > 11:
            print(current)
            while not i2c.try_lock():
                pass
            i2c.writeto(rheostat_addr, bytes([4]))
            i2c.writeto(rheostat_addr, bytes([20]))
            i2c.unlock()
            current = read_current(i2c, adc_addr, 10)

        while current < 8 and current > 0.3:
            print(current)
            while not i2c.try_lock():
                pass
            i2c.writeto(rheostat_addr, bytes([8]))
            i2c.writeto(rheostat_addr, bytes([24]))
            i2c.unlock()
            current = read_current(i2c, adc_addr, 10)

        current = read_current(i2c, adc_addr, 10)

        pass
    pass

splash.pop(-1)
splash.pop(-1)
splash.pop(-1)
splash.pop(-1)
text2 = "Run over"
text_area2 = label.Label(
    terminalio.FONT, text=text2, scale=1, color=0xFFFFFF, x=12, y=32
)
splash.append(text_area2)

time.sleep(5)




