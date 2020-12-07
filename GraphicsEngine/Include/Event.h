#pragma once

#include "Include\Common.h"



namespace GUI {
	struct MouseButtonProps {
		enum Buttons {
			unknown, left, right, middle
		};

		Vector2 pos;
		Buttons button;
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
		enum Keys {
			unknown,
			q, w, e, r, t, y, u, i, o, p, a, s, d, f, g, h, j, k, l, z, x, c, v, b, n, m,
			n0, n1, n2, n3, n4, n5, n6, n7, n8, n9,
			ctr, alt, shift, space, tab, enter, backspace, caps,
			num_lock, num0, num1, num2, num3, num4, num5, num6, num7, num8, num9, num_div,
			num_mul, num_minus, num_plus, num_enter, num_comma,
			escape, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
			semicolon, quote, comma, period, slash, backslash, square_bracket_open, quare_bracket_close,
			acute, hyphen, equal,
			print_screen, scroll_lock, insert, del, home, end, page_up, page_down,
			arrow_up, arrow_down, arros_left, arrow_right,
		};

		Keys key;
	};


	struct ScrollProps {
		Vector2 offset;
		Vector2 pos;
	};


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
			window_close
		};

		Event(Type type);

		Event(Type type, const MouseButtonProps& mouseButtonProps);

		Event(Type type, const MouseMoveProps& mouseMoveProps);

		Event(Type type, const MouseHoverProps& mouseHoverProps);

		Event(Type type, const KeyProps& keyProps);

		Event(Type type, const ScrollProps& scrollProps);


		union {
			MouseButtonProps mouseButtonProps;
			MouseMoveProps mouseMoveProps;
			MouseHoverProps mouseHoverProps;
			KeyProps keyProps;
			ScrollProps scrollProps;
		};

		void Stop();

		bool Stopped() const;

		Type GetType() const;

	private:

		Type type;
		bool stopped = false;
	};
}