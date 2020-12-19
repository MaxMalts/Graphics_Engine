#include <string>
#include <fstream>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "BMP_Img\BMP_Img.h"
#include "Include\Extra.h"
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Text.h"



namespace GUI {

	FontProps::FontProps(const int fontSize, const Color& color)
		: fontSize(fontSize), color(color) {}


	TextProps::TextProps(const std::string& content, const Vector2& pos, const FontProps& fontProps)
		: content(content), pos(pos), fontProps(fontProps) {}  // No problems with order due to declaration order

	std::string Text::fontFileName = "Fonts/ASCII.bmp";
	size_t Text::instanceCount = 0;
	BMP_Img* Text::bitmapImg = nullptr;
	size_t Text::charWidth = 0;
	size_t Text::charHeight = 0;

	Text::Text(Window& window, const TextProps& props)
		: Primitive(window), props(props), fontProps(this->props.fontProps) {

		if (0 == instanceCount) {
			const size_t charWidth = 16;
			const size_t charHeight = 16;

			InitFont(charWidth, charHeight);
		}

		++instanceCount;
	}


	void Text::DrawChar(const char ch, const Vector2& absPos) {
		//GlCoordinates glPos = WindowToGlCoords(window, absPos);
		//glRasterPos2f(glPos.x, glPos.y);

		Vector2 bmpCharPos = BmpCharPos(ch);

		const char* bmpCharPtr = bitmapImg->ImagePointer() + bmpCharPos.y * bitmapImg->Width() + bmpCharPos.x;
		/*glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmapImg->Width());

		glDrawPixels(charWidth, charHeight, GL_ALPHA, GL_UNSIGNED_BYTE, reinterpret_cast<const GLubyte*>(charPtr));

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);*/

		float scale = static_cast<float>(fontProps.fontSize) / charWidth;

		glPointSize(1.0);
		glColor3f(fontProps.color.Red(), fontProps.color.Green(), fontProps.color.Blue());

		glBegin(GL_POINTS);
		for (int curY = 0; curY < charHeight * scale; ++curY) {
			for (int curX = 0; curX < fontProps.fontSize; ++curX) {

				int pxOffsetX = Round(curX / scale);
				int pxOffsetY = Round(curY / scale);
				if (pxOffsetX > charWidth - 1)
					pxOffsetX = charWidth - 1;
				if (pxOffsetY > charHeight - 1)
					pxOffsetY = charHeight - 1;

				char curPx = *(bmpCharPtr + pxOffsetY * bitmapImg->Width() + pxOffsetX);

				if (curPx != 0) {
					GlCoordinates curPos =
						OSWindowToGlCoords(osWindow, Vector2(absPos.x + curX,
						                                     absPos.y + charHeight * scale - 1 - curY));

					glVertex2f(curPos.x, curPos.y);
				}

			}
		}
		glEnd();
	}


	Vector2 Text::GetSize() const {
		return Vector2(props.content.size() * fontProps.fontSize, fontProps.fontSize);
	}


	void Text::ChangePosition(const Vector2& newPos) {
		props.pos = newPos;
	}


	void Text::Draw() {
		osWindow.SetActive();

		Vector2 curPos(RelToAbsCoords(props.pos));
		for (int i = 0; i < props.content.size(); ++i) {
			char curChar = props.content[i];

			DrawChar(curChar, curPos);
			curPos.x += fontProps.fontSize;
		}
	}


	Vector2 Text::BmpCharPos(const unsigned char ch) const {
		size_t bmpWidth = bitmapImg->Width();
		size_t bmpHeight = bitmapImg->Height();

		size_t nColumns = bmpWidth / charWidth;
		size_t nRows = bmpHeight / charHeight;

		size_t chColumn = ch % nColumns + 1;
		size_t chRow = ch / nColumns + 1;

		Vector2 res{ 0, 0 };
		res.x = (chColumn - 1) * charWidth;
		res.y = bmpHeight - chRow * charHeight;

		return res;
	}


	Text::~Text() {
		assert(instanceCount >= 1);

		if (instanceCount == 1)
			DestroyFont();

		--instanceCount;
	}


	void Text::InitFont(const size_t chWidth, const size_t chHeight) {
		assert(chWidth > 0);
		assert(chHeight > 0);

		assert(nullptr == bitmapImg);

		std::ifstream bitmapFile(fontFileName, std::ios::binary);
		if (!bitmapFile.is_open()) {
			throw bad_init("Unable to open font file.");
		}

		bitmapImg = new BMP_Img(bitmapFile);

		bitmapFile.close();

		charWidth = chWidth;
		charHeight = chHeight;
	}


	void Text::DestroyFont() {
		assert(bitmapImg != nullptr);

		delete bitmapImg;
		charWidth = 0;
		charHeight = 0;
	}
}