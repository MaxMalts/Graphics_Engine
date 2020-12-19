#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Include\Extra.h"
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Point.h"



namespace GUI {

	PointProps::PointProps(const Vector2& pos, const size_t width, const Color& color)
		: pos(pos), width(width), color(color) {}


	Point::Point(Window& window, const PointProps& props) : Primitive(window), props(props) {}


	void Point::ChangePosition(const Vector2& newPos) {
		props.pos = newPos;
	}


	void Point::Draw() {
		GlCoordinates posGl = OSWindowToGlCoords(osWindow, RelToAbsCoords(props.pos));

		osWindow.SetActive();

		glColor3f(props.color.Red(), props.color.Green(), props.color.Blue());
		glPointSize(static_cast<GLfloat>(props.width));

		glBegin(GL_POINTS);
		glVertex2f(posGl.x, posGl.y);
		glEnd();
	}
}