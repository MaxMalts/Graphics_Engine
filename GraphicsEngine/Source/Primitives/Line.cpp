#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Include\Extra.h"
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Line.h"



namespace GUI {

	LineProps::LineProps(const Vector2& firstPoint, const Vector2& secondPoint, const size_t width, const Color& color)
		: firstPoint(firstPoint), secondPoint(secondPoint), width(width), color(color) {}

	
	Line::Line(OSWindow& osWindow, const LineProps& props)
		: Primitive(osWindow), props(props) {}


	void Line::Draw() {
		GlCoordinates begPosGl = OSWindowToGlCoords(osWindow, props.firstPoint);
		GlCoordinates endPosGl = OSWindowToGlCoords(osWindow, props.secondPoint);

		osWindow.SetActive();

		glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());
		glLineWidth(static_cast<GLfloat>(props.width));

		glBegin(GL_LINES);
		glVertex2f(begPosGl.x, begPosGl.y);
		glVertex2f(endPosGl.x, endPosGl.y);
		glEnd();
	}
}