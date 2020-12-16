#pragma once

#include <string>
#include <assert.h>
#include <GraphicsEngine.h>
#include "Tool.h"


namespace Tools {

	class Line : public Tool {
	public:

		Line(const ToolProps& props) : Tool(props) {}


		virtual void ToolMove(const GUI::Vector2& mousePos) {
			if (isDown) {
				curTrack->AddVertex(mousePos);
			}
		}


		virtual void ToolDown(const GUI::Vector2& mousePos) {
			isDown = true;

			curTrack = dynamic_cast<GUI::Polyline*>(
				activeCanvas->CreatePrimitive(GUI::Primitive::polyline,
			                                  GUI::PolylineProps({mousePos}, activeSize, activeColor))
			);
		}


		virtual void ToolUp(const GUI::Vector2& mousePos) {
			isDown = false;
			curTrack = nullptr;
		}


	private:

		bool isDown = false;

		GUI::Line* curLine = nullptr;
	};
}