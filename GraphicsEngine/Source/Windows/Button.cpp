#include <string>
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Rectangle.h"
#include "Include\Primitives\Text.h"
#include "Include\Windows\Button.h"



namespace GUI {

	ButtonProps::ButtonProps(const Color& color) : color(color) {}


	Button::Button(OSWindow& osWindow, Window* parent, const ButtonProps& props,
	               const Vector2& pos, const Vector2& size)
		: Window(osWindow, parent, pos, size), props(props) {
		
		rectangle = dynamic_cast<Rectangle*>(
			CreatePrimitive(Primitive::rectangle,RectangleProps(Vector2(0, 0), size, props.color))
		);
	}


	Text* Button::AddLabel(const std::string& label, const Vector2& labelPos,
	                       const FontProps& fontProps) {

		this->label = dynamic_cast<Text*>(
			CreatePrimitive(Primitive::text, TextProps(label, labelPos, fontProps))
		);

		return this->label;
	}


	Text* Button::AddLabel(const std::string label, FontProps fontProps) {
		this->label = dynamic_cast<Text*>(
			CreatePrimitive(Primitive::text, TextProps(label, Vector2(0, 0), fontProps))
		);

		Vector2 textSize = this->label->GetSize();

		Vector2 labelCenterPos = (Size() - textSize) / 2;
		this->label->ChangePosition(labelCenterPos);

		return this->label;
	}


	Color Button::GetColor() const {
		return props.color;
	}


    void Button::ChangeColor(const GUI::Color& newColor) {
		rectangle->ChangeColor(newColor);
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


	Button::~Button() = default;
}