/* Viết chương trình bấm phím CT1 đèn sáng, CT2 đèn tắt, CT3 đèn LED nhấp nháy theo trình tự sáng-tối-sáng-tối- sáng mãi. Trong lúc đang làm các tác vụ kể trên thị dòng chữ hello-world chay từ trái sang phải trên màn hình (trong timer)
*/


#include <io.h>
#include <alcd.h> //Màn hình LCD
#include <delay.h>

#define BT1 PINB.0 //Button1
#define BT2 PINB.2 //Button2
#define BT3 PINB.3 //Button3
#define LED PORTD.5 //PORTD bit 5?

unsigned char x=0;
char dem, x, dem_1; //Khởi tạo biến đếm

//hàm xử lý ngắt (interrupt service routine - ISR) được gọi khi Timer/Counter 0 tràn (overflow). Ngắt này có thể được cấu hình để xảy ra sau mỗi lần thanh ghi TCNT0 đếm từ 255 về 0.
interrupt[TIM0_OVF] void timer0_ovf_isr(void) {
    //Hàm ISR đặt lại giá trị của thanh ghi TCNT0 về 6. Điều này có thể là để đảm bảo rằng Timer/Counter 0 sẽ tràn sau mỗi 250 giá trị đếm (vì 256 - 6 = 250).
    TCNT0= 0x06;
    if(x==1){
        dem_1 = dem_1 + 1;
        //Biến dem_1 đếm từ 0 đến 99 và sau đó được đặt lại về 0. Mỗi khi dem_1 đạt đến 100, dem (biến đếm chính) sẽ được tăng lên một đơn vị và dem_1 sẽ được đặt lại về 0.
        if(dem_1 == 100) {
            dem = dem + 1;
            dem_1 = 0;}
        if(dem==10) {
            LED=1;
            }
        if(dem==20) {
            LED = 0;
            }
        if(dem==30) {
            LED = 1;
            }
        if(dem==40) { 
            LED = 0;
        }
        if(dem>=50){
            LED = 1;
            dem = 50;
        }
        }
        
    }


void main(void)
{
DDRB = 0x00;
PORTB = 0x0D;
DDRD = 0x20;
/*
DDRB = 0x00;: Đặt thanh ghi DDRB thành 0 để cấu hình tất cả các chân của Port B làm chân input.
PORTB = 0x0D;: Đặt thanh ghi PORTB thành 0x0D (00001101 trong hệ thập phân), có thể là để kích thích các chân input được kết nối với các nút nhấn hoặc điện trở pull-up nếu có.
DDRD = 0x20;: Đặt thanh ghi DDRD thành 0x20 (00100000 trong hệ thập phân), đặt chân PD5 làm chân output.
*/


/*
ASSR=0<<AS0;: Thiết lập chế độ không đồng bộ cho TC0 (Timer/Counter 0).
TCCR0=0x03;: Cài đặt thanh ghi TCCR0 với giá trị 0x03 (00000011 trong hệ nhị phân), có thể là để chọn chế độ Fast PWM với chia tỷ lệ là 64.
TCNT0=0x06;: Đặt giá trị của thanh ghi TCNT0 về 6.
OCR0=0x00;: Đặt giá trị của thanh ghi OCR0 về 0.
TIMSK=0x01;: Bật ngắt TOIE0 (Timer/Counter 0 Overflow Interrupt).
ETIMSK=0x00;: Vô hiệu hóa các ngắt mở rộng của Timer/Counter 0
*/
ASSR=0<<AS0;
TCCR0=0x03;
TCNT0=0x06;
OCR0=0x00;
TIMSK=0x01;
ETIMSK=0x00;

#asm("sei")
//Sử dụng mã lệnh Assembly (#asm) để bật cờ ngắt toàn cục (sei), cho phép chương trình xử lý ngắt.


/*
Trong vòng lặp chính, có các điều kiện kiểm tra trạng thái của nút nhấn và điều khiển đèn LED dựa trên trạng thái của chúng.

Nếu nút BT1 được nhấn, đèn LED sẽ bật (LED = 1) và biến x được đặt về 0.
Nếu nút BT2 được nhấn, đèn LED sẽ tắt (LED = 0) và biến x được đặt về 0.
Nếu nút BT3 được nhấn, biến x sẽ được đặt về 1.

*/
while (1)
  {
    if(BT1 == 0) {
        delay_ms(250);
        LED = 1;
        x = 0;
    }  
    
    if(BT2 == 0) {
        delay_ms(250);
        LED = 0;
        x = 0;
    }         
    
    if(BT3 == 0) {
        x = 1;
    }
    
    

  }
}
