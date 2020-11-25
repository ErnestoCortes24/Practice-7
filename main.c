// Ernesto Cortés Zamora - A01274539
// Laboratorio de Microcontroladores 
// Práctica 7 - Teclado

// Libraries/headers section
#include "device_config.h"

// Directives section
#define _XTAL_FREQ 1000000
#define ONE_SECOND 1000

// Data Type section

enum port_dir {output, input}; // output = 0, input = 1
enum port_ACDC {digital, analog};// digital = 0, analog = 1
enum resistor_state {set_ON, res_ON};// set_ON = 0, res_ON = 1
enum led_state {led_OFF, led_ON};// led_OFF = 0, led_ON = 1
enum butto_state {pushed, no_pushed};// pushed = 0, no_pushed = 1


// ISR for high-priority
void __interrupt (high_priority ) high_isr ( void );

//ISR for low-priority
void __interrupt (low_priority) low_isr (void);

//Functions declaration
void portsInit( void );  // Ports configuration

/* unsigned char bcdDecoder(unsigned char input){
    switch(input){
        case'': return 0x00;
        default: return 0x00;
    }
}*/

// Main Section
// Código principal con tabla de verdad para mi teclado matricial 4x4

void main(void){ 
    // Configuration
    portsInit();
    unsigned short column = 0;
    
    //Infinite Cycle
    while(1){
        if(column <4){
            column++;
        }else{
            column = 0;
        }
        LATB &= 0x00;
        switch(column){                 // columns - rows
            case 1: LATB |= 0xE0; break;// 1110 XXXX
            case 2: LATB |= 0xD0; break;// 1101 XXXX
            case 3: LATB |= 0xB0; break;// 1011 XXXX
            case 4: LATB |= 0x70; break;// 0111 XXXX
            default : LATB &= 0x00; break;// 0000
        }

        // En todos se coloca un corrimiento << 4 para PORTD, para que 
        // independientemente de lo que tenga, me lo recorra el número actual
        // mostrado a los bits más significativos y el nuevo a los menos 
        // significativos. 
        
        switch (PORTB){
            // Se presiona row 4
            case 0x77:        // Column 4 AND row 4 -> Key 1
                       LATD |= 0x01;
                       break;
            case 0xB7:        // Column 3 AND row 4 -> Key 2
                       LATD |= 0x02;
                       break;
            case 0xD7:        // Column 2 AND row 4 -> Key 3
                       LATD |= 0x03;
                       break;
            case 0xE7:        // Column 1 AND row 4 -> Key A
                       LATD |= 0x0A;
                       break;
                       
            // Se presiona row 3           
            case 0x7B:        // Column 4 AND row 3 -> Key 4
                       LATD |= 0x04;
                       break;
            case 0xBB:        // Column 3 AND row 3 -> Key 5
                       LATD |= 0x05;
                       break;
            case 0xDB:        // Column 2 AND row 3 -> Key 6
                       LATD |= 0x06;
                       break;
            case 0xEB:        // Column 1 AND row 3 -> Key B
                       LATD |= 0x0B;
                       break;

            // Se presiona row 2                       
            case 0x7D:        // Column 4 AND row 2 -> Key 7
                       LATD |= 0x07;
                       break;
            case 0xBD:        // Column 3 AND row 2 -> Key 8
                       LATD |= 0x08;
                       break;
            case 0xDD:        // Column 2 AND row 2 -> Key 9
                       LATD |= 0x09;
                       break;
            case 0xED:        // Column 1 AND row 2 -> Key C
                       LATD |= 0x0C;
                       break;
                       
            // Se presiona row 1            
            case 0x7E:        // Column 4 AND row 1 -> Key * = F
                       LATD |= 0x0F;
                       break;
            case 0xBE:        // Column 3 AND row 1 -> Key 0
                       LATD |= 0x00;
                       break;
            case 0xDE:        // Column 2 AND row 1 -> Key # = E
                       LATD |= 0x0E;
                       break;
            case 0xEE:        // Column 1 AND row 1 -> Key D
                       LATD |= 0x0D;
                       break;
        }
    LATD <<= 4;  
    }
}

// Functions Section

//Ports configuration
void portsInit(void){
    
    TRISB = 0x0F;   // Puertos B (PORTB) son columnas (outputs) del RB4 al RB7
                    // rows o filas (inputs) del RB0 al RB3
    
    TRISD = 0x00;   // Puertos D (PORTD) todas son salidas (van a los LEDs).
    
    ANSELB = 0;     // Puertos B y D se colocan de tipo digital
    ANSELD = 0;
    
}