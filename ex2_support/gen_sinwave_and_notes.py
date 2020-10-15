from ast import Str
from matplotlib.pyplot import plot, show, subplots, violinplot
import numpy as math
import numpy
from numpy.core.records import array
from pathlib import Path

print("start of program")

def sine_calc(_b, _sine_top, _sine_dc):
    """
    docstring
    """
    return math.sin(math.radians(_b)) * _sine_top + _sine_dc

#debugging
plot_test = False

# data arrays
sine_array = ""
note_array = ""
test_sine_array = []
note_dict = {
"C0":	16.35,
"CS0":	17.32,
"D0":	18.35,
"DS0":	19.45,
"E0":	20.60,
"F0":	21.83,
"FS0":	23.12,
"G0":	24.50,
"GS0":	25.96,
"A0":	27.50,
"AS0":	29.14,
"B0":	30.87,
"C1":	32.70,
"CS1":	34.65,
"D1":	36.71,
"DS1":	38.89,
"E1":	41.20,
"F1":	43.65,
"FS1":	46.25,
"G1":	49.00,
"GS1":	51.91,
"A1":	55.00,
"AS1":	58.27,
"B1":	61.74,
"C2":	65.41,
"CS2":	69.30,
"D2":	73.42,
"DS2":	77.78,
"E2":	82.41,
"F2":	87.31,
"FS2":	92.50,
"G2":	98.00,
"GS2":	103.83,
"A2":	110.00,
"AS2":	116.54,
"B2":	123.47,
"C3":	130.81,
"CS3":	138.59,
"D3":	146.83,
"DS3":	155.56,
"E3":	164.81,
"F3":	174.61,
"FS3":	185.00,
"G3":	196.00,
"GS3":	207.65,
"A3":	220.00,
"AS3":	233.08,
"B3":	246.94,
"C4":	261.63,
"CS4":	277.18,
"D4":	293.66,
"DS4":	311.13,
"E4":	329.63,
"F4":	349.23,
"FS4":	369.99,
"G4":	392.00,
"GS4":	415.30,
"A4":	440.00,
"AS4":	466.16,
"B4":	493.88,
"C5":	523.25,
"CS5":	554.37,
"D5":	587.33,
"DS5":	622.25,
"E5":	659.25,
"F5":	698.46,
"FS5":	739.99,
"G5":	783.99,
"GS5":	830.61,
"A5":	880.00,
"AS5":	932.33,
"B5":	987.77,
"C6":	1046.50,
"CS6":	1108.73,
"D6":	1174.66,
"DS6":	1244.51,
"E6":	1318.51,
"F6":	1396.91,
"FS6":	1479.98,
"G6":	1567.98,
"GS6":	1661.22,
"A6":	1760.00,
"AS6":	1864.66,
"B6":	1975.53,
"C7":	2093.00,
"CS7":	2217.46,
"D7":	2349.32,
"DS7":	2489.02,
"E7":	2637.02,
"F7":	2793.83,
"FS7":	2959.96,
"G7":	3135.96,
"GS7":	3322.44,
"A7":	3520.00,
"AS7":	3729.31,
"B7":	3951.07,
"C8":	4186.01,
"CS8":	4434.92,
"D8":	4698.63,
"DS8":	4978.03,
"E8":	5274.04,
"F8":	5587.65,
"FS8":	5919.91,
"G8":	6271.93,
"GS8":	6644.88,
"A8":	7040.00,
"AS8":	7458.62,
"B8":	7902.13
}

# variables
n = 0
colums = 7
frequency = 14*10**6
period_max = 6500

# sine form
sine_top_to_top = 0xfff
sine_dc = sine_top_to_top/2
sine_top = sine_top_to_top/2
max_angle = 360
number_of_elements = 64

# code

sine_array += "#ifndef SINEWAVE_H\n#define SINEWAVE_H\n"
sine_array += "#define SINEWAVE {"

step_size = max_angle/number_of_elements


for i in range(number_of_elements-1):
    b = i*step_size
    sine_array += (hex(int(sine_calc(b, sine_top, sine_dc))) + ", ")
    test_sine_array.append(sine_calc(b, sine_top, sine_dc))

sine_array += (hex(int(sine_calc(max_angle, sine_top, sine_dc))) + "}\n")
test_sine_array.append(sine_calc(max_angle, sine_top, sine_dc))



sine_array += "#define SINEWAVE_LENGTH " + str(number_of_elements) + "\n"

sine_array += "#endif"














































































































































































































































f1 = open(str(Path(__file__).parent)+"/sinewave.h", "w")
f1.write(sine_array)
f1.close()

fig, ax = subplots()

x = numpy.linspace(0, 1, number_of_elements)

line1, = ax.plot(x, test_sine_array)


for note in note_dict:
    top_value = int(frequency/(note_dict[note]*number_of_elements))
    note_array += "#define "
    note_array += note
    note_array += " "
    note_array += str(top_value)
    note_array +="\n"


f2 = open(str(Path(__file__).parent)+"/notes.h", "w")
f2.write(note_array)
f2.close()

if plot_test:
    show()



print("end of program")