from math import cos, pi 

def sine_sample(freq):
    mult = 255

    # 32 768 Hz Sample Rate

    # Sine wave at 1 Hz -> sin(2*Pi*x)

    samplerate = 48e3

    x = 0 
    
    samples = [cos(0)]
    
    val = -1 
    while x < 1/freq:
        x += 1/samplerate
        val = (mult/2)*(cos(2*pi*x*freq) + 1)
        samples.append(round(val))
    return samples


def main():

    samples = sine_sample(440)

    print(samples)

main()

