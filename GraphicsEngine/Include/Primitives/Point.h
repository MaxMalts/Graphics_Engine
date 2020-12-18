#pragma once

#include "Include\Common.h"
#include "Primitive.h"



namespace GUI {

	struct PointProps : public PrimitiveProps {
	public:

		PointProps(const Vector2& pos = Vector2(10, 10), const size_t width = 10,
		           const Color& color = Color(0, 0, 0));

		Vector2 pos;
		size_t width;
		Color color;
	};


	class Point : public Primitive {
	public:

		Point(Window& window, const PointProps& props = PointProps());

		Point(const Point& other) = delete;

		void ChangePosition(const Vector2& newPos);

		virtual void Draw();

	private:

		PointProps props;
	};
}