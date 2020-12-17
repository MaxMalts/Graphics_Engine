#include <vector>
#include <algorithm>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Include\Extra.h"
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Polyline.h"



namespace GUI {

	PolylineProps::PolylineProps(const std::vector<Vector2>& verteces, const size_t width, const Color& color)
		: verteces(verteces), width(width), color(color) {}


	PolylineProps::PolylineProps(const size_t width, const Color& color) : width(width), color(color) {}


	Polyline::Polyline(Window& window, const PolylineProps props)
		: Primitive(window), props(props) {}


	void Polyline::Draw() {
		if (props.verteces.size() >= 2) {
			osWindow.SetActive();

			//GLfloat lineRange[2] = { 0, 0 };
			//glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, lineRange);
			//GLfloat pointRange[2] = { 0, 0 };
			//glGetFloatv(GL_POINT_SIZE_RANGE, pointRange);

			//int actWidth = std::min({ props.width, static_cast<size_t>(lineRange[1]),
			//						static_cast<size_t>(pointRange[1]) });
			//printf("%f %f %d\n", lineRange[0], lineRange[1], props.width);

			glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());
			glLineWidth(static_cast<GLfloat>(props.width));
			glPointSize(static_cast<GLfloat>(props.width));

			GlCoordinates prevGlCoords = OSWindowToGlCoords(osWindow, RelToAbsCoords(props.verteces[0]));
			glBegin(GL_POINTS);
			glVertex2f(prevGlCoords.x, prevGlCoords.y);
			glEnd();
			for (int i = 1; i < props.verteces.size(); ++i) {
				GlCoordinates curGlCoords = OSWindowToGlCoords(osWindow, RelToAbsCoords(props.verteces[i]));

				glBegin(GL_LINES);
				glVertex2f(prevGlCoords.x, prevGlCoords.y);
				glVertex2f(curGlCoords.x, curGlCoords.y);
				glEnd();

				glBegin(GL_POINTS);
				glVertex2f(curGlCoords.x, curGlCoords.y);
				glEnd();

				prevGlCoords = curGlCoords;
			}
		}
	}


	void Polyline::AddVertex(const Vector2& pos) {
		props.verteces.push_back(pos);
	}
}