#include <vector>
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

			glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());
			glLineWidth(static_cast<GLfloat>(props.width));

			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < props.verteces.size(); ++i) {
				GlCoordinates curGlCoords = OSWindowToGlCoords(osWindow, RelToAbsCoords(props.verteces[i]));
				glVertex2f(curGlCoords.x, curGlCoords.y);
			}
			glEnd();
		}
	}


	void Polyline::AddVertex(const Vector2& pos) {
		props.verteces.push_back(pos);
	}
}