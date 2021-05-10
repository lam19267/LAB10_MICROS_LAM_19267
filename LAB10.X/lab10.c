/*
 * File:   lab10.c
 * Author: gelp0
 *
 * Created on 4 de mayo de 2021, 12:16 PM
 * * Laboratorio 10
 * Archivo:      lab10.S
 * Dispositivo:	PIC16F887
 * Autor:	Guillermo Lam
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa:	comunicacion serial
 * Hardware:	consola y leds
 *
 * Creado: 4 mayo, 2021
 * Ultima modificacion: 9 may, 2021
 */


//  Bits de configuracion   //
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)



#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  

void setup(void);   //variable de configuraciones
void putch(char data); //variable de transmision    
void text(void);    //variable de testp

//MAIN
void main(void) {
    setup();    //se llama a la configuracion
    while(1){   //loop
        text();} //se llama al texto
}

//PUTCH
void putch(char data){    //se deslpiega la cadena de caracteres  
    while(TXIF == 0);
    TXREG = data; 
    return;
}

//MENU DE LA CONSOLA
void text(void){
    __delay_ms(250); 
    printf("\r Elija una opcion: \r");
    __delay_ms(250);
    printf(" 1. Desplegar cadena de caracteres \r");
    __delay_ms(250);
    printf(" 2. Cambiar PORTA \r");
    __delay_ms(250);
    printf(" 3. Cambiar PORTB \r");
    //OPCIONES DE LA CONSOLA
    while (RCIF == 0);
    if (RCREG == '1'){ 
        __delay_ms(500);
        printf("\r TA TODO BIEN \r");
    }
    if (RCREG == '2'){ 
        printf("\r Insertar caracter para desplegar en PORTA: \r");
        while (RCIF == 0);
        PORTA = RCREG; 
    }
    if (RCREG == '3'){ 
        printf("\r Insertar caracter para desplegar en PORTB: \r");
        while (RCIF == 0);
        PORTB = RCREG;
    }
    else{ 
        NULL;  //seguridad para que el usuario no ponga otras opciones    
    }
    return;
}

//CONFIGURACIONES
void setup(void){
    
    //PINES DIGITALES
    ANSEL = 0;
    ANSELH = 0;
    
    //INPUTS Y OUTPUTS
    TRISA = 0x0;
    TRISB = 0x0;
    
    //LIMPIAR LOS PUERTOS
    PORTA = 0x00;
    PORTB = 0x00;
    
    //RELOJ
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;   
    OSCCONbits.SCS = 1;
    
    //INTERRUPCIONES
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;    
    PIE1bits.RCIE = 1;      
    PIE1bits.TXIE = 1;      


    //CONFIGUTACION TX Y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 208;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;
    
    PIR1bits.RCIF = 0;  
    PIR1bits.TXIF = 0;  

}