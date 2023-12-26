/*
 * elevator.c
 *
 * Created: 12/29/2022 10:30:54 PM
 * Author: Who
 */

#include <io.h>
#include <hardware.c>
#include <delay.h>
#include <stdlib.h>
#include <glcd.h>
#define servo_1 PORTC.7

int dem = 0, vi_tri = 10;
char what[8];
// dieu khien cac tang trong thang may
bool order[9] = {false, false, false, false, false, false, false, false, false};
// dieu khien trang thai len / xuong
bool len = true;
int currentLevel = 0;

// Timer1 overflow interrupt service routine (delay 0.1 ms)
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
    int input;
    // Reinitialize Timer1 value
    TCNT1H=0xFCE0 >> 8;
    TCNT1L=0xFCE0 & 0xff;
    
    // servo
    dem++;
    if(dem >= 200) 
        dem = 0;
    if (dem < vi_tri) 
        servo_1 = 1;
    else 
        servo_1 = 0;
    
    // ~ delay 199 ms ban phim ma tran   
    if (dem == 199) {
        input = read();
        if (input != -1)
            order[input] = true;
    }
}

void thang_may() {
    int i;
    // neu ko co tang nao dc bam thi thoi ko lam gi ca
    for (i = 0; i <= 8; i++)
         if (order[i] == true)
            break;
         if (i == 8)
            return;
            
    // thiet lap trang thai len/xuong
    if (len) {
        for (i = currentLevel; i <= 8; i++) {
            // neu len tang 8 roi ma ko co yeu cau thi chuyen che do cho di xuong
            if (i == 8 && order[i] == false) {
                len = false;
            }
            else if (order[i] == true) {
                break;
            }
        }        
    }
    if (!len) {
        for (i = currentLevel; i >= 0; i--) {
            // tuong tu voi doan di len. neu da o tang 0
            if (i == 0 && order[i] == false) {
                len = true;
                return;
            }
            else if (order[i] == true) {
                break;
            }
        }
    }    

    // trong truong hop co tang dc bam, mo servo 1 goc 90 do trong 2s 
    vi_tri = 15;
    delay_ms(2000);
    vi_tri = 10;
    
    if (len)
        quay_DC();
    else
        quay_nguoc_DC();
              
    while (order[currentLevel] == false) {      
        if (len)
            currentLevel++;
        else
            currentLevel--;
            
        glcd_clear();
        itoa(currentLevel, what);     
        glcd_outtextxy(30, 20, what);
        delay_ms(1000);
    }
    
    order[currentLevel] = false;
    dung_DC();
    vi_tri = 15;
    delay_ms(2000);
    vi_tri = 10;
    
}

void main(void)
{
    init_glcd(4, 66);
    init_register();
    init_timer();
    itoa(currentLevel, what);
    glcd_outtextxy(30, 20, what); 
    
while (1)
    {
    // Please write your application code here
        thang_may();
    }
}
