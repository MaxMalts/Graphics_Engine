#include <assert.h>
#include "Include\Event.h"



namespace GUI {

	Event::Event(Type type) : type(type) {
		assert(window_close == type);
	}


	Event::Event(Type type, const MouseButtonProps& mouseButtonProps)
		: type(type), mouseButtonProps(mouseButtonProps) {

		assert(mouse_up == type || mouse_down == type);
	}


	Event::Event(Type type, const MouseMoveProps& mouseMoveProps)
		: type(type), mouseMoveProps(mouseMoveProps) {

		assert(mouse_move == type);
	}


	Event::Event(Type type, const MouseHoverProps& mouseHoverProps)
		: type(type), mouseHoverProps(mouseHoverProps) {

		assert(mouse_hover == type);
	}


	Event::Event(Type type, const KeyProps& keyProps) : type(type), keyProps(keyProps) {
		assert(key_up == type || key_down == type);
	}


	Event::Event(Type type, const ScrollProps& scrollProps) : type(type), scrollProps(scrollProps) {
		assert(scroll == type);
	}

	Event::Event(Type type, const ColorPickProps& colorPickProps) : type(type), scrollProps(scrollProps) {
		assert(color_pick == type);
	}


	void Event::Stop() {
		stopped = true;
	}


	bool Event::Stopped() const {
		return stopped;
	}


	Event::Type Event::GetType() const {
		return type;
	}


	Window* Event::GetTarget() const {
		return target;
	}


	void Event::SetTarget(Window* newTarget) {
		assert(newTarget != nullptr);

		target = newTarget;
	}
}