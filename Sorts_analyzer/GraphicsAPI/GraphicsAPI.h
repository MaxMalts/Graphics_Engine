#pragma once

#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "BMP_Img/BMP_Img.h"



namespace GUI {

	class bad_init {
	public:

		bad_init();

		bad_init(const char* message);

		char* what();


	private:

		char* _what = nullptr;
	};



	struct Coordinates {
		Coordinates() = default;

		Coordinates(int x, int y);

		int x = 0;
		int y = 0;
	};


	struct WindowCoordinates {
		WindowCoordinates() = default;

		WindowCoordinates(int x, int y);

		int x = 0;
		int y = 0;
	};


	struct Size {

		Size() = default;

		Size(size_t width, size_t height);

		size_t width = 0;
		size_t height = 0;
	};


	class Color {
	public:

		Color();

		/*
		* Constructs Color. Arguments values must be betweeb 0.0 and 1.0
		*/
		Color(const float red, const float green, const float blue);

		/*
		* Constructs Color. Possible strings are "red", "green", "blue", "black", "white".
		*/
		Color(const char* color);

		Color(const Color& other);

		Color& operator=(const Color& other) {
			memcpy(RGB, other.RGB, sizeof(RGB));

			return *this;
		}

		/*
		* Returns specified color amount. Posiigble argument values are "red", "R", "green", "G", "blue", "B".
		*/
		float operator[](const char* color) const;

		/*
		* Returns specified color amount. 0 index represents red amount, 1 - green, 2 - blue
		*/
		float operator[](const size_t color) const;


		float RedAmt() const;

		float GreenAmt() const;

		float BlueAmt() const;

	private:

		float RGB[3] = { 0, 0, 0 };
	};



	class Window;
	class Line;
	class Polyline;
	class Rectangle;
	class Button;
	class Text;


	class Application {
	public:

		Application();

		Application(const Application& other) = delete;

		Window* CreateWindow(const int width = 640, const int height = 420, const char* name = "Window",
		                     const Color& backgroundColor = Color(0, 0, 0));

		void ProcessEventsWait() const;

		void ProcessEvents() const;

		~Application();

	private:

		std::vector<Window*> windows;
	};


	class Window {
	public:

		Window(const int width = 640, const int height = 420, const char* name = "Window",
		       const Color& backgroundColor = Color(0, 0, 0));

		Window(const Window& other) = delete;

		size_t Width() const;

		size_t Height() const;


		Rectangle* CreateRectangle(const Coordinates& pos = Coordinates(0, 0),
			const Size& size = Size(200, 100), const Color& color = Color(0, 0, 0));

		Button* CreateButton(const Coordinates& pos = Coordinates(0, 0),
			const Size& size = Size(100, 50), const Color& color = Color(0.5, 0.5, 0.5));

		Line* CreateLine(const Coordinates& begPos = Coordinates(10, 10),
			const Coordinates& endPos = Coordinates(100, 10),
			const size_t width = 1, const Color& color = Color(0, 0, 0));

		Polyline* CreatePolyline(const std::vector<Coordinates>& verteces, const size_t width = 1,
			const Color& color = Color(0, 0, 0));

		Polyline* CreatePolyline(const size_t width = 1, const Color& color = Color(0, 0, 0));

		Text* CreateText(const char* content, const Coordinates& pos = Coordinates(0, 0),
			const size_t fontSize = 16, const Color& color = Color(0, 0, 0));


		void DrawChanges();

		void SetActive() const;


		void AddLeftMouseUpListener(void (*Listener)(void*), void* addParam);


		WindowCoordinates CursorPos() const;


		~Window();


	private:

		static void LeftMouseUpCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
			assert(glfwWindow != nullptr);

			if (GLFW_MOUSE_BUTTON_LEFT == button && GLFW_RELEASE == action) {
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
				assert(window != nullptr);

				for (int i = 0; i < window->leftMouseUpListeners.size(); ++i) {
					std::pair<void (*)(void*), void*>& curListener = window->leftMouseUpListeners[i];
					curListener.first(curListener.second);
				}
			}
		}


