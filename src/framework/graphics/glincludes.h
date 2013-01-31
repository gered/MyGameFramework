 #ifndef __FRAMEWORK_GRAPHICS_GLINCLUDES_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_GLINCLUDES_H_INCLUDED__

#ifdef DESKTOP

// GLEW will include all necessary GL headers (e.g. gl.h, glu.h ... )
#include <glew.h>

#elif __S3E__

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#else

#error Unknown platform

#endif

#endif
