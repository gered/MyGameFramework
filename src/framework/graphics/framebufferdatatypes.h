#ifndef __FRAMEWORK_GRAPHICS_FRAMEBUFFERDATATYPES_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_FRAMEBUFFERDATATYPES_H_INCLUDED__

/**
 * Constant values used to refer to the type of data values that a 
 * framebuffer's texture/renderbuffer attachments contain.
 */
enum FRAMEBUFFER_DATA_TYPE
{
	FRAMEBUFFER_DATA_NONE = 0,
	FRAMEBUFFER_DATA_COLOR_RGB = 1,
	FRAMEBUFFER_DATA_COLOR_RGBA = 2,
	FRAMEBUFFER_DATA_DEPTH = 3,
	FRAMEBUFFER_DATA_STENCIL = 4
};

#endif
