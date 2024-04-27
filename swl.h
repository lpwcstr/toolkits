#ifndef SWL_H_
#define SWL_H_

#ifndef SWL_LOG_MESSAGE
#include <stdio.h>
#define SWL_LOG_MESSAGE(message) printf("%s\n", (message));
#endif

#ifndef SWL_MAXIMUM_WINDOW_CREATED
#define SWL_MAXIMUM_WINDOW_CREATED 8
#endif

#ifndef SWL_EVENT_QUEUE_CAPACITY
#define SWL_EVENT_QUEUE_CAPACITY 256
#endif

#ifdef _WIN32
#define SWL_DISPLAY_WIN32
#endif

#ifdef __linux__
#if !defined(SWL_DISPLAY_X11) || !defined(SWL_DISPLAY_WAYLAND)
#define SWL_DISPLAY_X11
#endif
#endif

typedef int swl_bool;
#define SWL_TRUE  1
#define SWL_FALSE 0
#define SWL_NULL ((void *)0)

typedef unsigned int swl_uint32;

typedef struct swl_window swl_window;

#ifdef SWL_DISPLAY_X11
typedef struct _XDisplay Display;
typedef unsigned long XID;
typedef XID Window;

Display *swl_x11_get_display(void);
Window swl_x11_get_window(swl_window *window);
#endif

typedef struct swl_event {
    int type;
    union {
        struct {
            swl_window *window;
            int x, y;
            unsigned int width, height;
        } window;
        struct {
            int keycode;
            int mods;
            int scancode;
        } key;
    };
} swl_event;

typedef struct swl_window_config {
    const char *title;
    swl_uint32 width, height;
    swl_bool is_visible, is_resizable;
} swl_window_config;

swl_bool swl_init(void);
void swl_deinit(void);
swl_bool swl_push_event(swl_event event);
swl_bool swl_shift_event(swl_event *event);

swl_window *swl_create_window(const swl_window_config *config);
void swl_destroy_window(swl_window *window);

void swl_set_window_title(swl_window *window, const char *title);
void swl_set_window_visible(swl_window *window, swl_bool is_visible);
void swl_set_window_resizable(swl_window *window, swl_bool is_resizable);
void swl_poll_window_events(void);

typedef enum {
    SWL_EVENT_UNKNOWN = 0,
    SWL_EVENT_WINDOW_CLOSED,
    SWL_EVENT_KEY_PRESSED,
    SWL_EVENT_KEY_RELEASED,
} rn_event_type;

enum swl_keycode {
    SWL_KEY_INVALID            = -1,

    /* Printable keys */
    SWL_KEY_SPACE              = 32,
    SWL_KEY_APOSTROPHE         = 39,  /* ' */
    SWL_KEY_COMMA              = 44,  /* , */
    SWL_KEY_MINUS              = 45,  /* - */
    SWL_KEY_PERIOD             = 46,  /* . */
    SWL_KEY_SLASH              = 47,  /* / */
    SWL_KEY_0                  = 48,
    SWL_KEY_1                  = 49,
    SWL_KEY_2                  = 50,
    SWL_KEY_3                  = 51,
    SWL_KEY_4                  = 52,
    SWL_KEY_5                  = 53,
    SWL_KEY_6                  = 54,
    SWL_KEY_7                  = 55,
    SWL_KEY_8                  = 56,
    SWL_KEY_9                  = 57,
    SWL_KEY_SEMICOLON          = 59,  /* ; */
    SWL_KEY_EQUAL              = 61,  /* = */
    SWL_KEY_A                  = 65,
    SWL_KEY_B                  = 66,
    SWL_KEY_C                  = 67,
    SWL_KEY_D                  = 68,
    SWL_KEY_E                  = 69,
    SWL_KEY_F                  = 70,
    SWL_KEY_G                  = 71,
    SWL_KEY_H                  = 72,
    SWL_KEY_I                  = 73,
    SWL_KEY_J                  = 74,
    SWL_KEY_K                  = 75,
    SWL_KEY_L                  = 76,
    SWL_KEY_M                  = 77,
    SWL_KEY_N                  = 78,
    SWL_KEY_O                  = 79,
    SWL_KEY_P                  = 80,
    SWL_KEY_Q                  = 81,
    SWL_KEY_R                  = 82,
    SWL_KEY_S                  = 83,
    SWL_KEY_T                  = 84,
    SWL_KEY_U                  = 85,
    SWL_KEY_V                  = 86,
    SWL_KEY_W                  = 87,
    SWL_KEY_X                  = 88,
    SWL_KEY_Y                  = 89,
    SWL_KEY_Z                  = 90,
    SWL_KEY_LEFT_BRACKET       = 91,  /* [ */
    SWL_KEY_BACKSLASH          = 92,  /* \ */
    SWL_KEY_RIGHT_BRACKET      = 93,  /* ] */
    SWL_KEY_GRAVE_ACCENT       = 96,  /* ` */
    SWL_KEY_WORLD_1            = 161, /* non-US #1 */
    SWL_KEY_WORLD_2            = 162, /* non-US #2 */

