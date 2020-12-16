#pragma once

#include "Include\Common.h"
#include "Primitive.h"



namespace GUI {

	struct LineProps : public PrimitiveProps {
	public:

		LineProps(const Vector2& firstPoint = Vector2(10, 10), const Vector2& secondPoint = Vector2(100, 10),
		          const size_t width = 1, const Color& color = Color(0, 0, 0));

		Vector2 firstPoint;
		Vector2 secondPoint;
		size_t width;
		Color color;
	};


	class Line : public Primitive {
	public:

		Line(Window& window, const LineProps& props = LineProps());

		Line(const Line& other) = delete;

		virtual void Draw();

	private:

		LineProps props;
	};
}