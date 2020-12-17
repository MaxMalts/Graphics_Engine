#pragma once

#include "Include\Common.h"


namespace GUI {
	class Window;


	struct PrimitiveProps {
	public:

		virtual ~PrimitiveProps() = default;

	private:

		Vector2 relativePos;
	};


	class Primitive : public Element {
	public:

		enum Type {
			line,
			polyline,
			rectangle,
			ellipse,
			text,
			image
		};

		Primitive(Window& window);

		virtual void Draw() = 0;


	protected:

		Vector2 RelToAbsCoords(const Vector2& coords) const;


	private:

		Window& window;
	};
}