    /* Function keys */
    SWL_KEY_ESCAPE             = 256,
    SWL_KEY_ENTER              = 257,
    SWL_KEY_TAB                = 258,
    SWL_KEY_BACKSPACE          = 259,
    SWL_KEY_INSERT             = 260,
    SWL_KEY_DELETE             = 261,
    SWL_KEY_RIGHT              = 262,
    SWL_KEY_LEFT               = 263,
    SWL_KEY_DOWN               = 264,
    SWL_KEY_UP                 = 265,
    SWL_KEY_PAGE_UP            = 266,
    SWL_KEY_PAGE_DOWN          = 267,
    SWL_KEY_HOME               = 268,
    SWL_KEY_END                = 269,
    SWL_KEY_CAPS_LOCK          = 280,
    SWL_KEY_SCROLL_LOCK        = 281,
    SWL_KEY_NUM_LOCK           = 282,
    SWL_KEY_PRINT_SCREEN       = 283,
    SWL_KEY_PAUSE              = 284,
    SWL_KEY_F1                 = 290,
    SWL_KEY_F2                 = 291,
    SWL_KEY_F3                 = 292,
    SWL_KEY_F4                 = 293,
    SWL_KEY_F5                 = 294,
    SWL_KEY_F6                 = 295,
    SWL_KEY_F7                 = 296,
    SWL_KEY_F8                 = 297,
    SWL_KEY_F9                 = 298,
    SWL_KEY_F10                = 299,
    SWL_KEY_F11                = 300,
    SWL_KEY_F12                = 301,
    SWL_KEY_F13                = 302,
    SWL_KEY_F14                = 303,
    SWL_KEY_F15                = 304,
    SWL_KEY_F16                = 305,
    SWL_KEY_F17                = 306,
    SWL_KEY_F18                = 307,
    SWL_KEY_F19                = 308,
    SWL_KEY_F20                = 309,
    SWL_KEY_F21                = 310,
    SWL_KEY_F22                = 311,
    SWL_KEY_F23                = 312,
    SWL_KEY_F24                = 313,
    SWL_KEY_F25                = 314,
    SWL_KEY_KP_0               = 320,
    SWL_KEY_KP_1               = 321,
    SWL_KEY_KP_2               = 322,
    SWL_KEY_KP_3               = 323,
    SWL_KEY_KP_4               = 324,
    SWL_KEY_KP_5               = 325,
    SWL_KEY_KP_6               = 326,
    SWL_KEY_KP_7               = 327,
    SWL_KEY_KP_8               = 328,
    SWL_KEY_KP_9               = 329,
    SWL_KEY_KP_DECIMAL         = 330,
    SWL_KEY_KP_DIVIDE          = 331,
    SWL_KEY_KP_MULTIPLY        = 332,
    SWL_KEY_KP_SUBTRACT        = 333,
    SWL_KEY_KP_ADD             = 334,
    SWL_KEY_KP_ENTER           = 335,
    SWL_KEY_KP_EQUAL           = 336,
    SWL_KEY_LEFT_SHIFT         = 340,
    SWL_KEY_LEFT_CONTROL       = 341,
    SWL_KEY_LEFT_ALT           = 342,
    SWL_KEY_LEFT_SUPER         = 343,
    SWL_KEY_RIGHT_SHIFT        = 344,
    SWL_KEY_RIGHT_CONTROL      = 345,
    SWL_KEY_RIGHT_ALT          = 346,
    SWL_KEY_RIGHT_SUPER        = 347,
    SWL_KEY_MENU               = 348,
    SWL_KEY_LAST = SWL_KEY_MENU,
};

#endif // SWL_H_

#ifdef SWL_IMPLEMENTATION

#ifdef SWL_DISPLAY_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
typedef struct swl_window__platform{
    Window window;
} swl_window__platform;

typedef struct swl_application__platform {
    Display *display;
    Colormap colormap;
    Atom wm_delete_window;

    int keycodes[256];
    int scancodes[SWL_KEY_LAST + 1];
} swl_application__platform;
#endif

typedef struct swl_window {
    const char *title;
    swl_uint32 width, height;
    swl_bool is_visible, is_resizable;

    swl_window__platform platform;
    swl_bool initialized;
} swl_window;

