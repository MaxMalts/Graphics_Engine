#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Windows\Container.h"



namespace GUI {

	Container::Container(OSWindow& osWindow, const ContainerProps& props,
	                     const Vector2& pos, const Vector2& size)
		: Window(osWindow, pos, size) {}


	void Container::Draw() {
		osWindow.SetActive();

		DrawInsides();
	}
}