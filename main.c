#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

void GPIOPortF_Handler(void);

volatile int x=0;

int main(void) {
    SYSCTL_RCGC2_R |= 0x00000020; /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B; /* unlock commit register */
    GPIO_PORTF_CR_R = 0x1F; /* make PORTF0 configurable */
    GPIO_PORTF_DEN_R = 0x1E; /* set PORTF pins 4 pin */
    GPIO_PORTF_DIR_R = 0x0E; /* set PORTF4 pin as input user switch pin */
    GPIO_PORTF_PUR_R = 0x10; /* PORTF4 is pulled up */

    // Set PF4 to trigger interrupt on falling edge (button press 1 to 0 transition)
    GPIO_PORTF_IS_R &= ~(1 << 4);    // Edge-sensitive
    GPIO_PORTF_IBE_R &= ~(1 << 4);   // Single edge
    GPIO_PORTF_IEV_R &= ~(1 << 4);   // Falling edge

    // Clear any prior interrupt on PF4
    GPIO_PORTF_ICR_R |= (1 << 4);

    // Enable interrupt on PF4
    GPIO_PORTF_IM_R |= (1 << 4);

    // Enable the interrupt in NVIC (IRQ30 for GPIO Port F)
    NVIC_EN0_R |= (1 << 30);


    // Initial state: turn off the LED
    GPIO_PORTF_DATA_R &= 0xD;
    int debounce = 0;
           while(debounce < 1000){debounce ++;} //for debouncing
    while(1) {
        // Main loop can remain empty as the interrupt will handle the logic
    }
}

// Interrupt handler for GPIO Port F
void GPIOPortF_Handler(void) {
    x++;

    // Check if interrupt occurred on PF4 (Switch)
    if (GPIO_PORTF_RIS_R & (1 << 4)) {
        GPIO_PORTF_ICR_R |= (1 << 4); // Clear the interrupt flag
        // Toggle the LED on PF1
        GPIO_PORTF_DATA_R ^= 0x02;  // Toggle LED on PF1 (red LED)



    }

}
