#pragma once

#include <string>
#include <assert.h>
#include <GraphicsEngine.h>
#include "Tool.h"


namespace Tools {

	class Rectangle : public Tool {
	public:

		Rectangle(const ToolProps& props) : Tool(props) {}


		virtual void ToolMove(const GUI::Vector2& mousePos) {
			if (isDown) {
				curRect->ChangeSize(mousePos - firstPoint);
			}
		}


		virtual void ToolDown(const GUI::Vector2& mousePos) {
			isDown = true;
			firstPoint = mousePos;

			curRect = dynamic_cast<GUI::Rectangle*>(
				activeCanvas->CreatePrimitive(GUI::Primitive::rectangle,
			                                  GUI::RectangleProps(mousePos, GUI::Vector2(0, 0), activeColor))
			);
		}


		virtual void ToolUp(const GUI::Vector2& mousePos) {
			isDown = false;
			curRect = nullptr;
		}


	private:

		bool isDown = false;

		GUI::Vector2 firstPoint;
		GUI::Rectangle* curRect = nullptr;
	};
}