#define _USE_MATH_DEFINES

#include <cmath>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Include\Extra.h"
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Circle.h"



namespace GUI {

	CircleProps::CircleProps(const Vector2& centerPos, const size_t radius, const Color& color)
		: centerPos(centerPos), radius(radius), color(color) {}


	Circle::Circle(Window& window, const CircleProps& props)
		: Primitive(window), props(props) {}


	void Circle::ChangeRadius(const size_t newRadius) {
		props.radius = newRadius;
	}


    void Circle::Draw() {

		int nVerteces = 2 * M_PI * props.radius / 3;

		osWindow.SetActive();

		glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());

		glBegin(GL_POLYGON);
		for (int i = 0; i < nVerteces; ++i) {
			int curX = props.radius * cosf((2 * M_PI / nVerteces) * i) + props.centerPos.x;
			int curY = props.radius * sinf((2 * M_PI / nVerteces) * i) + props.centerPos.y;
			GlCoordinates curVertGl =
				OSWindowToGlCoords(osWindow, RelToAbsCoords(Vector2(curX, curY)));

			glVertex2f(curVertGl.x, curVertGl.y);
		}
		glEnd();
	}
}