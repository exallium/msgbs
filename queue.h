/*
 * Queue.h -- FIFO Channel Based Queue System
 */

#define MAX_CHANNELS 8  // Max number of channels available
#define TRUE 1
#define FALSE 0

/*
 * ChannelInfo Structure
 * Represents a snapshot of a given channel at the time requested.
 *
 * These are meant to be disposable, in that the info object seen
 * by the application is not the same as that stored in the internal
 * channel object.
 */
typedef struct _INFO {
    int message_count;
    unsigned int id;
} ChannelInfo;

/*
 * Queue Status
 * Each function contained in this library will return one of these
 * statuses.  These then correspond to status_messages, externalized
 * and declared below, but implemented in queue.c
 */
typedef enum {
    REGISTRY_CREATED,       // Registry was successfully created
    REGISTRY_EXISTS,        // Registry already exists
    REGISTRY_DNE,           // Registry doesn't exist
    REGISTRY_FULL,          // MAX_CHANNELS channels already exist
    REGISTRY_FREED,         // Registry was successfully cleaned up
    CHANNEL_NULL,           // The channel requested is null
    CHANNEL_EMPTY,          // The channel requested is empty
    CHANNEL_NOT_EMPTY,      // The channel requested is not empty
    CHANNEL_REGISTERED,     // The channel was registered
    CHANNEL_UNREGISTERED,   // The channel was unregistered
    INFO_NULL               // The given info structure was null
} Status;

// String based status messages
extern char* status_messages[];

/*
 * Initialize Registry
 * This function will only work if registry doesn't exist.
 * Sets all channels in the registry to NULL for safety.
 * @returns REGISTRY_CREATED | REGISTRY_EXISTS
 */
Status init_registry();

/*
 * Free Registry
 * This function will only work if the registry exists.
 * Nullifies all channels, freeing all allocated data as we go.
 * @returns REGISTRY_FREED | REGISTRY_DNE
 */
Status free_registry();

/*
 * Register a channel.
 * Creates and registers a channel at an id determined at function
 * runtime.
 * @param info A non-null pointer to a ChannelInfo structure
 * @returns 
 *      INFO_NULL if the parameter is NULL
 *      REGISTRY_DNE if the registry was not initialized
 *      REGISTRY_FULL if every slot in registry is taken
 *      CHANNEL_REGISTERED if the channel registry succeeds
 */
Status register_channel(ChannelInfo *info);

/*
 * Unregister a channel
 * Unregisters and deallocates a channel and it's associated
 * allocated memory.
 * @param channel_id the ID of the channel to unregister.
 * @returns
 *      REGISTRY_DNE if the registry was not initialized
 *      CHANNEL_NULL if the requested channel is unregistered
 *      CHANNEL_UNREGISTERED if unregistering was successful
 */
Status unregister_channel(unsigned int channel_id);

/*
 * Get Channel Info
 * Gets a snapshot of the current info for a given channel.
 * @param channel_id The id of the requested channel
 * @param info The output info object, non null.
 * @returns
 *      INFO_NULL if the given info pointer is null
 *      REGISTRY_DNE if the registry is non-initialized
 *      CHANNEL_NULL if the requested channel is unregistered
 *      CHANNEL_NOT_EMPTY if the channel's message count is non-zero
 *      CHANNEL_EMPTY if the channel's message count is zero
 */
Status get_info(unsigned int channel_id, ChannelInfo* info);

/*
 * Add Message to Channel Queue
 * Pushes a message containing data onto the end of the given
 * channel's queue.
 * @param channel_id The id of the requested channel
 * @param *data A pointer to the data to send
 * @returns
 *      REGISTRY_DNE if the registry is non-initialized
 *      CHANNEL_NULL if the requested channel is unregistered
 *      CHANNEL_NOT_EMPTY if the add was successful.
 */
Status add_message(unsigned int channel_id, char *data);

/*
 * Get the next message available
 * Grabs the next message on the queue in FIFO fashion
 * @param channel_id The id of the requested channel
 * @param **data A wrapper in which to contain the resultant data
 * @returns
 *      REGISTRY_DNE if the registry is non-initialized
 *      CHANNEL_NULL if the requested channel is unregistered
 *      CHANNEL_EMPTY if there is no data available
 *      CHANNEL_NOT_EMPTY if there is more data available
 */
Status next_message(unsigned int channel_id, char **data);
