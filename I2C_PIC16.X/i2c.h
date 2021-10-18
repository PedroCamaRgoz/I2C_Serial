#ifndef I2C_H
#define I2C_H

void i2cInit (unsigned long F);
void i2cIdleWait (void);
void msspWait (void);
void i2cStar (void);
void i2cWrite (unsigned  dat);
unsigned char i2cRead (char ack);
void i2cAck (void);
void i2cNack (void);
void i2cStop (void);
void i2cRestart (void);

struct i2cT
{
    void (*init) (unsigned long F);
    void (*wait) (void);
    void (*star) (void);
    void (*write) (unsigned  dat);
    unsigned char (*read) (char ack);
    void (*ack) (void);
    void (*Nack) (void);
    void (*stop) (void);
    void (*restart) (void);
    
};

extern struct i2cT i2c;


#endif 