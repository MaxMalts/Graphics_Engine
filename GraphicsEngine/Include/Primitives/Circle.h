#pragma once

#include "Include\Common.h"
#include "Primitive.h"



namespace GUI {

	struct CircleProps : public PrimitiveProps {
	public:

		CircleProps(const Vector2& centerPos = Vector2(100, 100), const size_t radius = 50,
		            const Color& color = Color(0, 0, 0));

		Vector2 centerPos;
		size_t radius;
		Color color;
	};

	
	class Circle : public Primitive {
	public:

		Circle(Window& window, const CircleProps& props = CircleProps());

		Circle(const Circle& other) = delete;

		void ChangeRadius(const size_t newRadius);

		virtual void Draw();

	private:

		CircleProps props;
	};
}