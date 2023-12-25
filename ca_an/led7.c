#include <delay.h>

void hienThi_led7(int i, unsigned int j){ 
    if(i == 0) {
        PORTF = 0x80 + j; 
        PORTD = 0xF9; 
        PORTA = 0x7F; 
    } 
    if(i == 1) { 
        PORTF = 0x86 + j; 
        PORTD = 0xFF; 
        PORTA = 0x7F; 
    } 
    if(i == 2){ 
         PORTF = 0x84 + j; 
         PORTD = 0xF1;      
         PORTA = 0xFF; 
    } 
    if(i == 3) { 
         PORTF = 0x84 + j; 
         PORTD = 0xF5; 
         PORTE = 0x00; 
         PORTA = 0x7F; 
    } 
    if(i == 4){ 
         PORTF = 0x82 + j; 
         PORTD = 0xF7; 
         PORTA = 0x7F; 
    } 
    if(i == 5){ 
         PORTF = 0xA0 + j; 
         PORTD = 0xF5; 
         PORTA = 0x7F; 
    }
    if (i == 6){ 
         PORTF = 0xA0 + j; 
         PORTD = 0xF1;
         PORTA = 0x7F; 
    } 
    if (i == 7){ 
         PORTF = 0x84 + j; 
         PORTD = 0xFF; 
         PORTA = 0x7F; 
    }
    if(i == 8){ 
         PORTF = 0x80 + j; 
         PORTD = 0xF1;         
         PORTA = 0x7F; 
    } 
    if(i == 9){ 
         PORTF = 0x80 + j; 
         PORTD = 0xF5; 
         PORTA = 0x7F; 
    }
    if(i == 10){ 
         PORTF = 0x26 + j; 
         PORTD = 0xFF; 
         PORTA = 0xFF; 
    } 
    delay_ms(1); 
} 