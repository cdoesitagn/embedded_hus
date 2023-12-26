/*Viết chương trình điều khiển xe điện thực hiện các chức năng sau, (hiến thị tộc độ tương đôi lên LCD/GLCD)
Khi bấm tổ hợp phím 334 trên bàn phím thì đèn xanh sáng, Rơ le l bật
Dùng chiết áp xoay theo chiều kim đồng hồ, ở vị trí giữa>động cơ dừng xoay về phía max động cơ sẽ tăng dần tốc độ, nêu xoay về phía min động cơ sẽ tăng dần tốc độ nhưng ở chiều ngược lại.
Nếu bấm BTI, động cơ sẽ quay ở I tốc độ cố định (không còn chịu ảnh hưởng của chiết áp). Nếu bấm B1 một lần nữa, tốc độ của động cơ lại phụ thuộc vào chiết áp như lúc đầu.*/



#include <io.h>
#include <delay.h>
#include <stdint.h>
/*
#include <io.h>: Thư viện cung cấp các định nghĩa cho thanh ghi và các chức năng cơ bản của vi điều khiển AVR.
#include <delay.h>: Thư viện chứa các hàm liên quan đến đợi (delay), như delay_ms và delay_us.
#include <stdint.h>: Thư viện chứa các định nghĩa cho kiểu dữ liệu số nguyên có chiều dài cố định, như uint8_t
*/


#define ADC_PIN     0      //ADC0
int adc_value, pwm_value;
/*
ADC_PIN: Chỉ định chân ADC được sử dụng, trong trường hợp này là chân ADC0.
adc_value: Lưu trữ giá trị đọc từ chuyển đổi ADC.
pwm_value: Lưu trữ giá trị PWM sẽ được gửi đến đầu ra PWM.
*/

// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))


// Read the AD conversion result
/*
Hàm này đặt ADMUX để chọn chân ADC và áp tham chiếu.
Chờ đợi 10 micro giây để ổn định áp đầu vào ADC.
Bắt đầu chuyển đổi ADC (ADSC).
Đợi cho quá trình chuyển đổi hoàn thành và sau đó trả về giá trị ADC đọc được.
*/

unsigned int read_adc(unsigned char adc_input)
{

    ADMUX=adc_input | ADC_VREF_TYPE;
    // Delay needed for the stabilization of the ADC input voltage
    delay_us(10);
    // Start the AD conversion
    ADCSRA|=(1<<ADSC);
    // Wait for the AD conversion to complete
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA|=(1<<ADIF);
    return ADCW;
}


/*
Hàm này gán giá trị PWM (pwm_value) vào thanh ghi so sánh (OCR1A) của Timer1.
*/
void pwm_write(uint8_t pwm_value)
{     
    OCR1A=pwm_value;
}


/*
DDRB.5 = 1;: Đặt chân PB5 làm đầu ra cho PWM.
Cấu hình và khởi tạo PWM sử dụng Timer1 với các cài đặt như TCCR1A và TCCR1B.
Cấu hình và khởi tạo ADC với các cài đặt như ADMUX và ADCSRA.
Trong vòng lặp chính, đọc giá trị từ chuyển đổi ADC, chuyển đổi nó thành giá trị PWM, và gửi giá trị PWM đó ra chân PB5 thông qua Timer1. Các giá trị ADC được chia cho 1023 để đưa chúng về khoảng 0-255 (giá trị PWM tối đa).
Chờ 10 miligiây trước khi lặp lại quá trình.
*/
void main(void)
{          
    //output pwm pin  
    DDRB.5=1;  
                
    //PWM initialization
    TCCR1A=(1<<COM1A1) | (1<<WGM10);
    TCCR1B=(1<<WGM12) | (1<<CS11);

    
    // ADC initialization
    // ADC Clock frequency: 1000.000 kHz
    // ADC Voltage Reference: AVCC pin
    ADMUX=ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);
    
    //loop
    while (1)
    {
        adc_value=read_adc(ADC_PIN);
        pwm_value=adc_value*255.0/1023.0;
        pwm_write(pwm_value);
        delay_ms(10);
    }
}