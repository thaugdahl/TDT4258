#ifndef GPIO_H
#define GPIO_H

void setupGPIO();
void enableGPIOInterrupt();
void handle_gpio(uint16_t button_bitmask);

#endif
