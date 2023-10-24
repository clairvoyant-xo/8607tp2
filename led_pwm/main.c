#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY 200

ISR(PCINT0_vect){
    uint8_t estado = PINB;

    if((estado & 0x01) == 0x00){
        if(OCR1A == 0xFF){
        return;
        }

        if(OCR1A == 0x00){
            TCCR1A = (1 << COM1A1) | (1 << WGM10);
            TCCR1B = (1 << WGM12) | (1 << CS10);
        }

        OCR1A = OCR1A + 51;
        _delay_ms(DELAY);        
        return;
    }

    if((estado & 0x04) == 0x00){
        if(OCR1A == 0x00){
        return;
        }

        if(OCR1A == 51){
            TCCR1A = 0;
            TCCR1B = 0;
        }

        OCR1A = OCR1A - 51;
        _delay_ms(DELAY);
        return;
    }

}

void init_irq_pin_change(void){
    PCICR = (1 << PCIE0);
    PCMSK0 = (1 << PCINT0) | (1 << PCINT2);
}

void init_timer1(void){
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS10);
    TCNT1 = 0; 
    OCR1A = 0xFF;
}

void init_pins(void){
    DDRB = 0x02;
    PORTB = 0x05;
}

void reset(void){
    init_irq_pin_change();
    init_timer1();
    init_pins();
    sei();
}

int main(void){
    reset();
    while(1){
    }
    return 0;
}