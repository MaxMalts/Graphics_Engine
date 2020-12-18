#include <string>
#include <algorithm>
#include "Include\Extra.h"
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Primitives\Point.h"
#include "Include\Primitives\Rectangle.h"
#include "Include\Primitives\Text.h"
#include "Include\Windows\ColorPicker.h"



namespace GUI {

	ColorPickerProps::ColorPickerProps(const Color& bgColor, const Color& applyColor,
	                                   const std::string& applyLabel)
		: bgColor(bgColor), applyColor(applyColor), applyLabel(applyLabel) {}


	ColorPicker::ColorPicker(OSWindow& osWindow, Window* parent, const ColorPickerProps& props,
	                         const Vector2& pos, const Vector2& size)
		: Window(osWindow, parent, pos, size), props(props) {

		background = dynamic_cast<Rectangle*>(
			CreatePrimitive(Primitive::rectangle, RectangleProps(Vector2(0, 0), size, props.bgColor))
		);

		InitPalette();

		InitApplyBtn();

		InitPreview();
	}


	Color ColorPicker::GetColor() const {
		return appliedColor;
	}


	void ColorPicker::Draw() {
		DrawInsides();
	}


	void ColorPicker::InitPalette() {
		const int paletteSize = std::min(0.7 * Width(), 0.9 * Height());
		const int posX = 10;
		const int posY = (Height() - paletteSize) / 2;

		palette = dynamic_cast<Container*>(
			CreateWindow(Window::container, ContainerProps(),
			             Vector2(posX, posY), Vector2(paletteSize, paletteSize))
		);

		/* Drawing palette */
		for (int x = 0; x < paletteSize; ++x) {
			for (int y = 0; y < paletteSize; ++y) {
				Color curColor = ColorMap(static_cast<float>(x) / (paletteSize - 1),
				                          static_cast<float>(y) / (paletteSize - 1));

				palette->CreatePrimitive(Primitive::point, PointProps(Vector2(x, y), 1, curColor));
			}
		}

		palette->AddEventListener(Event::mouse_move, PaletteListener, this);
	}


	void ColorPicker::InitApplyBtn() {  // palette should be initialized!
		const int btnSizeX = 0.25 * Width();
		const int btnSizeY = btnSizeX / 2;
		const int posX = (Width() - btnSizeX + palette->Position().x + palette->Width()) / 2;
		const int marginBottom = (Height() - palette->Height()) / 2 + 0.1 * Height();

		applyBtn = dynamic_cast<Button*>(
			CreateWindow(Window::button, ButtonProps(props.applyColor),
			             Vector2(posX, Height() - marginBottom - btnSizeY), Vector2(btnSizeX, btnSizeY))
		);
		applyBtn->AddLabel(props.applyLabel, FontProps());

		applyBtn->AddEventListener(Event::mouse_up, ApplyButtonListener, this);
	}


	void ColorPicker::InitPreview() {  // palette and applyBtn should be initialized!
		const int marginTop = palette->Position().y + 0.1 * Height();
		const int previewSizeX = 0.2 * Width();
		const int previewSizeY = 0.6 * (applyBtn->Position().y - marginTop);
		const int posX = (Width() - previewSizeX + palette->Position().x + palette->Width()) / 2;

		appliedPreview = dynamic_cast<Rectangle*>(
			CreatePrimitive(Primitive::rectangle, RectangleProps(
				Vector2(posX, marginTop),Vector2(previewSizeX, previewSizeY / 2), appliedColor
			))
		);

		selectedPreview = dynamic_cast<Rectangle*>(
			CreatePrimitive(Primitive::rectangle, RectangleProps(
				Vector2(posX, marginTop + previewSizeY / 2),Vector2(previewSizeX, previewSizeY / 2), selectedColor
			))
		);
	}


	Color ColorPicker::ColorMap(double x, double y) const {
		return Color();
	}


	void ColorPicker::ApplyColor() {
		appliedColor = selectedColor;
		appliedPreview->ChangeColor(appliedColor);

		Event colorPick(Event::color_pick, ColorPickProps{ appliedColor });
		osWindow.StartEvent(colorPick);
	}
}