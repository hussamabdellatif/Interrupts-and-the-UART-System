#include "LPC17xx.H"
#include "GLCD.h"
#include <stdlib.h>
#include "uart.h"
#include "system_LPC17xx.h"

//UART Variables 
extern volatile uint32_t UART0_Count;
extern volatile uint8_t UART0_Buffer[BUFSIZE];

//Global variables - scope over entire file.
unsigned short random_colors[] = {Blue, Navy, Red, Yellow, Magenta, Orange, Pink, Brown};
int random_index = 0;

//IRQ handler for timer 0 match
void TIMER0_IRQHandler(void) {
		LPC_TIM0->IR |= 1; //Clear interrupt source
		GLCD_SetTextColor(random_colors[random_index]);
		random_index++;
		if (random_index == 8) random_index = 0;
}


int main() {
		int row = 0;
		int col = 0;
		char c = 'a';
	
		//Set up GLCD 
		GLCD_Init();
		GLCD_Clear(Black);
		GLCD_SetTextColor(Blue);
		GLCD_SetBackColor(Black);
	
		//Timer init for part B
		//Set up Timer0 to interrupt every 1s and reset
		//LPC_TIM0->MR0 = 12000000; //Set up 1s match value
		LPC_TIM0->MR0 = 24000000; //Set up 2s match value
	  LPC_TIM0->MCR |= 3;       //bit0 for interrupt, bit1 for reset
		NVIC_EnableIRQ(TIMER0_IRQn);
		LPC_TIM0->TCR |= 2;       //bit1 is reset bit
		LPC_TIM0->TCR &= ~2;      //clear reset
	  LPC_TIM0->TCR |= 1;			  //start timer (bit 0 is start bit)
	
		//UART init for part C
		UARTInit(0, 57600);
                  
    
		//Display characters a through z across the display
		while(1) {  
				int i;
				//UART code for Part C
				if ( UART0_Count != 0 ) {
						char char_from_pc = UART0_Buffer[0];						
						UARTSend( 0, (uint8_t *)UART0_Buffer, UART0_Count);
						UART0_Count = 0;
						GLCD_DisplayChar(row, col, char_from_pc);
				col++;
				if (col == 20) { //Loop back to beginning of next row
						row++;
						if(row == 10) row = 0; //Start back at beginning of screen.
						col = 0;
				}
				c++;
				if (c == 'z' + 1) c = 'a';
				for (i = 0; i < 1000000; i++); //Small delay between each letter.
				
				}
				
				//Code for Part B
//				GLCD_DisplayChar(row, col, c);
//				col++;
//				if (col == 20) { //Loop back to beginning of next row
//						row++;
//						if(row == 10) row = 0; //Start back at beginning of screen.
//						col = 0;
//				}
//				c++;
//				if (c == 'z' + 1) c = 'a';
//				for (i = 0; i < 1000000; i++); //Small delay between each letter.
//				
    }
}