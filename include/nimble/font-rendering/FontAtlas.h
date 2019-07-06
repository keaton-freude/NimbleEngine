#pragma once

// A font bitmap atlas, contains the ASCII charset, all in a single texture
// Also provides utility function for returning the correct UV offsets (and other glyph info??)

#include <ft2build.h>
#include FT_FREETYPE_H

#include <sstream>
#include <vector>

#include "nimble/font-rendering/Character.h"

namespace Nimble {
class FontAtlas {
	GLuint _texture;
	std::vector<Character> _characters;

public:
	FontAtlas() {
		FT_Library ft;
		if(FT_Init_FreeType(&ft)) {
			spdlog::error("Failed to init free type");
		}

		FT_Face face;
		if(FT_New_Face(ft, "/home/kdfreude/Downloads/ttf/FiraCode-Regular.ttf", 0, &face)) {
			spdlog::error("Failed to load FiraCode-Regular.tff");
		}

		FT_Set_Pixel_Sizes(face, 0, 18);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char *buffers[128];

		// Do a pass over all of the characters, storing off relevant information
		for(GLubyte c = 0; c < 128; c++) {
			if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				spdlog::warn("Failed to load glyph at index {}", c);
				continue;
			}

			Character character;
			// We need to figure out the width, height and offsets for every character in our atlas
			character.size = glm::ivec2({ face->glyph->bitmap.width, face->glyph->bitmap.rows });
			character.bearing = glm::ivec2({ face->glyph->bitmap_left, face->glyph->bitmap_top });
			character.advance = face->glyph->advance.x;
			_characters.push_back(character);

			buffers[c] = new unsigned char[face->glyph->bitmap.width * face->glyph->bitmap.rows];
			std::memcpy(buffers[c], face->glyph->bitmap.buffer,
						face->glyph->bitmap.width * face->glyph->bitmap.rows);
		}

		// Create a texture of size Width=Total Width of all glyphs, Height=tallest glyph

		uint16_t tallestGlyph = 0;
		uint32_t width = 0;
		for(GLubyte c = 0; c < 128; c++) {
			if(_characters[c].size.y > tallestGlyph) {
				tallestGlyph = _characters[c].size.y;
			}
			width += _characters[c].size.x;
		}
		spdlog::info("Tallest Glyph: {}, Width: {}", tallestGlyph, width);

		unsigned char *atlasBuffer = new unsigned char[tallestGlyph * width];
		memset(atlasBuffer, 0, width * tallestGlyph);

		int new_x = 0;
		int y = 0;
		int accumulated_width = 0;

		for(GLubyte c = 0; c < 128; c++) {
			for(int x = 0; x < _characters[c].size.x; ++x) {
				for(int y = 0; y < _characters[c].size.y; ++y) {
					int from = (y * _characters[c].size.x) + (x);
					int to = (y * width) + (x + accumulated_width);

					for(int channel = 0; channel < 1; ++channel) {
						atlasBuffer[to + channel] = buffers[c][from + channel];
					}
				}
			}

			accumulated_width += _characters[c].size.x;
		}

		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, tallestGlyph, 0, GL_RED, GL_UNSIGNED_BYTE, atlasBuffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void Test() {
		glBindTexture(GL_TEXTURE_2D, _texture);
	}
};
} // namespace Nimble