#ifndef SPI_H
#define SPI_H

class SPI
{
public:
    SPI();
    ~SPI();
    bool initialize();
    void getInfo();
    void transfer(uint8_t& data);
private:
    int spiFd;
};

#endif // SPI_H
