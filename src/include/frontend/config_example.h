#pragma once
#include <frontend/node.h>
#include <common/config.h>
#define BOT_TOKEN "[your token here]"
#define NUM_NODES 3
static node_t nodes[NUM_NODES] __attribute__((unused)) = {
	{
		.ip = "192.168.1.20",
		.port = DEFAULT_PORT,
		.description = "Techflash's Wii",
		.connection = "local-wifi",
	},
	{
		.ip = "10.2.30.5",
		.port = DEFAULT_PORT,
		.description = "Selim's Wii",
		.connection = "vpn-wifi",
	},
	{
		.ip = "10.2.30.6",
		.port = DEFAULT_PORT,
		.description = "Tech64's Wii",
		.connection = "vpn-ethernet",
	},
};
