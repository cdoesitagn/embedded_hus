#include <io.h>
#include <alcd.h>
#include <delay.h>

#define PIN_DHT PINB.7
#define PORT_DHT PORTB.7
#define DDR_DHT DDRB.7

char data[16];
int checksum=0;

char dht11(unsigned char *nhietdo, unsigned char *doam) {
    int i, j;
    int buffer[5] = {0,0,0,0,0};
    // buoc 1
    DDR_DHT = 1;
    PORT_DHT = 1;
    delay_us(60);
    // buoc 2
    PORT_DHT = 0;
    delay_ms(25);
    // buoc 3
    DDR_DHT = 0;
    PORT_DHT = 1; 
    
    // check pin
    delay_us(60);
    if (PIN_DHT) return 0;
    else while (!(PIN_DHT));
    delay_us(60);
    if (!PIN_DHT) return 0;
    else while ((PIN_DHT));
              
    
    for (i = 0;i < 5;++ i) {
        for (j = 0;j < 8;++ j) {
            while (!(PIN_DHT));
            delay_us(50);
            if (PIN_DHT) {
                buffer[i] |= (1<<(7-j));
                while ((PIN_DHT));
            }
        }
    }    
    checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];

    if((checksum)!=buffer[4]) return 0;
    
    *nhietdo = buffer[2];
    *doam = buffer[0];
    return 1; 
}
