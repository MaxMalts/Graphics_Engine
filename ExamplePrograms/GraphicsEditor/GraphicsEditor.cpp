#include <iostream>
#include <vector>
#include <GraphicsEngine.h>
#include "Tools\Tool.h"
#include "Tools\Pencil.h"



class GraphicsEditor {
private:

	GUI::Application application;
	GUI::OSWindow* osWindow = nullptr;

	GUI::Color curToolColor = GUI::Color("black");
	size_t curToolSize = 1;

	struct UsrInterface {

		GUI::DesktopWindow* desktop = nullptr;
		GUI::Container* canvas = nullptr;
		GUI::Container* toolbar = nullptr;
		std::vector<Tools::Tool*> tools;

	} usrInterface;


public:
	
	GraphicsEditor() {
		osWindow = application.CreateWindow(1600, 900, "Graphics Editor", GUI::Color(0.2, 0.2, 0.4));
		usrInterface.desktop = osWindow->GetDesktop();

		usrInterface.desktop->AddEventListener(GUI::Event::window_close, GUI::CloseWindow);
		InitUsrInterface();
	}


	void Start() {
		double prevTime = glfwGetTime();
		while (application.WindowsOpened() > 0) {
			usrInterface.desktop->Draw();
			application.ProcessEvents();
			double curTime = glfwGetTime();
			std::cout << 1 / (curTime - prevTime) << '\n';
			prevTime = curTime;
		}
	}

	
	~GraphicsEditor() {
		for (Tools::Tool* tool : usrInterface.tools) {
			delete tool;
		}
	}


private:

	void InitUsrInterface() {
		const GUI::Vector2 toolbarPos(30, 50);
		const GUI::Vector2 toolbarSize(70, 810);

		const GUI::Vector2 canvasPos(130, 50);
		const GUI::Vector2 canvasSize(1440, 810);

		usrInterface.toolbar = dynamic_cast<GUI::Container*>(
			usrInterface.desktop->CreateWindow(GUI::Window::container,
			                                   GUI::ContainerProps(), toolbarPos, toolbarSize)
		);
		usrInterface.toolbar->CreatePrimitive(
			GUI::Primitive::rectangle, GUI::RectangleProps(
				GUI::Vector2(0, 0), toolbarSize, GUI::Color(0.3, 0.3, 0.5)
			)
		);

		usrInterface.canvas = dynamic_cast<GUI::Container*>(
			usrInterface.desktop->CreateWindow(GUI::Window::container,
			                                   GUI::ContainerProps(), canvasPos, canvasSize)
		);
		usrInterface.canvas->CreatePrimitive(
			GUI::Primitive::rectangle, GUI::RectangleProps(GUI::Vector2(0, 0), canvasSize, GUI::Color("white"))
		);

		InitTools(usrInterface.toolbar, toolbarSize);
	}


	void InitTools(GUI::Container* toolbar, const GUI::Vector2& toolbarSize) {
		assert(toolbar != nullptr);

		const int iconSize = 50;
		const int marginLeft = (toolbarSize.x - iconSize) / 2;
		const int marginTop = 30;

		GUI::Vector2 curPos(marginLeft, marginTop);
		int nTools = 0;
		usrInterface.tools.push_back(new Tools::Pencil(Tools::ToolProps(
			toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Pencil.bmp", SelectTool, this
		)));
		++nTools;
	}


	static void SelectTool(Tools::Tool& tool, void* voidThis) {
		assert(voidThis != nullptr);

		GraphicsEditor* _this = static_cast<GraphicsEditor*>(voidThis);
		tool.SetActive(_this->usrInterface.canvas, _this->curToolSize, _this->curToolColor);
	}
};


int main() {

	GraphicsEditor editor;
	editor.Start();

	return 0;
}