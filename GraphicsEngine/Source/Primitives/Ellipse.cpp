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


	Vector2 Ellipse::GetPosition() const {
		return props.pos;
	}


	Vector2 Ellipse::GetSize() const {
		return props.size;
	}


	void Ellipse::ChangePosition(const Vector2& newPos) {
		props.pos = newPos;
	}


	void Ellipse::ChangeSize(const Vector2& newSize) {
		props.size = newSize;
	}


    void Ellipse::Draw() {

		int maxAxe = std::max(std::abs(props.size.x), std::abs(props.size.y));
		int nVerteces = 10 * std::sqrt(maxAxe) + 10;

		osWindow.SetActive();

		glColor3f(props.color.Red(), props.color.Green(), props.color.Blue());

		if (maxAxe > 0) {
			Vector2 center(props.pos.x + props.size.x / 2, props.pos.y + props.size.y / 2);
			GlCoordinates centerGl =
				OSWindowToGlCoords(osWindow, RelToAbsCoords(center));

			glPointSize(1.0);
			glBegin(GL_POINTS);
			glVertex2f(centerGl.x, centerGl.y);  // To draw at least one point if size != 0
			glEnd();
		}

		glBegin(GL_POLYGON);
		for (int i = 0; i < nVerteces; ++i) {
			float curX = props.size.x / 2 * std::cosf((2 * M_PI / nVerteces) * i) + (props.pos.x + props.size.x / 2);
			float curY = props.size.y / 2 * std::sinf((2 * M_PI / nVerteces) * i) + (props.pos.y + props.size.y / 2);
			GlCoordinates curVertGl =
				OSWindowToGlCoords(osWindow, RelToAbsCoords(Vector2(Round(curX), Round(curY))));

			glVertex2f(curVertGl.x, curVertGl.y);
		}
		glEnd();
	}
}