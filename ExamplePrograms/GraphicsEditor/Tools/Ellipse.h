#pragma once

#include <string>
#include <cmath>
#include <assert.h>
#include <GraphicsEngine.h>
#include "Tool.h"


namespace GEditor {

	class Ellipse : public Tool {
	public:

		Ellipse(const ToolProps& props) : Tool(props) {}


		virtual void ToolMove(const GUI::Vector2& mousePos) {
			if (isDown) {
				curEllipse->ChangeSize(mousePos - firstPoint);
			}
		}


		virtual void ToolDown(const GUI::Vector2& mousePos) {
			isDown = true;
			firstPoint = mousePos;

			curEllipse = dynamic_cast<GUI::Ellipse*>(
				activeCanvas->CreatePrimitive(GUI::Primitive::ellipse,
			                                  GUI::EllipseProps(mousePos, GUI::Vector2(0, 0), activeColor))
			);
		}


		virtual void ToolUp(const GUI::Vector2& mousePos) {
			isDown = false;
			curEllipse = nullptr;
		}


	private:

		bool isDown = false;

		GUI::Vector2 firstPoint;
		GUI::Ellipse* curEllipse = nullptr;
	};
}