#ifndef SOUNDS_H
#define SOUNDS_H

#include <stdint.h>
/**
 * frequencies[0] = 64*2^0 Hz 64
 * frequencies[1] = 64*2^1 Hz 128
 * frequencies[2] = 64*2^2 Hz 256
 * frequencies[3] = 64*2^3 Hz 512
 * frequencies[4] = 64*2^4 Hz 1024
 * frequencies[5] = 64*2^5 Hz 2048
 * frequencies[6] = 64*2^6 Hz 4096
 * frequencies[7] = 64*2^7 Hz 8192
*/

int16_t sinewave[] = {
4096, 4096, 4095, 4093, 4091, 4088, 4085, 4081, 4076, 4071, 4065, 4059, 4052, 4044, 4036, 4027, 4017, 4007, 3996, 3985, 3973, 3961, 3948, 3934, 3920, 3905, 3889, 3873, 3857, 3839, 3822, 3803, 3784, 3765, 3745, 3724, 3703, 3681, 3659, 3636, 3612, 3588, 3564, 3539, 3513, 3487, 3461, 3433, 3406, 3378, 3349, 3320, 3290, 3260, 3229, 3198, 3166, 3134, 3102, 3068, 3035, 3001, 2967, 2932, 2896, 2861, 2824, 2788, 2751, 2713, 2675, 2637, 2598, 2559, 2520, 2480, 2440, 2399, 2359, 2317, 2276, 2234, 2191, 2149, 2106, 2062, 2019, 1975, 1931, 1886, 1842, 1797, 1751, 1706, 1660, 1614, 1567, 1521, 1474, 1427, 1380, 1332, 1285, 1237, 1189, 1141, 1092, 1044, 995, 946, 897, 848, 799, 750, 700, 651, 601, 551, 501, 451, 401, 351, 301, 251, 201, 151, 101, 50, 0, -50, -101, -151, -201, -251, -301, -351, -401, -451, -501, -551, -601, -651, -700, -750, -799, -848, -897, -946, -995, -1044, -1092, -1141, -1189, -1237, -1285, -1332, -1380, -1427, -1474, -1521, -1567, -1614, -1660, -1706, -1751, -1797, -1842, -1886, -1931, -1975, -2019, -2062, -2106, -2149, -2191, -2234, -2276, -2317, -2359, -2399, -2440, -2480, -2520, -2559, -2598, -2637, -2675, -2713, -2751, -2788, -2824, -2861, -2896, -2932, -2967, -3001, -3035, -3068, -3102, -3134, -3166, -3198, -3229, -3260, -3290, -3320, -3349, -3378, -3406, -3433, -3461, -3487, -3513, -3539, -3564, -3588, -3612, -3636, -3659, -3681, -3703, -3724, -3745, -3765, -3784, -3803, -3822, -3839, -3857, -3873, -3889, -3905, -3920, -3934, -3948, -3961, -3973, -3985, -3996, -4007, -4017, -4027, -4036, -4044, -4052, -4059, -4065, -4071, -4076, -4081, -4085, -4088, -4091, -4093, -4095, -4096, -4096, -4096, -4095, -4093, -4091, -4088, -4085, -4081, -4076, -4071, -4065, -4059, -4052, -4044, -4036, -4027, -4017, -4007, -3996, -3985, -3973, -3961, -3948, -3934, -3920, -3905, -3889, -3873, -3857, -3839, -3822, -3803, -3784, -3765, -3745, -3724, -3703, -3681, -3659, -3636, -3612, -3588, -3564, -3539, -3513, -3487, -3461, -3433, -3406, -3378, -3349, -3320, -3290, -3260, -3229, -3198, -3166, -3134, -3102, -3068, -3035, -3001, -2967, -2932, -2896, -2861, -2824, -2788, -2751, -2713, -2675, -2637, -2598, -2559, -2520, -2480, -2440, -2399, -2359, -2317, -2276, -2234, -2191, -2149, -2106, -2062, -2019, -1975, -1931, -1886, -1842, -1797, -1751, -1706, -1660, -1614, -1567, -1521, -1474, -1427, -1380, -1332, -1285, -1237, -1189, -1141, -1092, -1044, -995, -946, -897, -848, -799, -750, -700, -651, -601, -551, -501, -451, -401, -351, -301, -251, -201, -151, -101, -50, 0, 50, 101, 151, 201, 251, 301, 351, 401, 451, 501, 551, 601, 651, 700, 750, 799, 848, 897, 946, 995, 1044, 1092, 1141, 1189, 1237, 1285, 1332, 1380, 1427, 1474, 1521, 1567, 1614, 1660, 1706, 1751, 1797, 1842, 1886, 1931, 1975, 2019, 2062, 2106, 2149, 2191, 2234, 2276, 2317, 2359, 2399, 2440, 2480, 2520, 2559, 2598, 2637, 2675, 2713, 2751, 2788, 2824, 2861, 2896, 2932, 2967, 3001, 3035, 3068, 3102, 3134, 3166, 3198, 3229, 3260, 3290, 3320, 3349, 3378, 3406, 3433, 3461, 3487, 3513, 3539, 3564, 3588, 3612, 3636, 3659, 3681, 3703, 3724, 3745, 3765, 3784, 3803, 3822, 3839, 3857, 3873, 3889, 3905, 3920, 3934, 3948, 3961, 3973, 3985, 3996, 4007, 4017, 4027, 4036, 4044, 4052, 4059, 4065, 4071, 4076, 4081, 4085, 4088, 4091, 4093, 4095, 4096
};

#endif
