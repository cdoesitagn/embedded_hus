// VIết chương trình mô phỏng tay ga của xe đạp điện/máy điện/ôtô (dẫn động l banh -> điều khiển tốc độ động cơ theo thay đổi của chiết áp. hiển thị tốc độ tương đối lên GLCD/LCD (tùy chọn)
// gợi ý: sử dụng chiết áp thay đổi giá trị điện áp ADC vào một kênh nào đó và thay dội giả tri PWM để điều khiển tốc độ động cơ)

#include <io.h>
#include <delay.h>
#include <stdint.h>
/*
Các thư viện
#include <io.h>: Đây là một thư viện cung cấp các định nghĩa cho thanh ghi (register) và các chức năng cơ bản của vi điều khiển AVR.
#include <delay.h>: Thư viện chứa các hàm liên quan đến đợi (delay), như delay_ms và delay_us.
#include <stdint.h>: Thư viện chứa các định nghĩa cho kiểu dữ liệu số nguyên có chiều dài cố định, như uint8_t.

*/

#define ADC_PIN     0      //ADC0

// Khởi tạo giá trị adc, pwm
/*
ADC_PIN: Định nghĩa chân ADC được sử dụng, trong trường hợp này là chân ADC0.
adc_value: Biến lưu trữ giá trị đọc từ chuyển đổi ADC.
pwm_value: Biến lưu trữ giá trị PWM sẽ được gửi đến đầu ra PWM.
*/

int adc_value, pwm_value;

// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))
// Read the AD conversion result
/*
Hàm này đọc giá trị từ chuyển đổi ADC trên chân được chỉ định (adc_input).
Cài đặt ADMUX để chọn chân ADC và áp tham chiếu.
Thực hiện chuyển đổi ADC và đợi cho đến khi nó hoàn thành.
Trả về giá trị ADC đọc được.
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

//Hàm này ghi giá trị PWM (Điều chế độ rộng xung PWM)
//Hàm này ghi giá trị PWM vào thanh ghi so sánh (OCR1A) của Timer1.
void pwm_write(uint8_t pwm_value)
{     
    OCR1A=pwm_value;
}

/*
Khai báo biến cần thiết.
Thiết lập chân PB5 làm đầu ra cho PWM.
Khởi tạo cấu hình PWM cho Timer1.
Khởi tạo cấu hình ADC và các thông số liên quan.
Trong vòng lặp chính, đọc giá trị ADC từ chân ADC0, chuyển đổi nó thành giá trị PWM, và gửi giá trị PWM đó ra chân PB5 thông qua Timer1.
Các giá trị được đọc từ ADC được chia cho 1023 để đưa chúng về khoảng 0-255 (giá trị PWM tối đa).
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