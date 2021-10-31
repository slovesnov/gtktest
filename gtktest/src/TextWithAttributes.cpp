/*
 * TextWithAttributes.cpp
 *
 *       Created on: 09.09.2014
 *           Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include "TextWithAttributes.h"

double TextWithAttributes::defaultFontHeight=14;
std::string TextWithAttributes::fontFamily="Times New Roman";

TextWithAttributes::TextWithAttributes(std::string text, int height,
		bool underline, bool bold, bool blackColor) {
	init(text, height, underline, bold, blackColor);
}

void TextWithAttributes::init(std::string text, int height, bool underline,
		bool bold, bool blackColor) {
	m_text = text;
	m_height = height;
	m_underline = underline;
	m_bold = bold;
	m_blackColor = blackColor;
}

TextWithAttributes::TextWithAttributes(std::string text, bool underlined) {
	init(text, defaultFontHeight, underlined, false, false);
}

TextWithAttributes::TextWithAttributes(const char* text) {
	init(text, defaultFontHeight, false, false, false);
}

TextWithAttributes::~TextWithAttributes() {
}

PangoFontDescription* TextWithAttributes::getFontDescription() {
	auto s=fontFamily+", "+std::to_string(m_height);
	return pango_font_description_from_string(s.c_str());
}
