/*
 * bai8.c
 *
 * Created: 11/11/2022 9:25:24 AM
 * Author: Who
 */

#include <io.h>
#include <delay.h>
#include <glcd.h>
#include <font5x7.h>
#include <dht11.c>

char x = 0;
unsigned char nhietdo = 0, doam = 0;
unsigned char nhietdo_naodo = 999, nhietdo_naodo2 = 1000, doam_naodo = 999;
bool say_xong = false;
int counter = 0;

// Interrupt cua timer1
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
    // Reinitialize Timer1 value
    TCNT1H=0xE0C0 >> 8;
    TCNT1L=0xE0C0 & 0xff;
    // Place your code here
    if (say_xong) {
        if (counter == 2000)
            counter  = 0;
            
        if (counter == 0) {
            PORTD.4 = 0;
            PORTD.5 = 1;
        }
            
        if (counter == 1000) {
            PORTD.5 = 0;
            PORTD.4 = 1;
        }
        counter++;
    }
}

void adjustSpeed(int speed) {
    OCR0 = 255 - speed;
}

void hien_thi() {
    x = dht11(&nhietdo, &doam);
        
    if (x == 1) {
        lcd_gotoxy(0, 0);
        lcd_putsf("TEMP:");
        lcd_putchar(nhietdo / 10 + 48);
        lcd_putchar(nhietdo % 10 + 48);
        lcd_gotoxy(0, 1);
        lcd_putsf("HUMI:");
        lcd_putchar(doam / 10 + 48);
        lcd_putchar(doam % 10 + 48);
    }
}

void main(void)
{
    //init GLCD
    GLCDINIT_t glcd_init_data;
    glcd_init_data.font=font5x7;
    glcd_init_data.temp_coef=140;
    glcd_init_data.bias=4;
    glcd_init_data.vlcd=66;
    glcd_init(&glcd_init_data);
    
    //init LCD
    lcd_init(16);
     
    // some port      
    DDRB.4 = 1;
    DDRD.7 = 1;
    DDRD.4 = 1;
    DDRD.5 = 1;
    // khoi tao timer 0 cho dong co
    ASSR=0<<AS0;
    TCCR0=(1<<WGM00) | (1<<COM01) | (1<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x00;
    adjustSpeed(0);
    
    // khoi tao timer 1 voi chu ki 1 ms
    TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
    TCNT1H=0xE0;
    TCNT1L=0xC0;
    
    // khoi tao interrupt cho timer1
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
    ETIMSK=(0<<TICIE3) | (0<<OCIE3A) | (0<<OCIE3B) | (0<<TOIE3) | (0<<OCIE3C) | (0<<OCIE1C);
    
    //pull up BT1
    DDRB.2 = 0;
    PORTB.2 = 1;
    //pull up BT2
    DDRB.3 = 0;
    PORTB.3 = 1;
    #asm("sei")
while (1)
    {
    // Please write your application code here
        hien_thi();
        if (nhietdo == nhietdo_naodo && doam == doam_naodo)
            adjustSpeed(255);
        if (OCR0 == 0 && nhietdo == nhietdo_naodo2) {
            adjustSpeed(0);
            glcd_outtextxy(20, 20, "SAY XONG!");
            say_xong = true;
            while(1){};
        }
        delay_ms(250); 
    }
}
