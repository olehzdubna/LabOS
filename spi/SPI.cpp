/**
 * @brief 
 * @return 
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/spigenio.h>


#include "SPI.h"

class transfer;
SPI::SPI()
{
}

SPI::~SPI()
{
    ::close(spiFd);
}

bool SPI::initialize()
{
    static const char* ipiName = "/dev/spigen0.0"; 
    
    spiFd = ::open(ipiName, 0);
    if(spiFd < 0)
    {
        perror("+++ Cannot open SPI device");
        return false;
    }
    printf("*** SPI initialized ***\n");
    
    return true;
}

void SPI::getInfo()
{
    uint32_t clockSpeed;
    uint32_t spiMode;
    
    if(::ioctl(spiFd, SPIGENIOC_GET_CLOCK_SPEED, &clockSpeed) < 0)
    {
        perror("+++ Cannot get clock speed");
        return;
    }
 
    printf("SPI clock speed: %d\n", clockSpeed);
    
    if(::ioctl(spiFd, SPIGENIOC_GET_SPI_MODE, &spiMode) < 0)
    {
        perror("+++ Cannot get spi mode");
        return;
    }
    printf("SPI mode: %8.8x\n", spiMode);
}

void SPI::transfer(uint8_t& data)
{
    struct spigen_transfer spiTransfer;
    spiTransfer.st_command.iov_base = &data;
    spiTransfer.st_command.iov_len = sizeof(data);
    spiTransfer.st_data.iov_base = nullptr;
    spiTransfer.st_data.iov_len = 0;
//    spiTransfer.st_data.iov_len = sizeof(dataIn);

    printf("+++ SPI transfer in: %2.2x\n", data);
    
    if(::ioctl(spiFd, SPIGENIOC_TRANSFER, &spiTransfer) < 0)
    {
        perror("+++ Cannot do SPI transfer");
        return;
    }
    
    printf("+++ SPI transfer out: %2.2x\n", data);
}
