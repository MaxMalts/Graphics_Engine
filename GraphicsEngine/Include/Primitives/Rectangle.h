#pragma once

#include "Include\Common.h"
#include "Primitive.h"



namespace GUI {

	struct RectangleProps : public PrimitiveProps {
	public:

		RectangleProps(const Vector2& pos = Vector2(0, 0), const Vector2& size = Vector2(200, 100),
		               const Color& color = Color(0, 0, 0));

		Vector2 pos;
		Vector2 size;
		Color color;
	};

	
	class Rectangle : public Primitive {
	public:

		Rectangle(Window& window, const RectangleProps& props = RectangleProps());

		Rectangle(const Rectangle& other) = delete;

		virtual void Draw();

	private:

		RectangleProps props;
	};
}