#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY 200

volatile uint8_t velocidad;
volatile uint8_t direccion;

const uint8_t vel[3] = {210,230,255};
const uint8_t dir[3] = {0x04,0x60,0x00};

ISR(PCINT2_vect){
    uint8_t estado = PIND;

    if((estado & 0x01) == 0x00){
        if(velocidad == 2){
        return;
        }

        if(direccion == 0){
        OCR0A = vel[++velocidad];
        OCR0B = vel[++velocidad];
        _delay_ms(DELAY);        
        return;
        }

        if(direccion == 1){
        OCR2A = vel[++velocidad];
        OCR2B = vel[++velocidad];
        _delay_ms(DELAY);        
        return;
        }

        return;
    }

    if((estado & 0x02) == 0x00){
        if(velocidad == 0){
        return;
        }

        if(direccion == 0){
        OCR0A = vel[--velocidad];
        OCR0B = vel[--velocidad];
        _delay_ms(DELAY);        
        return;
        }

        if(direccion == 1){
        OCR2A = vel[--velocidad];
        OCR2B = vel[--velocidad];
        _delay_ms(DELAY);        
        return;
        }

        return;
    }

}

void init_irq_pin_change(void){
    PCICR = (1 << PCIE2);
    PCMSK2 = (1 << PCINT16) | (1 << PCINT17);
}

void init_timer(void){
    if(direccion == 0){
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS00);
    TCNT0 = 0; 
    OCR0A = vel[velocidad];
    OCR0B = vel[velocidad];
    }

    if(direccion == 1){
    TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS20);
    TCNT2 = 0; 
    OCR2A = vel[velocidad];
    OCR2B = vel[velocidad];
    }
}

void init_shield(void){
    for(uint8_t i = 0; i < 8; i++){
        uint8_t aux = dir[direccion] >> i;
        PORTB |= (aux & 0x01);
        PORTD |= 0x10;
        PORTD &= 0xEF; 
    }
    PORTB |= 0x10;
    PORTB &= 0xEF;
}

void init_pins(void){
    DDRB = 0x5F;
    DDRD = 0xF8;
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
    direccion = 0;
    reset();
    while(1){
    }
    return 0;
}
