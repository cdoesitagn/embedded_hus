/*Thiết kế một chiếc máy giặt với các chức năng Hiển thị các phím chức năng trên LCD (-giặt 2-xả, 3-vắt)

Bấm BTI thực hiện chế độ giặt ban đầu role bật trong 2s (để cung cấp nước), sau đó động cơ sẽ quay nhanh dần (sau mỗi giây, tốc độ động cơ tăng lên 20%o- tương đối), sau l0s động cơ dừng quay, role 2 bật trong 2s (để xả nước), sau đó động cơ sẽ quay với tốc độ I00% trong vòng 3s để vắt. sau khi vắt xong đen xanh-đỏ nhấp nháy với
chu ky l s.

Bấm BT2 thực hiện chế độ vắt (thực hiện chương trình từ lúc xa xong)*/
	
#include <io.h>
#include <delay.h>
#include <glcd.h>
#include <font5x7.h>
//Import các thư viện cần sử dụng, delay để tạm ngưng và glcd cho màn lcd


/*
curSpeed: Tốc độ hiện tại của động cơ (khởi tạo là 0).
led: Một biến boolean để điều khiển đèn LED.
counter: Một biến đếm được sử dụng trong hàm ngắt của bộ hẹn giờ.
*/
int curSpeed = 0;
bool led = false;
int counter = 0;

// Interrupt cua timer1
/*
 Hàm ngắt dành cho sự tràn của Timer1. 
 Nó đặt lại giá trị Timer1 và chứa mã lệnh để kiểm soát đèn LED dựa trên biến led.
*/
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
    // Reinitialize Timer1 value
    TCNT1H=0xE0C0 >> 8;
    TCNT1L=0xE0C0 & 0xff;
    // LED control
    if (led) {
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


/*
adjustSpeed: Điều chỉnh tốc độ của động cơ sử dụng Register So sánh Đầu ra của Timer0 (OCR0).
speedUpdater: Cập nhật tốc độ dựa trên option được cung cấp.
*/
void adjustSpeed(int speed) {
    OCR0 = 255 - speed;
}

void speedUpdater(int option) {
    int newSpeed;
    if (curSpeed == 0) {
        curSpeed = 63;
    }
    else {
        if (option == 20)
            newSpeed = curSpeed + curSpeed / 5;
        else if (option == 100)
            newSpeed = curSpeed * 2;
            
        if (newSpeed > 255)
            curSpeed = 255;
        else
            curSpeed = newSpeed;
    }
    adjustSpeed(curSpeed);
}


//Hàm được tạo dùng để vắt quần áo
void vat() {
    int second = -1;
    //bat role 2 trong 2s 
    PORTC.2 = 1;
    delay_ms(2000);
    PORTC.2 = 0;
    
    //quay dong co trong 3s
    while (second != 3) {
        speedUpdater(100);
        second++;
        delay_ms(1000);
    }
    
    curSpeed = 0;
    adjustSpeed(0);
    led = true;
}


// Hàm để giặt quần áo
void giat() {
    int second = -1;
    // bat role 1 trong 2s
    PORTC.3 = 1;
    delay_ms(2000);
    PORTC.3 = 0;
    // quay dong co trong 10s
    while (second != 10) {
        speedUpdater(20);
        second++;
        delay_ms(1000);
    }
    
    curSpeed = 0;
    adjustSpeed(0);
    vat();   
}


/*
Hàm main khởi tạo GLCD, thiết lập cổng, bộ hẹn giờ và ngắt. Sau đó, nó bắt đầu một vòng lặp vô hạn, 
trong đó nó liên tục kiểm tra trạng thái của các nút (BT1 và BT2) và thực hiện các hoạt động tương ứng.
*/
void main(void)
{
    //init GLCD
    GLCDINIT_t glcd_init_data;
    glcd_init_data.font=font5x7;
    glcd_init_data.temp_coef=140;
    glcd_init_data.bias=4;
    glcd_init_data.vlcd=66;
    glcd_init(&glcd_init_data);
    
    //pull up BT1
    DDRB.2 = 0;
    PORTB.2 = 1;
    //pull up BT2
    DDRB.3 = 0;
    PORTB.3 = 1;
    // role 1
    DDRC.3 = 1;
    // role 2
    DDRC.2 = 1;
    // 2 led
    DDRD.5 = 1;
    DDRD.4 = 1;
    
    //Khởi tạo động cơ timer 0 và timer 1

    /*
DDRB.4 = 1; và DDRD.7 = 1;: Thiết lập chân làm chân đầu ra. Trong trường hợp này, chân 4 của PORTB và chân 7 của PORTD được cấu hình để làm chân đầu ra.

ASSR=0<<AS0;: Đặt AS0 (Asynchronous mode bit 0) về 0. Có vẻ như đây không phải là chế độ không đồng bộ.

TCCR0=(1<<WGM00) | (1<<COM01) | (1<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);: Cấu hình thanh ghi điều khiển bộ đếm Timer0 (TCCR0). Đây là các cài đặt cho Timer0, bao gồm chế độ WGM (Waveform Generation Mode) và các chế độ so sánh (COM) cũng như các bit chia tần số (CS) để chọn tần số đếm.

TCNT0=0x00;: Khởi tạo giá trị của bộ đếm Timer0 về 0.

adjustSpeed(0);: Gọi hàm adjustSpeed với tham số 0. Hàm này có vẻ được sử dụng để điều chỉnh tốc độ của động cơ.

TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);: Cấu hình thanh ghi điều khiển bộ đếm Timer1 (TCCR1A). Cũng tương tự như Timer0, nhưng tại đây là cài đặt cho Timer1.

TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);: Cấu hình thanh ghi điều khiển bộ đếm Timer1 (TCCR1B). Các bit ICNC, ICES, WGM, và CS được cài đặt tương tự như Timer0.

TCNT1H=0xE0; và TCNT1L=0xC0;: Khởi tạo giá trị của bộ đếm Timer1 về giá trị cụ thể.
    
    */
    DDRB.4 = 1;
    DDRD.7 = 1;
    ASSR=0<<AS0;
    TCCR0=(1<<WGM00) | (1<<COM01) | (1<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x00;
    adjustSpeed(0);
    TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
    TCNT1H=0xE0;
    TCNT1L=0xC0;
    
    //khoi tao interrupt cho timer1
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
    ETIMSK=(0<<TICIE3) | (0<<OCIE3A) | (0<<OCIE3B) | (0<<TOIE3) | (0<<OCIE3C) | (0<<OCIE1C);
    #asm("sei")
       
    glcd_outtextxy(10, 10, "MAY GIAT");
    glcd_outtextxy(10, 20, "BT1. GIAT");
    glcd_outtextxy(10, 30, "BT2. VAT");
    
while (1)
    {
    // Please write your application code here
        if (PINB.2 == 0)
            giat();
        if (PINB.3 == 0)
            vat();
        delay_ms(250);
    }
}