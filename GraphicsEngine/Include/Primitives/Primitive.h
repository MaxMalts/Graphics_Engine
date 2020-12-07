#pragma once

#include "Include\Common.h"


namespace GUI {
	class OSWindow;


	struct PrimitiveProps {
	public:

		virtual ~PrimitiveProps() = default;
	};


	class Primitive : public Element {
	public:

		enum Type {
			line,
			polyline,
			rectangle,
			text
		};

		Primitive(OSWindow& osWindow);

		virtual void Draw() = 0;
	};
}