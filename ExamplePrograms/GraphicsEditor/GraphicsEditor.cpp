#include <GraphicsEngine.h>



int main() {

	GUI::Application app;
	GUI::OSWindow* osWindow = app.CreateWindow(1600, 900, "Graphics Editor", GUI::Color("white"));
	GUI::DesktopWindow* desktop = osWindow->GetDesktop();


	while (true) {
		app.ProcessEvents();
	}

	return 0;
}