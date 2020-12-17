#pragma once

#include <cmath>
#include <stdexcept>
#include <GraphicsEngine.h>



namespace GEditor {

	struct SizeControllerProps {

		SizeControllerProps(GUI::Container* previewContainer, const GUI::Vector2& previewPos,
		                    const int maxSize, const int initialSize = 5,
		                    const GUI::Color& previewColor = GUI::Color(GUI::Color::grey))
			: previewContainer(previewContainer), previewPos(previewPos),
			  maxSize(maxSize < 1 ? 100 : maxSize), size(initialSize < 1 ? 10 : initialSize),
			  previewColor(previewColor) {
			
			assert(maxSize > 0);
			assert(initialSize > 0);

			if (previewContainer == nullptr) {
				throw std::invalid_argument("The previewContainer was nullptr.");
			}
		}


		GUI::Container* previewContainer;  // Preview will be placed there
		GUI::Vector2 previewPos;
		GUI::Color previewColor;
		int maxSize;
		int size;
	};


	class SizeController {
	public:

		SizeController(const SizeControllerProps& props) : props(props) {

			circleContainer = dynamic_cast<GUI::Container*>(
				props.previewContainer->CreateWindow(GUI::Window::container,
					GUI::ContainerProps(), props.previewPos, GUI::Vector2(props.maxSize, props.maxSize))
			);

			previewCircle = dynamic_cast<GUI::Ellipse*>(
				circleContainer->CreatePrimitive(GUI::Primitive::ellipse,
					GUI::EllipseProps(GUI::Vector2(), GUI::Vector2(), props.previewColor))
			);

			ChangeSize(props.size);

			circleContainer->AddEventListener(GUI::Event::scroll, ScrollListener, this);
		}

		SizeController(const SizeController& other) = delete;


		void ChangeSize(int newSize) {
			if (newSize < 1) {
				newSize = 1;
			} else if (newSize > props.maxSize) {
				newSize = props.maxSize;
			}

			int newOffset = std::round((props.maxSize - newSize) / 2.0);

			previewCircle->ChangeSize(GUI::Vector2(newSize, newSize));
			previewCircle->ChangePosition(GUI::Vector2(newOffset, newOffset));

			props.size = newSize;
		}


		int GetSize() const {
			return props.size;
		}


		void Remove() {
			props.previewContainer->RemoveWindow(circleContainer);
		}


		~SizeController() = default;


	private:

		static void ScrollListener(GUI::Event& event, void* voidThis) {
			assert(voidThis != nullptr);

			SizeController* _this = static_cast<SizeController*>(voidThis);

			const int multiplier = 1.5 * std::sqrtf(_this->props.size);
			int scrollAmt = event.scrollProps.offset.y;

			_this->ChangeSize(_this->props.size + scrollAmt * multiplier);
		}


		SizeControllerProps props;

		GUI::Container* circleContainer = nullptr;  // This will listen to events
		GUI::Ellipse* previewCircle = nullptr;
	};
}