#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "GraphicsEngine.h"


template<typename Arr>
void DeleteArrayElements(Arr arr) {
	for (int i = 0; i < arr.size(); ++i)
		delete arr[i];
}


namespace GUI {

	/* Exceptions */

	bad_init::bad_init() {
		const char* message = "Bad initialization exception";

		_what = new char[strlen(message) + 1];
		strcpy(_what, message);
	}


	bad_init::bad_init(const char* message) {
		assert(message != nullptr);

		_what = new char[strlen(message) + 1];
		strcpy(_what, message);
	}


	char* bad_init::what() {
		return _what;
	}



	/* Additional functions */

	struct GlCoordinates {
		GLfloat x = 0;
		GLfloat y = 0;
	};

	GlCoordinates OSWindowToGlCoords(const OSWindow& window, const Vector2& coords) {
		float resX = static_cast<float>(coords.x) / window.Width() * 2 - 1;
		float resY = -(static_cast<float>(coords.y) / window.Height() * 2 - 1);

		return { resX, resY };
	}


	constexpr int Log10Compile(const int n) {
		return ((n < 10) ? 1 : 1 + Log10Compile(n / 10));
	}


	int Round(double val) {
		int floor = static_cast<int>(val);
		return (val - floor < 0.5) ? floor : floor + 1;
	}


	/* Structures */

	Vector2::Vector2(int x, int y) : x(x), y(y) {}



	/* Color implementation */

	Color::Color() = default;

	Color::Color(const float red, const float green, const float blue) : RGB{ red, green, blue } {
		if (red > 1.0 || red < 0.0)
			throw std::invalid_argument("Invalid value of red argument");

		else if (green > 1.0 || green < 0.0)
			throw std::invalid_argument("Invalid value of green argument");

		else if (blue > 1.0 || blue < 0.0)
			throw std::invalid_argument("Invalid value of blue argument");
	}


	Color::Color(const char* color) {
		const std::map<const char*, const RGB_t> strColors{
			{"black", RGB_t{0.0, 0.0, 0.0}},
			{"red", RGB_t{1.0, 0.0, 0.0}},
			{"green", RGB_t{0.0, 1.0, 0.0}},
			{"blue", RGB_t{0.0, 0.0, 1.0}},
			{"yellow", RGB_t{1.0, 1.0, 0.0}},
			{"magenta", RGB_t{1.0, 0.0, 1.0}},
			{"cyan", RGB_t{0.0, 1.0, 1.0}},
			{"white", RGB_t{1.0, 1.0, 1.0}},
		};

		bool colorFound = false;
		for (auto& curStrColor : strColors) {
			if (!strcmp(color, curStrColor.first)) {
				RGB = curStrColor.second;
				colorFound = true;
				break;
			}
		}

		if (!colorFound) {
			throw std::invalid_argument("Invalid value of color argument");
		}
	}


	Color::Color(const Color& other) {
		RGB = other.RGB;
	}


	Color& Color::operator=(const Color& other) {
		RGB = other.RGB;

		return *this;
	}


	float Color::operator[](const char* color) const {
		if (!strcmp(color, "red") || !strcmp(color, "R"))
			return RGB.red;

		else if (!strcmp(color, "green") || !strcmp(color, "G"))
			return RGB.green;

		else if (!strcmp(color, "blue") || !strcmp(color, "B"))
			return RGB.blue;

		else
			throw std::invalid_argument("Invalid value of color argument");
	}


	float Color::Redness() const {
		return RGB.red;
	}


	float Color::Greenness() const {
		return RGB.green;
	}


	float Color::Blueness() const {
		return RGB.blue;
	}



	/* Event implementation */

	Event::Event(Type type) : type(type) {
		assert(window_close == type);
	}


	Event::Event(Type type, const MouseButtonProps& mouseButtonProps) : type(type), mouseButtonProps(mouseButtonProps) {
		assert(mouse_up == type || mouse_down == type);
	}


