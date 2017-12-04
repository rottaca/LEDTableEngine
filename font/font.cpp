/*
Font.cpp
Copyright (C) 2010-2012 Marc GILLERON
This file is part of the zCraft project.
*/

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../opengl/opengl.hpp"
#include "../config.hpp"
#include "../bmfont/Font.hpp"
#include "../core/stringutils.hpp" // for cropStr()

// Character to use when a character has not been found
#define PLACEHOLDER_CHAR '?'

namespace zn
{
namespace bmfont
{
	/*
		FontSettings
	*/

	void CharSet::addKerning(Kerning k)
	{
		m_kernings.push_back(k);
	}

	short CharSet::getKerning(int first, int second) const
	{
		for(auto & k : m_kernings)
		{
			if(k.first == first && k.second != second)
				return k.amount;
		}
		return 0;
	}

	void CharSet::addChar(CharDescriptor cd)
	{
		m_chars[cd.id] = cd;
	}

	const CharDescriptor * CharSet::getChar(int id) const
	{
		// Find the character
		auto it = m_chars.find(id);
		if(it != m_chars.end())
			return &(it->second);
		// If not found, find a placeholder
		it = m_chars.find(PLACEHOLDER_CHAR);
		if(it != m_chars.end())
			return &(it->second);
		return nullptr; // Uh uh...
	}

	/*
		Font
	*/

	Font::Font()
	{
		m_textures = nullptr;
	}

	Font::~Font()
	{
		if(m_textures != nullptr)
			delete[] m_textures;
	}

	bool Font::loadFromFile(const std::string fpath)
	{
		if(m_textures != nullptr)
		{
			std::cout << "WARNING: Font::loadFromFile(): "
				<< "called twice. Second call is ignored." << std::endl;
			return false;
		}

		// Parse .fnt file

		std::cout << "Font: parsing \"" << fpath << "\"..." << std::endl;
		if(!parse(fpath))
			return false;

		// Load resources

		std::cout << "Font: loading textures..." << std::endl;

		int dirCharPos = fpath.find_last_of("/\\");
		std::string dir = fpath.substr(0, dirCharPos + 1);

		m_textures = new sf::Texture[m_chars.pages.size()];

		for(unsigned int i = 0; i < m_chars.pages.size(); i++)
		{
			const std::string texPath = dir + m_chars.pages[i];
			if(!m_textures[i].loadFromFile(texPath))
			{
				std::cout << "ERROR: Font::loadFromFile(): "
					<< "Couldn't load texture file \"" << texPath << "\"" << std::endl;
				return false;
			}
		}

		return true;
	}

	bool Font::parse(const std::string fpath)
	{
		std::ifstream ifs(fpath);
		if(!ifs.good())
		{
			std::cout << "ERROR: Font::loadFromFile: "
				<< "couldn't open file \"" << fpath << "\"" << std::endl;
			return false;
		}

		// Note : the '>>' operator is formatting, so we use short typed values
		// to be sure that they will be read as integers

		while(!ifs.eof())
		{
			unsigned int i = 0;
			std::stringstream lineStream;
			std::string tag, pair, key, value;
			std::string line;

			std::getline(ifs, line);
			lineStream << line;
			lineStream >> tag;

			//std::cout << lineStream.str() << std::endl;

			if(tag == "info")
			{
				// Not implemented yet
				lineStream.str("");
			}
			else if(tag == "common")
			{
				while(!lineStream.eof())
				{
					lineStream >> pair;
					i = pair.find('=');
					key = pair.substr(0, i);
					value = pair.substr(i + 1);
					std::stringstream converter;
					converter << value;

					if(key == "lineHeight")
						converter >> m_chars.lineHeight;
					else if(key == "base")
						converter >> m_chars.base;
					else if(key == "scaleW")
						converter >> m_chars.scaleW;
					else if(key == "scaleH")
						converter >> m_chars.scaleH;
					else if(key == "packed")
						converter >> m_chars.packed;
					else if(key == "alphaChnl")
						converter >> m_chars.alphaChnl;
					else if(key == "redChnl")
						converter >> m_chars.redChnl;
					else if(key == "greenChnl")
						converter >> m_chars.greenChnl;
					else if(key == "blueChnl")
						converter >> m_chars.blueChnl;
					/*else if(key == "pages") // pages are automatically counted
						converter >> ?*/
				}
			}
			else if(tag == "page")
			{
				unsigned short id = 0;
				while(!lineStream.eof())
				{
					lineStream >> pair;
					i = pair.find('=');
					key = pair.substr(0, i);
					value = pair.substr(i + 1);
					std::stringstream converter;
					converter << value;

					if(key == "id")
					{
						converter >> id;
						if(id >= m_chars.pages.size())
							m_chars.pages.resize(id + 1);
					}
					else if(key == "file")
					{
						// Remove quotes
						const std::string filename = cropStr(value, '"');
						m_chars.pages[id] = filename;
					}
				}
			}
			else if(tag == "char")
			{
				CharDescriptor cd;
				// Note : char count is ignored because not needed
				while(!lineStream.eof())
				{
					lineStream >> pair;
					i = pair.find('=');
					key = pair.substr(0, i);
					value = pair.substr(i + 1);
					std::stringstream converter;
					converter << value;

					if(key == "id")
						converter >> cd.id;
					else if(key == "x")
						converter >> cd.x;
					else if(key == "y")
						converter >> cd.y;
					else if(key == "width")
						converter >> cd.width;
					else if(key == "height")
						converter >> cd.height;
					else if(key == "xoffset")
						converter >> cd.xoffset;
					else if(key == "yoffset")
						converter >> cd.yoffset;
					else if(key == "xadvance")
						converter >> cd.xadvance;
					else if(key == "page")
						converter >> cd.page;
					else if(key == "chnl")
						converter >> cd.chnl;
				}

				m_chars.addChar(cd);
			}
			else if(tag == "kerning")
			{
				Kerning k;
				// Note : Kerning count is ignored because not needed
				while(!lineStream.eof())
				{
					lineStream >> pair;
					i = pair.find('=');
					key = pair.substr(0, i);
					value = pair.substr(i + 1);
					std::stringstream converter;
					converter << value;

					if(key == "first")
						converter >> k.first;
					else if(key == "second")
						converter >> k.second;
					else if(key == "amount")
						converter >> k.amount;
				}

				m_chars.addKerning(k);
			}
		}

		return true;
	}

