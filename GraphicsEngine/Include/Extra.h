#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Include\Application.h"



namespace GUI {

	template<typename Container>
	void DeleteConatinerElements(Container container) {
		for (auto curElem : container) {
			delete curElem;
		}
	}


	constexpr int Log10Compile(const int n) {
		return ((n < 10) ? 1 : 1 + Log10Compile(n / 10));
	}


	struct GlCoordinates {
		GLfloat x = 0;
		GLfloat y = 0;
	};

	GlCoordinates OSWindowToGlCoords(const OSWindow& window, const Vector2& coords);

	int Round(double val);
}