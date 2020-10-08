from math import sin, pi 

def sine_sample(freq):
    mult = 4096

    # 32 768 Hz Sample Rate

    # Sine wave at 1 Hz -> sin(2*Pi*x)

    samplerate = 48*10e3

    x = 0
    
    samples = [0]
    
    val = -1 
    while val != 0:
        x += 1/mult
        val = mult*sin(2*pi*x)
        print(val)
        samples.append(round(val))

    return samples


def main():

    samples = sine_sample(32.076)

    print(samples)

main()

