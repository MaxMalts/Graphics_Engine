#define _USE_MATH_DEFINES

#include <cmath>
#include <algorithm>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Include\Extra.h"
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Ellipse.h"



namespace GUI {

	EllipseProps::EllipseProps(const Vector2& pos, const Vector2& size, const Color& color)
		: pos(pos), size(size), color(color) {}


	Ellipse::Ellipse(Window& window, const EllipseProps& props)
		: Primitive(window), props(props) {}


	void Ellipse::ChangeSize(const Vector2& newSize) {
		props.size = newSize;
	}


    void Ellipse::Draw() {

		int coeff = M_PI * std::max(abs(props.size.x), abs(props.size.y));
		int nVerteces = 5 * sqrt(coeff) + 10;

		osWindow.SetActive();

		glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());

		glBegin(GL_POLYGON);
		for (int i = 0; i < nVerteces; ++i) {
			int curX = props.size.x / 2 * cosf((2 * M_PI / nVerteces) * i) + (props.pos.x + props.size.x / 2);
			int curY = props.size.y / 2 * sinf((2 * M_PI / nVerteces) * i) + (props.pos.y + props.size.y / 2);
			GlCoordinates curVertGl =
				OSWindowToGlCoords(osWindow, RelToAbsCoords(Vector2(curX, curY)));

			glVertex2f(curVertGl.x, curVertGl.y);
		}
		glEnd();
	}
}