#include <stdexcept>
#include <typeinfo>
#include <assert.h>

#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Event.h"

#include "Include\Primitives\Primitive.h"
#include "Include\Primitives\Point.h"
#include "Include\Primitives\Line.h"
#include "Include\Primitives\Polyline.h"
#include "Include\Primitives\Rectangle.h"
#include "Include\Primitives\Ellipse.h"
#include "Include\Primitives\Text.h"
#include "Include\Primitives\Image.h"

#include "Include\Windows\Window.h"
#include "Include\Windows\Container.h"
#include "Include\Windows\Button.h"
#include "Include\Windows\ColorPicker.h"
#include "Include\Windows\Graph.h"



namespace GUI {

	Window::Window(OSWindow& osWindow, Window* parent, const Vector2& pos, const Vector2& size)
		: Element(osWindow), parent(parent), pos(pos), size(size) {

		RecalcAbsPos();
	}


	Window* Window::CreateWindow(const Type type, const WindowProps& props,
	                             const Vector2& pos, const Vector2& size) {

		Window* newWindow = nullptr;

		try {
			switch (type) {
			case container:
				newWindow = new Container(osWindow, this, dynamic_cast<const ContainerProps&>(props), pos, size);
				break;

			case button:
				newWindow = new Button(osWindow, this, dynamic_cast<const ButtonProps&>(props), pos, size);
				break;

			case color_picker:
				newWindow = new ColorPicker(osWindow, this, dynamic_cast<const ColorPickerProps&>(props), pos, size);
				break;

			case graph:
				newWindow = new Graph(osWindow, this, dynamic_cast<const GraphProps&>(props), pos, size);
				break;

			default:
				throw std::invalid_argument("Unknown window type passed.");
			}

		} catch (std::bad_cast&) {
			throw std::invalid_argument("Props type doesn't match to window type.");
		}

		windows.push_back(newWindow);
		
		return newWindow;
	}


	Primitive* Window::CreatePrimitive(const Primitive::Type type, const PrimitiveProps& props) {

		Primitive* newPrimitive = nullptr;

		try {
			switch (type) {
			case Primitive::Type::point:
				newPrimitive = new Point(*this, dynamic_cast<const PointProps&>(props));
				break;

			case Primitive::Type::line:
				newPrimitive = new Line(*this, dynamic_cast<const LineProps&>(props));
				break;

			case Primitive::Type::polyline:
				newPrimitive = new Polyline(*this, dynamic_cast<const PolylineProps&>(props));
				break;

			case Primitive::Type::rectangle:
				newPrimitive = new Rectangle(*this, dynamic_cast<const RectangleProps&>(props));
				break;

			case Primitive::Type::ellipse:
				newPrimitive = new Ellipse(*this, dynamic_cast<const EllipseProps&>(props));
				break;

			case Primitive::Type::text:
				newPrimitive = new Text(*this, dynamic_cast<const TextProps&>(props));
				break;

			case Primitive::Type::image:
				newPrimitive = new Image(*this, dynamic_cast<const ImageProps&>(props));
				break;

			default:
				throw std::invalid_argument("Unknown window type passed.");
			}

		} catch (std::bad_cast&) {
			throw std::invalid_argument("Props type doesn't match to primitive type.");
		}

		primitives.push_back(newPrimitive);

		return newPrimitive;
	}


	void Window::RemoveWindow(Window* window) {
		delete window;
		windows.remove(window);
	}


	void Window::AddEventListener(Event::Type type, void(*listener)(Event&, void*), void* additParam) {
		assert(type != Event::Type::unknown);

		eventsListeners[type].emplace_back(listener, additParam);
	}


	void Window::RemoveEventListener(Event::Type type, void (*listener)(Event&, void*)) {
		assert(type != Event::Type::unknown);

		for (auto& curListener : eventsListeners[type]) {
			if (curListener.first == listener) {
				eventsListeners[type].remove(curListener);
				break;
			}
		}
	}


	void Window::HandleEvent(Event event) {
		event.SetTarget(this);

		Event::Type type = event.GetType();
		if (Event::mouse_down == type || Event::mouse_up == type ||
		    Event::mouse_move == type || Event::scroll == type) {

			Vector2 mousePos;
			if (Event::scroll == type) {
				event.scrollProps.pos -= pos;
				mousePos = event.scrollProps.pos;

			} else if (Event::mouse_move == type) {
				event.mouseMoveProps.pos -= pos;
				mousePos = event.mouseMoveProps.pos;

				HandleHoverEvent(event.mouseMoveProps);

			} else {
				event.mouseButtonProps.pos -= pos;
				mousePos = event.mouseButtonProps.pos;
			}

			if (mousePos.x >= 0 && mousePos.x < size.x &&
				mousePos.y >= 0 && mousePos.y < size.y) {
				for (auto& curListener : eventsListeners[type]) {
					curListener.first(event, curListener.second);

					if (event.Stopped()) {
						return;
					}
				}
			}

		} else {
			for (auto& curListener : eventsListeners[type]) {
				curListener.first(event, curListener.second);

				if (event.Stopped()) {
					return;
				}
			}
		}

		for (Window* curWindow : windows) {
			curWindow->HandleEvent(event);

			if (event.Stopped()) {
				return;
			}
		}
	}


	int Window::Width() const {
		return size.x;
	}


	int Window::Height() const {
		return size.y;
	}

    Vector2 Window::Size() const {
        return size;
    }


	Vector2 Window::AbsPosition() const {
		return absPos;
	}


	Vector2 Window::Position() const {
		return pos;
	}


	void Window::DrawInsides() {
		for (Primitive* primitive : primitives) {
			primitive->Draw();
		}
		for (Window* window : windows) {
			window->Draw();
		}
	}


	void Window::HandleHoverEvent(const MouseMoveProps& mouseMoveProps) {
		Vector2 mousePos = mouseMoveProps.pos;

		bool hoverChanged = false;
		MouseHoverProps hoverProps;
		if (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
			mousePos.y >= pos.y && mousePos.y <= pos.y + size.y) {
			if (!hovered) {
				hoverProps.hoverType = MouseHoverProps::HoverType::hovered;
				hoverChanged = true;
			}
		} else {
			if (hovered) {
				hoverProps.hoverType = MouseHoverProps::HoverType::unhovered;
				hoverChanged = true;
			}
		}

		if (hoverChanged) {
			hovered = !hovered;

			Event hoverEvent(Event::Type::mouse_hover, hoverProps);

			for (auto& curListener : eventsListeners[Event::Type::mouse_hover]) {
				curListener.first(hoverEvent, curListener.second);

				if (hoverEvent.Stopped()) {
					return;
				}
			}
		}
	}


	void Window::RecalcAbsPos() {
		if (parent == nullptr) {
			absPos = Vector2(0, 0);
		} else {
			absPos = parent->absPos + pos;
		}
	}


	Window::~Window() {
		for (Window* window : windows) {
			delete window;
		}
		for (Primitive* primitive : primitives) {
			delete primitive;
		}
	}
}