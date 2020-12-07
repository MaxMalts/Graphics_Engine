#pragma once

#include <vector>
#include "Include\Common.h"
#include "Include\Application.h"
#include "Primitive.h"



namespace GUI {
	struct PolylineProps : public PrimitiveProps {
	public:

		PolylineProps(const std::vector<Vector2>& verteces,
			const size_t width = 1, const Color& color = Color(0, 0, 0));

		PolylineProps(const size_t width = 1, const Color& color = Color(0, 0, 0));

		std::vector<Vector2> verteces;
		size_t width;
		Color color;
	};


	class Polyline : public Primitive {
	public:

		Polyline(OSWindow& window, const PolylineProps props = PolylineProps());

		Polyline(const Polyline& other) = delete;

		void AddVertex(const Vector2& pos);

		virtual void Draw();


	private:

		PolylineProps props;
	};
}