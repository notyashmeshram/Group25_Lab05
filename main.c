#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

void GPIOPortF_Handler(void);

int main(void) {
    // Enable clock for GPIO Port F
    SYSCTL_RCGCGPIO_R |= (1 << 5); // Enable clock for Port F
    while ((SYSCTL_PRGPIO_R & (1 << 5)) == 0);  // Wait until Port F is ready

    // Unlock PF4 (switch)
    GPIO_PORTF_LOCK_R = 0x4C4F434B;  // Unlock GPIO Port F
    GPIO_PORTF_CR_R |= (1 << 4);     // Allow changes to PF4

    // Configure PF4 (Switch) as input and PF1 (LED) as output
    GPIO_PORTF_DIR_R &= ~(1 << 4);   // Set PF4 as input (switch)
    GPIO_PORTF_DIR_R |= (1 << 1);    // Set PF1 as output (LED)
    GPIO_PORTF_DEN_R |= (1 << 4) | (1 << 1); // Enable digital for PF4 and PF1
    GPIO_PORTF_PUR_R |= (1 << 4);    // Enable pull-up on PF4

    // Disable interrupt for PF4 during configuration
    GPIO_PORTF_IM_R &= ~(1 << 4);

    // Set PF4 to trigger interrupt on falling edge (button press)
    GPIO_PORTF_IS_R &= ~(1 << 4);    // Edge-sensitive
    GPIO_PORTF_IBE_R &= ~(1 << 4);   // Single edge
    GPIO_PORTF_IEV_R &= ~(1 << 4);   // Falling edge

    // Clear any prior interrupt on PF4
    GPIO_PORTF_ICR_R |= (1 << 4);

    // Enable interrupt on PF4
    GPIO_PORTF_IM_R |= (1 << 4);

    // Enable the interrupt in NVIC (IRQ30 for GPIO Port F)
    NVIC_EN0_R |= (1 << 30);

    // Enable global interrupts
    __asm("     CPSIE I");

    // Initial state: turn off the LED
    GPIO_PORTF_DATA_R &= ~(1 << 1);

    while(1) {
        // Main loop can remain empty as the interrupt will handle the logic
    }
}

// Interrupt handler for GPIO Port F
void GPIOPortF_Handler(void) {
    // Check if interrupt occurred on PF4 (Switch)
    if (GPIO_PORTF_RIS_R & (1 << 4)) {
        // Toggle the LED on PF1
        GPIO_PORTF_DATA_R ^= (1 << 1);  // Toggle LED on PF1 (red LED)

        GPIO_PORTF_ICR_R |= (1 << 4);  // Clear the interrupt flag

    }
}
