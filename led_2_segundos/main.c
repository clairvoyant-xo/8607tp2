#include <avr/io.h>
#include <avr/interrupt.h>

ISR(PCINT0_vect){
    if((PINB & 0x01) == 0x01){
        return;
    }

    PORTB |= 0x02;  
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
}

ISR(TIMER1_COMPA_vect){
    PORTB &= 0xFD;
    TCCR1B = 0;
    TCNT1 = 0; 
}

void init_irq_pin_change(void){
    PCICR = (1 << PCIE0);
    PCMSK0 = (1 << PCINT0);
}

void init_timer1(void){
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0; 
    OCR1A = 31250;
    TIMSK1 = (1 << OCIE1A);
}

void init_pins(void){
    DDRB = 0x02;
    PORTB = 0x01;
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