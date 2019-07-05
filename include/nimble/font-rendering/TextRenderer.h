#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "nimble/font-rendering/Character.h"
#include "nimble/opengl-wrapper/ShaderProgram.h"
#include "nimble/resource-manager/ResourceManager.h"
#include "spdlog/spdlog.h"

// Renders fonts to the screen!
namespace Nimble {
class TextRenderer {
private:
	using FontCharacterTextureStorageTy = std::unordered_map<int, Character>;
	FontCharacterTextureStorageTy _fontTextures;

	// The projection we are going to use for rendering
	glm::mat4 _projection;
	std::shared_ptr<ShaderProgram> _fontShader;
	GLuint VAO, VBO;

public:
	TextRenderer() {
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
		for(GLubyte c = 0; c < 128; c++) {
			if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				spdlog::warn("Failed to load glyph at index {}", c);
				continue;
			}

			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
						 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			Character character;
			character._textureId = texture;
			character.size = glm::ivec2({ face->glyph->bitmap.width, face->glyph->bitmap.rows });
			character.bearing = glm::ivec2({ face->glyph->bitmap_left, face->glyph->bitmap_top });
			character.advance = face->glyph->advance.x;

			_fontTextures.insert({ c, character });
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		_fontShader = ResourceManager::Get().GetShader("font");

		spdlog::debug("Finished creating font textures");
	}

	void SetProjectionDimensions(float width, float height) {
		spdlog::debug("Creating TextRenderer projection with dimensions {}x{}", width, height);
		_projection = glm::ortho(0.0f, width, 0.0f, height);
	}

	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
		_fontShader->Use();
		glUniform3f(glGetUniformLocation(_fontShader->ShaderHandle(), "textColor"), color.x,
					color.y, color.z);
		glUniformMatrix4fv(glGetUniformLocation(_fontShader->ShaderHandle(), "projection"), 1,
						   GL_FALSE, &_projection[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		for(auto c : text) {
			Character ch = _fontTextures[c];
			GLfloat xpos = x + ch.bearing.x * scale;
			GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

			GLfloat w = ch.size.x * scale;
			GLfloat h = ch.size.y * scale;

			GLfloat vertices[6][4] = { { xpos, ypos + h, 0.0, 0.0 },
									   { xpos, ypos, 0.0, 1.0 },
									   { xpos + w, ypos, 1.0, 1.0 },
									   { xpos, ypos + h, 0.0, 0.0 },
									   { xpos + w, ypos, 1.0, 1.0 },
									   { xpos + w, ypos + h, 1.0, 0.0 } };

			glBindTexture(GL_TEXTURE_2D, ch._textureId);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (ch.advance >> 6) * scale;
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};
} // namespace Nimble