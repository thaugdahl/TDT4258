from math import sin, pi 

def sine_sample(freq):
    mult = 4096

    # 32 768 Hz Sample Rate

    # Sine wave at 1 Hz -> sin(2*Pi*x)

    samplerate = 48*10e3

    x = 0
    
    samples = [0]
    
    val = -1 
    while x < 1 and not -0.1 < val < 0.1 :
        x += 1/freq
        val = mult*sin(2*pi*x)
        print(x)
        samples.append(round(val))

    return samples


def main():

    samples = sine_sample(32.076)

    print(samples)

main()

