#include <string>
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Rectangle.h"
#include "Include\Primitives\Text.h"
#include "Include\Windows\Button.h"



namespace GUI {

	ButtonProps::ButtonProps(const Color& color) : color(color) {}


	Button::Button(OSWindow& osWindow, const ButtonProps& props,
	               const Vector2& pos, const Vector2& size)
		: Window(osWindow, pos, size), props(props) {
		
		rectangle = new Rectangle(*this, RectangleProps(Vector2(0, 0), size, props.color));
	}


	Text* Button::AddLabel(const std::string label, const Vector2& labelPos,
	                       const size_t fontSize, const Color& labelColor) {

		this->label = new Text(*this, TextProps(label, labelPos, fontSize, labelColor));

		return this->label;
	}


	Color Button::GetColor() const {
		return props.color;
	}


	//void Button::AddLeftMouseUpListener(void(*Listener)(void*), void* addParam) {
	//	leftMouseUpListeners.push_back(std::pair<void (*)(void*), void*>(Listener, addParam));
	//}


	void Button::Draw() {
		osWindow.SetActive();

		rectangle->Draw();
		if (label != nullptr) {
			label->Draw();
		}

		DrawInsides();
	}


	Button::~Button() {
		delete rectangle;
		delete label;
	}
}