typedef struct swl_application {
    swl_bool initialized;
    swl_window windows[SWL_MAXIMUM_WINDOW_CREATED];

    struct {
        swl_event items[SWL_EVENT_QUEUE_CAPACITY];
        swl_uint32 head, tail;
        swl_bool carry;
    } event_queue;

    swl_application__platform platform;
} swl_application;

static swl_application APP = {0};

#ifdef SWL_DISPLAY_X11
static swl_bool swl_init__platform(void)
{
    APP.platform.display = XOpenDisplay(NULL);
    if(!APP.platform.display) {
        SWL_LOG_MESSAGE("X11 display is failed to be opened");
    }

    APP.platform.colormap = XCreateColormap(APP.platform.display, XDefaultRootWindow(APP.platform.display), 
            XDefaultVisualOfScreen(XDefaultScreenOfDisplay(APP.platform.display)), AllocNone);

    APP.platform.wm_delete_window = XInternAtom(APP.platform.display, "WM_DELETE_WINDOW", False);

    return SWL_TRUE;
}

static void swl_deinit__platform(void)
{
    XFreeColormap(APP.platform.display, APP.platform.colormap);
    XCloseDisplay(APP.platform.display);
}

static swl_bool swl_create_window__platform(swl_window *window, const swl_window_config *config)
{
    XSetWindowAttributes swa;
    swa.override_redirect = True;
    swa.border_pixel = None;
    swa.background_pixel = XBlackPixel(APP.platform.display, XDefaultScreen(APP.platform.display));
    swa.colormap = APP.platform.colormap;
    swa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                    PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                    ExposureMask | FocusChangeMask | VisibilityChangeMask |
                    EnterWindowMask | LeaveWindowMask | PropertyChangeMask;

    Window parent = XDefaultRootWindow(APP.platform.display);
    Window handle = XCreateWindow(APP.platform.display, parent,
            0, 0, config->width, config->height, 0,
            CopyFromParent, InputOutput,
            CopyFromParent, 
            CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
            &swa);

    if(!window) {
        SWL_LOG_MESSAGE("Failed to create X11 window");
        return SWL_FALSE;
    }

    XSetWMProtocols(APP.platform.display, handle, &APP.platform.wm_delete_window, 1);

    window->platform.window = handle;
    window->initialized = SWL_TRUE;

    swl_set_window_title(window, config->title);
    swl_set_window_visible(window, config->is_visible);
    swl_set_window_resizable(window, config->is_resizable);

    return SWL_TRUE;
}

static void swl_destroy_window__platform(swl_window *window)
{
    window->initialized = SWL_FALSE;
}

void swl_set_window_title(swl_window *window, const char *title)
{
    if(!window) {
        SWL_LOG_MESSAGE("Invalid window at swl_set_window_title");
        return;
    }

    XStoreName(APP.platform.display, window->platform.window, title);
    window->title = title;
    XSync(APP.platform.display, False);
}

void swl_set_window_visible(swl_window *window, swl_bool is_visible)
{
    if(!window) {
        SWL_LOG_MESSAGE("Invalid window passed at swl_set_window_visible");
        return;
    }

    if(is_visible) 
        XMapWindow(APP.platform.display, window->platform.window);
    else 
        XUnmapWindow(APP.platform.display, window->platform.window);
    window->is_visible = is_visible;
    XSync(APP.platform.display, False);
}

void swl_set_window_resizable(swl_window *window, swl_bool is_resizable)
{
    Display *display = APP.platform.display;
    Window x11_window = window->platform.window;
    XSizeHints *hints = XAllocSizeHints();
    long supplied;
    XGetWMNormalHints(display, x11_window, hints, &supplied);
    // Reset min and max size when enabling resize
    if (is_resizable) {
        SWL_LOG_MESSAGE("Window is resizable");
        hints->flags &= ~(PMinSize | PMaxSize);
    } else {
        SWL_LOG_MESSAGE("Window is not resizable");
        // Set min and max size to current size when disabling resize
        hints->flags |= (USPosition | PAspect | PMinSize | PMaxSize);
        hints->min_width = hints->max_width = window->width;
        hints->min_height = hints->max_height = window->height;
    }
    XSetWMNormalHints(display, x11_window, hints);
    XFree(hints);
    XSync(display, False);

    window->is_resizable = is_resizable;
}

