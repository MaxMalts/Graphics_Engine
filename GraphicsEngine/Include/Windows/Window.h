#pragma once

#include <set>
#include <unordered_set>
#include <unordered_map>
#include "Include\Common.h"
#include "Include\Event.h"
#include "Include\Primitives\Primitive.h"



namespace GUI {
	class OSWindow;


	struct WindowProps {
	public:

		virtual ~WindowProps() = default;
	};
	
	
	class Window : public Element {
	public:

		enum Type {
			button,
			graph
		};

		Window(OSWindow& osWindow, const Vector2& pos, const Vector2& size);

		Window* CreateWindow(const Type type, const WindowProps& props, const Vector2& pos = Vector2(10, 10),
		                     const Vector2& size = Vector2(100, 50));

		Primitive* CreatePrimitive(const Primitive::Type type, const PrimitiveProps& props);

		void RemoveWindow(Window* window);

		virtual void Draw() = 0;


		void AddEventListener(Event::Type type, void (*listener)(Event&, void*), void* additParam = nullptr);

		void RemoveEventListener(Event::Type type, void(*listener)(Event&, void*));

		void HandleEvent(Event event);


		size_t Width() const;

		size_t Height() const;


		~Window();

	protected:

		void DrawInsides();

		void HandleHoverEvent(const MouseMoveProps& mouseMoveProps);

		bool hovered = false;

		Vector2 pos;
		Vector2 size;

		std::unordered_set<Window*> windows;
		std::unordered_set<Primitive*> primitives;

	private:

		// Second element will be tranfered to listener
		std::unordered_map<Event::Type,
			std::set<std::pair<void (*)(Event&, void*), void*>>> eventsListeners;
	};
}