/*

    `glc.h` - A multiplatform headeronly library for creating OpenGL Context.

    Do this:
        #define GLC_IMPLEMENTATION

    before you include this file in *one* C or C++ file to create the
    implementation.
    In the same place define one of the following display system. It 
    will choose the corresponding OpenGL context creation library.
        #define GLC_DISPLAY_WIN32 // win32 display will use wgl backend
        #define GLC_DISPLAY_X11 // X11 will use GLX backend
        #define GLC_DISPLAY_ANDROID 
        #define GLC_DISPLAY_WAYLAND

    Optionally provide the following defines with your own implementations:
        #define GLC_LOG_MESSAGE(message) // If you define this, GLC will log message when error happened
        #define GLC_MALLOC // malloc. both free and malloc should be defined
        #define GLC_FREE // free. both free and malloc should be defined

    Link with the following system libraries:
    - on Linux with GLC_DISPLAY_X11: X11, GL

*/
#ifndef GLC_H_
#define GLC_H_

#include <string.h> // strstr, 

#ifndef GLC_LOG_MESSAGE
#include <stdio.h>
#define GLC_LOG_MESSAGE(message) printf("%s\n", (message));
#endif

#if !defined(GLC_MALLOC) && !defined(GLC_FREE)
#include <stdlib.h>
#define GLC_MALLOC malloc
#define GLC_FREE free
#endif

#if !defined(GLC_MALLOC) || !defined(GLC_FREE)
#error "Please define both GLC_MALLOC and GLC_FREE macros"
#endif

#ifndef GLCDEF
#define GLCDEF
#endif

#ifdef __linux__
#if !defined(GLC_DISPLAY_X11) || !defined(GLC_DISPLAY_WAYLAND)
#define GLC_DISPLAY_X11
#endif
#endif

#ifdef GLC_DISPLAY_X11
typedef struct _XDisplay Display;
typedef unsigned long XID;
typedef XID Window;
#endif

#ifdef GLC_DISPLAY_WIN32
#include <windows.h>
#endif

#ifdef GLC_DISPLAY_ANDROID
#error "Not implemented yet"
#endif

#ifdef GLC_DISPLAY_WAYLAND
#error "Not implemented yet"
#endif

typedef struct GLCBackend GLCBackend;

typedef struct GLCBackendConfig {
#ifdef GLC_DISPLAY_X11
    struct {
        Display *display;
    } x11;
#endif
#ifdef GLC_DISPLAY_WIN32
    struct {
        HINSTANCE hInstance;
    } win32;
#endif
} GLCBackendConfig;

typedef struct GLCContext GLCContext;

typedef struct GLCContextConfig {
#ifdef GLC_DISPLAY_X11
    struct {
        Window window;
    } x11;
#endif
#ifdef GLC_DISPLAY_WIN32
    struct {
        HWND hWnd;
    } win32;
#endif

    struct {
        int major;
        int minor;
    } version;

    char use_debug_context;
    char use_opengles;
    char is_backward_compatible;
    char is_core_profile;
} GLCContextConfig;

GLCDEF GLCBackend *glcCreateBackend(const GLCBackendConfig *config);
GLCDEF void glcDestroyBackend(GLCBackend *backend);

GLCDEF GLCContext *glcCreateContext(GLCBackend *backend, const GLCContextConfig *config);
GLCDEF void glcDestroyContext(GLCBackend *backend, GLCContext *context);
GLCDEF void glcMakeContextCurrent(GLCBackend *backend, GLCContext* context);
GLCDEF void glcSwapBuffer(GLCBackend *backend, GLCContext *context);

#endif // GLC_H_

#ifdef GLC_IMPLEMENTATION

#ifdef GLC_DISPLAY_X11
#include <GL/glx.h>
#include <X11/Xlib.h>
#endif

