#include <GL/gl.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>

#define GLC_IMPLEMENTATION
#include "glc.h"

int main(void)
{
    Display *display = XOpenDisplay(NULL);

    GLCBackendConfig backend_config;
    backend_config.x11.display = display;
    GLCBackend *backend = glcCreateBackend(&backend_config);
    if(!backend) {
        printf("Failed to create backend\n");
        return -1;
    }

    Colormap colormap = XCreateColormap(display, XDefaultRootWindow(display), 
            XDefaultVisualOfScreen(XDefaultScreenOfDisplay(display)), AllocNone);

    XSetWindowAttributes swa;
    swa.override_redirect = True;
    swa.border_pixel = None;
    swa.background_pixel = XBlackPixel(display, XDefaultScreen(display));
    swa.colormap = colormap;
    swa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                    PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                    ExposureMask | FocusChangeMask | VisibilityChangeMask |
                    EnterWindowMask | LeaveWindowMask | PropertyChangeMask;

    Window window = XCreateWindow(
            display,
            XDefaultRootWindow(display),
            0, 0, 800, 600, 0,
            CopyFromParent, InputOutput, CopyFromParent,
            CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
            &swa);

    XMapWindow(display, window);
    XSync(display, False);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    GLCContextConfig context_config;
    context_config.is_core_profile = 1;
    context_config.version.major = 3;
    context_config.version.minor = 3;
    context_config.x11.window = window;
    GLCContext *context = glcCreateContext(backend, &context_config);
    glcMakeContextCurrent(backend, context);

    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GLSL_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    XEvent event;
    Bool running = True;
    while(running) {
        while(XPending(display)) {
            XNextEvent(display, &event);
            switch(event.type) {
                case ClientMessage:
                    {
                        if((Atom)event.xclient.data.l[0] == wm_delete_window) {
                            running = False;
                            break;
                        }
                    } break;
                default:
                    break;
            }
        }

        glClearColor(1.0f, 0.2f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glcSwapBuffer(backend, context);
    }

    glcDestroyContext(backend, context);
    XDestroyWindow(display, window);

    glcDestroyBackend(backend);
    XFreeColormap(display, colormap);
    XCloseDisplay(display);
}
