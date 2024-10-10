
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

//void printString(char * string);
char readChar(void);
void printChar(int);


void GPIO_F_Interrupt(void);

int c = 0x00 ;
int d;
int main(void)
{
    SYSCTL_RCGCUART_R |= ( 1 << 1);

    SYSCTL_RCGC2_R |= (1 << 1);
    GPIO_PORTB_LOCK_R = 0x4C4F434B;
    GPIO_PORTB_CR_R = (1 << 1) | ( 1 << 0);
    GPIO_PORTB_DEN_R = (1 << 1) | ( 1 << 0);
    GPIO_PORTB_DIR_R = (1 << 1);
    GPIO_PORTB_AFSEL_R = (1 << 1) | ( 1 << 0);
    GPIO_PORTB_PCTL_R = (1 << 0) | ( 1 << 4);




    UART1_CTL_R &= ~(1 << 0);
    UART1_IBRD_R = 104;
    UART1_FBRD_R = 0x11;
    UART1_LCRH_R = (0x3 << 5)|(1 << 1);
    UART1_CC_R = 0x0;

    UART1_CTL_R = (1 << 0)|(1 << 8)|(1 << 9);

    SYSCTL_RCGC2_R |= (1 << 5);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = (1 << 1) | ( 1 << 2)|(1 << 3)|( 1 << 0)|(1 << 4);
    GPIO_PORTF_DEN_R = (1 << 1) | ( 1 << 2)|(1 << 3)|( 1 << 0)|(1 << 4);
    GPIO_PORTF_DIR_R = (1 << 1) | ( 1 << 2)|(1 << 3);
    GPIO_PORTF_DATA_R &= ~((1 << 1)|( 1 << 2)|(1 << 3));
    GPIO_PORTF_PUR_R = 0x11;

    GPIO_PORTF_IS_R &= ~0x11;     // Make PF0 edge-sensitive
             GPIO_PORTF_IBE_R &= ~0x11;    // Disable interrupt on both edges
             GPIO_PORTF_IEV_R &= ~0x11;    // Interrupt on falling edge (when button is pressed)
             GPIO_PORTF_IM_R |= 0x11;  // Unmask the interrupt for PF0

             NVIC_EN0_R |= (1 << 30);  // Enable interrupt for Port F (bit 30 for Port F)

    while(1){

       d = readChar();

       if (d == 0xAA) {
           GPIO_PORTF_DATA_R = (1 << 3);
       }

       else if( d == 0xF0){
            GPIO_PORTF_DATA_R = (1 << 2);
       }

       else {
                GPIO_PORTF_DATA_R = (1 << 1);
       }


       }

    }




char readChar(void){

    int a ;
    while((UART1_FR_R & (1 << 4)) != 0);

    a =  UART1_DR_R;
    return a;

}

void printChar(int c){

  // while(( UART1_FR_R & (1 << 5) != 0));
    UART1_DR_R = c;

}



void GPIO_F_Interrupt(void){
    if (GPIO_PORTF_RIS_R & 0x10) {  // Interrupt for PF4 //sw1
        GPIO_PORTF_ICR_R |= 0x10;
        c = 0xF0;
        printChar(c);
    }

    if (GPIO_PORTF_RIS_R & 0x01) {  // Interrupt for PF0 //sw2
           GPIO_PORTF_ICR_R |= 0x01;
           c = 0xAA;
           printChar(c);
    }

}


