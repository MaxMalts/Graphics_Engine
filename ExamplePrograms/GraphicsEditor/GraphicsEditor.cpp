#include <iostream>
#include <vector>
#include <GraphicsEngine.h>
#include "SizeController\SizeController.h"
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

		GUI::Color activeColor = GUI::Color(GUI::Color::black);
		Tool* activeTool = nullptr;

		struct UsrInterface {

			GUI::DesktopWindow* desktop = nullptr;
			GUI::Container* canvas = nullptr;

			struct {
				GUI::Container* container = nullptr;
				std::vector<Tool*> tools;
			} toolbar;

			struct {
				GUI::Container* container = nullptr;
				SizeController* controller = nullptr;
			} sizeController;

			struct {
				GUI::OSWindow* osWindow = nullptr;
				GUI::Button* button = nullptr;
			} colorPicker;

		} usrInterface;


	public:

		GraphicsEditor() {
			GUI::Text::SetFontFile("../Fonts/Ascii.bmp");

			osWindow = application.CreateWindow(1600, 900, "Graphics Editor", GUI::Color(0.2, 0.2, 0.4));
			usrInterface.desktop = osWindow->GetDesktop();

			usrInterface.desktop->AddEventListener(GUI::Event::window_close, GUI::OSWindowCloseListener);
			InitUsrInterface();
		}


		void Start() {
			double prevTime = glfwGetTime();
			while (application.WindowsOpened() > 0) {
				application.UpdateAllWindows();
				application.ProcessEvents();
				double curTime = glfwGetTime();
				std::cout << static_cast<int>(1 / (curTime - prevTime)) << '\n';
				prevTime = curTime;
			}
		}


		~GraphicsEditor() {
			for (Tool* tool : usrInterface.toolbar.tools) {
				delete tool;
			}
			delete usrInterface.sizeController.controller;
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
			const GUI::Vector2 toolbarPos(30, 30);
			const GUI::Vector2 toolbarSize(70, 700);
			const GUI::Color toolbarColor(0.3, 0.3, 0.5);

			const GUI::Vector2 canvasPos(130, 30);
			const GUI::Vector2 canvasSize(1440, 810);

			usrInterface.canvas = DrawFilledContainer(usrInterface.desktop, canvasPos, canvasSize,
			                                          GUI::Color(GUI::Color::white));

			DrawEnvironment(canvasPos, canvasSize);

			usrInterface.toolbar.container = DrawFilledContainer(usrInterface.desktop, toolbarPos,
			                                                     toolbarSize, toolbarColor);

			InitTools(usrInterface.toolbar.container, toolbarSize);

			InitSizeController();

			InitColorPicker();
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

			usrInterface.toolbar.tools.push_back(new Eraser(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Eraser.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.toolbar.tools.push_back(new Pencil(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Pencil.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.toolbar.tools.push_back(new Line(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Line.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.toolbar.tools.push_back(new Polyline(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Polyline.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.toolbar.tools.push_back(new Rectangle(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Rectangle.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;

			usrInterface.toolbar.tools.push_back(new Ellipse(ToolProps(
				toolbar, curPos, GUI::Vector2(iconSize, iconSize), "ToolsIcons/Ellipse.bmp", SelectTool, this
			)));
			curPos.y += iconSize + marginTop;
		}


		static void SelectTool(Tool* tool, void* voidThis) {
			assert(voidThis != nullptr);

			GraphicsEditor* _this = static_cast<GraphicsEditor*>(voidThis);

			if (_this->activeTool != nullptr) {
				_this->activeTool->SetInactive();
			}
			tool->SetActive(_this->usrInterface.canvas,
			                _this->usrInterface.sizeController.controller->GetSize(),
			                _this->activeColor);

			_this->activeTool = tool;
		}


		void InitSizeController() {
			const int marginBottom = 20;
			const int marginLeft = 25;
			const int containerSize = 80;
			const GUI::Color bgdColor(0.7, 0.7, 0.9);
			const int maxSize = 63;
			const GUI::Color previewColor(GUI::Color::black);

			const GUI::Vector2 pos(marginLeft, usrInterface.desktop->Height() - marginBottom - containerSize);

			GUI::Container* container = 
				DrawFilledContainer(usrInterface.desktop, pos, GUI::Vector2(containerSize, containerSize), bgdColor);
			usrInterface.sizeController.container = container;

			GUI::Vector2 innerPos((containerSize - maxSize) / 2, (containerSize - maxSize) / 2);
			usrInterface.sizeController.controller = new SizeController(
				SizeControllerProps(container, innerPos, maxSize, 10, previewColor, UpdateSize, this)
			);

		}


		static void UpdateSize(int newSize, void* voidThis) {
			assert(voidThis != nullptr);

			GraphicsEditor* _this = static_cast<GraphicsEditor*>(voidThis);

			if (_this->activeTool != nullptr) {
				_this->activeTool->ChangeSize(newSize);
			}
		}


		void InitColorPicker() {  // canvas and SizeController should be initialised
			const GUI::Vector2 size(75, 45);
			const int posX = (usrInterface.canvas->Position().x - size.x) / 2;
			const int marginBottom = 12;

			int sizeCtrlrPosY = usrInterface.sizeController.container->Position().y;
			GUI::Button* button = dynamic_cast<GUI::Button*> (
				usrInterface.desktop->CreateWindow(
					GUI::Window::button,
					GUI::ButtonProps(activeColor),
					GUI::Vector2(posX, sizeCtrlrPosY - marginBottom - size.y),
					size
				)
			);
			usrInterface.colorPicker.button = button;

			button->AddEventListener(GUI::Event::mouse_up, ColorButtonListener, this);
		}


		void OpenColorPicker() {
			assert(usrInterface.colorPicker.osWindow == nullptr);

			const GUI::Vector2 size(500, 300);
			const GUI::Color bgColor(0.2, 0.2, 0.4);
			const GUI::Color appplyColor(0.9, 0.9, 0.9);

			GUI::OSWindow* pickerWindow = application.CreateWindow(size, "Color Picker", bgColor);
			usrInterface.colorPicker.osWindow = pickerWindow;

			GUI::DesktopWindow* desktop = pickerWindow->GetDesktop();

			GUI::ColorPicker* picker = dynamic_cast<GUI::ColorPicker*>(
				desktop->CreateWindow(
					GUI::Window::color_picker,
					GUI::ColorPickerProps(bgColor, appplyColor),
					GUI::Vector2(0, 0), size)
			);

			desktop->AddEventListener(GUI::Event::window_close, ColorPkrClose, this);
			picker->AddEventListener(GUI::Event::color_pick, ColorPickListener, this);
			desktop->AddEventListener(GUI::Event::color_pick, ColorPkrClose, this);
		}


		static void ColorButtonListener(GUI::Event& event, void* voidThis) {
			assert(voidThis != nullptr);

			GraphicsEditor* _this = static_cast<GraphicsEditor*>(voidThis);

			if (_this->usrInterface.colorPicker.osWindow != nullptr) {
				return;
			}
			
			_this->OpenColorPicker();
		}


		static void ColorPickListener(GUI::Event& event, void* voidThis) {
			assert(voidThis != nullptr);

			GraphicsEditor* _this = static_cast<GraphicsEditor*>(voidThis);
			_this->activeColor = event.colorPickProps.color;
			_this->activeTool->ChangeColor(_this->activeColor);
		}


		static void ColorPkrClose(GUI::Event& event, void* voidThis) {
			assert(voidThis != nullptr);

			GraphicsEditor* _this = static_cast<GraphicsEditor*>(voidThis);;
			_this->usrInterface.colorPicker.osWindow = nullptr;
			GUI::OSWindowCloseListener(event, nullptr);
		}
	};
}


int main() {

	GEditor::GraphicsEditor editor;
	editor.Start();

	return 0;
}