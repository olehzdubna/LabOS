/**
 * @brief 
 * @param argc
 * @param argv
 * @return 
 */

#include <unistd.h>

#include <iostream>
#include "SPI.h"
#include "GPIO.h"

int spiTest()
{
    SPI spi;
    GPIO gpio;
    
    std::cout << "SPI test" << std::endl;

    gpio.initialize();

    spi.initialize();
    spi.getInfo();

    uint8_t data = 0x00; 
    spi.transfer(data);
    ::usleep(50000);

    gpio.setCPFP(1);
    gpio.cycleFPRT();

    sleep(1);
 /*  
    while(1)
    {
        bool val = true;
        gpio.getFPCP(val);
        if(!val)
        {
            printf("+++ low FPCP detected\n");
            break;
        }
    }

    gpio.setCPFP(0);
 
    data = 0x00; 
//    data = 0x81; 
    spi.transfer(data);
    ::usleep(1000);

    data = 0x00; 
//    data = 0x5a;
    spi.transfer(data);
    ::sleep(1);

    data = 0x00; 
//    data = 0x0d;
    spi.transfer(data);
    ::usleep(1000);

    data = 0x00;
    spi.transfer(data);
    ::usleep(1000);

    data = 0x00;
    spi.transfer(data);
    ::sleep(1);

    gpio.setCPFP(1);
*/
    while(1)
    {
        bool val = true;
        gpio.getFPCP(val);
        if(!val)
        {
            printf("+++ low FPCP detected\n");

            gpio.setCPFP(0);

            data = 0x00;
            spi.transfer(data);
            
            gpio.setCPFP(1);
        }
    }

    return 0;
}