	void Font::draw(const std::wstring &text, bool invertYAxis)
	{
		draw(text, 0, 0, invertYAxis);
	}

	int Font::getLineHeight()
	{
		return m_chars.lineHeight;
	}

	Vector2i Font::getTextSize(const std::wstring &text, int begin, int end)
	{
		if(begin < 0)
			return Vector2i();
		if(end < 0 || end >= static_cast<int>(text.size()))
			end = text.size()-1;

		// TODO Font::getTextSize() : exact height instead of max height

		Vector2i size(0, getLineHeight());
		for(int i = begin; i <= end; ++i)
		{
			const CharDescriptor * cd = m_chars.getChar(text[i]);
			if(cd == nullptr)
			{
				// No character found, even no placeholder.
				break; // Stop
			}
			size.x += cd->xadvance;
		}
		return size;
	}

	// TODO Font: add kerning support
	// It is parsed but not used

	// TODO Font: optimize rendering

	void Font::draw(const std::wstring &text, float x0, float y0, bool invertYAxis)
	{
		unsigned int originX = x0, originY = y0; // Cursor position
		unsigned int x, y; // Shifted position
		float tx, ty, tw, th; // Texture sub-rect coordinates
		wchar_t c; // Current read character

	#if defined ZN_OPENGL2
		glEnable(GL_TEXTURE_2D);
	#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for(unsigned int i = 0; i < text.size(); i++)
		{
			c = text[i];

			// Line endings
			if(c == '\n') // Newline
			{
				originY += m_chars.lineHeight;
				originX = x0;
				continue;
			}
			else if(c == '\r') // Carriage return
			{
				continue;
			}

			// Get character descriptor
			const CharDescriptor * cd = m_chars.getChar(c);
			if(cd == nullptr)
			{
				// No character found, even no placeholder.
				break; // Stop drawing.
			}

			// Use the glyph atlas texture
			const sf::Texture & tex = m_textures[cd->page];

			// Get glyph texture sub-rect
			const sf::Vector2u ts = tex.getSize();
			tx = static_cast<float>(cd->x) / static_cast<float>(ts.x);
			ty = static_cast<float>(cd->y) / static_cast<float>(ts.y);
			tw = static_cast<float>(cd->width) / static_cast<float>(ts.x);
			th = static_cast<float>(cd->height) / static_cast<float>(ts.y);

			// Get drawing coordinates
			x = originX + cd->xoffset;
			y = originY + cd->yoffset;

			// Draw glyph

			sf::Texture::bind(&tex);
			gl::drawTexturedRect(
				x, y, cd->width, cd->height,
				tx, ty, tw, th);

//			glBegin(GL_QUADS);
//
//			glTexCoord2f(tx, ty);
//			glVertex2i(x, y);
//
//			glTexCoord2f(tx + tw, ty);
//			glVertex2i(x + cd->width, y);
//
//			glTexCoord2f(tx + tw, ty + th);
//			glVertex2i(x + cd->width, y + cd->height);
//
//			glTexCoord2f(tx, ty + th);
//			glVertex2i(x, y + cd->height);
//
//			glEnd();

			// Advance cursor
			originX += cd->xadvance;

			// DEBUG
			// This code draws glyph rectangles
			/*
			glDisable(GL_TEXTURE_2D);
			glColor4f(1,1,0,1);
			glBegin(GL_LINE_LOOP);
			glVertex2i(originX, originY);
			glVertex2i(originX + cd->width, originY);
			glVertex2i(originX + cd->width, originY + cd->height);
			glVertex2i(originX, originY + cd->height);
			glEnd();
			glColor4f(0,1,1,1);
			glBegin(GL_LINE_LOOP);
			glVertex2i(x, y);
			glVertex2i(x + cd->width, y);
			glVertex2i(x + cd->width, y + cd->height);
			glVertex2i(x, y + cd->height);
			glEnd();
			glColor4f(1,1,1,1);
			glEnable(GL_TEXTURE_2D);
			*/
		}

	#if defined ZN_OPENGL2
		glDisable(GL_TEXTURE_2D);
	#endif

	}

} // namespace bmfont
} // namespace zn