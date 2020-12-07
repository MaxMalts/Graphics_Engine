#include <iostream>
#include <vector>
#include <GraphicsEngine.h>



void CloseWindow(GUI::Event& closeEvent, void* voidWindow) {
	GUI::OSWindow* window = static_cast<GUI::OSWindow*>(voidWindow);
	window->GetApplication().CloseWindow(window);
	closeEvent.Stop();
}


void MousedownListener(GUI::Event& event, void*) {
	std::cout << "clicked\n";
}


void HoverListener(GUI::Event& event, void*) {
	if (event.mouseHoverProps.hoverType == event.mouseHoverProps.hovered) {
		std::cout << "hovered\n";
	} else {
		assert(event.mouseHoverProps.hoverType == event.mouseHoverProps.unhovered);
		std::cout << "unhovered\n";
	}
}


void MoveListener(GUI::Event& event, void*) {
	std::cout << "moved " << event.mouseMoveProps.pos.x << ' ' << event.mouseMoveProps.pos.y << '\n';
}


int main() {

	GUI::Application app;
	GUI::OSWindow* window = app.CreateWindow(1080, 720, "Hello", GUI::Color("green"));
	GUI::DesktopWindow* desktop = window->GetDesktop();

	GUI::Line* line = dynamic_cast<GUI::Line*>(desktop->CreatePrimitive(GUI::Primitive::Type::line,
		GUI::LineProps(GUI::Vector2(500, 500), GUI::Vector2(1000, 100), 5, GUI::Color("black"))));

	GUI::Text* text = dynamic_cast<GUI::Text*>(desktop->CreatePrimitive(GUI::Primitive::Type::text,
		GUI::TextProps("Test text", GUI::Vector2(50, 100), 32, GUI::Color("green"))));

	GUI::Button* button = dynamic_cast<GUI::Button*>(desktop->CreateWindow(GUI::Window::Type::button,
		GUI::ButtonProps(), GUI::Vector2(50, 50)));
	button->AddLabel("test", GUI::Vector2(10, 10), 30, GUI::Color("red"));
	button->AddEventListener(GUI::Event::mouse_down, MousedownListener);
	button->AddEventListener(GUI::Event::mouse_hover, HoverListener);

	GUI::GraphProps props(-100, 100, -50, 100);
	props.axesWidth = 3;
	GUI::Graph* graph = dynamic_cast<GUI::Graph*>(desktop->CreateWindow(GUI::Window::Type::graph, props,
		GUI::Vector2(10, 100), GUI::Vector2(500, 500)));
	GUI::Graph::Diagram* diag = graph->CreateDiagram(3, GUI::Color("red"));
	diag->AddData(18 - 100, 22 - 50);
	diag->AddData(0 - 100, 0 - 50);

	graph->AddEventListener(GUI::Event::mouse_move, MoveListener);

	GUI::Graph::Diagram* diag1 = graph->CreateDiagram(3, GUI::Color("green"));
	diag1->AddData(40 - 100, 69 - 50);
	diag1->AddData(1 - 100, 1 - 50);
	diag->AddData(100 - 100, 100 - 50);

	desktop->AddEventListener(GUI::Event::window_close, CloseWindow, window);
	//button->AddLeftMouseUpListener(TestFunc, window);

	double prevTime = glfwGetTime();
	while (app.WindowsOpened() > 0) {
		desktop->Draw();
		app.ProcessEvents();
		double curTime = glfwGetTime();
		//std::cout << 1 / (curTime - prevTime) << '\n';
		prevTime = curTime;
	}

	return 0;
}