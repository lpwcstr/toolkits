#define SWL_IMPLEMENTATION
#include "swl.h"

#define GLC_IMPLEMENTATION
#include "glc.h"

int main(void)
{
    if(!swl_init()) {
        return -1;
    }

    GLCBackendConfig glc_config;
    glc_config.x11.display = swl_x11_get_display();
    GLCBackend *glc = glcCreateBackend(&glc_config);

    swl_window_config window_config;
    window_config.title = "Hello, World";
    window_config.width = 800;
    window_config.height = 600;
    window_config.is_visible = 1;
    window_config.is_resizable = 0;
    swl_window *window = swl_create_window(&window_config);

    GLCContextConfig context_config;
    context_config.x11.window = swl_x11_get_window(window);
    context_config.version.major = 3;
    context_config.version.minor = 3;
    context_config.is_core_profile = 1;
    GLCContext *context = glcCreateContext(glc, &context_config);

    glcMakeContextCurrent(glc, context);

    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GLSL_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

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

        glClearColor(1.0f, 0.2f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);        
        glcSwapBuffer(glc, context);
    }


    glcDestroyContext(glc, context);
    swl_destroy_window(window);

    glcDestroyBackend(glc);
    swl_deinit();
}