	Event::Event(Type type, const MouseMoveProps& mouseMoveProps) : type(type), mouseMoveProps(mouseMoveProps) {
		assert(mouse_move == type);
	}


	Event::Event(Type type, const MouseHoverProps& mouseHoverProps) : type(type), mouseHoverProps(mouseHoverProps) {
		assert(mouse_hover == type);
	}


	Event::Event(Type type, const KeyProps& keyProps) : type(type), keyProps(keyProps) {
		assert(key_up == type || key_down == type);
	}


	Event::Event(Type type, const ScrollProps& scrollProps) : type(type), scrollProps(scrollProps) {
		assert(scroll == type);
	}


	void Event::Stop() {
		stopped = true;
	}


	bool Event::Stopped() const {
		return stopped;
	}


	Event::Type Event::GetType() const {
		return type;
	}


	/* Props implementation */

	ButtonProps::ButtonProps(const Color& color) : color(color) {}

	GraphProps::GraphProps(const int startX, const int rangeX, const int startY, const int rangeY)
		: startX(startX), rangeX(rangeX), startY(startY), rangeY(rangeY) {}


	LineProps::LineProps(const Vector2& firstPoint, const Vector2& secondPoint, const size_t width, const Color& color)
		: firstPoint(firstPoint), secondPoint(secondPoint), width(width), color(color) {}


	PolylineProps::PolylineProps(const std::vector<Vector2>& verteces, const size_t width, const Color& color)
		: verteces(verteces), width(width), color(color) {}

	PolylineProps::PolylineProps(const size_t width, const Color& color) : width(width), color(color) {}


	RectangleProps::RectangleProps(const Vector2& pos, const Vector2& size, const Color& color)
		: pos(pos), size(size), color(color) {}


	TextProps::TextProps(const std::string& content, const Vector2& pos, const size_t fontSize, const Color& color)
		: content(content), pos(pos), fontSize(fontSize), color(color) {}



	/* Application implementation */

	Application::Application() {
		if (!glfwInit())
			throw bad_init("Error while initializing Application)");
	}


	OSWindow* Application::CreateWindow(const int width, const int height, const char* name,
		const Color& backgroundColor) {
		assert(width >= 0);
		assert(height >= 0);
		assert(name != nullptr);

		OSWindow* newWindow = new OSWindow(*this, width, height, name, backgroundColor);
		osWindows.insert(newWindow);

		return newWindow;
	}


	size_t Application::WindowsOpened() const {
		return osWindows.size();
	}


	void Application::CloseWindow(OSWindow* window) {
		if (window == nullptr)
			return;

		std::unordered_set<OSWindow*>::iterator windowIter = osWindows.find(window);
		if (osWindows.end() == windowIter)
			throw std::invalid_argument("Passed window not found.");

		delete *windowIter;
		osWindows.erase(windowIter);
	}


	void Application::ProcessEventsWait() const {
		glfwWaitEvents();
	}


	void Application::ProcessEvents() const {
		glfwPollEvents();
	}


	Application::~Application() {
		std::unordered_set<OSWindow*>::iterator windowsIter = osWindows.begin();
		while (windowsIter != osWindows.end()) {
			delete* windowsIter;
			++windowsIter;
		}

		glfwTerminate();
	}



	/* OSWindow Implementation */

	OSWindow::OSWindow(Application& application, const int width, const int height,
	                   const char* name, const Color& desktopColor)
		: application(application), width(width), height(height) {

		assert(width >= 0);
		assert(height >= 0);
		assert(name != nullptr);

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
		glfwWindow = glfwCreateWindow(width, height, name, NULL, NULL);
		if (!glfwWindow) {
			glfwTerminate();
			throw bad_init("Error occurred while creating the window");
		}
		glfwSetWindowUserPointer(glfwWindow, this);

		InitCallbacks();

		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);

