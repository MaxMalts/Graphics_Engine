#pragma once

#include "Include\Common.h"
#include "Primitive.h"



namespace GUI {

	struct EllipseProps : public PrimitiveProps {
	public:

		EllipseProps(const Vector2& pos = Vector2(0, 0), const Vector2& size = Vector2(200, 100),
		             const Color& color = Color(0, 0, 0));

		Vector2 pos;
		Vector2 size;
		Color color;
	};

	
	class Ellipse : public Primitive {
	public:

		Ellipse(Window& window, const EllipseProps& props = EllipseProps());

		Ellipse(const Ellipse& other) = delete;

		void ChangeSize(const Vector2& newSize);

		virtual void Draw();

	private:

		EllipseProps props;
	};
}