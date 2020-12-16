#pragma once

#include <string>
#include "BMP_Img\BMP_Img.h"
#include "Include\Common.h"
#include "Primitive.h"



namespace GUI {

	struct ImageProps : public PrimitiveProps {
	public:

		ImageProps(const std::string& fileName, const Vector2& pos = Vector2(0, 0));

		std::string fileName;
		Vector2 pos;
	};


	class Image : public Primitive {
	public:

		Image(Window& window, const ImageProps& props);

		Image(const Image& other) = delete;

		virtual void Draw();

	private:

		ImageProps props;

		BMP_Img bmpImg;
		Vector2 size;
	};
}