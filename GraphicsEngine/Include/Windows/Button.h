#pragma once

#include "Include\Common.h"
#include "Include\Application.h"
#include "Window.h"
#include "Include\Primitives\Rectangle.h"
#include "Include\Primitives\Text.h"



namespace GUI {
	struct ButtonProps : public WindowProps {
	public:

		ButtonProps(const Color& color = Color(0.7, 0.7, 0.7));

		Color color;
	};


	class Button : public Window {
	public:

		Button(OSWindow& window, const ButtonProps& buttonProps,
			const Vector2& pos, const Vector2& size);

		Button(const Button& other) = delete;

		Text* AddLabel(const std::string label, const Vector2& labelPos = Vector2(10, 10),
			const size_t fontSize = 16, const Color& labelColor = Color(0, 0, 0));

		Color GetColor() const;

		virtual void Draw();


		//void AddLeftMouseUpListener(void (*Listener)(void*), void* addParam);


		~Button();


	private:

		//static void LeftMouseUpCallback(void* buttonArg) {
		//	assert(buttonArg != nullptr);

		//	Button* button = static_cast<Button*>(buttonArg);

		//	WindowCoordinates cursorPos = button->osWindow.CursorPos();
		//	if (cursorPos.x >= button->pos.x && cursorPos.x <= button->pos.x + button->size.x &&
		//		cursorPos.y >= button->pos.y && cursorPos.y <= button->pos.y + button->size.y) {

		//		for (int i = 0; i < button->leftMouseUpListeners.size(); ++i) {
		//			std::pair<void (*)(void*), void*>& curListener = button->leftMouseUpListeners[i];
		//			curListener.first(curListener.second);
		//		}
		//	}
		//}

		//std::vector<std::pair<void (*)(void*), void*>> leftMouseUpListeners;

		ButtonProps props;

		Rectangle* rectangle = nullptr;
		Text* label = nullptr;
	};
}