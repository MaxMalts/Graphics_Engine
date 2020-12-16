#pragma once

#include <string>
#include "BMP_Img\BMP_Img.h"
#include "Include\Common.h"
#include "Primitive.h"



namespace GUI {

	struct TextProps : public PrimitiveProps {
	public:

		TextProps(const std::string& content = "text", const Vector2& pos = Vector2(0, 0),
		          const size_t fontSize = 16, const Color& color = Color(0, 0, 0));


		std::string content = nullptr;
		Vector2 pos;
		size_t fontSize;
		Color color;

	private:

		int contentLen = 0;
	};


	class Text : public Primitive {
	public:

		Text(Window& window, const TextProps& props = TextProps());

		Text(const Text& other) = delete;

		virtual void Draw();

		~Text();

		static void SetFontFile(const std::string fileName) {
			fontFileName = fileName;
		}

	private:

		void InitFont(const size_t chWidth, const size_t chHeight);

		void DestroyFont();

		void DrawChar(const char ch, const Vector2& pos);

		Vector2 BmpCharPos(const unsigned char ch) const;


		TextProps props;

		static std::string fontFileName;
		static size_t instanceCount;
		static BMP_Img* bitmapImg;
		static size_t charWidth;
		static size_t charHeight;
	};
}