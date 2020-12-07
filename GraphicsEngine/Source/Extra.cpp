#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Include\Application.h"
#include "Include\Extra.h"



namespace GUI {

	GlCoordinates OSWindowToGlCoords(const OSWindow& window, const Vector2& coords) {
		float resX = static_cast<float>(coords.x) / window.Width() * 2 - 1;
		float resY = -(static_cast<float>(coords.y) / window.Height() * 2 - 1);

		return { resX, resY };
	}


	int Round(double val) {
		int floor = static_cast<int>(val);
		return (val - floor < 0.5) ? floor : floor + 1;
	}
}