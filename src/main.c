#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>


#define SLEEP_TIME_MS   100

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
#define SWITCH0_NODE DT_ALIAS(button)
#define SWITCH1_NODE DT_ALIAS(buttonxup)
#define SWITCH2_NODE DT_ALIAS(buttonxdown)
#define SWITCH3_NODE DT_ALIAS(buttonyup)
#define SWITCH4_NODE DT_ALIAS(buttonydown)


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


static const struct gpio_dt_spec* leds[] = {
	&led0, &led1, &led2, &led3
};


void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n",
		       button.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led1, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led2, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led3, GPIO_OUTPUT);
	

	gpio_pin_configure_dt(&buttonxup, GPIO_INPUT);
	gpio_pin_configure_dt(&buttonxdown, GPIO_INPUT);
	gpio_pin_configure_dt(&buttonyup, GPIO_INPUT);
	gpio_pin_configure_dt(&buttonydown, GPIO_INPUT);

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button.port->name, button.pin);
		return 0;
	}

	
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);
	int previusState = 0;
	int previusStatexup = 0;	
	int previusStatexdown = 0;
	int previusStateyup = 0;
	int previusStateydown = 0;
	int x = 0;
	int y = 0;
	while (1) {
		
		int val = gpio_pin_get_dt(&button);
		int standxup = gpio_pin_get_dt(&buttonxup);
		int standxdown = gpio_pin_get_dt(&buttonxdown);
		int standyup = gpio_pin_get_dt(&buttonyup);
		int standydown = gpio_pin_get_dt(&buttonydown);
		//printk("Button value: %d\n", val);
		
		if (val == 1 && previusState == 0){
			for(int i = 0; i < 4; i++){
				gpio_pin_toggle_dt( leds[i]);
			}// eigenlijk x of y + of -1
			printk("Toggled LED \n");
		}
		
		if(standxup == 1 && previusStatexup == 0){
			x++;
			printk("X up \n");
			gpio_pin_toggle_dt( leds[0]);
		}

		if(standxdown == 1 && previusStatexdown == 0){
			x--;
			printk("X down \n");
			gpio_pin_toggle_dt( leds[1]);
		}

		if(standyup == 1 && previusStateyup == 0){
			y++;
			printk("Y up \n");
			gpio_pin_toggle_dt( leds[2]);
		}

		if(standydown == 1 && previusStateydown == 0){
			y--;
			printk("Y down \n");
			gpio_pin_toggle_dt( leds[3]);
		}
		previusState = val;
		previusStatexup = 	standxup;
		previusStatexdown = standxdown;
		previusStateyup = 	standyup;
		previusStateydown = standydown;

		k_msleep(SLEEP_TIME_MS);			
	}	
	return 0;
}
