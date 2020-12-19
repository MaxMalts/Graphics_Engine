#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Windows\Desktop.h"



namespace GUI {

	void OSWindowCloseListener(GUI::Event& closeEvent, void*) {
		GUI::OSWindow& window = closeEvent.GetTarget()->GetOSWindow();
		window.GetApplication().CloseWindow(window);
	}


	DesktopWindow::DesktopWindow(OSWindow& osWindow, const Vector2& pos, const Vector2& size, const Color& color)
		: Window(osWindow, nullptr, pos, size), color(color) {}


	void DesktopWindow::Draw() {
		osWindow.SetActive();

		glClearColor(color.Red(), color.Green(), color.Blue(), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		DrawInsides();
	}
}