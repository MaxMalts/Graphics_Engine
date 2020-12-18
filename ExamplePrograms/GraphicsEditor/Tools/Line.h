#pragma once

#include <string>
#include <assert.h>
#include <GraphicsEngine.h>
#include "Tool.h"


namespace GEditor {

	class Line : public Tool {
	public:

		Line(const ToolProps& props) : Tool(props) {}


		virtual void ToolMove(const GUI::Vector2& mousePos) {
			if (isDown) {
				curLine->ChangeSecondPoint(mousePos);
			}
		}


		virtual void ToolDown(const GUI::Vector2& mousePos) {
			isDown = true;

			curLine = dynamic_cast<GUI::Line*>(
				activeCanvas->CreatePrimitive(GUI::Primitive::line,
			                                  GUI::LineProps(mousePos, mousePos, activeSize, activeColor))
			);
		}


		virtual void ToolUp(const GUI::Vector2& mousePos) {
			isDown = false;
			curLine = nullptr;
		}


	private:

		bool isDown = false;

		GUI::Line* curLine = nullptr;
	};
}