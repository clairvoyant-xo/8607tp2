#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY 200

volatile uint8_t velocidad;
volatile uint8_t sentido;

const uint8_t vel_m3[5] = {0,136,141,147,149};
const uint8_t vel_m4[5] = {0,136,141,147,149};
const uint8_t sent[3] = {0x00,0x84,0x03};

ISR(PCINT2_vect){
    uint8_t estado = PIND;

    if((estado & 0x01) == 0x00){
        if(velocidad == 4){
        return;
        }

        OCR0A = vel_m3[++velocidad];
        OCR0B = vel_m4[velocidad];
        _delay_ms(DELAY);        
        return;
    }

    if((estado & 0x02) == 0x00){
        if(velocidad == 0){
        return;
        }

        OCR0A = vel_m3[--velocidad];
        OCR0B = vel_m4[velocidad];
        _delay_ms(DELAY);        
        return;
    }

}

void init_irq_pin_change(void){
    PCICR = (1 << PCIE2);
    PCMSK2 = (1 << PCINT16) | (1 << PCINT17);
}

void init_timer(void){
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS00);
    TCNT0 = 0; 
    OCR0A = vel_m3[velocidad];
    OCR0B = vel_m4[velocidad];
}

void init_shield(void){
    PORTB &= 0xEF;
    for(uint8_t i = 0; i < 8; i++){
        PORTD &= 0xEF;
        if((sent[sentido] & (1 << i)) == 0){
            PORTB &= 0xFE;
        } else{
            PORTB |= 0x01;            
        }
        PORTD |= 0x10;
    }
    PORTB |= 0x10;
}

void init_pins(void){
    DDRB = 0x11;
    DDRD = 0xF0;
    PORTB = 0x00;
    PORTD = 0x03;
}

void reset(void){
    init_pins();
    init_timer();
    init_shield();
    init_irq_pin_change();    
    sei();
}

int main(void){
    velocidad = 0;
    sentido = 0;
    reset();
    while(1){
    }
    return 0;
}
