// Toggles the D2 LED to test upload

#include "stm32f4xx.h"

int main(void)
{
    // Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Set PA1 as output
    GPIOA->MODER &= ~(3 << (1 * 2));   // Clear mode bits
    GPIOA->MODER |=  (1 << (1 * 2));   // Set to output mode

    while (1)
    {
        GPIOA->ODR ^= (1 << 1);        // Toggle PA1 (D2 LED)
        for (volatile int i = 0; i < 500000; i++);
    }
}
