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

	ColorPickerProps::ColorPickerProps(const Color& bgColor, const Color& applyColor)
		: bgColor(bgColor), applyColor(applyColor) {}


	ColorPicker::ColorPicker(OSWindow& osWindow, Window* parent, const ColorPickerProps& props,
	                         const Vector2& pos, const Vector2& size)
		: Window(osWindow, parent, pos, size), props(props) {

		selectedColor = appliedColor = props.initialColor;

		const int paletteSize = std::min(0.7 * Width(), 0.9 * Height());
		PrecalcColorMap(paletteSize, paletteSize);

		background = dynamic_cast<Rectangle*>(
			CreatePrimitive(Primitive::rectangle, RectangleProps(Vector2(0, 0), size, props.bgColor))
		);

		InitPalette(paletteSize);

		InitApplyBtn();

		InitPreview();
	}


	Color ColorPicker::GetColor() const {
		return appliedColor;
	}


	void ColorPicker::Draw() {
		DrawInsides();
	}


	void ColorPicker::PrecalcColorMap(const int width, const int height) {
		assert(width > 0);
		assert(height > 0);

		colorMap.assign(width, std::vector<Color>(0));

		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				Color curColor = CalcColorByPoint(1.0f * x / (width - 1),
				                                  1.0f * y / (height - 1));

				colorMap.at(x).push_back(curColor);
			}
		}
	}

	void ColorPicker::InitPalette(const int paletteSize) {
		const int posX = 10;
		const int posY = (Height() - paletteSize) / 2;

		palette = dynamic_cast<Container*>(
			CreateWindow(Window::container, ContainerProps(),
			             Vector2(posX, posY), Vector2(paletteSize, paletteSize))
		);

		/* Drawing palette */
		for (int x = 0; x < paletteSize; ++x) {
			for (int y = 0; y < paletteSize; ++y) {
				palette->CreatePrimitive(Primitive::point, PointProps(Vector2(x, y),
				                         1, colorMap.at(x).at(y)));
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


	Color ColorPicker::CalcColorByPoint(double x, double y) const {
		assert(x >= 0.0 && x <= 1.0);
		assert(y >= 0.0 && y <= 1.0);

		std::vector<Color> colorsOrder{
			Color(1, 0, 0),
			Color(0, 1, 0),
			Color(0, 0, 1),
		};
		int nColors = colorsOrder.size();
		double halfInterval = 1.0 / (2 * nColors);

		int prevColor = static_cast<int>(x * nColors) % nColors;
		int curColor = (prevColor + 1) % nColors;
		double brightness = (0.5 - y) * 2;

		double intervalX = std::fmod(x, halfInterval * 2);
		double prevAmt = intervalX < halfInterval ? 1 : (-2 * nColors * intervalX + 2);
		double curAmt = intervalX < halfInterval ? (2 * nColors * intervalX) : 1 ;
		Color hue = prevAmt * colorsOrder.at(prevColor) + curAmt * colorsOrder.at(curColor);

		if (brightness < 0) {
			return hue * (brightness + 1.0);

		} else {
			Color inverted(1.0 - hue.Red(), 1.0 - hue.Green(), 1.0 - hue.Blue());
			return Color(1.0, 1.0, 1.0) - inverted * (-brightness + 1.0);
		}
	}


	void ColorPicker::ApplyColor() {
		appliedColor = selectedColor;
		appliedPreview->ChangeColor(appliedColor);

		Event colorPick(Event::color_pick, ColorPickProps{ appliedColor });
		osWindow.StartEvent(colorPick);
	}
}