#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
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



	struct Vector2 {
		Vector2() = default;

		Vector2(int x, int y);

		int x = 0;
		int y = 0;
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


		float Redness() const;

		float Greenness() const;

		float Blueness() const;

	private:

		struct RGB_t {
			float red = 0.0, green = 0.0, blue = 0.0;
		};

		RGB_t RGB;
	};


	class OSWindow;
	class Primitive;
	class Line;
	class Polyline;
	class Rectangle;
	class Text;
	class Window;
	class DesktopWindow;
	class Button;
	class Graph;


	struct MouseButtonProps {
		enum Buttons {
			unknown, left, right, middle
		};

		Vector2 pos;
		Buttons button;
	};


	struct MouseMoveProps {
		Vector2 pos;
	};


	struct MouseHoverProps {
		enum HoverType {
			unknown, hovered, unhovered
		};

		Vector2 pos;
		HoverType hoverType;
	};


	struct KeyProps {
		enum Keys {
			unknown,
			q, w, e, r, t, y, u, i, o, p, a, s, d, f, g, h, j, k, l, z, x, c, v, b, n, m,
			n0, n1, n2, n3, n4, n5, n6, n7, n8, n9,
			ctr, alt, shift, space, tab, enter, backspace, caps,
			num_lock, num0, num1, num2, num3, num4, num5, num6, num7, num8, num9, num_div,
			num_mul, num_minus, num_plus, num_enter, num_comma,
			escape, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
			semicolon, quote, comma, period, slash, backslash, square_bracket_open, quare_bracket_close,
			acute, hyphen, equal,
			print_screen, scroll_lock, insert, del, home, end, page_up, page_down,
			arrow_up, arrow_down, arros_left, arrow_right,
		};

		Keys key;
	};


	struct ScrollProps {
		Vector2 offset;
		Vector2 pos;
	};


	class Event {
	public:

		enum Type {
			unknown,
			mouse_down,
			mouse_up,
			mouse_move,
			mouse_hover,
			key_down,
			key_up,
			key_repeat,
			scroll,
			window_close
		};

		Event(Type type);

		Event(Type type, const MouseButtonProps& mouseButtonProps);

		Event(Type type, const MouseMoveProps& mouseMoveProps);

		Event(Type type, const MouseHoverProps& mouseHoverProps);

		Event(Type type, const KeyProps& keyProps);

		Event(Type type, const ScrollProps& scrollProps);


		union {
			MouseButtonProps mouseButtonProps;
			MouseMoveProps mouseMoveProps;
			MouseHoverProps mouseHoverProps;
			KeyProps keyProps;
			ScrollProps scrollProps;
		};

		void Stop();

		bool Stopped() const;

		Type GetType() const;

	private:

		Type type;
		bool stopped = false;
	};


	struct WindowProps {
	public:

		virtual ~WindowProps() = default;
	};


	struct ButtonProps : public WindowProps {
	public:

		ButtonProps(const Color& color = Color(0.7, 0.7, 0.7));

		Color color;
	};


	struct GraphProps : public WindowProps {
	public:

		GraphProps() = default;

		GraphProps(const int startX, const int rangeX, const int startY, const int rangeY);

		int startX = 0, rangeX = 100, startY = 0, rangeY = 100;
		Color bgColor = Color(0.9, 0.9, 0.9);
		size_t fontSize = 16, axesWidth = 3, hatchSize = 10;
		Color axesColor = Color(0, 0, 0), fontColor = Color(0, 0, 0);
	};



	struct PrimitiveProps {
	public:

		virtual ~PrimitiveProps() = default;
	};


	struct LineProps : public PrimitiveProps {
	public:

		LineProps(const Vector2& firstPoint = Vector2(10, 10), const Vector2& secondPoint = Vector2(100, 10),
			const size_t width = 1, const Color& color = Color(0, 0, 0));

		Vector2 firstPoint;
		Vector2 secondPoint;
		size_t width;
		Color color;
	};


	struct PolylineProps : public PrimitiveProps {
	public:

		PolylineProps(const std::vector<Vector2>& verteces,
			const size_t width = 1, const Color& color = Color(0, 0, 0));

		PolylineProps(const size_t width = 1, const Color& color = Color(0, 0, 0));

		std::vector<Vector2> verteces;
		size_t width;
		Color color;
	};


	struct RectangleProps : public PrimitiveProps {
	public:

		RectangleProps(const Vector2& pos = Vector2(0, 0), const Vector2& size = Vector2(200, 100),
			const Color& color = Color(0, 0, 0));

		Vector2 pos;
		Vector2 size;
		Color color;
	};


	struct TextProps : public PrimitiveProps {
	public:

		TextProps(const std::string& content = "text", const Vector2& pos = Vector2(0, 0),
			const size_t fontSize = 16, const Color& color = Color(0, 0, 0));


		std::string content = nullptr;
		Vector2 pos;
		size_t fontSize;
		Color color;

	private:

		int contentLen = 0;
	};


	class Element {
	public:

		Element(OSWindow& osWindow);

	protected:

		OSWindow& osWindow;
	};


	class Primitive : public Element {
	public:

		enum Type {
			line,
			polyline,
			rectangle,
			text
		};

		Primitive(OSWindow& osWindow);

		virtual void Draw() = 0;
	};


	class Line : public Primitive {
	public:

		Line(OSWindow& window, const LineProps& props = LineProps());

		Line(const Line& other) = delete;

		virtual void Draw();

	private:

		LineProps props;
	};


	class Polyline : public Primitive {
	public:

		Polyline(OSWindow& window, const PolylineProps props = PolylineProps());

		Polyline(const Polyline& other) = delete;

		void AddVertex(const Vector2& pos);

		virtual void Draw();


	private:

		PolylineProps props;
	};


	class Rectangle : public Primitive {
	public:

		Rectangle(OSWindow& window, const RectangleProps& props = RectangleProps());

		Rectangle(const Rectangle& other) = delete;

		virtual void Draw();

	private:

		RectangleProps props;
	};


	class Text : public Primitive {
	public:

		Text(OSWindow& window, const TextProps& props = TextProps());

		Text(const Text& other) = delete;

		virtual void Draw();

		~Text();

	private:

		void InitFont(const char bitmapFileName[], const size_t chWidth, const size_t chHeight);

		void DestroyFont();

		void DrawChar(const char ch, const Vector2& pos);

		Vector2 BmpCharPos(const unsigned char ch) const;


		TextProps props;

		static size_t instanceCount;
		static BMP_Img* bitmapImg;
		static size_t charWidth;
		static size_t charHeight;
	};


	class Window : public Element {
	public:

		enum Type {
			button,
			graph
		};

		Window(OSWindow& osWindow, const Vector2& pos, const Vector2& size);

		Window* CreateWindow(const Type type, const WindowProps& props, const Vector2& pos = Vector2(10, 10),
			const Vector2& size = Vector2(100, 50));

		Primitive* CreatePrimitive(const Primitive::Type type, const PrimitiveProps& props);

		void RemoveWindow(Window* window);

		virtual void Draw() = 0;


		void AddEventListener(Event::Type type, void (*listener)(Event&, void*), void* additParam = nullptr);

		void RemoveEventListener(Event::Type type, void(*listener)(Event&, void*));

		void HandleEvent(Event& event);


		~Window();

	protected:

		void DrawInsides();

		void HandleHoverEvent(const MouseMoveProps& mouseMoveProps);

		bool hovered = false;

		Vector2 pos;
		Vector2 size;

		std::unordered_set<Window*> windows;
		std::unordered_set<Primitive*> primitives;

	private:

		// Second element will be tranfered to listener
		std::unordered_map<Event::Type,
			std::set<std::pair<void (*)(Event&, void*), void*>>> eventsListeners;
	};


	class DesktopWindow : public Window {
	public:

		DesktopWindow(OSWindow& osWindow, const Vector2 pos, const Vector2 size,
			const Color& color = Color(1, 1, 1));

		DesktopWindow(const DesktopWindow& other) = delete;

		virtual void Draw();

	private:

		Color color;
	};


	class Button : public Window {
	public:

		Button(OSWindow& window, const ButtonProps& buttonProps,
			const Vector2& pos, const Vector2& size);

		Button(const Button& other) = delete;

		Text* AddLabel(const std::string label, const Vector2& labelPos = Vector2(10, 10),
			const size_t fontSize = 16, const Color& labelColor = Color(0, 0, 0));

		Color GetColor() const;

		virtual void Draw();


		//void AddLeftMouseUpListener(void (*Listener)(void*), void* addParam);


		~Button();


	private:

		//static void LeftMouseUpCallback(void* buttonArg) {
		//	assert(buttonArg != nullptr);

		//	Button* button = static_cast<Button*>(buttonArg);

		//	WindowCoordinates cursorPos = button->osWindow.CursorPos();
		//	if (cursorPos.x >= button->pos.x && cursorPos.x <= button->pos.x + button->size.x &&
		//		cursorPos.y >= button->pos.y && cursorPos.y <= button->pos.y + button->size.y) {

		//		for (int i = 0; i < button->leftMouseUpListeners.size(); ++i) {
		//			std::pair<void (*)(void*), void*>& curListener = button->leftMouseUpListeners[i];
		//			curListener.first(curListener.second);
		//		}
		//	}
		//}

		//std::vector<std::pair<void (*)(void*), void*>> leftMouseUpListeners;

		ButtonProps props;

		Rectangle* rectangle = nullptr;
		Text* label = nullptr;
	};


	class Graph : public Window {
	public:

		class Diagram {
		public:

			Diagram(Graph& graph, OSWindow& window, const size_t width = 1, const Color& color = Color(0, 0, 0));

			void AddData(int column, int value);

			virtual void Draw();

		private:

			Graph& graph;

			struct DiagramData {
				DiagramData() = default;

				DiagramData(int column, int value);

				int column = 0, value = 0;
			};

			std::vector<DiagramData> data;

			Polyline polyline;
		};


		Graph(OSWindow& window, const GraphProps& props,
			const Vector2& pos, const Vector2& size);

		Graph(const Graph& other) = delete;


		Diagram* CreateDiagram(const size_t lineWidth = 1, const Color& color = Color(0, 0, 0));

		void Draw();

		~Graph();

	private:

		void InitGraphParts();

		void InitArrows(const int arrowBackOffset, const int arrowSideOffset);

		void InitLabels(const int rowLabelsOffset, const int columnLabelsOffset);


		GraphProps props;

		std::vector<Diagram*> diagrams;

		Rectangle* background = nullptr;
		Polyline* axes = nullptr;
		std::vector<Text*> labels;
		std::vector<Line*> hatches;

		Vector2 innerPos;    // Position without axes and labels
		Vector2 innerSize;    // Size of graph without axes and labels
	};


	class Application {
	public:

		Application();

		Application(const Application& other) = delete;

		OSWindow* CreateWindow(const int width = 640, const int height = 420, const char* name = "Window",
			const Color& backgroundColor = Color(0, 0, 0));

		size_t WindowsOpened() const;

		void CloseWindow(OSWindow* window);

		void ProcessEventsWait() const;

		void ProcessEvents() const;

		~Application();

	private:

		std::unordered_set<OSWindow*> osWindows;
	};


	class OSWindow {
	public:

		OSWindow(Application& application, const int width = 640, const int height = 420,
			const char* name = "Window", const Color& desktopColor = Color(0, 0, 0));

		OSWindow(const OSWindow& other) = delete;

		size_t Width() const;

		size_t Height() const;


		/*Rectangle* CreateRectangle(const Vector2& pos = Vector2(0, 0),
								   const Vector2& size = Vector2(200, 100), const Color& color = Color(0, 0, 0));

		Button* CreateButton(const Vector2& pos = Vector2(0, 0),
							 const Vector2& size = Vector2(100, 50), const Color& color = Color(0.7, 0.7, 0.7));

		Line* CreateLine(const Vector2& begPos = Vector2(10, 10),
						 const Vector2& endPos = Vector2(100, 10),
						 const size_t width = 1, const Color& color = Color(0, 0, 0));

		Polyline* CreatePolyline(const std::vector<Vector2>& verteces, const size_t width = 1,
								 const Color& color = Color(0, 0, 0));

		Polyline* CreatePolyline(const size_t width = 1, const Color& color = Color(0, 0, 0));

		Text* CreateText(const char* content, const Vector2& pos = Vector2(0, 0),
						 const size_t fontSize = 16, const Color& color = Color(0, 0, 0));

		Graph* CreateGraph(const GraphProps& props = GraphProps(),
						   const Vector2& pos = Vector2(0, 0),
						   const Vector2& size = Vector2(100, 100),
						   const Color& bgColor = Color(0.9, 0.9, 0.9));*/


		void Update();

		void SetActive() const;


		Vector2 CursorPos() const;


		Application& GetApplication() const;

		DesktopWindow* GetDesktop() const;


		~OSWindow();


	private:

		void InitCallbacks();

		static void WindowCloseCallback(GLFWwindow* glfwWindow) {
			assert(glfwWindow != nullptr);

			OSWindow* window = static_cast<OSWindow*>(glfwGetWindowUserPointer(glfwWindow));
			assert(window != nullptr);

			Event closeEvent(Event::window_close);

			if (!closeEvent.Stopped()) {
				window->desktop->HandleEvent(closeEvent);
			}
		}


		static void MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
			assert(glfwWindow != nullptr);

			OSWindow* window = static_cast<OSWindow*>(glfwGetWindowUserPointer(glfwWindow));
			assert(window != nullptr);

			MouseButtonProps mouseProps{ window->CursorPos() };
			switch (button) {
			case GLFW_MOUSE_BUTTON_LEFT:
				mouseProps.button = MouseButtonProps::left;
				break;

			case GLFW_MOUSE_BUTTON_RIGHT:
				mouseProps.button = MouseButtonProps::right;
				break;

			case GLFW_MOUSE_BUTTON_MIDDLE:
				mouseProps.button = MouseButtonProps::middle;
				break;

			default:
				mouseProps.button = MouseButtonProps::unknown;
			}

			Event mouseEvent((GLFW_RELEASE == action ? Event::mouse_up : Event::mouse_down), mouseProps);

			if (!mouseEvent.Stopped()) {
				window->desktop->HandleEvent(mouseEvent);
			}
		}


		static void MouseMoveCallback(GLFWwindow* glfwWindow, double xpos, double ypos) {
			assert(glfwWindow != nullptr);

			OSWindow* window = static_cast<OSWindow*>(glfwGetWindowUserPointer(glfwWindow));
			assert(window != nullptr);

			MouseMoveProps mousePosProps{ window->CursorPos() };

			Event mouseEvent(Event::mouse_move, mousePosProps);

			if (!mouseEvent.Stopped()) {
				window->desktop->HandleEvent(mouseEvent);
			}
		}


		static void KeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
			assert(glfwWindow != nullptr);

			OSWindow* window = static_cast<OSWindow*>(glfwGetWindowUserPointer(glfwWindow));
			assert(window != nullptr);

			KeyProps keyProps{ static_cast<KeyProps::Keys>(key) };

			Event keyEvent((GLFW_RELEASE == action ? Event::key_up :
			               (GLFW_PRESS == action ? Event::key_down : Event::key_repeat)),
			               keyProps);

			if (!keyEvent.Stopped()) {
				window->desktop->HandleEvent(keyEvent);
			}
		}


		static void ScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
			assert(glfwWindow != nullptr);

			OSWindow* window = static_cast<OSWindow*>(glfwGetWindowUserPointer(glfwWindow));
			assert(window != nullptr);

			ScrollProps scrollProps{ Vector2(static_cast<int>(xoffset), static_cast<int>(yoffset)),
									 window->CursorPos() };

			Event scrollEvent(Event::scroll, scrollProps);

			if (!scrollEvent.Stopped()) {
				window->desktop->HandleEvent(scrollEvent);
			}
		}


		Application& application;

		GLFWwindow* glfwWindow = nullptr;

		DesktopWindow* desktop = nullptr;

		//std::vector<Line*> lines;
		//std::vector<Polyline*> polylines;
		//std::vector<Rectangle*> rectangles;
		//std::vector<Text*> texts;
		//std::vector<Button*> buttons;
		//std::vector<Graph*> graphs;

		size_t width = 0;
		size_t height = 0;
		char* name = nullptr;
	};
}