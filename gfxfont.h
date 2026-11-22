// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#pragma once

// Stops the build: it is only called for a glyph the packed record below
// cannot hold.
uint8_t gfx_glyph_does_not_fit(void) __attribute__((
    error("GFXglyph: this glyph does not fit MCL's 3-byte glyph record")));

// Data stored PER GLYPH. MCL's AVR fonts are small (a glyph is under 16
// pixels each way, sits on or above the baseline with no x offset, and a
// font's bitmaps are under 256 bytes), so a glyph takes three bytes here in
// place of Adafruit's seven. The constructor takes Adafruit's six values, so
// font tables are written exactly as for the stock library.
typedef struct GFXglyph {
	uint8_t bitmapOffset; // Pointer into GFXfont->bitmap
	uint8_t size;         // width (low nibble) and height (high nibble)
	uint8_t place;        // xAdvance (low nibble) and -yOffset (high nibble)
	constexpr GFXglyph(uint8_t offset, uint8_t width, uint8_t height,
	                   uint8_t xAdvance, int8_t xOffset, int8_t yOffset)
	    : bitmapOffset((width < 16 && height < 16 && xAdvance < 16 &&
	                    xOffset == 0 && yOffset <= 0 && yOffset > -16)
	                       ? offset
	                       : gfx_glyph_does_not_fit()),
	      size(width | (height << 4)), place(xAdvance | (-yOffset << 4)) {}
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
	uint8_t  *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	uint8_t   first, last; // ASCII extents
	uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;

