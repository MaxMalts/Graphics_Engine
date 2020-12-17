#pragma once

#include <string>
#include <GraphicsEngine.h>


namespace GEditor {

	class GraphicsEditor;
	class Tool;

	struct ToolProps {

		ToolProps(GUI::Container* iconContainer, const GUI::Vector2& iconPos, const GUI::Vector2& iconSize,
		          const std::string& iconFileName, void (*OnIconClick)(Tool*, void*), void* callbackParam)
			: iconContainer(iconContainer), iconPos(iconPos), iconSize(iconSize),
			  iconFileName(iconFileName), OnIconClick(OnIconClick), callbackParam(callbackParam) {}


		GUI::Container* iconContainer;
		GUI::Vector2 iconPos;
		GUI::Vector2 iconSize;
		std::string iconFileName;
		void (*OnIconClick)(Tool*, void*) = nullptr;
		void* callbackParam = nullptr;
	};


	class Tool {
	public:

		Tool(const ToolProps& props) : props(props) {
			assert(!props.iconFileName.empty());

			iconBtn = dynamic_cast<GUI::Button*>(
				props.iconContainer->CreateWindow(GUI::Window::button,
				                                  GUI::ButtonProps(), props.iconPos, props.iconSize)
			);

			iconBtn->CreatePrimitive(GUI::Primitive::image, GUI::ImageProps(props.iconFileName, GUI::Vector2(0, 0)));

			iconBtn->AddEventListener(GUI::Event::mouse_up, ToolClickListener, this);
		}


		void SetActive(GUI::Container* canvas, size_t size, GUI::Color color) {
			assert(canvas != nullptr);

			activeCanvas = canvas;
			activeSize = size;
			activeColor = color;

			activeCanvas->AddEventListener(GUI::Event::mouse_move, MouseMoveListener, this);
			activeCanvas->AddEventListener(GUI::Event::mouse_down, MouseDownListener, this);
			activeCanvas->AddEventListener(GUI::Event::mouse_up, MouseUpListener, this);

			ToolActivated();
		}


		void SetInactive() {
			activeCanvas->RemoveEventListener(GUI::Event::mouse_move, MouseMoveListener);
			activeCanvas->RemoveEventListener(GUI::Event::mouse_down, MouseDownListener);
			activeCanvas->RemoveEventListener(GUI::Event::mouse_up, MouseUpListener);

			activeCanvas = nullptr;

			ToolDeactivated();
		}


		void Remove() {
			props.iconContainer->RemoveWindow(iconBtn);
		}


		virtual ~Tool() = default;


	protected:

		GUI::Container* activeCanvas = nullptr;
		size_t activeSize = 0;
		GUI::Color activeColor;


	private:

		static void ToolClickListener(GUI::Event& event, void* voidThis) {
			assert(voidThis != nullptr);

			Tool* _this = static_cast<Tool*>(voidThis);
			_this->props.OnIconClick(_this, _this->props.callbackParam);
		}


		virtual void ToolActivated() {};

		virtual void ToolMove(const GUI::Vector2& mousePos) {};

		virtual void ToolDown(const GUI::Vector2& mousePos) {};

		virtual void ToolUp(const GUI::Vector2& mousePos) {};

		virtual void ToolDeactivated() {};
		
		
		static void MouseMoveListener(GUI::Event& event, void* voidThis) {
			assert(voidThis != nullptr);

			Tool* _this = static_cast<Tool*>(voidThis);
			_this->ToolMove(event.mouseMoveProps.pos);
		}


		static void MouseDownListener(GUI::Event& event, void* voidThis) {
			assert(voidThis != nullptr);

			if (event.mouseButtonProps.button == GUI::MouseButtonProps::left) {
				Tool* _this = static_cast<Tool*>(voidThis);
				_this->ToolDown(event.mouseButtonProps.pos);
			}
		}


		static void MouseUpListener(GUI::Event& event, void* voidThis) {
			assert(voidThis != nullptr);

			if (event.mouseButtonProps.button == GUI::MouseButtonProps::left) {
				Tool* _this = static_cast<Tool*>(voidThis);
				_this->ToolUp(event.mouseButtonProps.pos);
			}
		}


		ToolProps props;

		GUI::Button* iconBtn = nullptr;
		GUI::Image* iconImg = nullptr;
	};
}