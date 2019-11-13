/* Download it from http://j.mp/GQ2yuD */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <asm/delay.h>
#include <linux/delay.h>

static char buff[6];
static char buffer[2];

static void nunchuk_read_registers(struct i2c_client *client)
{
	mdelay(10);
	buffer[0] = 0x00;
	if(i2c_master_send(client, buffer, 1) >=0)
	{
		pr_info("Okay\n");
	}
	else
	{
		pr_info("Not okay\n");
	};
	mdelay(10);
	if(i2c_master_recv(client, buff, 6) >=0)
	{
		pr_info("Okay\n");
	}
	else
	{
		pr_info("Not okay\n");
	};

	pr_info("Called nunchuk_read_registers\n");
}

static int nunchuk_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	pr_info("Called nunchuk_probe\n");
	buffer[0] = 0xf0;
	buffer[1] = 0x55;

	if(i2c_master_send(client, buffer, 2) >=0)
	{
		pr_info("Okay\n");
	}
	else
	{
		pr_info("Not okay\n");
	};

	udelay(1000);

	buffer[0] = 0xfb;
	buffer[1] = 0x0;
	if(i2c_master_send(client, buffer, 2) >=0)
	{
		pr_info("Okay\n");
	}
	else
	{
		pr_info("Not okay\n");
	};

	nunchuk_read_registers(client);
	nunchuk_read_registers(client);

	if((buff[5] & 0b00000001)==0)
	{
		pr_info("Z pressed\n");
	}
	else
	{
		pr_info("Z not pressed\n");
	};

	if((buff[5] & 0b00000010)==0)
	{
		pr_info("C pressed\n");
	}
	else
	{
		pr_info("C not pressed\n");
	};

	return 0;
}

static int nunchuk_remove(struct i2c_client *client)
{
	pr_info("Called nunchuk_remove\n");
	return 0;
}

static const struct i2c_device_id nunchuk_id[] = {
	{ "nunchuk", 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, nunchuk_id);

#ifdef CONFIG_OF
static struct of_device_id nunchuk_dt_match[] = {
	{ .compatible = "nintendo,nunchuk" },
	{ },
};
#endif

static struct i2c_driver nunchuk_driver = {
	.driver = {
		.name = "nunchuk",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(nunchuk_dt_match),
	},
	.probe = nunchuk_probe,
	.remove = nunchuk_remove,
	.id_table = nunchuk_id,
};

module_i2c_driver(nunchuk_driver);

MODULE_LICENSE("GPL");