		desktop = new DesktopWindow(*this, Vector2(0, 0), Vector2(width, height), desktopColor);
		desktop->Draw();
		Update();
	}


	size_t OSWindow::Width() const {
		return width;
	}


	size_t OSWindow::Height() const {
		return height;
	}


	/*Rectangle* OSWindow::CreateRectangle(const Vector2& pos, const Vector2& size, const Color& color) {

		Rectangle* newRect = new Rectangle(*this, pos, size, color);
		rectangles.push_back(newRect);

		return newRect;
	}


	Line* OSWindow::CreateLine(const Vector2& begPos, const Vector2& endPos,
	                         const size_t width, const Color& color) {

		Line* newLine = new Line(*this, begPos, endPos, width, color);
		lines.push_back(newLine);

		return newLine;
	}


	Polyline* OSWindow::CreatePolyline(const std::vector<Vector2>& verteces,
	                                 const size_t width, const Color& color) {

		Polyline* newPolyline = new Polyline(*this, verteces, width, color);
		polylines.push_back(newPolyline);

		return newPolyline;
	}


	Polyline* OSWindow::CreatePolyline(const size_t width, const Color& color) {

		Polyline* newPolyline = new Polyline(*this, width, color);
		polylines.push_back(newPolyline);

		return newPolyline;
	}


	Text* OSWindow::CreateText(const char* content, const Vector2& pos,
	                         const size_t fontSize, const Color& color) {

		SetActive();

		Text* newText = new Text(*this, content, pos, fontSize, color);
		texts.push_back(newText);

		return newText;
	}


	Button* OSWindow::CreateButton(const Vector2& pos, const Vector2& size, const Color& color) {

		Button* newButton = new Button(*this, pos, size, color);
		buttons.push_back(newButton);

		return newButton;
	}


	Graph* OSWindow::CreateGraph(const GraphProps& props, const Vector2& pos,
	                             const Vector2& size, const Color& bgColor) {

		Graph* newGraph = new Graph(*this, props, pos, size, bgColor);
		graphs.push_back(newGraph);

		return newGraph;
	}*/


	/*void OSWindow::Draw() {
		SetActive();

		glClearColor(desktopColor.RedAmt(), desktopColor.GreenAmt(), desktopColor.BlueAmt(), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto curObj : lines)
			curObj->Draw();
		for (auto curObj : polylines)
			curObj->Draw();
		for (auto curObj : rectangles)
			curObj->Draw();
		for (auto curObj : texts)
			curObj->Draw();
		for (auto curObj : buttons)
			curObj->Draw();
		for (auto curObj : graphs)
			curObj->Draw();

		glfwSwapBuffers(window);
	}*/


	void OSWindow::Update() {
		SetActive();
		glfwSwapBuffers(glfwWindow);
	}


	void OSWindow::SetActive() const {
		assert(glfwWindow != nullptr);

		if (glfwGetCurrentContext() != glfwWindow) {
			glfwMakeContextCurrent(glfwWindow);
		}
	}


	Vector2 OSWindow::CursorPos() const {
		double posX = 0, posY = 0;
		glfwGetCursorPos(glfwWindow, &posX, &posY);

		return Vector2(static_cast<int>(posX), static_cast<int>(posY));
	}


	Application& OSWindow::GetApplication() const {
		return application;
	}


	DesktopWindow* OSWindow::GetDesktop() const {
		return desktop;
	}


	void OSWindow::InitCallbacks() {
		glfwSetWindowCloseCallback(glfwWindow, WindowCloseCallback);
		glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
		glfwSetCursorPosCallback(glfwWindow, MouseMoveCallback);
		glfwSetKeyCallback(glfwWindow, KeyCallback);
		glfwSetScrollCallback(glfwWindow, ScrollCallback);
	}


	OSWindow::~OSWindow() {
		//DeleteArrayElements(lines);
		//DeleteArrayElements(polylines);
		//DeleteArrayElements(rectangles);
		//DeleteArrayElements(texts);
		//DeleteArrayElements(buttons);
		//DeleteArrayElements(graphs);

		delete desktop;
		glfwDestroyWindow(glfwWindow);
	}



	/* Element inplementation */

	Element::Element(OSWindow& osWindow) : osWindow(osWindow) {}



	/* Window implementation */

	Window::Window(OSWindow& osWindow, const Vector2& pos, const Vector2& size)
		: Element(osWindow), pos(pos), size(size) {}


	Window* Window::CreateWindow(const Type type, const WindowProps& props,
	                             const Vector2& pos, const Vector2& size) {

		Window* newWindow = nullptr;

		try {
			switch (type) {
			case button:
				try {
					newWindow = new Button(osWindow, dynamic_cast<const ButtonProps&>(props), pos, size);

				} catch (std::bad_cast&) {
					throw std::invalid_argument("Button is to create but props are not of ButtonProps type.");
				}
				break;

			case graph:
				try {
					newWindow = new Graph(osWindow, dynamic_cast<const GraphProps&>(props), pos, size);

				} catch (std::bad_cast&) {
					throw std::invalid_argument("Button is to create but props are not of ButtonProps type.");
				}
				break;

			default:
				throw std::invalid_argument("Unknown window type passed.");
			}

		} catch (std::bad_cast&) {
			throw std::invalid_argument("Props type doesn't match to window type.");
		}

		windows.insert(newWindow);
		
		return newWindow;
	}


	Primitive* Window::CreatePrimitive(const Primitive::Type type, const PrimitiveProps& props) {

		Primitive* newPrimitive = nullptr;

		try {
			switch (type) {
			case Primitive::Type::line:
				newPrimitive = new Line(osWindow, dynamic_cast<const LineProps&>(props));
				break;

			case Primitive::Type::polyline:
				newPrimitive = new Polyline(osWindow, dynamic_cast<const PolylineProps&>(props));
				break;

			case Primitive::Type::rectangle:
				newPrimitive = new Rectangle(osWindow, dynamic_cast<const RectangleProps&>(props));
				break;

			case Primitive::Type::text:
				newPrimitive = new Text(osWindow, dynamic_cast<const TextProps&>(props));
				break;

			default:
				throw std::invalid_argument("Unknown window type passed.");
			}

		} catch (std::bad_cast&) {
			throw std::invalid_argument("Props type doesn't match to primitive type.");
		}

		primitives.insert(newPrimitive);

		return newPrimitive;
	}


	void Window::RemoveWindow(Window* window) {
		delete window;
		windows.erase(window);
	}


	void Window::AddEventListener(Event::Type type, void(*listener)(Event&, void*), void* additParam) {
		assert(type != Event::Type::unknown);

		eventsListeners[type].emplace(listener, additParam);
	}


	void Window::RemoveEventListener(Event::Type type, void (*listener)(Event&, void*)) {
		assert(type != Event::Type::unknown);

		for (auto& curListener : eventsListeners[type]) {
			if (curListener.first == listener) {
				eventsListeners[type].erase(curListener);
			}
		}
	}


	void Window::HandleEvent(Event& event) {
		Event::Type type = event.GetType();
		if (Event::mouse_down == type || Event::mouse_up == type ||
		    Event::mouse_move == type || Event::scroll == type) {

			Vector2 mousePos;
			if (Event::scroll == type) {
				mousePos = event.scrollProps.pos;
			} else if (Event::mouse_move == type) {
				mousePos = event.mouseMoveProps.pos;

				HandleHoverEvent(event.mouseMoveProps);
			} else {
				mousePos = event.mouseButtonProps.pos;
			}

			if (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
				mousePos.y >= pos.y && mousePos.y <= pos.y + size.y) {
				for (auto& curListener : eventsListeners[type]) {
					curListener.first(event, curListener.second);

					if (event.Stopped()) {
						return;
					}
				}
			}

		} else {
			for (auto& curListener : eventsListeners[type]) {
				curListener.first(event, curListener.second);

				if (event.Stopped()) {
					return;
				}
			}
		}

		for (Window* curWindow : windows) {
			curWindow->HandleEvent(event);

			if (event.Stopped()) {
				return;
			}
		}
	}


	Window::~Window() {
		for (Window* window : windows) {
			delete window;
		}
		for (Primitive* primitive : primitives) {
			delete primitive;
		}
	}


	void Window::DrawInsides() {
		for (Window* window : windows) {
			window->Draw();
		}
		for (Primitive* primitive : primitives) {
			primitive->Draw();
		}
	}


	void Window::HandleHoverEvent(const MouseMoveProps& mouseMoveProps) {
		Vector2 mousePos = mouseMoveProps.pos;

		bool hoverChanged = false;
		MouseHoverProps hoverProps;
		if (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
			mousePos.y >= pos.y && mousePos.y <= pos.y + size.y) {
			if (!hovered) {
				hoverProps.hoverType = MouseHoverProps::HoverType::hovered;
				hoverChanged = true;
			}
		} else {
			if (hovered) {
				hoverProps.hoverType = MouseHoverProps::HoverType::unhovered;
				hoverChanged = true;
			}
		}

		if (hoverChanged) {
			hovered = !hovered;

			Event hoverEvent(Event::Type::mouse_hover, hoverProps);

			for (auto& curListener : eventsListeners[Event::Type::mouse_hover]) {
				curListener.first(hoverEvent, curListener.second);

				if (hoverEvent.Stopped()) {
					return;
				}
			}
		}
	}



	/* DesktopWindow implementation */

	DesktopWindow::DesktopWindow(OSWindow& osWindow, const Vector2 pos, const Vector2 size, const Color& color)
		: Window(osWindow, pos, size), color(color) {}


	void DesktopWindow::Draw() {
		osWindow.SetActive();

		glClearColor(color.Redness(), color.Greenness(), color.Blueness(), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		DrawInsides();

		osWindow.Update();
	}



	/* Object implementation */

	Primitive::Primitive(OSWindow& osWindow) : Element(osWindow) {}



	/* Line implementation */

	Line::Line(OSWindow& osWindow, const LineProps& props)
		: Primitive(osWindow), props(props) {}


	void Line::Draw() {
		GlCoordinates begPosGl = OSWindowToGlCoords(osWindow, props.firstPoint);
		GlCoordinates endPosGl = OSWindowToGlCoords(osWindow, props.secondPoint);

		osWindow.SetActive();

		glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());
		glLineWidth(static_cast<GLfloat>(props.width));

		glBegin(GL_LINES);
		glVertex2f(begPosGl.x, begPosGl.y);
		glVertex2f(endPosGl.x, endPosGl.y);
		glEnd();
	}



	/* Polyline implementation */

	Polyline::Polyline(OSWindow& osWindow, const PolylineProps props)
		: Primitive(osWindow), props(props) {}


	void Polyline::Draw() {
		if (props.verteces.size() >= 2) {
			osWindow.SetActive();

			glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());
			glLineWidth(static_cast<GLfloat>(props.width));

			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < props.verteces.size(); ++i) {
				GlCoordinates curGlCoords = OSWindowToGlCoords(osWindow, props.verteces[i]);
				glVertex2f(curGlCoords.x, curGlCoords.y);
			}
			glEnd();
		}
	}


	void Polyline::AddVertex(const Vector2& pos) {
		props.verteces.push_back(pos);
	}



	/* Rectangle implementation */

	Rectangle::Rectangle(OSWindow& osWindow, const RectangleProps& props)
		: Primitive(osWindow), props(props) {}


	void Rectangle::Draw() {
		GlCoordinates begPosGl =
			OSWindowToGlCoords(osWindow, props.pos);
		GlCoordinates endPosGl =
			OSWindowToGlCoords(osWindow, Vector2(props.pos.x + props.size.x, props.pos.y + props.size.y));

		osWindow.SetActive();

		glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());

		glBegin(GL_QUADS);
		glVertex2f(begPosGl.x, begPosGl.y);
		glVertex2f(endPosGl.x, begPosGl.y);
		glVertex2f(endPosGl.x, endPosGl.y);
		glVertex2f(begPosGl.x, endPosGl.y);
		glEnd();
	}



	/* Text implementation */

	size_t Text::instanceCount = 0;
	BMP_Img* Text::bitmapImg = nullptr;
	size_t Text::charWidth = 0;
	size_t Text::charHeight = 0;

	Text::Text(OSWindow& osWindow, const TextProps& props) : Primitive(osWindow), props(props) {

		if (0 == instanceCount) {
			const char bitmapFileName[] = "../Fonts/ASCII.bmp";
			const size_t charWidth = 16;
			const size_t charHeight = 16;

			InitFont(bitmapFileName, charWidth, charHeight);
		}

		++instanceCount;
	}


	void Text::DrawChar(const char ch, const Vector2& pos) {
		//GlCoordinates glPos = WindowToGlCoords(window, pos);
		//glRasterPos2f(glPos.x, glPos.y);

		Vector2 bmpCharPos = BmpCharPos(ch);

		const char* bmpCharPtr = bitmapImg->ImagePointer() + bmpCharPos.y * bitmapImg->Width() + bmpCharPos.x;
		/*glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmapImg->Width());

		glDrawPixels(charWidth, charHeight, GL_ALPHA, GL_UNSIGNED_BYTE, reinterpret_cast<const GLubyte*>(charPtr));

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);*/

		float scale = static_cast<float>(props.fontSize) / charWidth;

		glPointSize(1.0);
		glColor3f(props.color.Redness(), props.color.Greenness(), props.color.Blueness());

		glBegin(GL_POINTS);
		for (int curY = 0; curY < charHeight * scale; ++curY) {
			for (int curX = 0; curX < props.fontSize; ++curX) {

				int pxOffsetX = Round(curX / scale);
				int pxOffsetY = Round(curY / scale);
				if (pxOffsetX > charWidth - 1)
					pxOffsetX = charWidth - 1;
				if (pxOffsetY > charHeight - 1)
					pxOffsetY = charHeight - 1;

				char curPx = *(bmpCharPtr + pxOffsetY * bitmapImg->Width() + pxOffsetX);

				if (curPx != 0) {
					GlCoordinates curPos =
						OSWindowToGlCoords(osWindow, Vector2(pos.x + curX,
						                                     pos.y + charHeight * scale - 1 - curY));

					glVertex2f(curPos.x, curPos.y);
				}

			}
		}
		glEnd();
	}


	void Text::Draw() {
		osWindow.SetActive();

		Vector2 curPos(props.pos.x, props.pos.y);
		for (int i = 0; i < props.content.size(); ++i) {
			char curChar = props.content[i];

			DrawChar(curChar, curPos);
			curPos.x += props.fontSize;
		}
	}


	Vector2 Text::BmpCharPos(const unsigned char ch) const {
		size_t bmpWidth = bitmapImg->Width();
		size_t bmpHeight = bitmapImg->Height();

		size_t nColumns = bmpWidth / charWidth;
		size_t nRows = bmpHeight / charHeight;

		size_t chColumn = ch % nColumns + 1;
		size_t chRow = ch / nColumns + 1;

		Vector2 res = { 0, 0 };
		res.x = (chColumn - 1) * charWidth;
		res.y = bmpHeight - chRow * charHeight;

		return res;
	}


	Text::~Text() {
		assert(instanceCount >= 1);

		if (instanceCount == 1)
			DestroyFont();

		--instanceCount;
	}


	void Text::InitFont(const char bitmapFileName[], const size_t chWidth, const size_t chHeight) {
		assert(bitmapFileName != nullptr);
		assert(chWidth > 0);
		assert(chHeight > 0);

		assert(nullptr == bitmapImg);

		std::ifstream bitmapFile(bitmapFileName, std::ios::binary);
		if (!bitmapFile.is_open())
			throw bad_init("Error while initializing font");

		bitmapImg = new BMP_Img(bitmapFile);

		bitmapFile.close();

		charWidth = chWidth;
		charHeight = chHeight;
	}


	void Text::DestroyFont() {
		assert(bitmapImg != nullptr);

		delete bitmapImg;
		charWidth = 0;
		charHeight = 0;
	}



	/* Button implementation */

	Button::Button(OSWindow& osWindow, const ButtonProps& props,
	               const Vector2& pos, const Vector2& size)
		: Window(osWindow, pos, size), props(props) {
		
		rectangle = new Rectangle(osWindow, RectangleProps(pos, size, props.color));
	}


	Text* Button::AddLabel(const std::string label, const Vector2& labelPos,
	                       const size_t fontSize, const Color& labelColor) {

		Vector2 globalPos(labelPos.x + pos.x, labelPos.y + pos.y);
		this->label = new Text(osWindow, TextProps(label, globalPos, fontSize, labelColor));

		return this->label;
	}


	Color Button::GetColor() const {
		return props.color;
	}


	//void Button::AddLeftMouseUpListener(void(*Listener)(void*), void* addParam) {
	//	leftMouseUpListeners.push_back(std::pair<void (*)(void*), void*>(Listener, addParam));
	//}


	void Button::Draw() {
		osWindow.SetActive();

		rectangle->Draw();
		label->Draw();
	}


	Button::~Button() {
		delete rectangle;
		delete label;
	}



	/* Graph implementation */

	Graph::Diagram::Diagram(Graph& graph, OSWindow& window, const size_t width, const Color& color)
		: graph(graph), polyline(window, PolylineProps(width, color)) {}


	void Graph::Diagram::AddData(int column, int value) {
		int curVertexX = graph.innerPos.x +
			(column - graph.props.startX) / static_cast<float>(graph.props.rangeX) * graph.innerSize.x;

		int curVertexY = graph.innerPos.y + graph.innerSize.y -
			(value - graph.props.startY) / static_cast<float>(graph.props.rangeY) * graph.innerSize.y;

		data.push_back(DiagramData(column, value));
		polyline.AddVertex(Vector2(curVertexX, curVertexY));
	}


	void Graph::Diagram::Draw() {
		polyline.Draw();
	}


	Graph::Graph(OSWindow& osWindow, const GraphProps& props, const Vector2& pos, const Vector2& size)
		: Window(osWindow, pos, size), props(props) {

		const size_t axesWidth = 3;

		background = new Rectangle(osWindow, RectangleProps(pos, size, props.bgColor));

		InitGraphParts();
	}


	Graph::Diagram::DiagramData::DiagramData(int column, int value) : column(column), value(value) {}

	Graph::Diagram* Graph::CreateDiagram(const size_t lineWidth, const Color& color) {
		Diagram* newDiag = new Diagram(*this, osWindow, lineWidth, color);

		diagrams.push_back(newDiag);

		return newDiag;
	}


	Graph::~Graph() {
		delete axes;
		delete background;
		DeleteArrayElements(labels);
		DeleteArrayElements(hatches);

		DeleteArrayElements(diagrams);
	}


	void Graph::InitGraphParts() {
		constexpr size_t maxIntStrLen = Log10Compile(INT_MAX) + 2;
		char tempBuf1[maxIntStrLen + 1] = "";
		char tempBuf2[maxIntStrLen + 1] = "";

		const int innerPadding = size.x / 15;
		const int arrowFrontOffset = innerPadding / 1.3;
		const int arrowSideOffset = arrowFrontOffset / 3;

		const int hatchSize = 3;

		const int rowLabelsOffset = props.fontSize + 20;
		const int columnLabelsOffset = std::max(strlen(itoa(props.startX, tempBuf1, 10)),
			strlen(itoa(props.startX + props.rangeX, tempBuf2, 10))) *
			props.fontSize + props.fontSize * 2;

		innerPos.x = pos.x + std::max(strlen(itoa(props.startY, tempBuf1, 10)),
			strlen(itoa(props.startY + props.rangeY, tempBuf2, 10))) * props.fontSize +
			props.hatchSize;

		innerSize.x = size.x - (innerPos.x - pos.x) - innerPadding;
		innerPos.y = pos.y + innerPadding;
		innerSize.y = size.y - props.fontSize - props.hatchSize - innerPadding;

		InitArrows(arrowFrontOffset, arrowSideOffset);

		InitLabels(rowLabelsOffset, columnLabelsOffset);
	}


	void Graph::InitArrows(const int arrowFrontOffset, const int arrowSideOffset) {

		Vector2 innerEndPos(innerPos.x + innerSize.x, innerPos.y + innerSize.y);

		axes = new Polyline(osWindow, PolylineProps(props.axesWidth, props.axesColor));
		axes->AddVertex(Vector2(innerPos.x, innerPos.y));
		axes->AddVertex(Vector2(innerPos.x - arrowSideOffset, innerPos.y));
		axes->AddVertex(Vector2(innerPos.x, innerPos.y - arrowFrontOffset));
		axes->AddVertex(Vector2(innerPos.x + arrowSideOffset, innerPos.y));
		axes->AddVertex(Vector2(innerPos.x, innerPos.y));
		axes->AddVertex(Vector2(innerPos.x, innerEndPos.y));

		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y));
		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y - arrowSideOffset));
		axes->AddVertex(Vector2(innerEndPos.x + arrowFrontOffset, innerEndPos.y));
		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y + arrowSideOffset));
		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y));
	}


	void Graph::InitLabels(const int rowLabelsOffset, const int columnLabelsOffset) {
		constexpr size_t maxIntStrLen = Log10Compile(INT_MAX) + 2;
		char tempBuf[maxIntStrLen + 1] = "";

		int curY = props.fontSize / 2;
		while (curY < innerSize.y) {
			int curLabelVal = (curY - props.fontSize / 2) / static_cast<float>(innerSize.y) *
				props.rangeY + props.startY;

			int curYWindow = innerPos.y + innerSize.y - curY;

			Text* curLabel = new Text(osWindow, TextProps(itoa(curLabelVal, tempBuf, 10),
				Vector2(pos.x, curYWindow), props.fontSize, props.fontColor));

			Line* curHatch = new Line(osWindow, LineProps(Vector2(innerPos.x, curYWindow + props.fontSize / 2),
				Vector2(innerPos.x - props.hatchSize, curYWindow + props.fontSize / 2),
				props.axesWidth, props.axesColor));

			labels.push_back(curLabel);
			hatches.push_back(curHatch);

			curY += rowLabelsOffset;
		}


		int curX = 0;
		while (curX < innerSize.x) {
			int curLabelVal = curX / static_cast<float>(innerSize.x) *
				props.rangeX + props.startX;

			int curXWindow = innerPos.x + curX;
			Text* curLabel = new Text(osWindow, TextProps(itoa(curLabelVal, tempBuf, 10),
				Vector2(curXWindow, pos.y + size.y - props.fontSize),
				props.fontSize, props.fontColor));

			Line* curHatch = new Line(osWindow, LineProps(Vector2(curXWindow, innerPos.y + innerSize.y),
				Vector2(curXWindow, innerPos.y + innerSize.y + props.hatchSize),
				props.axesWidth, props.axesColor));

			labels.push_back(curLabel);
			hatches.push_back(curHatch);
			curX += columnLabelsOffset;
		}
	}


	void Graph::Draw() {
		osWindow.SetActive();

		background->Draw();
		axes->Draw();

		for (int i = 0; i < diagrams.size(); ++i)
			diagrams[i]->Draw();

		for (int i = 0; i < labels.size(); ++i)
			labels[i]->Draw();

		for (int i = 0; i < hatches.size(); ++i)
			hatches[i]->Draw();
	}
}