struct GLCBackend {
#ifdef GLC_DISPLAY_WIN32
    struct {
        HINSTANCE hInstance;
    } win32;
#endif
#ifdef GLC_DISPLAY_X11
    struct {
        Display *display;
        XVisualInfo *visual_info;
    } x11;
    struct {
        int event_base, error_base;
        int major, minor;
        struct {
            Bool EXT_swap_control;
            Bool SGI_swap_control;
            Bool MESA_swap_control;
            Bool ARB_multisample;
            Bool ARB_framebuffer_sRGB;
            Bool EXT_framebuffer_sRGB;
            Bool ARB_create_context;
            Bool ARB_create_context_robustness;
            Bool ARB_create_context_profile;
            Bool EXT_create_context_es2_profile;
            Bool ARB_create_context_no_error;
            Bool ARB_context_flush_control;
        } extensions;
        struct {
            PFNGLXSWAPINTERVALEXTPROC SwapIntervalEXT;
            PFNGLXSWAPINTERVALSGIPROC SwapIntervalSGI;
            PFNGLXSWAPINTERVALMESAPROC SwapIntervalMESA;
            PFNGLXCREATECONTEXTATTRIBSARBPROC CreateContextAttribsARB;
        } api;
        GLXFBConfig fbconfig;
    } glx;
#endif
};

struct GLCContext {
#ifdef GLC_DISPLAY_WIN32
    struct {
        HWND hWnd;
    } x11;
    struct {
    } wgl;
#endif
#ifdef GLC_DISPLAY_X11
    struct {
        Window window;
    } x11;
    struct {
        GLXContext context;
        GLXWindow window;
    } glx;
#endif
};

#ifdef GLC_DISPLAY_WIN32

static int glcInitBackend_Platform(GLCBackend *backend, const GLCBackendConfig *config);
static void glcDeinitBackend_Platform(GLCBackend *backend);
static int glcInitContext_Platform(GLCBackend *backend, GLCContext *context, const GLCContextConfig *config);
static void glcDeinitContext_Platform(GLCBackend *backend, GLCContext *context);
static void glcMakeContextCurrent_Platform(GLCBackend *backend, GLCContext *context);
static void glcSwapBuffer_Platform(GLCBackend *backend, GLCContext *context);

#endif

#ifdef GLC_DISPLAY_X11
static int glx_extensions_has(const char *extensions, const char *name) {
    const char* start = extensions;

    for (;;)
    {
        const char* where;
        const char* terminator;

        where = strstr(start, name);
        if (!where)
            return 0;

        terminator = where + strlen(name);
        if (where == start || *(where - 1) == ' ') {
            if (*terminator == ' ' || *terminator == '\0')
                break;
        }

        start = terminator;
    }

    return 1;
}

static int choose_glx_framebuffer_config(Display *display, int screenID, 
        GLCBackend *backend) // TODO: configurable
{
	GLint glx_attribs[] = {
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		None
	};

	int fb_count;
    GLXFBConfig* fbc = glXChooseFBConfig(display, screenID, glx_attribs, &fb_count);
    if (fbc == 0) {
        GLC_LOG_MESSAGE("Failed to retrive GLX Framebuffer");
        return 0;
    }

	// Pick the FB config/visual with the most samples per pixel
	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
	for (int i = 0; i < fb_count; ++i) {
		XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[i]);
		if (vi != 0) {
			int samp_buf, samples;
			glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
			glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);

			if(best_fbc < 0 || (samp_buf && samples > best_num_samp)) {
				best_fbc = i;
				best_num_samp = samples;
			}
			if(worst_fbc < 0 || !samp_buf || samples < worst_num_samp) {
				worst_fbc = i;
            }
			worst_num_samp = samples;
		}
		XFree(vi);
	}

    backend->glx.fbconfig = fbc[best_fbc];
	XFree(fbc);
    return 1;
}

