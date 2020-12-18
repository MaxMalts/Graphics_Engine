#pragma once

#include <string>
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Event.h"
#include "Window.h"
#include "Include\Windows\Button.h"
#include "Include\Windows\Container.h"
#include "Include\Primitives\Point.h"
#include "Include\Primitives\Rectangle.h"
#include "Include\Primitives\Text.h"



namespace GUI {

	struct ColorPickerProps : public WindowProps {
	public:

		ColorPickerProps(const Color& bgColor = Color(0.85, 0.85, 0.85),
		                 const Color& applyColor = Color(0.7, 0.7, 0.7),
		                 const std::string& applyLabel = "Apply");

		Color bgColor;
		Color applyColor;
		std::string applyLabel;
	};


	class ColorPicker : public Window {
	public:

		ColorPicker(OSWindow& osWindow, Window* parent, const ColorPickerProps& props,
		            const Vector2& pos, const Vector2& size = GUI::Vector2(300, 200));

		ColorPicker(const ColorPicker& other) = delete;

		Color GetColor() const;

		virtual void Draw();

		~ColorPicker() = default;


	private:

		void InitPalette();

		void InitApplyBtn();

		void InitPreview();

		Color ColorMap(double x, double y) const;

		void ApplyColor();


		static void PaletteListener(Event& event, void* voidThis) {
			assert(event.GetType() == Event::mouse_move);
			assert(voidThis != nullptr);

			ColorPicker* _this = static_cast<ColorPicker*>(voidThis);

			bool mouseDown = _this->osWindow.MouseButtonPressed(MouseButton::left);
			if (mouseDown) {
				Vector2 mousePos = event.mouseMoveProps.pos;
				_this->selectedColor = _this->ColorMap(mousePos.x / _this->Width(), mousePos.y / _this->Height());
			}
		}


		static void ApplyButtonListener(Event& event, void* voidThis) {
			assert(voidThis != nullptr);

			ColorPicker* _this = static_cast<ColorPicker*>(voidThis);
			_this->ApplyColor();
		}


		ColorPickerProps props;

		Rectangle* background = nullptr;
		Container* palette = nullptr;
		Button* applyBtn = nullptr;
		Rectangle* appliedPreview = nullptr;
		Rectangle* selectedPreview = nullptr;

		Color appliedColor;
		Color selectedColor;
	};
}