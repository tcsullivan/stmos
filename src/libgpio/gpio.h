#ifndef STMOS_GPIO_H_ 
#define STMOS_GPIO_H_

#define GP(a) ((a - 'A') << 4)

/**
 * Defines possible modes for a gpio pin.
 */
enum GPIO_MODE
{
	INPUT = 0, /**< digital input */
	OUTPUT,    /**< digital output */
	ALTERNATE, /**< alternate function */
	ANALOG     /**< analog function */
};

/**
 * Defines whether to use push-pull or open drain.
 */
enum GPIO_TYPE
{
	PUSHPULL = 0, /**< push-pull */
	OPENDRAIN     /**< open drain */
};

/**
 * Defines the pin's speed
 */
enum GPIO_SPEED
{
	LOW = 0, /**< low */
	MEDIUM,  /**< medium */
	HIGH,    /**< high */
	VERYHIGH /**< very high/maximum */
};

/**
 * Defines if a pullup or pulldown should be used.
 */
enum GPIO_PUPD
{
	NOPUPD,  /**< no pullup/pulldown */
	PULLUP,  /**< use pullup */
	PULLDOWN /**< use pulldown */
};


typedef unsigned int gpio_pin_t;

void gpioMode(gpio_pin_t pin, int mode);
void gpioType(gpio_pin_t pin, int type);
void gpioPuPd(gpio_pin_t pin, int pupd);
void gpioSpeed(gpio_pin_t pin, int speed);
void gpioWrite(gpio_pin_t pin, int value);
int gpioRead(gpio_pin_t pin);

#endif // STMOS_GPIO_H_