void swl_poll_window_events(void)
{
    XEvent xevent;
    while(XPending(APP.platform.display) > 0) {
        XNextEvent(APP.platform.display, &xevent);
        swl_event result;
        int scancode;

        // HACK: Save scancode as some IMs clear the field in XFilterEvent
        if (xevent.type == KeyPress || xevent.type == KeyRelease) scancode = xevent.xkey.keycode;

        Bool filtered = False;
        filtered = XFilterEvent(&xevent, None);
        (void)filtered;

        switch(xevent.type) {
            case ClientMessage:
                {
                    if((Atom)xevent.xclient.data.l[0] == APP.platform.wm_delete_window) {
                        result.type = SWL_EVENT_WINDOW_CLOSED;
                        result.window.window = SWL_NULL;
                        for(swl_uint32 i = 0; i < SWL_MAXIMUM_WINDOW_CREATED; ++i) {
                            swl_window *window = &APP.windows[i];
                            if(window->platform.window == xevent.xclient.window) {
                                result.window.window = window;
                            }
                        }
                        swl_push_event(result);
                    }
                } break;
            case KeyPress:
            case KeyRelease:
                {
                    if(xevent.type == KeyPress) result.type = SWL_EVENT_KEY_PRESSED;
                    if(xevent.type == KeyRelease) result.type = SWL_EVENT_KEY_RELEASED;
                    result.key.scancode = scancode;
                    // result.key.mods = translate_platform.key_state(xevent.xkey.state);
                    // result.key.keycode = translate_platform.key(scancode);
                    // if(result.key.keycode < 0) break;
                    swl_push_event(result);
                } break;
            case ButtonPress:
            case ButtonRelease:
                {
                } break;
            default:
                break;
        }

    }
}

Display *swl_x11_get_display(void)
{
    return APP.platform.display;
}

Window swl_x11_get_window(swl_window *window)
{
    return window->platform.window;
}

#endif // SWL_DISPLAY_X11

swl_bool swl_init(void)
{
    if(APP.initialized) {
        SWL_LOG_MESSAGE("Application is already initialized");
        return SWL_FALSE;
    }

    if(!swl_init__platform()) {
        SWL_LOG_MESSAGE("Failed to initialize platform spesific data of application");
        return SWL_FALSE;
    }

    for(swl_uint32 i = 0; i < SWL_MAXIMUM_WINDOW_CREATED; ++i) {
        APP.windows[i].initialized = SWL_FALSE;
    }

    return SWL_TRUE;
}

void swl_deinit(void)
{
    if(!APP.initialized) return;
    swl_deinit__platform();
}

swl_window *swl_create_window(const swl_window_config *config)
{
    swl_window *window = 0;
    for(swl_uint32 i = 0; i < SWL_MAXIMUM_WINDOW_CREATED; ++i) {
        if(!APP.windows[i].initialized) {
            window = &APP.windows[i];
            break;
        }
    }

    if(!window) {
        SWL_LOG_MESSAGE("Could not create window more than SWL_MAXIMUM_WINDOW_CREATED");
        return SWL_NULL;
    }

    swl_window_config real_config;
    real_config.title = "Example SWL Window";
    real_config.width = 800;
    real_config.height = 600;
    real_config.is_resizable = SWL_FALSE;
    real_config.is_visible = SWL_TRUE;
    if(config) {
        real_config.title = config->title;
        real_config.width = config->width;
        real_config.height = config->height;
        real_config.is_resizable = config->is_resizable;
        real_config.is_visible = config->is_visible;
    }

    if(!swl_create_window__platform(window, &real_config)) {
        SWL_LOG_MESSAGE("Failed to initialize the platform spesific data of window");
        return SWL_NULL;
    }

    window->initialized = SWL_TRUE;
    return window;
}

void swl_destroy_window(swl_window *window)
{
    swl_destroy_window__platform(window);
    window->initialized = SWL_FALSE;
}

swl_bool swl_push_event(swl_event event)
{
    unsigned int index = APP.event_queue.head;
    if(APP.event_queue.head + 1 > SWL_EVENT_QUEUE_CAPACITY) {
        if(APP.event_queue.carry) {
            return SWL_FALSE; // Event queue is full
        }
        APP.event_queue.carry = SWL_TRUE;
        APP.event_queue.head = 0;
    } else {
        APP.event_queue.head += 1;
    }

    APP.event_queue.items[index] = event;
    return SWL_TRUE;
}

swl_bool swl_shift_event(swl_event *event)
{
    unsigned int index = APP.event_queue.tail;
    if(APP.event_queue.tail + 1 > APP.event_queue.head) return SWL_FALSE;
    if(APP.event_queue.tail + 1 > SWL_EVENT_QUEUE_CAPACITY) {
        if(!APP.event_queue.carry) {
            return SWL_FALSE; // Event queue is empty
        }
        APP.event_queue.carry = SWL_FALSE;
        APP.event_queue.tail = 0;
    } else {
        APP.event_queue.tail += 1;
    }

    *event = APP.event_queue.items[index];
    return SWL_TRUE;
}

#endif // SWL_IMPLEMENTATION
