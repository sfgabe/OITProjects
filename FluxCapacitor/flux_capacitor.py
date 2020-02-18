# Flux Capacitor for NeoPixels on Raspberry Pi
import time
import board
import neopixel

# Choose an open pin connected to the Data In of the NeoPixel strip, i.e. board.D18
# NeoPixels must be connected to D10, D12, D18 or D21 to work.
pixel_pin = board.D18

# The number of NeoPixels
num_pixels = 12

# The order of the pixel colors - RGB or GRB. Some NeoPixels have red and green reversed!
# For RGBW NeoPixels, simply change the ORDER to RGBW or GRBW.
ORDER = neopixel.GRB

pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.2, auto_write=False,
                           pixel_order=ORDER)

#This is in for adding options later
def wheel(pos):
    # Input a value 0 to 255 to get a color value.
    # The colours are a transition r - g - b - back to r.
    if pos < 0 or pos > 255:
        r = g = b = 0
    elif pos < 85:
        r = int(pos * 3)
        g = int(255 - pos*3)
        b = 0
    elif pos < 170:
        pos -= 85
        r = int(255 - pos*3)
        g = 0
        b = int(pos*3)
    else:
        pos -= 170
        r = 0
        g = int(pos*3)
        b = int(255 - pos*3)
    return (r, g, b) if ORDER == neopixel.RGB or ORDER == neopixel.GRB else (r, g, b, 0)

#This is in for adding options later
def rainbow_cycle(wait):
    for j in range(255):
        for i in range(num_pixels):
            pixel_index = (i * 256 // num_pixels) + j
            pixels[i] = wheel(pixel_index & 255)
        pixels.show()
        time.sleep(wait)

#This is the fluxing script, you may need to renumber LED addresses (pixels[0]) according to your setup
def flux(wait):
            pixels[0] = (252, 223, 95)
            pixels[4] = (252, 223, 95)
            pixels[11] = (252, 223, 95)
            pixels[1] = (0, 0, 0)
            pixels[5] = (0, 0, 0)
            pixels[10] = (0, 0, 0)
            pixels[2] = (0, 0, 0)
            pixels[6] = (0, 0, 0)
            pixels[9] = (0, 0, 0)
            pixels[3] = (0, 0, 0)
            pixels[7] = (0, 0, 0)
            pixels[8] = (0, 0, 0)
            pixels.show()
            time.sleep(wait)
            pixels[0] = (252, 223, 95)
            pixels[4] = (252, 223, 95)
            pixels[11] = (252, 223, 95)
            pixels[1] = (252, 225, 118)
            pixels[5] = (252, 225, 118)
            pixels[10] = (252, 225, 118)
            pixels[2] = (0, 0, 0)
            pixels[6] = (0, 0, 0)
            pixels[9] = (0, 0, 0)
            pixels[3] = (0, 0, 0)
            pixels[7] = (0, 0, 0)
            pixels[8] = (0, 0, 0)
            pixels.show()
            time.sleep(wait)
            pixels[0] = (249, 231, 157)
            pixels[4] = (249, 231, 157)
            pixels[11] = (249, 231, 157)
            pixels[1] = (188, 156, 22)
            pixels[5] = (188, 156, 22)
            pixels[10] = (188, 156, 22)
            pixels[2] = (247, 222, 123)
            pixels[6] = (247, 222, 123)
            pixels[9] = (247, 222, 123)
            pixels[3] = (0, 0, 0)
            pixels[7] = (0, 0, 0)
            pixels[8] = (0, 0, 0)
            pixels.show()
            time.sleep(wait)
            pixels[0] = (142, 116, 5)
            pixels[4] = (142, 116, 5)
            pixels[11] = (142, 116, 5)
            pixels[1] = (252, 225, 118)
            pixels[5] = (252, 225, 118)
            pixels[10] = (252, 225, 118)
            pixels[2] = (188, 156, 22)
            pixels[6] = (188, 156, 22)
            pixels[9] = (188, 156, 22)
            pixels[3] = (255, 212, 45)
            pixels[7] = (255, 212, 45)
            pixels[8] = (255, 212, 45)
            pixels.show()
            time.sleep(wait)

while True:
    flux(0.05)    # flux cycle with .05ms delay per step
