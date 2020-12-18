#pragma once

#include "Include\Common.h"
#include "Include\Application.h"
#include "Window.h"



namespace GUI {

	struct ContainerProps : public WindowProps {};


	class Container : public Window {
	public:

		Container(OSWindow& osWindow, Window* parent, const ContainerProps& containerProps,
		          const Vector2& pos, const Vector2& size);

		Container(const Container& other) = delete;

		virtual void Draw();

		~Container() = default;
	};
}