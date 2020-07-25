#ifndef GPIO_H
#define GPIO_H

#include <libgpio.h>

class GPIO
{
public:
    GPIO();
    ~GPIO();
    bool initialize();
    bool cycleFPRT();
    bool setCPFP(bool aVal);
    bool getFPCP(bool& aVal);
private:
    gpio_handle_t gpio1Hdl;
    gpio_handle_t gpio3Hdl;
};

#endif // GPIO_H
