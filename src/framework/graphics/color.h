#ifndef __FRAMEWORK_GRAPHICS_COLOR_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_COLOR_H_INCLUDED__

#include "../common.h"

const float COLOR_ALPHA_TRANSPARENT = 0.0f;
const float COLOR_ALPHA_OPAQUE = 1.0f;
const uint8_t COLOR_ALPHA_TRANSPARENT_INT = 0;
const uint8_t COLOR_ALPHA_OPAQUE_INT = 255;

/**
 * Stores the RGBA components of a color and contains methods for easy
 * conversion of various other representations of a color.
 */
struct Color
{
	/**
	 * Creates a new color, initialized to opaque black.
	 */
	Color();

	/**
	 * Creates a new color.
	 * @param red the red component
	 * @param green the green component
	 * @param blue the blue component
	 * @param alpha the alpha component
	 */
	Color(float red, float green, float blue, float alpha = COLOR_ALPHA_OPAQUE);

	/**
	 * Creates a new color.
	 * @param red the red component (0-255)
	 * @param green the green component (0-255)
	 * @param blue the blue component (0-255)
	 * @param alpha the alpha component (0-255)
	 */
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = COLOR_ALPHA_OPAQUE_INT);

	/**
	 * @return a 32-bit hex representation of this color
	 */
	uint32_t ToInt() const;

	/**
	 * Converts and returns the RGBA color components to their integer
	 * (0-255) equivalents.
	 * @param out_r will contain the red component as an integer if not NULL
	 * @param out_g will contain the green component as an integer if not NULL
	 * @param out_b will contain the blue component as an integer if not NULL
	 * @param out_a will contain the alpha component as an integer if not NULL
	 */
	void ToInts(uint8_t *out_r, uint8_t *out_g, uint8_t *out_b, uint8_t *out_a) const;

	/**
	 * Extracts the individual RGBA components out of a 32-bit hex 
	 * representation of a color as integers (0-255).
	 * @param color the 32-bit hex color value
	 * @param out_r will contain the red component as an integer if not NULL
	 * @param out_g will contain the green component as an integer if not NULL
	 * @param out_b will contain the blue component as an integer if not NULL
	 * @param out_a will contain the alpha component as an integer if not NULL
	 */
	static void FromInt(uint32_t color, uint8_t *out_r, uint8_t *out_g, uint8_t *out_b, uint8_t *out_a);

	/**
	 * Extracts the individual RGBA components out of a 32-bit hex 
	 * representation of a color.
	 * @param color the 32-bit hex color value
	 * @param out_r will contain the red component if not NULL
	 * @param out_g will contain the green component if not NULL
	 * @param out_b will contain the blue component if not NULL
	 * @param out_a will contain the alpha component if not NULL
	 */
	static void FromInt(uint32_t color, float *out_r, float *out_g, float *out_b, float *out_a);

	/**
	 * Converts a 32-bit hex representation of a color to a color object.
	 * @param color the 32-bit hex color value to convert
	 * @return an equivalent color object
	 */
	static Color FromInt(uint32_t color);

	/**
	 * Packs RGBA color components into a 32-bit hex representation.
	 * @param r the red component (0-255)
	 * @param g the green component (0-255)
	 * @param b the blue component (0-255)
	 * @param a the alpha component (0-255)
	 * @return an equivalent 32-bit hex representation
	 */
	static uint32_t ToInt(uint8_t r, uint8_t g, uint8_t b, uint8_t a = COLOR_ALPHA_OPAQUE_INT);

	/**
	 * Packs RGBA color components into a 32-bit hex representation.
	 * @param r the red component
	 * @param g the green component
	 * @param b the blue component
	 * @param a the alpha component
	 * @return an equivalent 32-bit hex representation
	 */
	static uint32_t ToInt(float r, float g, float b, float a = COLOR_ALPHA_OPAQUE);

	float r;
	float g;
	float b;
	float a;
};

#define COLOR_WHITE Color(1.0f, 1.0f, 1.0f)
#define COLOR_RED Color(1.0f, 0.0f, 0.0f)
#define COLOR_GREEN Color(0.0f, 1.0f, 0.0f)
#define COLOR_BLUE Color(0.0f, 0.0f, 1.0f)
#define COLOR_YELLOW Color(1.0f, 1.0f, 0.0f)
#define COLOR_CYAN Color(0.0f, 1.0f, 1.0f)
#define COLOR_MAGENTA Color(1.0f, 0.0f, 1.0f)
#define COLOR_BLACK Color(0.0f, 0.0f, 0.0f)

inline Color::Color()
{
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = COLOR_ALPHA_OPAQUE;
}

inline Color::Color(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

inline Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	uint32_t color = ToInt(red, green, blue, alpha);
	FromInt(color, &r, &g, &b, &a);
}

inline uint32_t Color::ToInt() const
{
	return ((uint32_t)(a * 255) << 24) | ((uint32_t)(r * 255) << 16) | ((uint32_t)(g * 255) << 8) | (uint32_t)(b * 255);
}

inline void Color::ToInts(uint8_t *out_r, uint8_t *out_g, uint8_t *out_b, uint8_t *out_a) const
{
	if (out_r)
		*out_r = (int)(r * 255);
	if (out_g)
		*out_g = (int)(g * 255);
	if (out_b)
		*out_b = (int)(b * 255);
	if (out_a)
		*out_a = (int)(a * 255);
}

inline void Color::FromInt(uint32_t color, uint8_t *out_r, uint8_t *out_g, uint8_t *out_b, uint8_t *out_a)
{
	if (out_a)
		*out_a = (color & 0xff000000) >> 24;
	if (out_r)
		*out_r = (color & 0x00ff0000) >> 16;
	if (out_g)
		*out_g = (color & 0x0000ff00) >> 8;
	if (out_b)
		*out_b = (color & 0x000000ff);
}

inline void Color::FromInt(uint32_t color, float *out_r, float *out_g, float *out_b, float *out_a)
{
	if (out_a)
		*out_a = ((float)((color & 0xff000000) >> 24)) / 255;
	if (out_r)
		*out_r = ((float)((color & 0x00ff0000) >> 16)) / 255;
	if (out_g)
		*out_g = ((float)((color & 0x0000ff00) >> 8)) / 255;
	if (out_b)
		*out_b = ((float)((color & 0x000000ff))) / 255;
}

inline Color Color::FromInt(uint32_t color)
{
	Color out;
	FromInt(color, &out.r, &out.g, &out.b, &out.a);
	return out;
}

inline uint32_t Color::ToInt(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return (a << 24) | (r << 16) | (g << 8) | b;
}

inline uint32_t Color::ToInt(float r, float g, float b, float a)
{
	return ((int)(a * 255) << 24) | ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | (int)(b * 255);
}

#endif

