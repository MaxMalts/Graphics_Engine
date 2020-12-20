#pragma once

#include <list>
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
			container,
			button,
			color_picker,
			graph
		};

		Window(OSWindow& osWindow, Window* parent, const Vector2& pos, const Vector2& size);


		Window* CreateWindow(const Type type, const WindowProps& props, const Vector2& pos = Vector2(10, 10),
		                     const Vector2& size = Vector2(100, 50));

		Primitive* CreatePrimitive(const Primitive::Type type, const PrimitiveProps& props);

		void RemoveWindow(Window* window);

		void RemovePrimitive(Primitive* primitive);

		std::list<Window*> GetWindows() const;

		std::list<Primitive*> GetPrimitives() const;


		virtual void Draw() = 0;


		void AddEventListener(Event::Type type, void (*listener)(Event&, void*), void* additParam = nullptr);

		void RemoveEventListener(Event::Type type, void(*listener)(Event&, void*));

		bool HandleEvent(Event event);


		int Width() const;

		int Height() const;

		Vector2 Size() const;

		Vector2 AbsPosition() const;

		Vector2 Position() const;


		virtual ~Window();


	protected:

		void DrawInsides();


	private:

		bool hovered = false;
		void HandleHoverEvent(const MouseMoveProps& mouseMoveProps);

		void RecalcAbsPos();


		Vector2 pos;
		Vector2 absPos;
		Vector2 size;

		std::list<Window*> windows;
		std::list<Primitive*> primitives;

		// Second element will be tranfered to listener
		std::unordered_map<Event::Type,
			std::list<std::pair<void (*)(Event&, void*), void*>>> eventsListeners;

		Window* parent = nullptr;
	};
}