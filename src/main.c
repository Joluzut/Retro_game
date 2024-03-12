#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS 100

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
#define SWITCH0_NODE DT_ALIAS(button)
#define SWITCH1_NODE DT_ALIAS(buttonxup)
#define SWITCH2_NODE DT_ALIAS(buttonxdown)
#define SWITCH3_NODE DT_ALIAS(buttonyup)
#define SWITCH4_NODE DT_ALIAS(buttonydown)

#include "bullet.h"

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SWITCH0_NODE, gpios);
static const struct gpio_dt_spec buttonxup = GPIO_DT_SPEC_GET(SWITCH1_NODE, gpios);
static const struct gpio_dt_spec buttonxdown = GPIO_DT_SPEC_GET(SWITCH2_NODE, gpios);
static const struct gpio_dt_spec buttonyup = GPIO_DT_SPEC_GET(SWITCH3_NODE, gpios);
static const struct gpio_dt_spec buttonydown = GPIO_DT_SPEC_GET(SWITCH4_NODE, gpios);
static struct gpio_callback button_cb_data;

int Xtank1;
int Ytank1;
int Xtank2;
int Ytank2;
int power; // power
int angle; // max 10
int previusStateBigButton = 0;
int previusStatexup = 0;
int previusStatexdown = 0;
int previusStateyup = 0;
int previusStateydown = 0;

int state = 1;

static const struct gpio_dt_spec *leds[] = {
	&led0, &led1, &led2, &led3};

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

void configuer()
{
	Xtank1 = 5; // laater in te stellen naar begin positie tankt
	Ytank1 = 5;
	Xtank2 = 635; // 640 - 5 want sprite is 10 breed en je wil het midden punt
	Ytank2 = 5;
	gpio_pin_configure_dt(&led0, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led1, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led2, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led3, GPIO_OUTPUT);

	gpio_pin_configure_dt(&buttonxup, GPIO_INPUT);
	gpio_pin_configure_dt(&buttonxdown, GPIO_INPUT);
	gpio_pin_configure_dt(&buttonyup, GPIO_INPUT);
	gpio_pin_configure_dt(&buttonydown, GPIO_INPUT);
}

int readXButtons()
{
	int standxup = gpio_pin_get_dt(&buttonxup);
	int standxdown = gpio_pin_get_dt(&buttonxdown);

	if (standxup == 1 && previusStatexup == 0)
	{
		printk("X up \n");
		gpio_pin_toggle_dt(leds[0]);
		previusStatexup = standxup;
		previusStatexdown = standxdown;
		return 1;
	}
	else if (standxdown == 1 && previusStatexdown == 0)
	{
		printk("X down \n");
		gpio_pin_toggle_dt(leds[1]);
		previusStatexup = standxup;
		previusStatexdown = standxdown;
		return -1;
	}
	else
	{
		previusStatexup = standxup;
		previusStatexdown = standxdown;
		printk("X no input \n");
		return 0;
	}
	
	
}
int ReadYButtons()
{
	int standyup = gpio_pin_get_dt(&buttonyup);
	int standydown = gpio_pin_get_dt(&buttonydown);

	if (standyup == 1 && previusStateyup == 0)
	{
		printk("Y up \n");
		gpio_pin_toggle_dt(leds[2]);
		previusStateyup = standyup;
		previusStateydown = standydown;
		return 1;
	}
	else if (standydown == 1 && previusStateydown == 0)
	{
		printk("Y down \n");
		gpio_pin_toggle_dt(leds[3]);
		previusStateyup = standyup;
		previusStateydown = standydown;
		return -1;
	}
	else
	{
		previusStateyup = standyup;
		previusStateydown = standydown;
		printk("Y no imput");
		return 0;
	}
	
	
}

bool readBigButton()
{
	int val = gpio_pin_get_dt(&button);

	if (val == 1 && previusStateBigButton == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			gpio_pin_toggle_dt(leds[i]);
		} 
		printk("Toggled LED \n");
		previusStateBigButton = val;
		return true;
	}
	else
	{
		previusStateBigButton = val;
		return false;
	}

	
}

void readButtons()
{
	switch (state)
	{
	case 1: // van menu naar X gebeuren
		bool isknopingedrukt = readBigButton();
		if (isknopingedrukt)
		{
			state = 2;
		}
		else
		{
			state = 1;
		}
		break;
	case 2:						// van X naar Y bepalen
		int x = readXButtons(); // om tank te bewegen
		Xtank1 = Xtank1 + x;
		isknopingedrukt = readBigButton();
		if (isknopingedrukt == true)
		{
			state = 3;
		}
		else
		{
			state = 2;
		}
		break;
	case 3:
		int y = ReadYButtons();
		angle = angle + y;
		if (angle > 10)
		{
			angle = 10;
		}
		if (angle < 0)
		{
			angle = 0;
		}
		x = readXButtons();
		power = power + x;
		/* hier moet nog code komen om  power van schot te controleren.*/
		isknopingedrukt = readBigButton();
		if (isknopingedrukt == true)
		{
			state = 4;
		}
		else
		{
			state = 3;
		}
		break;
	case 4:
		/*code voor schieten:
		set(power, angle);
		berekenX(Xtank1);
		berekenY(Ytank1);
		if(geen collision){
			state = 4;
		*/
		state = 5;
		break;
	case 5:
		x = readXButtons();
		Xtank2 = Xtank2 + x;
		// stuur tank locatie via spi
		isknopingedrukt = readBigButton();
		if (isknopingedrukt == true)
		{
			state = 6;
		}
		else
		{
			state = 5;
		}
		break;
	case 6:
		y = ReadYButtons();
		angle = angle + y;
		if (angle > 10)
		{
			angle = 10;
		}
		if (angle < 0)
		{
			angle = 0;
		}
		x = readXButtons();
		power = power + x;
		/* hier moet nog code komen om  power van schot te controleren.*/
		isknopingedrukt = readBigButton();
		if (isknopingedrukt == true)
		{
			state = 7;
		}
		else
		{
			state = 6;
		}
		break;
	case 7:
		/*code voor schieten:
		set(power, angle);
		berekenX(Xtank1);
		berekenY(Ytank1);
		if(geen collision){
			state = 4;
		*/
		state = 2;
		break;
	case 8:
		int test1 = readXButtons();
		int test2 = ReadYButtons();
		bool test3 = readBigButton();
		break;
	default:
		state = 1;
		break;
	}
}

int main(void)
{
	configuer();
	if (!gpio_is_ready_dt(&button))
	{
		printk("Error: button device %s is not ready\n",
			   button.port->name);
		return 0;
	}

	int ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0)
	{
		printk("Error %d: failed to configure %s pin %d\n",
			   ret, button.port->name, button.pin);
		return 0;
	}

	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	// int x = 0;
	// int y = 0;
	while (1)
	{
		state = 8;
		readButtons();
	}
	return 0;
}