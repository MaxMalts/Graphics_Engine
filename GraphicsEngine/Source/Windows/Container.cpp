#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Windows\Container.h"



namespace GUI {

	Container::Container(OSWindow& osWindow, Window* parent, const ContainerProps& props,
	                     const Vector2& pos, const Vector2& size)
		: Window(osWindow, parent, pos, size) {}


	void Container::Draw() {
		osWindow.SetActive();

		DrawInsides();
	}
}