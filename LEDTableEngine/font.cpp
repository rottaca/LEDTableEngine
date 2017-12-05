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
#include <assert.h>

#include "font.hpp"

// Character to use when a character has not been found
#define PLACEHOLDER_CHAR '?'

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

	}

	Font::~Font()
	{
		for(Image* i: m_textures){
			delete i;
		}
		m_textures.clear();
	}

	bool Font::loadFromFile(const std::string fpath)
	{
		if(m_textures.size()> 0)
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

		m_textures.resize(m_chars.pages.size());

		for(unsigned int i = 0; i < m_chars.pages.size(); i++)
		{
			const std::string texPath = dir + m_chars.pages[i];
			SDL_Surface* surf = SDL_LoadBMP(texPath.c_str());
			if(!surf)
			{
				std::cout << "ERROR: Font::loadFromFile(): "
					<< "Couldn't load texture file \"" << texPath << "\"" << std::endl;
				return false;
			}
			Image* img = new Image(surf->w, surf->h, surf->pitch/surf->w);
			memcpy(img->data, surf->pixels,img->size);
			SDL_FreeSurface(surf);
			m_textures[i] = img;
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
						value.erase(0,1);
						value.erase(value.size()-1,1);
						m_chars.pages[id] = value;
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

	void Font::draw(Image& img, const std::string &text, std::vector<uint8_t> color)
	{
		draw(img,text, 0, 0, color);
	}

	int Font::getLineHeight()
	{
		return m_chars.lineHeight;
	}

	Point Font::getTextSize(const std::string &text, int begin, int end)
	{
		if(begin < 0)
			return Point(0,0);
		if(end < 0 || end >= static_cast<int>(text.size()))
			end = text.size()-1;

		// TODO Font::getTextSize() : exact height instead of max height

		Point size(0, getLineHeight());
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

	void Font::draw(Image& img, const std::string &text, float x0, float y0, std::vector<uint8_t> color)
	{
		assert(color.size() == img.channels);

		ssize_t originX = x0, originY = y0; // Cursor position
		wchar_t c; // Current read character

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
			const Image* tex = m_textures[cd->page];

	    for (ssize_t yFnt = 0; yFnt < cd->height; yFnt++) {
	       unsigned char* pFnt = tex->data + (yFnt+cd->y)*tex->rowstride + tex->channels*(cd->x);
		     for (ssize_t xFnt = 0; xFnt < cd->width; xFnt++) {
			     if(*pFnt > 0){
						 ssize_t xImg = originX + cd->xoffset + xFnt;
						 ssize_t yImg = originY + cd->yoffset + yFnt;
						 if(xImg >= 0 && xImg < img.width && yImg >= 0 && yImg <= img.height)
						  {
								int idx = (yImg)*img.width + xImg;
							  for(uint8_t c: color)
							 	img.data[idx++] = c;
							}
			     }
					 pFnt+=tex->channels;
	       }
		  }

			// Advance cursor
			originX += cd->xadvance;

		}

	}

} // namespace bmfont
