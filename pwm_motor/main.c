#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY 200

volatile uint8_t pos;

const uint8_t vel[3] = {20,120,255};

ISR(PCINT0_vect){
    uint8_t estado = PINB;

    if((estado & 0x01) == 0x00){
        if(pos == 2){
        return;
        }

        OCR0A = vel[++pos];
        _delay_ms(DELAY);        
        return;
    }

    if((estado & 0x02) == 0x00){
        if(pos == 0){
        return;
        }

        OCR0A = vel[--pos];
        _delay_ms(DELAY);
        return;
    }

}

void init_irq_pin_change(void){
    PCICR = (1 << PCIE0);
    PCMSK0 = (1 << PCINT0) | (1 << PCINT1);
}

void init_timer0(void){
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS00);
    TCNT0 = 0; 
    OCR0A = vel[0];
}

void init_pins(void){
    DDRB = 0x00;
    DDRD = 0x40;
    PORTB = 0x03;
}

void reset(void){
    init_irq_pin_change();
    init_pins();
    init_timer0();
    sei();
}

int main(void){
    reset();
    pos = 0;
    while(1){
    }
    return 0;
}