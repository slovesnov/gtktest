/*
 * TextWithAttributes.h
 *
 *       Created on: 09.09.2014
 *           Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef TEXTWITHATTRIBUTES_H_
#define TEXTWITHATTRIBUTES_H_

#include <gtk/gtk.h>
#include <string>

class TextWithAttributes {
public:
	int m_height;
	bool m_underline;
	bool m_bold;
	bool m_blackColor;
	std::string m_text;
	GdkRGBA m_rgba;
	void init(std::string text, int height, bool underline, bool bold,
			bool blackColor);
//public:
	static double defaultFontHeight;
	static std::string fontFamily;

	static TextWithAttributes createUnderlinedText(std::string text) {
		TextWithAttributes t(text, defaultFontHeight, true, false, false);
		return t;
	}

	static TextWithAttributes createEstimateText(std::string text,
			int height) {
		TextWithAttributes t(text, height, false, true, true);
		return t;
	}

	static TextWithAttributes createAllTricksText(int tricks,
			bool underline) {
		TextWithAttributes t(std::to_string(tricks), defaultFontHeight, underline, false,
				true);
		return t;
	}

	TextWithAttributes(std::string text, bool underlined = false);
	TextWithAttributes(const char* text);
	TextWithAttributes(std::string text, int height, bool underline, bool bold,
			bool blackColor);

	inline std::string getText() const {
		return m_text;
	}

	inline int getHeight() const {
		return m_height;
	}

	inline bool isBold() const {
		return m_bold;
	}

	inline bool isUnderline() const {
		return m_underline;
	}

	inline bool isBlackColor() const {
		return m_blackColor;
	}

	PangoFontDescription* getFontDescription();

	virtual ~TextWithAttributes();
};

#endif /* TEXTWITHATTRIBUTES_H_ */