static int glcInitBackend_Platform(GLCBackend *backend, const GLCBackendConfig *config)
{
    Display *display = config->x11.display;
    int screenID = XDefaultScreen(display);

    if (!glXQueryExtension(display, &backend->glx.error_base, &backend->glx.event_base)) {
        GLC_LOG_MESSAGE("GLX extension is not found");
        return 0;
    }

    const char *extensions = glXQueryExtensionsString(display, screenID);
    if(!extensions) {
        GLC_LOG_MESSAGE("GLX extensions string is not found");
        return 0;
    }

    if (!glXQueryVersion(display, &backend->glx.major, &backend->glx.minor)) {
        GLC_LOG_MESSAGE("Failed to query GLX version");
        return 0;
    }

    if (backend->glx.major == 1 && backend->glx.minor < 3) {
        GLC_LOG_MESSAGE("GLC require GLX version 1.3 or above");
        return 0;
    }

    if (glx_extensions_has(extensions, "GLX_EXT_swap_control")) {
        backend->glx.api.SwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)
            glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalEXT");

        if (backend->glx.api.SwapIntervalEXT)
            backend->glx.extensions.EXT_swap_control = 1;
    }

    if (glx_extensions_has(extensions, "GLX_SGI_swap_control")) {
        backend->glx.api.SwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC)
            glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalSGI");

        if (backend->glx.api.SwapIntervalSGI)
            backend->glx.extensions.SGI_swap_control = 1;
    }

    if (glx_extensions_has(extensions, "GLX_MESA_swap_control")) {
        backend->glx.api.SwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC)
            glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalMESA");

        if (backend->glx.api.SwapIntervalMESA)
            backend->glx.extensions.MESA_swap_control = 1;
    }

    if (glx_extensions_has(extensions, "GLX_ARB_create_context")) {
        backend->glx.api.CreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)
            glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");

        if (backend->glx.api.CreateContextAttribsARB)
            backend->glx.extensions.ARB_create_context = 1;
    }

    if (glx_extensions_has(extensions, "GLX_ARB_multisample"))
        backend->glx.extensions.ARB_multisample = 1;
    if (glx_extensions_has(extensions, "GLX_ARB_framebuffer_sRGB"))
        backend->glx.extensions.ARB_framebuffer_sRGB = 1;
    if (glx_extensions_has(extensions, "GLX_EXT_framebuffer_sRGB"))
        backend->glx.extensions.EXT_framebuffer_sRGB = 1;
    if (glx_extensions_has(extensions, "GLX_ARB_create_context_robustness"))
        backend->glx.extensions.ARB_create_context_robustness = 1;
    if (glx_extensions_has(extensions, "GLX_ARB_create_context_profile"))
        backend->glx.extensions.ARB_create_context_profile = 1;
    if (glx_extensions_has(extensions, "GLX_EXT_create_context_es2_profile"))
        backend->glx.extensions.EXT_create_context_es2_profile = 1;
    if (glx_extensions_has(extensions, "GLX_ARB_create_context_no_error"))
        backend->glx.extensions.ARB_create_context_no_error = 1;
    if (glx_extensions_has(extensions, "GLX_ARB_context_flush_control"))
        backend->glx.extensions.ARB_context_flush_control = 1;

    if(!choose_glx_framebuffer_config(display, screenID, backend)) {
        GLC_LOG_MESSAGE("Failed to choose framebuffer config");
        return 0;
    }

    backend->x11.visual_info = glXGetVisualFromFBConfig(display, backend->glx.fbconfig);
    if (backend->x11.visual_info == 0) {
        GLC_LOG_MESSAGE("Could not create correct visual info");
        return 0;
    }


    if (screenID != backend->x11.visual_info->screen) {
        GLC_LOG_MESSAGE("Screen ID doesnt match visual screen ID");
        return 0;
    }

    backend->x11.display = display;

    return 1;
}

static void glcDeinitBackend_Platform(GLCBackend *backend)
{
    XFree(backend->x11.visual_info);
}

