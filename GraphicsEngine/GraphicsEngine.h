#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <assert.h>
#include <GL/glew.h>
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

		Color& operator=(const Color& other);

		/*
		* Returns specified color amount. Posiigble argument values are "red", "R", "green", "G", "blue", "B".
		*/
		float operator[](const char* color) const;


		float RedAmt() const;

		float GreenAmt() const;

		float BlueAmt() const;

	private:

		struct RGB_t {
			float red = 0.0, green = 0.0, blue = 0.0;
		};

		RGB_t RGB;
	};


	struct GraphProps {

		GraphProps() = default;

		GraphProps(const int startX, const int rangeX, const int startY, const int rangeY);

		int startX = 0, rangeX = 100, startY = 0, rangeY = 100;
		size_t fontSize = 16, axesWidth = 3, hatchSize = 10;
		Color axesColor = Color(0, 0, 0), fontColor = Color(0, 0, 0);
	};



	class Window;
	class Line;
	class Polyline;
	class Rectangle;
	class Text;
	class Button;
	class Graph;


	class Application {
	public:

		Application();

		Application(const Application& other) = delete;

		Window* CreateWindow(const int width = 640, const int height = 420, const char* name = "Window",
		                     const Color& backgroundColor = Color(0, 0, 0));

		size_t WindowsOpened() const;

		void CloseWindow(Window* window);

		void ProcessEventsWait() const;

		void ProcessEvents() const;

		~Application();

	private:

		std::set<Window*> windows;
	};


	class Window {
	public:

		Window(Application& application, const int width = 640, const int height = 420,
		       const char* name = "Window", const Color& backgroundColor = Color(0, 0, 0));

		Window(const Window& other) = delete;

		size_t Width() const;

		size_t Height() const;


		Rectangle* CreateRectangle(const Coordinates& pos = Coordinates(0, 0),
			const Size& size = Size(200, 100), const Color& color = Color(0, 0, 0));

		Button* CreateButton(const Coordinates& pos = Coordinates(0, 0),
			const Size& size = Size(100, 50), const Color& color = Color(0.7, 0.7, 0.7));

		Line* CreateLine(const Coordinates& begPos = Coordinates(10, 10),
			const Coordinates& endPos = Coordinates(100, 10),
			const size_t width = 1, const Color& color = Color(0, 0, 0));

		Polyline* CreatePolyline(const std::vector<Coordinates>& verteces, const size_t width = 1,
			const Color& color = Color(0, 0, 0));

		Polyline* CreatePolyline(const size_t width = 1, const Color& color = Color(0, 0, 0));

		Text* CreateText(const char* content, const Coordinates& pos = Coordinates(0, 0),
			const size_t fontSize = 16, const Color& color = Color(0, 0, 0));

		Graph* CreateGraph(const GraphProps& props = GraphProps(),
		                   const Coordinates& pos = Coordinates(0, 0),
		                   const Size& size = Size(100, 100),
		                   const Color& bgColor = Color(0.9, 0.9, 0.9));


		void Draw();

		void SetActive() const;


		void AddLeftMouseUpListener(void (*Listener)(void*), void* addParam);

		void AddWindowCloseListener(void (*Listener)(void*), void* addParam);


		WindowCoordinates CursorPos() const;

		Application& GetApplication() const;


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


		static void WindowCloseCallback(GLFWwindow* glfwWindow) {
			assert(glfwWindow != nullptr);

			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			assert(window != nullptr);

			for (int i = 0; i < window->leftMouseUpListeners.size(); ++i) {
				std::pair<void (*)(void*), void*>& curListener = window->windowCloseListeners[i];
				curListener.first(curListener.second);
			}
		}


		Application& application;

		GLFWwindow* window = nullptr;

		std::vector<Line*> lines;
		std::vector<Polyline*> polylines;
		std::vector<Rectangle*> rectangles;
		std::vector<Text*> texts;
		std::vector<Button*> buttons;
		std::vector<Graph*> graphs;

		// Second element will be tranfered to listener
		std::vector<std::pair<void (*)(void*), void*>> leftMouseUpListeners;
		std::vector<std::pair<void (*)(void*), void*>> windowCloseListeners;

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

		void Draw();

	private:

		Window& window;

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

		void Draw();


	private:

		Window& window;

		std::vector<Coordinates> verteces;
		size_t width;
		Color color;
	};


	class Rectangle {
	public:

		Rectangle(Window& window, const Coordinates& pos = Coordinates(0, 0),
		          const Size& size = Size(200, 100), const Color& color = Color(0, 0, 0));

		Rectangle(const Rectangle& other) = delete;

		void Draw();

	private:

		Window& window;

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
		
		void Draw();

		~Text();

	private:

		void InitFont(const char bitmapFileName[], const size_t chWidth, const size_t chHeight);

		void DestroyFont();

		void DrawChar(const char ch, const Coordinates& pos);

		Coordinates BmpCharPos(const unsigned char ch) const;


		char* content = nullptr;
		int contentLen = 0;

		Window& window;

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
		       const Size& size = Size(100, 50), const Color& color = Color(0.7, 0.7, 0.7));

		Button(const Button& other) = delete;

		Text* AddLabel(const char* label, const Coordinates& labelPos = Coordinates(10, 10),
		               const size_t fontSize = 16, const Color& labelColor = Color(0, 0, 0));

		Color GetColor() const;

		void Draw();


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


		Window& window;

		std::vector<std::pair<void (*)(void*), void*>> leftMouseUpListeners;

		Coordinates pos;
		Size size;
		Color color;

		Rectangle* rectangle = nullptr;
		Text* label = nullptr;
	};


	class Graph {
	public:

		Graph(Window& window, const GraphProps& props = GraphProps(),
		      const Coordinates& pos = Coordinates(0, 0), const Size& size = Size(100, 100),
		      const Color& bgColor = Color(0.9, 0.9, 0.9));

		Graph(const Graph& other) = delete;


		int AddDiagram(const size_t lineWidth = 1, const Color& color = Color(0, 0, 0));

		void AddData(const int diagramInd, int x, int y);
		
		void Draw();

		~Graph();

	private:

		void InitGraphParts();

		void InitArrows(const int arrowBackOffset, const int arrowSideOffset);

		void InitLabels(const int rowLabelsOffset, const int columnLabelsOffset);


		Window& window;


		struct DiagramNode {
			DiagramNode() = default;

			DiagramNode(int column, int value);

			int column = 0, value = 0;
		};

		// Diargrams data with corresponding polylines
		std::vector<std::pair<std::vector<DiagramNode>, Polyline*>> diagrams;

		std::vector<Text*> labels;
		std::vector<Line*> hatches;

		GraphProps props;

		Rectangle* background = nullptr;
		Polyline* axes = nullptr;

		Coordinates pos;
		Size size;
		Color bgColor;

		Coordinates innerPos;    // Position without axes and labels
		Size innerSize;    // Size of graph without axes and labels
	};
}