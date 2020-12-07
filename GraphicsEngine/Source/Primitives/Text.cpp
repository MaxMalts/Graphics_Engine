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

	TextProps::TextProps(const std::string& content, const Vector2& pos, const size_t fontSize, const Color& color)
		: content(content), pos(pos), fontSize(fontSize), color(color) {}

	std::string Text::fontFileName = "Fonts/ASCII.bmp";
	size_t Text::instanceCount = 0;
	BMP_Img* Text::bitmapImg = nullptr;
	size_t Text::charWidth = 0;
	size_t Text::charHeight = 0;

	Text::Text(OSWindow& osWindow, const TextProps& props) : Primitive(osWindow), props(props) {

		if (0 == instanceCount) {
			const size_t charWidth = 16;
			const size_t charHeight = 16;

			InitFont(charWidth, charHeight);
		}

		++instanceCount;
	}


	void Text::DrawChar(const char ch, const Vector2& pos) {
		//GlCoordinates glPos = WindowToGlCoords(window, pos);
		//glRasterPos2f(glPos.x, glPos.y);

		Vector2 bmpCharPos = BmpCharPos(ch);

		const char* bmpCharPtr = bitmapImg->ImagePointer() + bmpCharPos.y * bitmapImg->Width() + bmpCharPos.x;
		/*glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmapImg->Width());

		glDrawPixels(charWidth, charHeight, GL_ALPHA, GL_UNSIGNED_BYTE, reinterpret_cast<const GLubyte*>(charPtr));

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);*/

		float scale = static_cast<float>(props.fontSize) / charWidth;

		glPointSize(1.0);
		glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());

		glBegin(GL_POINTS);
		for (int curY = 0; curY < charHeight * scale; ++curY) {
			for (int curX = 0; curX < props.fontSize; ++curX) {

				int pxOffsetX = Round(curX / scale);
				int pxOffsetY = Round(curY / scale);
				if (pxOffsetX > charWidth - 1)
					pxOffsetX = charWidth - 1;
				if (pxOffsetY > charHeight - 1)
					pxOffsetY = charHeight - 1;

				char curPx = *(bmpCharPtr + pxOffsetY * bitmapImg->Width() + pxOffsetX);

				if (curPx != 0) {
					GlCoordinates curPos =
						OSWindowToGlCoords(osWindow, Vector2(pos.x + curX,
						                                     pos.y + charHeight * scale - 1 - curY));

					glVertex2f(curPos.x, curPos.y);
				}

			}
		}
		glEnd();
	}


	void Text::Draw() {
		osWindow.SetActive();

		Vector2 curPos(props.pos.x, props.pos.y);
		for (int i = 0; i < props.content.size(); ++i) {
			char curChar = props.content[i];

			DrawChar(curChar, curPos);
			curPos.x += props.fontSize;
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
			throw bad_init("Error while initializing font");
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