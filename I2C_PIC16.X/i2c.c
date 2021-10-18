/*
 * File:   i2c.c
 * Author: 20187263
 *
 * Created on 14 de Outubro de 2021, 14:22
 */

// Módulo Master Synchronous Serial Port (MSSP) : i2c ? Inter-Integrated Circuit (I2C)

#include <xc.h>
#include "i2c.h"
#include "delay.h"


#define MODO_SERIAL 0x08
#define HAB_I2C 1
#define FREQ( X ) (_XTAL_FREQ/(4*X*100))-1

struct i2cT i2c = {i2cInit ,msspWait ,i2cStar ,i2cWrite ,i2cRead ,i2cAck ,i2cNack ,i2cStop ,i2cRestart};

void i2cInit (unsigned long F)
{
    TRISCbits.TRISC3 = 1; // SCL
    TRISCbits.TRISC4 = 1; // SDA   
    SSPCONbits.SSPEN = HAB_I2C;
    SSPCONbits.SSPM =  MODO_SERIAL; 
    SSPCON2 = 0;
    SSPADD = FREQ( F );
    SSPSTAT = 0;
}

void i2cIdleWait (void) // verifica se a transmissão de dados;
{
    while( ( SSPCON2 & 0x1F || SSPSTAT  & 0x04 ) );
}

void msspWait (void)  // gerado a cada evento no barramento 
{
    while(!SSPIF); // ocorreu evento?
    SSPIF = 0;     // zero eventos
}

void i2cStar (void)
{
    i2cIdleWait();  // verifica se esta inativo
    SEN = 1;  // inicia partida;
    msspWait(); 
}

void i2cWrite (unsigned  dat)
{
     //i2cIdleWait();
    repeat: 
    SSPBUF = dat;
    msspWait();
       
    if(SSPCON2bits.ACKSTAT) // reconhecimento por parte do slave
    {      
       i2cRestart(); // reinicia o processo e manda o dat dnv;   
       msspWait();
       goto repeat ;
    }   
}

unsigned char i2cRead (char ack)
{
    RCEN = 1; // Habilita recebimento
    while(!SSPSTATbits.BF);   // BF: 0 - VAZIO ; 1 - CHEIO
    
    (ack = 1)? i2cAck() : i2cNack(); // dados confirmados?
    
    RCEN = 0; // desabilita recebimento
    
    return( SSPBUF );   
}

void i2cAck (void)
{
    ACKDT = 0; // reconhecer 
    ACKEN = 1; // sequencia de reconhecimento
    msspWait();
}

void i2cNack (void)
{
    ACKDT = 1; // n reconhecer 
    ACKEN = 1;  // sequencia de n reconhecimento
    msspWait(); // aguarda o final do evento     
}

void i2cStop (void)
{
    PEN = 1;
    msspWait();
    SSPEN = 0;   // desativa portas serial i2c 
}

void i2cRestart (void)
{
    RSEN = 1; // habilita a reinicialização
    msspWait(); // espera
}

