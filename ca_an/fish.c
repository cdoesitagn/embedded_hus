/*
 * fish.c
 *
 * Created: 12/28/2022 4:03:17 PM
 * Author: Who
 */

#include <io.h>
#include <hardware.c>
#include <glcd.h>
#include <delay.h>
#include <ds1307.h>
#include <stdlib.h>
#define servo_1 PORTC.7

int counter = 0;
int dem = -1;
int vi_tri = 100;
unsigned char hour, minute, second;
char tmp[10];
int hour_1, hour_2, min_1, min_2;
int time_set[4] = {-1, -1, -1, -1};
bool eat_much = false;
bool den = false;

// Timer1 overflow interrupt service routine (delay 0.1 ms)
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
    // Reinitialize Timer1 value
    TCNT1H=0xFCE0 >> 8;
    TCNT1L=0xFCE0 & 0xff;
    // Place your code here
    
    // servo
    dem++;
    if(dem >= 200) 
        dem = 0;
    if (dem < vi_tri) 
        servo_1 = 1;
    else 
        servo_1 = 0;
    
    if (OCR0 != 0xff && PINB.2 == 0) {
        den = true;
        dung_DC();
    }
            
    // den
    if (den) {
        if (counter == 20000)
            counter  = 0;
            
        if (counter == 0) {
            PORTD.4 = 0;
            PORTD.5 = 1;
        }
            
        if (counter == 10000) {
            PORTD.5 = 0;
            PORTD.4 = 1;
        }
        counter++;
    }
}

void update_time() {
    // Place your code here
      rtc_get_time(&hour,&minute,&second);
      hour_1 = hour / 10;
      hour_2 = hour % 10;;
      min_1 = minute / 10;
      min_2 = minute % 10;
      lcd_gotoxy(0, 0);
      lcd_putchar(hour_1 + 48);
      lcd_putchar(hour_2 + 48);
      lcd_putchar(':') ;
      lcd_putchar(min_1 + 48);
      lcd_putchar(min_2 + 48);
      lcd_putchar(':') ;
      lcd_putchar((second/10) + 48);
      lcd_putchar((second%10) + 48);
}

void hen_gio() {
    int i = 0, input;
    glcd_clear();
    while (i != 4) {
        input = read();
        if (input != -1) {
            if (i == 0 && (input >= 0 && input <= 2)) 
                time_set[i] = input;
            else if (i == 1 && ((time_set[0] == 2 && input >= 0 && input <= 3) || time_set[0] != 2))
                time_set[i] = input;
            else if ((i == 2 && (input >= 0 && input <= 5)) || i == 3)
                time_set[i] = input;
                 
            itoa(time_set[i], tmp);
            glcd_outtextxy(25 + 7 * i, 20, tmp);
            i++;
        }
        delay_ms(250);
    }
    
    glcd_clear();
    glcd_outtextxy(15, 10, "AN NHIEU?");
    glcd_outtextxy(15, 20, "BT1 - CO");
    glcd_outtextxy(15, 30, "BT2 - KO");

    while (i != -1) {
        if (PINB.2 == 0) {
            eat_much = true;
            i = -1;
        }
        if (PINB.3 == 0) {
            eat_much = false;
            i = -1;
        }
    }
    
    glcd_clear();
    glcd_outtextxy(15, 20, "SUCCESS!");
    delay_ms(1000);
    glcd_clear();
}

void main(void)
{
    //init GLCD
    init_glcd(4, 66);
    init_lcd();
    init_register();     
    init_timer();
     
    i2c_init(); 
    //rtc_init(3,1,0);
    rtc_init(0,0,0); 
    //rtc_set_time(0,0,0); 
    #asm("sei")
    
    while (1)
    {           
        update_time();
        // Please write your application code here
        glcd_outtextxy(25, 15, "HEN GIO");
        glcd_outtextxy(30, 25, "BT1");
        if (PINB.2 == 0) {
            hen_gio();
        }
        
        if (hour_1 == time_set[0] && hour_2 == time_set[1] && min_1 == time_set[2] && min_2 == time_set[3] && second == 0) {
            quay_DC();
            // servo 90 do
            vi_tri = 15;
            if (eat_much)
                delay_ms(2000);
            else
                delay_ms(1000);
            dung_DC();
            vi_tri = 10;         
        }
            
        delay_ms(100);
        
    }