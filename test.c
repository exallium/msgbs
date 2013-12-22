#include <stdlib.h>
#include <stdio.h>
#include "queue.h"


int main(int argc, char** argv) {

    // Initialize Channel Registry
    init_registry();

    // Register a channel and add some messages
    ChannelInfo info;
    register_channel(&info);
    add_message(info.id, "m1");
    add_message(info.id, "m2");
    add_message(info.id, "m3");
    add_message(info.id, "m4");
    
    // Print out all messages added
    char* data[1];
    while(next_message(info.id, data) == CHANNEL_NOT_EMPTY) {
        printf("%s\n", *data);
        free(*data);
    }

    // Unregister our channels
    unregister_channel(info.id);
    unregister_channel(3);

    // Free channel registry
    free_registry();

    return EXIT_SUCCESS;
}
