#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define MODO 0x00
#define DELAY 200

volatile uint8_t pos;

const uint8_t vel[3] = {100,120,150};

ISR(PCINT2_vect){
    uint8_t estado = PIND;

    if((estado & 0x01) == 0x00){
        if(pos == 2){
        return;
        }

        OCR0A = vel[++pos];
        OCR0B = vel[++pos];
        _delay_ms(DELAY);        
        return;
    }

    if((estado & 0x02) == 0x00){
        if(pos == 0){
        return;
        }

        OCR0A = vel[--pos];
        OCR0B = vel[--pos];
        _delay_ms(DELAY);
        return;
    }

}

void init_irq_pin_change(void){
    PCICR = (1 << PCIE2);
    PCMSK0 = (1 << PCINT16) | (1 << PCINT17);
}

void init_timer0(void){
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS00);
    TCNT0 = 0; 
    OCR0A = vel[0];
    OCR0B = vel[0];
}

void init_pins(void){
    DDRB = 0x11;
    DDRD = 0xF0;
    PORTB = 0x00;
    PORTD = 0x03;
}

void init_shield(void){
    uint8_t modo = MODO;
    for(uint8_t i = 0; i < 8; i++){      
        PORTB |= (modo & 0x01);
        PORTD |= 0x10;
        PORTD &= 0xEF; 
        modo = modo >> 1;
    }
    PORTB |= 0x10;
    PORTB &= 0xEF;
}

void reset(void){
    init_pins();
    init_timer0();    
    init_shield();
    init_irq_pin_change();    
    sei();
}

int main(void){
    reset();
    pos = 0;
    while(1){
    }
    return 0;
}