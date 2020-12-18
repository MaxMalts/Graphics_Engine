#pragma once

#include "Include\Common.h"



namespace GUI {

	struct MouseButtonProps {
		Vector2 pos;
		MouseButton button;
	};


	struct MouseMoveProps {
		Vector2 pos;
	};


	struct MouseHoverProps {
		enum HoverType {
			unknown, hovered, unhovered
		};

		Vector2 pos;
		HoverType hoverType;
	};


	struct KeyProps {
		KeyboardKey key;
	};


	struct ScrollProps {
		Vector2 offset;
		Vector2 pos;
	};


	struct ColorPickProps {
		Color color;
	};


	class Window;

	class Event {
	public:

		enum Type {
			unknown,
			mouse_down,
			mouse_up,
			mouse_move,
			mouse_hover,
			key_down,
			key_up,
			key_repeat,
			scroll,
			window_close,
			color_pick
		};

		Event(Type type);

		Event(Type type, const MouseButtonProps& mouseButtonProps);

		Event(Type type, const MouseMoveProps& mouseMoveProps);

		Event(Type type, const MouseHoverProps& mouseHoverProps);

		Event(Type type, const KeyProps& keyProps);

		Event(Type type, const ScrollProps& scrollProps);

		Event(Type type, const ColorPickProps& colorPickProps);


		union {
			MouseButtonProps mouseButtonProps;
			MouseMoveProps mouseMoveProps;
			MouseHoverProps mouseHoverProps;
			KeyProps keyProps;
			ScrollProps scrollProps;
			ColorPickProps colorPickProps;
		};

		void Stop();

		bool Stopped() const;

		Type GetType() const;

		Window* GetTarget() const;

	private:

		void SetTarget(Window* newTarget);


		Type type = unknown;
		Window* target = nullptr;
		bool stopped = false;


		friend class Window;
	};
}