#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include"queue.h"

/*
 * Message structure
 * Link element containing pointer to the data being sent.
 */
typedef struct _MESSAGE {
    char* data;
    int data_length;
    struct _MESSAGE *next;
} Message;

/*
 * Channel structure
 * Represents a single channel in our queue system.
 * Contains a pointer to the head and tail of a linked list.
 */
typedef struct _CHANNEL {
    Message* messages;
    Message* last_message;
    ChannelInfo info;
} Channel;

// Channel registry
Channel* channel_registry[MAX_CHANNELS];

// Set via initialization / freeing of registry
int registered = FALSE;

// Log messages for better readability and debugging
char* status_messages[11] = {
    "Registry Created",
    "Registry Exists",
    "Registry Doesn't Exist",
    "Registry Full", 
    "Registry Freed",
    "Channel is Null",
    "Channel Empty",
    "Channel Not Empty",
    "Channel Registered",
    "Channel Unregistered",
    "Info is Null"
};

Status init_registry() {

    if (registered)
        return REGISTRY_EXISTS;

    for (int i = 0; i < MAX_CHANNELS; i++) {
        channel_registry[i] = NULL;
    }

    registered = TRUE;
    return REGISTRY_CREATED;
}

Status free_registry() {

    if (!registered) 
        return REGISTRY_DNE;

    for (int i = 0; i < MAX_CHANNELS; i++) {
        unregister_channel(i);
    }

    registered = FALSE;
    return REGISTRY_FREED;
}

Status register_channel(ChannelInfo *info) {
    if (info == NULL)
        return INFO_NULL;

    if (!registered)
        return REGISTRY_DNE;

    Channel *channel = (Channel *) malloc(sizeof(Channel));
    if (channel == NULL)
        return CHANNEL_NULL;

    int id = -1;
    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (channel_registry[i] == NULL) {
            id = i;
            break;
        }
    }

    if (id == -1)
        return REGISTRY_FULL;

    channel->info.id = id;
    channel->info.message_count = 0;
    channel->messages = NULL;
    channel->last_message = NULL;
    channel_registry[id] = channel;
    return CHANNEL_REGISTERED;
}

Status unregister_channel(unsigned int channel_id) {
    if (!registered)
        return REGISTRY_DNE;

    Channel* channel = channel_registry[channel_id];
    if (channel == NULL)
        return CHANNEL_NULL;

    Message* message = channel->messages;
    for (int i = 0; i < channel->info.message_count; i++) {
        free(message->data);
        message = message->next;
    }

    free(channel);
    channel_registry[channel_id] = NULL;
    return CHANNEL_UNREGISTERED;
}

Status get_info(unsigned int channel_id, ChannelInfo* info) {
    if (info == NULL)
        return INFO_NULL;
    
    if (!registered)
        return REGISTRY_DNE;

    Channel* channel = channel_registry[channel_id];
    if (channel == NULL)
        return CHANNEL_NULL;

    info->id = channel_id;
    info->message_count = channel->info.message_count;

    return info->message_count ? CHANNEL_NOT_EMPTY : CHANNEL_EMPTY;
}

Status add_message(unsigned int channel_id, char* data) {
    if (!registered)
        return REGISTRY_DNE;

    Channel* channel = channel_registry[channel_id];
    if (channel == NULL)
        return CHANNEL_NULL;

    Message* message = (Message*)malloc(sizeof(Message));
    int length = strlen(data);
    message->data = (char*)malloc(sizeof(char)*length);
    strcpy(message->data, data);

    if (channel->last_message == NULL) {
        channel->messages = message;
        channel->last_message = message;
    } else {
        channel->last_message->next = message;
        channel->last_message = message;
    }

    channel->info.message_count++;
    return CHANNEL_NOT_EMPTY;
}

Status next_message(unsigned int channel_id, char **data) {
    if (!registered)
        return REGISTRY_DNE;

    Channel* channel = channel_registry[channel_id];
    if (channel == NULL)
        return CHANNEL_NULL;

    if (channel->messages == NULL)
        return CHANNEL_EMPTY;

    Message* message = channel->messages;
    channel->messages = message->next;
    channel->info.message_count--;

    *data = (char *)malloc(sizeof(char)*message->data_length);
    strcpy(*data, message->data);
    free(message->data);
    free(message);

    return CHANNEL_NOT_EMPTY;
}
