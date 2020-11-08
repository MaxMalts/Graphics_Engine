#include <iostream>
#include <vector>
#include <GraphicsEngine.h>



void CloseWindow(void* voidWindow) {
	GUI::OSWindow* window = static_cast<GUI::OSWindow*>(voidWindow);
	window->GetApplication().CloseWindow(window);
}


int main() {

	GUI::Application app;
	GUI::OSWindow* window = app.CreateWindow(1080, 720, "Hello", GUI::Color("green"));

	GUI::Line* line = window->CreateLine(GUI::Vector2(10, 10), GUI::Vector2(500, 500), 5, GUI::Color("red"));
	line->Draw();

	window->CreateText("Test text", GUI::Vector2(50, 100), 32, GUI::Color("green"));

	GUI::Button* button = window->CreateButton(GUI::Vector2(50, 50));
	button->AddLabel("test", GUI::Vector2(10, 10), 30, GUI::Color("red"));

	GUI::GraphProps props(-100, 100, -50, 100);
	props.axesWidth = 3;
	GUI::Graph* graph = window->CreateGraph(props, GUI::Vector2(10, 100), GUI::Vector2(500, 500));
	int diag = graph->CreateDiagram(3, GUI::Color("red"));
	graph->AddData(diag, 18 - 100, 22 - 50);
	graph->AddData(diag, 0 - 100, 0 - 50);

	int diag1 = graph->CreateDiagram(3, GUI::Color("green"));
	graph->AddData(diag1, 40 - 100, 69 - 50);
	graph->AddData(diag1, 1 - 100, 1 - 50);
	graph->AddData(diag, 100 - 100, 100 - 50);

	window->Update();

	window->AddWindowCloseListener(CloseWindow, window);
	//button->AddLeftMouseUpListener(TestFunc, window);

	while (app.WindowsOpened() > 0) {
		app.ProcessEvents();
	}

	GUI::Graph::Diagram test;
	test.AddVertex();

	return 0;
}