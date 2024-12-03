#include <concord/discord_codecs.h>
#include <concord/types.h>
#include <string.h>
#include <concord/discord.h>
#include <concord/log.h>
#include <concord/error.h>
#include <stdlib.h>
#include <string.h>
#include <util.h>
#include <config.h>

u64snowflake ourID = 0;


void on_ready(struct discord *client, const struct discord_ready *event) {
	(void)client;
	log_info("Logged in as %s!", event->user->username);
	ourID = event->user->id;
}

void on_message(struct discord *client, const struct discord_message *event) {
	// don't recursively operate on our own messages
	if (event->author->id == ourID) {
		return;
	}

	bool isReply = event->referenced_message != NULL/* && event->referenced_message->type == DISCORD_MESSAGE_REPLY)*/;
	bool mentionsMe = false;
	struct discord_message_reference ref = {
		.message_id = event->id,
		.channel_id = event->channel_id,
		.guild_id = event->guild_id
	};

	for (int i = 0; i < event->mentions->size; i++) {
		// if it was ever true, keep it true
		mentionsMe = event->mentions->array[i].id == ourID || mentionsMe;
	}

	if (mentionsMe && !isReply) {
		struct discord_create_message params = { .content = "You rang?", .message_reference = &ref };
		discord_create_message(client, event->channel_id, &params, NULL);
	}

	if (isReply) {
		if (event->referenced_message->author->id == ourID) {
			struct discord_create_message params = { .content = "Hey this is a reply to me!", .message_reference = &ref };
			discord_create_message(client, event->channel_id, &params, NULL);
		}
	}



	// struct discord_create_message params = { .content = str };
	// discord_create_message(client, event->channel_id, &params, NULL);
}

int main(void) {
	struct discord *client = discord_init(BOT_TOKEN);

	discord_add_intents(client, DISCORD_GATEWAY_MESSAGE_CONTENT);
	discord_set_on_ready(client, &on_ready);
	discord_set_on_message_create(client, &on_message);

	CCORDcode ret = discord_run(client);
	if (ret != CCORD_OK) {
		printf("discord_run returned error: %s (%d)\n", ccordcode_to_str(ret), ret);
		return 1;
	}

	return 0;
}