static int glcInitContext_Platform(GLCBackend *backend, GLCContext *context, const GLCContextConfig *config)
{
    GLXContext share = NULL;
    GLXContext result_context = NULL;

    if(config->use_opengles) {
        if (!backend->glx.extensions.ARB_create_context ||
                !backend->glx.extensions.ARB_create_context_profile ||
                !backend->glx.extensions.EXT_create_context_es2_profile) {
            GLC_LOG_MESSAGE("GLX: OpenGL ES requested but GLX_EXT_create_context_es2_profile is unavailable");
            return 0;
        }
    }

    if (!config->is_backward_compatible) {
        if (!backend->glx.extensions.ARB_create_context) {
            GLC_LOG_MESSAGE("GLX: Forward compatibility requested but GLX_ARB_create_context_profile is unavailable");
            return 0;
        }
    }

    if(backend->glx.extensions.ARB_create_context) {
        int attribs[40];
#define SET_ATTRIB(a, v) \
        do { \
            if(((size_t)index + 1) < (sizeof(attribs) / sizeof(attribs[0]))) { \
                attribs[index++] = a; \
                attribs[index++] = v; \
            } else { \
                GLC_LOG_MESSAGE("Failed to create opengl context while assigning attribute"); \
                return 0; \
            } \
        } while(0)

        int index = 0, mask = 0, flags = 0;
        if(config->use_opengles) {
            mask |= GLX_CONTEXT_ES2_PROFILE_BIT_EXT;
        } else {
            if (config->is_core_profile) mask |= GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
            else mask |= GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
        }

        if (config->use_debug_context) flags |= GLX_CONTEXT_DEBUG_BIT_ARB;
        if (config->version.major != 1 || config->version.minor != 0)
        {
            SET_ATTRIB(GLX_CONTEXT_MAJOR_VERSION_ARB, config->version.major);
            SET_ATTRIB(GLX_CONTEXT_MINOR_VERSION_ARB, config->version.minor);
        }

        if (mask) SET_ATTRIB(GLX_CONTEXT_PROFILE_MASK_ARB, mask);
        if (flags) SET_ATTRIB(GLX_CONTEXT_FLAGS_ARB, flags);

        SET_ATTRIB(None, None);
        result_context = backend->glx.api.CreateContextAttribsARB(backend->x11.display,
                backend->glx.fbconfig,
                share,
                True,
                attribs);

        if (!result_context) {
            GLC_LOG_MESSAGE("Failed to create GLX context with ARB");
        }
    } else {
        result_context = glXCreateNewContext(backend->x11.display, 
                backend->glx.fbconfig, GLX_RGBA_TYPE, share, True);
    }

    context->glx.context = result_context;
    context->glx.window = glXCreateWindow(backend->x11.display,
            backend->glx.fbconfig, config->x11.window, NULL);

    if(!context->glx.window) {
        glXDestroyContext(backend->x11.display, context->glx.context);
        return 0;
    }

    return 1;
}

static void glcDeinitContext_Platform(GLCBackend *backend, GLCContext *context)
{
    glXDestroyWindow(backend->x11.display, context->glx.window);
    glXDestroyContext(backend->x11.display, context->glx.context);
}

static void glcMakeContextCurrent_Platform(GLCBackend *backend, GLCContext *context)
{
    if(context)
        glXMakeCurrent(backend->x11.display, context->glx.window, context->glx.context);
    else
        glXMakeCurrent(backend->x11.display, 0, 0);
}

static void glcSwapBuffer_Platform(GLCBackend *backend, GLCContext *context)
{
    glXSwapBuffers(backend->x11.display, context->glx.window);
}

#endif

GLCBackend *glcCreateBackend(const GLCBackendConfig *config)
{
    if(!config->x11.display) {
        GLC_LOG_MESSAGE("Expecting a valid X11 in glcCreateBackend()");
        return NULL;
    }

    GLCBackend *result = (GLCBackend *)GLC_MALLOC(sizeof(*result));
    if(!result) {
        GLC_LOG_MESSAGE("Failed to allocate memory for GLCBackend at glcCreateBackend()");
        return NULL;
    }

    if(!glcInitBackend_Platform(result, config)) {
        GLC_LOG_MESSAGE("Failed to initialize backend in platform code");
        GLC_FREE(result);
        return NULL;
    }

    return result;
}

void glcDestroyBackend(GLCBackend *backend)
{
    if(!backend) return;

    glcDeinitBackend_Platform(backend);
    GLC_FREE(backend);
}

GLCContext *glcCreateContext(GLCBackend *backend, const GLCContextConfig *config)
{
    if(!backend || !config) {
        GLC_LOG_MESSAGE("Failed to create context due to invalid function glcCreateContext arguments");
        return NULL;
    }

    GLCContext *result = (GLCContext *)GLC_MALLOC(sizeof(*result));
    if(!glcInitContext_Platform(backend, result, config)) {
        GLC_FREE(result);
        return NULL;
    }
    return result;
}

void glcDestroyContext(GLCBackend *backend, GLCContext *context)
{
    if(!backend || !context) return;
    glcDeinitContext_Platform(backend, context);
    GLC_FREE(context);
}

void glcMakeContextCurrent(GLCBackend *backend, GLCContext* context)
{
    if(!backend) {
        GLC_LOG_MESSAGE("glcMakeContextCurrent expecting a valid backend");
        return;
    }
    glcMakeContextCurrent_Platform(backend, context);
}

void glcSwapBuffer(GLCBackend *backend, GLCContext *context)
{
    if(!(backend && context)) return;
    glcSwapBuffer_Platform(backend, context);
}

#endif // GLC_IMPLEMENTATION
