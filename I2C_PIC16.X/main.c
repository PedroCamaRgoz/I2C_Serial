/*
 * File:   main.c
 * Author: 20187263
 *
 * Created on 14 de Outubro de 2021, 14:11
 */

#include "config.h"
#include <xc.h>
#include "delay.h"
#include "i2c.h"

void main(void)
{
    i2c.init(100);
    
    while(1)
    {
        i2c.star();
        i2c.write(32);
        delay(100);
        i2c.stop();
        
   
       
        
        
    }
   
}
