#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Windows\Window.h"
#include "Include\Primitives\Primitive.h"



namespace GUI {

	Primitive::Primitive(Window& window) : Element(window.GetOSWindow()), window(window) {}


	Vector2 Primitive::RelToAbsCoords(const Vector2& coords) const {
		return coords + window.AbsPosition();
	}
}