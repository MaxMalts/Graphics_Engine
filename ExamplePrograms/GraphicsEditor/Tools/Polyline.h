#pragma once

#include <string>
#include <assert.h>
#include <GraphicsEngine.h>
#include "Tool.h"


namespace GEditor {

	class Polyline : public Tool {
	public:

		Polyline(const ToolProps& props) : Tool(props) {}


		virtual void ToolActivated() {
			if (curPolyline == nullptr) {
				curPolyline = dynamic_cast<GUI::Polyline*>(
					activeCanvas->CreatePrimitive(GUI::Primitive::polyline,
						GUI::PolylineProps(activeSize, activeColor))
					);
			}
		}


		virtual void ToolUp(const GUI::Vector2& mousePos) {
			curPolyline->AddVertex(mousePos);
		}


		virtual void ToolDeactivated() {
			curPolyline = nullptr;
		}


	private:

		GUI::Polyline* curPolyline = nullptr;
	};
}