/*Viết chương trình bấm BT 1 trước, bầm BT2 sau thi đếm sự kiện tăng dần, nếu bấm BT2 trước, bấm BT1 sau thì đếm sự kiện giảm dần, nếu sự kiện lớn hơn 0 thì bật đèn và đóng rơle 1: nếu sự kiện nhỏ hơn 0 thì xác  lập kiến bằng 0 và tắt đèn đồng thời nhà role I. Nếu sự kiện lớn hơn 5 thì bật thêm role 2, sự kiện nhỏ hơn 5 thì nhà rơ le 2
/*
#include <io.h>
#include <delay.h>

#define TC_1    PINB.2		//bt 1
#define TC_2    PINB.3		//bt 2
#define RL_1    PORTC.3		//role 1
#define RL_2    PORTC.4		//role 2
#define LED 	PORTD.4 	//đèn 

int dem = 0;
int counter = 0;

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    TCNT0=0x06;
    counter++;
}

void ct_delay_ms(unsigned int ms)
{
    counter = 0;
    while (counter < ms){}  
}

void main(void)
{

DDRB = 0x00;
PORTB = 0x0C;

DDRC = 0b00011000;

DDRD = 0x10;

ASSR=0<<AS0;
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
TCNT0=0x06;
OCR0=0x00;
            
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);
ETIMSK=(0<<TICIE3) | (0<<OCIE3A) | (0<<OCIE3B) | (0<<TOIE3) | (0<<OCIE3C) | (0<<OCIE1C);
// Global enable interrupts
#asm("sei")



while (1)
    {
    // Please write your application code here
    	//nhấn nút 2 trc 1 thì sk sẽ giảm dần
        if(TC_2 == 0){    
            while(TC_1);  
                ct_delay_ms(250);
                dem--;
        }
	//nhấn nút 1 trc 2 thì sk sẽ tăng dần
        if(TC_1 == 0){    
            while(TC_2);
                ct_delay_ms(250);
                dem++;
        }
        if(dem < 0) dem = 0; 	//sk nhỏ hơn 0 thì xác lập sự kiện = 0

        if(dem > 0) 		//sk lớn hơn 0 thì bật đèn và đóng role 1	
	{
		RL_1 = 1;
		LED = 1;
	}		
        if(dem < 0) 		//sk lớn hơn 0 thì bật đèn và nhả role 1	
	{
		RL_1 = 0;
		LED = 0;
	}
        
        if(dem >= 5) RL_2 = 1;  //sk lớn hơn or = 5 thì đóng role 2
        if(dem < 5) RL_2 = 0; 	//sk bé hơn 5 thì nhả role 2
    }
}