		GLFWwindow* window = nullptr;

		std::vector<Line*> lines;
		std::vector<Polyline*> polylines;
		std::vector<Rectangle*> rectangles;
		std::vector<Text*> texts;
		std::vector<Button*> buttons;

		// Second element will be tranfered to listener
		std::vector<std::pair<void (*)(void*), void*>> leftMouseUpListeners;

		size_t width = 0;
		size_t height = 0;
		char* name = nullptr;
		Color backgroundColor;
	};


	class Line {
	public:

		Line(Window& window, const Coordinates& begPos = Coordinates(10, 10),
		     const Coordinates& endPos = Coordinates(100, 10), const size_t width = 1,
		     const Color& color = Color(0, 0, 0));

		Line(const Line& other) = delete;

	private:

		const Window& window;

		Coordinates firstPoint;
		Coordinates secondPoint;
		size_t width;
		Color color;
	};


	class Polyline {
	public:

		Polyline(Window& window, const std::vector<Coordinates>& verteces,
		         const size_t width = 1, const Color& color = Color(0, 0, 0));

		Polyline(Window& window, const size_t width = 1, const Color& color = Color(0, 0, 0));

		Polyline(const Polyline& other) = delete;

		void AddVertex(const Coordinates& pos);


	private:

		const Window& window;

		std::vector<Coordinates> verteces;
		size_t width;
		Color color;
	};


	class Rectangle {
	public:

		Rectangle(Window& window, const Coordinates& pos = Coordinates(0, 0),
		          const Size& size = Size(200, 100), const Color& color = Color(0, 0, 0));

		Rectangle(const Rectangle& other) = delete;

	private:

		const Window& window;

		Coordinates pos;
		Size size;
		Color color;
	};


	class Text {
	public:

		Text(Window& window, const char* string,
		     const Coordinates& pos = Coordinates(0, 0), const size_t fontSize = 16,
		     const Color& color = Color(0, 0, 0));

		Text(const Text& other) = delete;

		~Text();


	private:

		void InitFont(const char bitmapFileName[], const size_t chWidth, const size_t chHeight);

		void DestroyFont();

		void DrawChar(const char ch, const Coordinates& pos);

		void Draw();

		Coordinates BmpCharPos(const unsigned char ch) const;


		char* content = nullptr;

		const Window& window;

		Coordinates pos;
		size_t fontSize;    // width in window pixels
		Color color;

		static size_t instanceCount;
		static BMP_Img* bitmapImg;
		static size_t charWidth;
		static size_t charHeight;
	};


	class Button {
	public:

		Button(Window& window, const Coordinates& pos = Coordinates(0, 0),
		       const Size& size = Size(100, 50), const Color& color = Color(0.5, 0.5, 0.5));

		Button(const Button& other) = delete;


		void AddLeftMouseUpListener(void (*Listener)(void*), void* addParam);


		~Button();


	private:

		static void LeftMouseUpCallback(void* buttonArg) {
			assert(buttonArg != nullptr);

			Button* button = static_cast<Button*>(buttonArg);

			WindowCoordinates cursorPos = button->window.CursorPos();
			if (cursorPos.x >= button->pos.x && cursorPos.x <= button->pos.x + button->size.width &&
				cursorPos.y >= button->pos.y && cursorPos.y <= button->pos.y + button->size.height) {

				for (int i = 0; i < button->leftMouseUpListeners.size(); ++i) {
					std::pair<void (*)(void*), void*>& curListener = button->leftMouseUpListeners[i];
					curListener.first(curListener.second);
				}
			}
		}


		const Window& window;

		std::vector<std::pair<void (*)(void*), void*>> leftMouseUpListeners;

		Coordinates pos;
		Size size;
		Color color;

		Rectangle* rectangle;
	};
}