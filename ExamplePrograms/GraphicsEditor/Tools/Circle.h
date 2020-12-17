#pragma once

#include <string>
#include <cmath>
#include <assert.h>
#include <GraphicsEngine.h>
#include "Tool.h"


namespace Tools {

	class Circle : public Tool {
	public:

		Circle(const ToolProps& props) : Tool(props) {}


		virtual void ToolMove(const GUI::Vector2& mousePos) {
			if (isDown) {
				int newRadius = sqrtf((mousePos.x - centerPos.x) * (mousePos.x - centerPos.x) +
				                      (mousePos.y - centerPos.y) * (mousePos.y - centerPos.y));

				curCircle->ChangeRadius(newRadius);
			}
		}


		virtual void ToolDown(const GUI::Vector2& mousePos) {
			isDown = true;
			centerPos = mousePos;

			curCircle = dynamic_cast<GUI::Circle*>(
				activeCanvas->CreatePrimitive(GUI::Primitive::circle,
			                                  GUI::CircleProps(mousePos, 0, activeColor))
			);
		}


		virtual void ToolUp(const GUI::Vector2& mousePos) {
			isDown = false;
			curCircle = nullptr;
		}


	private:

		bool isDown = false;

		GUI::Vector2 centerPos;
		GUI::Circle* curCircle = nullptr;
	};
}