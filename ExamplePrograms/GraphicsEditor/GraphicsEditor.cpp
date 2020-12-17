#include <iostream>
#include <vector>
#include <GraphicsEngine.h>
#include "Tools\Tool.h"
#include "Tools\Eraser.h"
#include "Tools\Pencil.h"
#include "Tools\Line.h"
#include "Tools\Polyline.h"
#include "Tools\Rectangle.h"
#include "Tools\Ellipse.h"


namespace GEditor {

	class GraphicsEditor {
	private:

		GUI::Application application;
		GUI::OSWindow* osWindow = nullptr;

		GUI::Color curToolColor = GUI::Color("black");
		size_t curToolSize = 1;
		Tool* curTool = nullptr;

		struct UsrInterface {

			GUI::DesktopWindow* desktop = nullptr;
			GUI::Container* canvas = nullptr;
			GUI::Container* toolbar = nullptr;
			std::vector<Tool*> tools;


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
				std::cout << static_cast<int>(1 / (curTime - prevTime)) << '\n';
				prevTime = curTime;
			}
		}


		~GraphicsEditor() {
			for (Tool* tool : usrInterface.tools) {
				delete tool;
			}
		}


	private:


		GUI::Container* DrawFilledContainer(GUI::Window* parent, const GUI::Vector2& pos,
			const GUI::Vector2& size, const GUI::Color& color) {

			GUI::Container* res = dynamic_cast<GUI::Container*>(
				parent->CreateWindow(GUI::Window::container, GUI::ContainerProps(), pos, size)
				);

			res->CreatePrimitive(GUI::Primitive::rectangle, GUI::RectangleProps(GUI::Vector2(0, 0), size, color));

			return res;
		}


		void InitUsrInterface() {
			const GUI::Vector2 toolbarPos(30, 50);
			const GUI::Vector2 toolbarSize(70, 810);

			const GUI::Vector2 canvasPos(130, 50);
			const GUI::Vector2 canvasSize(1440, 810);

			usrInterface.canvas = DrawFilledContainer(usrInterface.desktop, canvasPos, canvasSize, GUI::Color("white"));

			DrawEnvironment(canvasPos, canvasSize);

			usrInterface.toolbar = DrawFilledContainer(usrInterface.desktop, toolbarPos, toolbarSize, GUI::Color(0.3, 0.3, 0.5));

			InitTools(usrInterface.toolbar, toolbarSize);
		}


		void DrawEnvironment(const GUI::Vector2& canvasPos, const GUI::Vector2& canvasSize) {
			const GUI::Color envColor(0.2, 0.2, 0.4);

			const size_t desktopWidth = usrInterface.desktop->Width();
			const size_t desktopHeight = usrInterface.desktop->Height();

			// top
			GUI::Vector2 curPos(0, 0);
			GUI::Vector2 curSize(desktopWidth, canvasPos.y);
			DrawFilledContainer(usrInterface.desktop, curPos, curSize, envColor);

			// left
			curSize.x = canvasPos.x;
			curSize.y = desktopHeight;
			DrawFilledContainer(usrInterface.desktop, curPos, curSize, envColor);

			// right
			curPos.x = canvasPos.x + canvasSize.x;
			curSize.x = desktopWidth - curPos.x;
			DrawFilledContainer(usrInterface.desktop, curPos, curSize, envColor);

			// bottom
			curPos.x = 0;
			curPos.y = canvasPos.y + canvasSize.y;
			curSize.x = desktopWidth;
			curSize.y = desktopHeight - curPos.y;
			DrawFilledContainer(usrInterface.desktop, curPos, curSize, envColor);
		}


		void InitTools(GUI::Container* toolbar, const GUI::Vector2& toolbarSize) {
			assert(toolbar != nullptr);

			const int iconSize = 50;
			const int marginLeft = (toolbarSize.x - iconSize) / 2;
			const int marginTop = 30;

			GUI::Vector2 curPos(marginLeft, marginTop);

			usrInterface.tools.push_back(new Eraser(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Eraser.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.tools.push_back(new Pencil(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Pencil.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.tools.push_back(new Line(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Line.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.tools.push_back(new Polyline(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Polyline.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.tools.push_back(new Rectangle(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Rectangle.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.tools.push_back(new Ellipse(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Ellipse.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;
		}


		static void SelectTool(Tool* tool, void* voidThis) {
			assert(voidThis != nullptr);

			GraphicsEditor* _this = static_cast<GraphicsEditor*>(voidThis);

			if (_this->curTool != nullptr) {
				_this->curTool->SetInactive();
			}
			tool->SetActive(_this->usrInterface.canvas, _this->curToolSize, _this->curToolColor);
			_this->curTool = tool;
		}
	};
}


int main() {

	GEditor::GraphicsEditor editor;
	editor.Start();

	return 0;
}