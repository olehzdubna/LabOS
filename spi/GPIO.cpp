/**
 * @brief 
 * @return 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "GPIO.h"
#include <errno.h>
#include <err.h>

#define _CPFP_PIN 19 // GPIO_3_19
#define _FPCP_PIN 21 // GPIO_3_21
#define _FPRT_PIN 19 // GPIO_1_19
#define _BOOT_PIN 16 // GPIO_1_16
#define _XXXX_PIN 18 // GPIO_1_18

GPIO::GPIO()
    : gpio1Hdl(-1)
    , gpio3Hdl(-1)
{
}

GPIO::~GPIO()
{
    gpio_close(gpio3Hdl);
    gpio_close(gpio1Hdl);
}

bool GPIO::initialize()
{
    gpio_config_t pin_config;
    int res = 0;

    memset(&pin_config, 0, sizeof(gpio_config_t));
    
    static const char* gpio1Name = "/dev/gpioc1"; 

	gpio1Hdl = gpio_open_device(gpio1Name);
	if (gpio1Hdl == GPIO_INVALID_HANDLE)
    {
		err(EXIT_FAILURE, "Cannot open GPIO1 device %s", gpio1Name);
        return false;
    }
    
    pin_config.g_pin = _FPRT_PIN;
	pin_config.g_flags = GPIO_PIN_OUTPUT | GPIO_PIN_PULLUP;
	res = gpio_pin_set_flags(gpio1Hdl, &pin_config);
	if (res < 0)
    {
		err(EXIT_FAILURE, "configuration of FPRT pin %d on %s "
			    "failed (flags=%d)", pin_config.g_pin, gpio1Name,
			    pin_config.g_flags);
        return false;
    }

    printf("*** GPIO1 initialized ***\n");

    static const char* gpio3Name = "/dev/gpioc3"; 
    
	gpio3Hdl = gpio_open_device(gpio3Name);
	if (gpio3Hdl == GPIO_INVALID_HANDLE)
    {
		err(EXIT_FAILURE, "Cannot open GPIO3 device %s", gpio3Name);
        return false;
    }
    
    pin_config.g_pin = _FPCP_PIN;
	pin_config.g_flags = GPIO_PIN_INPUT | GPIO_PIN_PULLUP;
	res = gpio_pin_set_flags(gpio3Hdl, &pin_config);
	if (res < 0)
    {
		err(EXIT_FAILURE, "configuration of FPCP pin %d on %s "
			    "failed (flags=%d)", pin_config.g_pin, gpio3Name,
			    pin_config.g_flags);
        return false;
    }

    pin_config.g_pin = _CPFP_PIN;
	pin_config.g_flags = GPIO_PIN_OUTPUT | GPIO_PIN_PULLUP;
	res = gpio_pin_set_flags(gpio3Hdl, &pin_config);
	if (res < 0)
    {
		err(EXIT_FAILURE, "configuration of CPFP pin %d on %s "
			    "failed (flags=%d)", pin_config.g_pin, gpio3Name,
			    pin_config.g_flags);
        return false;
    }

    printf("*** GPIO3 initialized ***\n");

    return true;
}

bool GPIO::cycleFPRT()
{
    if(gpio_pin_low(gpio1Hdl, _FPRT_PIN))
    {
		err(EXIT_FAILURE, "Cannot set GPIO1 FPRT pin low");
        return false;
    }
        
    usleep(10);
    
    if(gpio_pin_high(gpio1Hdl, _FPRT_PIN))
    {
		err(EXIT_FAILURE, "Cannot set GPIO1 FPRT pin high");
        return false;
    }

    return true;
}

bool GPIO::setCPFP(bool aVal)
{
    if(gpio_pin_set(gpio3Hdl, _CPFP_PIN, ((aVal == 0) ? GPIO_VALUE_LOW : GPIO_VALUE_HIGH)))
    {
		err(EXIT_FAILURE, "Cannot set GPIO3 _CPFP pin %d", aVal);
        return false;
    }

    return true;
}

bool GPIO::getFPCP(bool& aVal)
{
    int val = 0;
    if((val = gpio_pin_get(gpio3Hdl, _FPCP_PIN)) < 0)
    {
		err(EXIT_FAILURE, "Cannot get GPIO3 _FPCP pin %d", aVal);
        return false;
    }

    aVal = val;
    
    return true;
}
