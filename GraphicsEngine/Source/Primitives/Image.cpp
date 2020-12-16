#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include "BMP_Img\BMP_Img.h"
#include "Include\Extra.h"
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Image.h"



namespace GUI {

	ImageProps::ImageProps(const std::string& fileName, const Vector2& pos)
		: fileName(fileName), pos(pos) {}


	Image::Image(Window& window, const ImageProps& props) : Primitive(window), props(props),
		bmpImg(*&std::ifstream(props.fileName, std::ios::binary)) {

		size = Vector2(bmpImg.Width(), bmpImg.Height());
	}


	void Image::Draw() {
		osWindow.SetActive();

		Vector2 absPos = RelToAbsCoords(props.pos);
		absPos.y += size.y;
		GlCoordinates glPos = OSWindowToGlCoords(osWindow, absPos);

		const char* bmpImgPtr = bmpImg.ImagePointer();

		glRasterPos2f(glPos.x, glPos.y);

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5);

		glDrawPixels(size.x, size.y, GL_BGRA, GL_UNSIGNED_BYTE, reinterpret_cast<const GLubyte*>(bmpImgPtr));

		glDisable(GL_ALPHA_TEST);
	}
}