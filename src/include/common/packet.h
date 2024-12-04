#include <stdint.h>

#define PACKET_MAGIC (uint8_t[]){ 0x7F, 'P', 'K', 'T' }
typedef enum packetType {
	// corrupted somehow
	PACKET_TYPE_INVALID = 0,

	// normal packets
	/* execute a command on the node
	 *
	 * ALLOWED ORIGINS:
	 * can only be sent from central server to node
	 *
	 * DATA TYPES:
	 * data will be a null-terminated string of the command to execute
	 */
	PACKET_TYPE_CMD_EXEC_REQUEST,

	/* confirmtation that a command is being executed
	 * as well as the command's ID
	 *
	 * ALLOWED ORIGINS:
	 * can only be sent from node to central server.
	 *
	 * DATA TYPES:
	 * data's first byte is an unsigned integer of the command ID that is being executed
	 */
	PACKET_TYPE_CMD_EXEC_CONFIRMATION,

	/* response data of command execution
	 *
	 * ALLOWED ORIGINS:
	 * can only be sent from node to central server.
	 * Does not need to be in reponse to anything, as the command can continue executing after a CMD_EXEC.
	 *
	 * DATA TYPES:
	 * data's first byte is an unsigned integer of the command ID that is being executed
	 * data's next 4 bytes are an unsigned integer of the length of the output
	 * The rest of the data is the output from the command, as unsigned chars (can contain binary data)
	 */
	PACKET_TYPE_CMD_RESPONSE_DATA,

	/* command execution is done
	 *
	 * ALLOWED ORIGINS:
	 * can only be sent from node to central server.
	 *
	 * DATA TYPES:
	 * data's first byte is an unsigned integer of the command ID that is being executed
	 * data's second byte is an unsigned integer of the exit code of the command
	 */
	PACKET_TYPE_CMD_DONE,
	// more here

	// special packets

	/* get the system info of a node
	 *
	 * ALLOWED ORIGINS:
	 * can only be sent from central server to node
	 *
	 * DATA TYPES:
	 * none
	 */
	PACKET_TYPE_SYSTEM_INFO_REQUEST = 0xF9,

	/* send the system info of a node
	 *
	 * ALLOWED ORIGINS:
	 * can only be sent from node to central server in response to a SYSTEM_INFO_REQUEST
	 *
	 * DATA TYPES:
	 * data will be a systemInfo_t
	 */
	PACKET_TYPE_SYSTEM_INFO_RESPONSE = 0xFA,

	/* get server version
	 *
	 * ALLOWED ORIGINS:
	 * can be sent from central server to node to get the node version
	 * can also be sent from node to central server to get the central server version
	 *
	 * DATA TYPES:
	 * none
	 */
	PACKET_TYPE_VERSION_REQUEST = 0xFB,

	/* send server version
	 *
	 * ALLOWED ORIGINS:
	 * can be sent from node to central server in response to a VERSION_REQUEST
	 * can also be sent from central server to node in response to a VERSION_REQUEST
	 *
	 * DATA TYPES:
	 * data will be a version_t
	 */
	PACKET_TYPE_VERSION_RESPONSE = 0xFC,

	/*
	 * Keepalive packets
	 *
	 * ALLOWED ORIGINS:
	 * PING can be sent from either side
	 * PONG can be sent from either side in response to a PING
	 *
	 * DATA TYPES:
	 * none
	 */
	PACKET_TYPE_KEEPALIVE_PING = 0xFD,
	PACKET_TYPE_KEEPALIVE_PONG = 0xFE,

	// to be returned when we don't understand the packet
	PACKET_TYPE_ERROR = 0xFF,
}  packetType_t;

typedef struct packet {
	uint8_t magic[4];
	packetType_t type;

	// 1024 - 8 (magic + type)
	uint8_t data[1016];
} packet_t;
