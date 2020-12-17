#pragma once

#include <string>
#include <assert.h>
#include <GraphicsEngine.h>
#include "Tool.h"


namespace Tools {

	class Polyline : public Tool {
	public:

		Polyline(const ToolProps& props) : Tool(props) {}


		virtual void ToolMove(const GUI::Vector2& mousePos) {}


		virtual void ToolDown(const GUI::Vector2& mousePos) {}


		virtual void ToolUp(const GUI::Vector2& mousePos) {
			if (polyline == nullptr) {
				polyline = dynamic_cast<GUI::Polyline*>(
					activeCanvas->CreatePrimitive(GUI::Primitive::polyline,
						GUI::PolylineProps(activeSize, activeColor))
					);
			}

			polyline->AddVertex(mousePos);
		}


	private:

		bool isDown = false;

		GUI::Polyline* polyline = nullptr;
	};
}