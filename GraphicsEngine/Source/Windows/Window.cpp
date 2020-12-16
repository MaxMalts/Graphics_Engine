#include <stdexcept>
#include <typeinfo>
#include <assert.h>

#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Event.h"

#include "Include\Primitives\Primitive.h"
#include "Include\Primitives\Line.h"
#include "Include\Primitives\Polyline.h"
#include "Include\Primitives\Rectangle.h"
#include "Include\Primitives\Text.h"
#include "Include\Primitives\Image.h"

#include "Include\Windows\Window.h"
#include "Include\Windows\Container.h"
#include "Include\Windows\Button.h"
#include "Include\Windows\Graph.h"



namespace GUI {

	Window::Window(OSWindow& osWindow, const Vector2& pos, const Vector2& size)
		: Element(osWindow), pos(pos), size(size) {}


	Window* Window::CreateWindow(const Type type, const WindowProps& props,
	                             const Vector2& pos, const Vector2& size) {

		Vector2 absPos = pos + this->pos;
		Window* newWindow = nullptr;

		try {
			switch (type) {
			case container:
				try {
					newWindow = new Container(osWindow, dynamic_cast<const ContainerProps&>(props), absPos, size);

				} catch (std::bad_cast&) {
					throw std::invalid_argument("Container is to create but props are not of ContainerProps type.");
				}
				break;

			case button:
				try {
					newWindow = new Button(osWindow, dynamic_cast<const ButtonProps&>(props), absPos, size);

				} catch (std::bad_cast&) {
					throw std::invalid_argument("Button is to create but props are not of ButtonProps type.");
				}
				break;

			case graph:
				try {
					newWindow = new Graph(osWindow, dynamic_cast<const GraphProps&>(props), absPos, size);

				} catch (std::bad_cast&) {
					throw std::invalid_argument("Button is to create but props are not of ButtonProps type.");
				}
				break;

			default:
				throw std::invalid_argument("Unknown window type passed.");
			}

		} catch (std::bad_cast&) {
			throw std::invalid_argument("Props type doesn't match to window type.");
		}

		windows.insert(newWindow);
		
		return newWindow;
	}


	Primitive* Window::CreatePrimitive(const Primitive::Type type, const PrimitiveProps& props) {

		Primitive* newPrimitive = nullptr;

		try {
			switch (type) {
			case Primitive::Type::line:
				newPrimitive = new Line(*this, dynamic_cast<const LineProps&>(props));
				break;

			case Primitive::Type::polyline:
				newPrimitive = new Polyline(*this, dynamic_cast<const PolylineProps&>(props));
				break;

			case Primitive::Type::rectangle:
				newPrimitive = new Rectangle(*this, dynamic_cast<const RectangleProps&>(props));
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

		primitives.insert(newPrimitive);

		return newPrimitive;
	}


	void Window::RemoveWindow(Window* window) {
		delete window;
		windows.erase(window);
	}


	void Window::AddEventListener(Event::Type type, void(*listener)(Event&, void*), void* additParam) {
		assert(type != Event::Type::unknown);

		eventsListeners[type].emplace(listener, additParam);
	}


	void Window::RemoveEventListener(Event::Type type, void (*listener)(Event&, void*)) {
		assert(type != Event::Type::unknown);

		for (auto& curListener : eventsListeners[type]) {
			if (curListener.first == listener) {
				eventsListeners[type].erase(curListener);
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
				mousePos = event.scrollProps.pos;
			} else if (Event::mouse_move == type) {
				mousePos = event.mouseMoveProps.pos;

				HandleHoverEvent(event.mouseMoveProps);
			} else {
				mousePos = event.mouseButtonProps.pos;
			}

			if (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
				mousePos.y >= pos.y && mousePos.y <= pos.y + size.y) {
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


	size_t Window::Width() const {
		return size.x;
	}


	size_t Window::Height() const {
		return size.y;
	}


	Vector2 Window::Pos() const {
		return pos;
	}


	void Window::DrawInsides() {
		for (Window* window : windows) {
			window->Draw();
		}
		for (Primitive* primitive : primitives) {
			primitive->Draw();
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


	Window::~Window() {
		for (Window* window : windows) {
			delete window;
		}
		for (Primitive* primitive : primitives) {
			delete primitive;
		}
	}
}