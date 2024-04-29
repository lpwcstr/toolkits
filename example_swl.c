#define SWL_IMPLEMENTATION
#include "swl.h"

int main(void)
{
    if(!swl_init()) {
        return -1;
    }

    swl_window_config window_config;
    window_config.title = "Hello, World";
    window_config.width = 800;
    window_config.height = 600;
    window_config.is_visible = SWL_TRUE;
    window_config.is_resizable = SWL_FALSE;
    swl_window *window = swl_create_window(&window_config);

    swl_bool running = SWL_TRUE;
    swl_event event;
    while(running) {
        swl_poll_window_events();
        while(swl_shift_event(&event)) {
            if(event.type == SWL_EVENT_WINDOW_CLOSED) {
                running = SWL_FALSE;
                break;
            }
        }
    }

    swl_destroy_window(window);
    swl_deinit();
    return 0;
}
