#pragma once

#include "Include\Common.h"
#include "Window.h"



namespace GUI {
	class OSWindow;


	class DesktopWindow : public Window {
	public:

		DesktopWindow(OSWindow& osWindow, const Vector2 pos, const Vector2 size,
			const Color& color = Color(1, 1, 1));

		DesktopWindow(const DesktopWindow& other) = delete;

		virtual void Draw();

	private:

		Color color;
	};
}