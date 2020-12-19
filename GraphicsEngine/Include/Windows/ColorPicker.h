#pragma once

#include <string>
#include <vector>
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
		                 const Color& applyColor = Color(0.7, 0.7, 0.7));

		Color bgColor;
		Color applyColor;
		std::string applyLabel = "Apply";
		Color initialColor = Color(0, 0, 0);
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

		Color CalcColorByPoint(double x, double y) const;

		void PrecalcColorMap(const int width, const int height);

		void InitPalette(const int paletteSize);

		void InitApplyBtn();

		void InitPreview();

		void ApplyColor();


		static void PaletteListener(Event& event, void* voidThis) {
			assert(event.GetType() == Event::mouse_move);
			assert(voidThis != nullptr);

			ColorPicker* _this = static_cast<ColorPicker*>(voidThis);

			bool mouseDown = _this->osWindow.MouseButtonPressed(MouseButton::left);
			if (mouseDown) {
				Vector2 mousePos = event.mouseMoveProps.pos;
				_this->selectedColor = _this->colorMap.at(mousePos.x).at(mousePos.y);

				_this->selectedPreview->ChangeColor(_this->selectedColor);
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
		Rectangle* selectedPreview = nullptr;
		Rectangle* appliedPreview = nullptr;

		Color selectedColor;
		Color appliedColor;

		std::vector<std::vector<Color>> colorMap;
	};
}