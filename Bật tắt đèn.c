//Sử dụng LED màu RGB, viết chương trình bấm BIl, lần 1 đèn R sáng, lần 2 đèn R tắt đen G sáng. lần 3 đèn G
//tắt đèn B sáng, lặp đi lặp lại liên tục. Trong lúc đang làm các tác vụ kệ trên thị dòng chữ hello-world chạy từ trái sang phải trên màn hình (trong timer)


#include <io.h>
#include <delay.h>

//gán tên cho các hardware ta sẽ tương tác vs
#define BT1 PINB.0
#define BT2 PINB.2
#define BT3 PINB.3
#define RED PORTD.4
#define GREEN PORTD.5
#define BLUE PORTD.6


void main(void)
{
DDRB = 0x00;    
PORTB = 0x0D;   //0b0000 1101
DDRD = 0x70;    //0b0111 0000
PORTD = 0x00;   
while (1)
    {
        //Bấm nút 1, bật đèn led đỏ lên
        if(BT1 == 0) {
            RED = 1;
            GREEN = 0;
            BLUE = 0;
            delay_ms(250);     //delay việc tắt đèn khi nhả nút trong 0.25s
        }    
        //Bấm nút 2, bật đèn led xang lá lên
        if(BT2 == 0) {    
            RED = 0;
            GREEN = 1;
            BLUE = 0;  
            delay_ms(250);    //delay việc tắt đèn khi nhả nút trong 0.25s
        }
        //Bấm nút 3, bật đèn led dương lên
        if(BT3 == 0) {
            RED = 0;
            GREEN = 0;
            BLUE = 1;  
            delay_ms(250);    //delay việc tắt đèn khi nhả nút trong 0.25s
        }

    }
}