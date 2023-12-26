/*Viết chương trình sử dụng 1 nút bấm để điều chỉnh thời gian trên đồng hồ 25 (nhở lại cái đồng hồ casio đeo tay dùng duy nhất l nút bấm, khi bấm lần l, ta chỉnh giờ, bấm lần 2 thì con trỏ nhảy sang để chỉnh phút, bấm lần 3 Con trỏ chạy sang để chinh giây . ngày, tháng, năm) Khi con trỏ đang ở vị trí chỉnh giờ, nếu ta bấm giữ button thì ngày/giờ/phút nó sẽ nhảy từ gia trị thấp nhất đến giá trị cao nhất (đối với năm chỉ thay đổi từ 2020- 2030) làm sao để thoát. Khi bạn không tác động vào trong khoảng 3s thì chương trình sẽ nhảy ra và lưu giá trị cuối cùng các
bạn bấm vào.
/*
#include <io.h>
#include <alcd.h>
#include <stdint.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <DS1307.h>

#define BUTTON  PINA.0


//millis
volatile unsigned long millis=0;
//program var
uint8_t blink_var=0,blink_count=0,blink_status=0;
uint32_t time_blink=0;
char buffer1[20],buffer2[20];
unsigned char gio,phut,giay,thu,ngay,thang,nam;
uint8_t button_old=1,button_new=1,bt_value=0;
uint32_t time_button=0;
uint8_t button_status;   
int setup_status=0;
uint32_t setup_time=0;


interrupt [TIM2_COMP] void timer2_comp_isr(void)
{
    millis++;
}
void millis_setup()
{
    TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (1<<CS20);
    TCNT2=0x00;
    OCR2=0x7C;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=(1<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
    // Global enable interrupts
    #asm("sei")
}
int read_button()    //0: ko nhấn    | 1:nhấn nhả    | 2:nhấn giữ
{
    button_old=button_new;
    button_new=BUTTON;   
    if(!BUTTON)
        setup_time=millis;  
    //neu co nhan
    if(button_old && !button_new) 
    {            
        //kiem tra trong 1s ke tu luc nhan
        time_button=millis;
        while((unsigned long)(millis-time_button)<500)
        { 
            //trong 1s ma nha thi la nhan nha
            if(BUTTON)  
                return 1;
            delay_ms(10);
        }
        //qua 1s ma van giu thi la nhan giu
        return 2;
    } 
    //neu ko nhan 
    else
        return 0;
}   
void setup_date_and_time()
{
    button_status=read_button();  
    //nếu nhấn một lần thì chuyển sang chế độ chỉnh tg, tùy vào lần bấm thì sẽ chỉnh giờ, phút, giây,..
    if(button_status==1)
    {
        setup_status++;
        if(setup_status==7)
            setup_status=1;
        blink_var=setup_status; 
    }
    //nếu là nhấn giữ thì các giá trị thời gian sẽ tăng dần cho đến nhả (tăng theo giới hạn của đơn vị thời gian đó thui) 
    // Còn nếu nhả ra thì sẽ lưu lại thời gian cuối cùng và cài đặt thời gian của đồng hồ lại theo đó.
    else if(button_status==2 || (button_status==0 && !BUTTON))
    {
        if(blink_var==1)
        {
            gio++;
            if(gio>23)
                gio=0;
        }
        else if(blink_var==2)
        {
            phut++;
            if(phut>59)
                phut=0;
        } 
        else if(blink_var==3)
        {
            giay++;
            if(giay>59)
                giay=0;
        }  
        else if(blink_var==4)
        {
            ngay++;
            if((ngay>28 && thang==2 && nam%4!=0) || (ngay>29 && thang==2 && nam%4==0))
                ngay=1;
            else if(ngay>30 && (thang==4||thang==6||thang==9||thang==11))
                ngay=1;  
            else if(ngay>31)
                ngay=1;
        }
        else if(blink_var==5)
        {
            thang++;
            if(thang>12)
                thang=1;
        } 
        else if(blink_var==6)
        {
            nam++;
            if(nam>30)
                nam=20;
        } 
        rtc_set_time(gio,phut,giay); 
        rtc_set_date(thu,ngay,thang,nam); 
    }  
    if(millis-setup_time>3000)
    {
        setup_status=0;
        blink_var=0;
    }
}
void LCD_show()
{
    if(blink_var!=0)  
    {  
        blink_count=(blink_count+1)%10;    
        if(blink_count==0)
            blink_status^=1;  
        if(blink_status)   
        {
            sprintf(buffer1,"Time: %d%d:%d%d:%d%d  ", gio/10, gio%10, phut/10, phut%10, giay/10, giay%10);     
            sprintf(buffer2,"Date: %d%d:%d%d:20%d  ", ngay/10, ngay%10, thang/10, thang%10, nam);   
        }
        else if(BUTTON)
        {  
            if(blink_var==1)
                sprintf(buffer1,"Time: __:%d%d:%d%d  ", phut/10, phut%10, giay/10, giay%10);  
            else if(blink_var==2)
                sprintf(buffer1,"Time: %d%d:__:%d%d  ", gio/10, gio%10, giay/10, giay%10);  
            else if(blink_var==3) 
                sprintf(buffer1,"Time: %d%d:%d%d:__  ", gio/10, gio%10, phut/10, phut%10); 
            else if(blink_var==4)   
                sprintf(buffer2,"Date: __:%d%d:20%d  ", thang/10, thang%10, nam);  
            else if(blink_var==5)   
                sprintf(buffer2,"Date: %d%d:__:20%d  ", ngay/10, ngay%10, nam); 
            else    
                sprintf(buffer2,"Date: %d%d:%d%d:____  ", ngay/10, ngay%10, thang/10, thang%10);  
        }  
    }   
    else 
    {
        sprintf(buffer1,"Time: %d%d:%d%d:%d%d  ", gio/10, gio%10, phut/10, phut%10, giay/10, giay%10); 
        sprintf(buffer2,"Date: %d%d:%d%d:20%d  ", ngay/10, ngay%10, thang/10, thang%10, nam);   
    }
    lcd_gotoxy(0,0);
    lcd_puts(buffer1);   
    lcd_gotoxy(0,1);
    lcd_puts(buffer2);
}

void main(void)
{
    //output pin
    DDRD=0xFF;
    DDRE=0xFF; 
    
    //input pullup pin
    DDRA.0=0;
    PORTA.0=1;
         
    lcd_init(16);   // khoi tao LCD1602
    lcd_clear();    // xoa toan bo man hinh LCD1602     
    
    //rtc
    i2c_init();
    rtc_init(0,1,0);  
    //rtc_set_time(20,55,0);  
    
    millis_setup();
    
    
    while (1)
    {                  
        //lay thoi gian tu ds1307
        rtc_get_time(&gio,&phut,&giay); 
        rtc_get_date(&thu,&ngay,&thang,&nam);  
        
        setup_date_and_time();
               
        //hien thi len LCD 
        LCD_show();    
        
        delay_ms(20); 
    }
}