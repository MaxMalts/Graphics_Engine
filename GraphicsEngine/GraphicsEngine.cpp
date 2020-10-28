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

	GlCoordinates WindowToGlCoords(const Window& window, const Coordinates& coords) {
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

	Coordinates::Coordinates(int x, int y) : x(x), y(y) {}

	WindowCoordinates::WindowCoordinates(int x, int y) : x(x), y(y) {}

	Size::Size(size_t width, size_t height) : width(width), height(height) {}



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


	float Color::RedAmt() const {
		return RGB.red;
	}


	float Color::GreenAmt() const {
		return RGB.green;
	}


	float Color::BlueAmt() const {
		return RGB.blue;
	}



	/* Application implementation */

	Application::Application() {
		if (!glfwInit())
			throw bad_init("Error while initializing Application)");
	}


	Window* Application::CreateWindow(const int width, const int height, const char* name,
		const Color& backgroundColor) {
		assert(width >= 0);
		assert(height >= 0);
		assert(name != nullptr);

		Window* newWindow = new Window(*this, width, height, name, backgroundColor);
		windows.insert(newWindow);

		return newWindow;
	}


	size_t Application::WindowsOpened() const {
		return windows.size();
	}


	void Application::CloseWindow(Window* window) {
		if (window == nullptr)
			return;

		std::set<Window*>::iterator windowIter = windows.find(window);
		if (windows.end() == windowIter)
			throw std::invalid_argument("Passed window not found.");

		delete* windowIter;
		windows.erase(windowIter);
	}


	void Application::ProcessEventsWait() const {
		glfwWaitEvents();
	}


	void Application::ProcessEvents() const {
		glfwPollEvents();
	}


	Application::~Application() {
		std::set<Window*>::iterator windowsIter = windows.begin();
		while (windowsIter != windows.end()) {
			delete* windowsIter;
			++windowsIter;
		}

		glfwTerminate();
	}



	/* Window Implementation */

	Window::Window(Application& application, const int width, const int height,
	               const char* name, const Color& backgroundColor)
		: application(application), width(width), height(height), backgroundColor(backgroundColor) {

		assert(width >= 0);
		assert(height >= 0);
		assert(name != nullptr);

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
		window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (!window) {
			glfwTerminate();
			throw bad_init("Error occurred while creating the window");
		}
		glfwSetWindowUserPointer(window, this);
		glfwSetMouseButtonCallback(window, LeftMouseUpCallback);
		glfwSetWindowCloseCallback(window, WindowCloseCallback);

		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
	}


	size_t Window::Width() const {
		return width;
	}


	size_t Window::Height() const {
		return height;
	}


	Rectangle* Window::CreateRectangle(const Coordinates& pos, const Size& size, const Color& color) {

		Rectangle* newRect = new Rectangle(*this, pos, size, color);
		rectangles.push_back(newRect);

		return newRect;
	}


	Line* Window::CreateLine(const Coordinates& begPos, const Coordinates& endPos,
	                         const size_t width, const Color& color) {

		Line* newLine = new Line(*this, begPos, endPos, width, color);
		lines.push_back(newLine);

		return newLine;
	}


	Polyline* Window::CreatePolyline(const std::vector<Coordinates>& verteces,
	                                 const size_t width, const Color& color) {

		Polyline* newPolyline = new Polyline(*this, verteces, width, color);
		polylines.push_back(newPolyline);

		return newPolyline;
	}


	Polyline* Window::CreatePolyline(const size_t width, const Color& color) {

		Polyline* newPolyline = new Polyline(*this, width, color);
		polylines.push_back(newPolyline);

		return newPolyline;
	}


	Text* Window::CreateText(const char* content, const Coordinates& pos,
	                         const size_t fontSize, const Color& color) {

		SetActive();

		Text* newText = new Text(*this, content, pos, fontSize, color);
		texts.push_back(newText);

		return newText;
	}


	Button* Window::CreateButton(const Coordinates& pos, const Size& size, const Color& color) {

		Button* newButton = new Button(*this, pos, size, color);
		buttons.push_back(newButton);

		return newButton;
	}


	Graph* Window::CreateGraph(const GraphProps& props, const Coordinates& pos,
	                           const Size& size, const Color& bgColor) {

		Graph* newGraph = new Graph(*this, props, pos, size, bgColor);
		graphs.push_back(newGraph);

		return newGraph;
	}


	void Window::Draw() {
		SetActive();

		glClearColor(backgroundColor.RedAmt(), backgroundColor.GreenAmt(), backgroundColor.BlueAmt(), 1.0);
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
	}


	void Window::SetActive() const {
		assert(window != nullptr);

		if (glfwGetCurrentContext() != window) {
			glfwMakeContextCurrent(window);
		}
	}


	void Window::AddLeftMouseUpListener(void (*Listener)(void*), void* addParam) {
		leftMouseUpListeners.push_back(std::pair<void (*)(void*), void*>(Listener, addParam));
	}


	void Window::AddWindowCloseListener(void(*Listener)(void*), void* addParam) {
		windowCloseListeners.push_back(std::pair<void (*)(void*), void*>(Listener, addParam));
	}


	WindowCoordinates Window::CursorPos() const {
		double posX = 0, posY = 0;
		glfwGetCursorPos(window, &posX, &posY);

		return WindowCoordinates(static_cast<int>(posX), static_cast<int>(posY));
	}


	Application& Window::GetApplication() const {
		return application;
	}


	Window::~Window() {
		DeleteArrayElements(lines);
		DeleteArrayElements(polylines);
		DeleteArrayElements(rectangles);
		DeleteArrayElements(texts);
		DeleteArrayElements(buttons);
		DeleteArrayElements(graphs);

		glfwDestroyWindow(window);
	}



	/* Line implementation */

	Line::Line(Window& window, const Coordinates& begPos, const Coordinates& endPos,
	           const size_t width, const Color& color)
		: window(window), firstPoint(begPos), secondPoint(endPos), width(width), color(color) {}


	void Line::Draw() {
		GlCoordinates begPosGl = WindowToGlCoords(window, firstPoint);
		GlCoordinates endPosGl = WindowToGlCoords(window, secondPoint);

		window.SetActive();

		glColor3f(color.RedAmt(), color.GreenAmt(), color.BlueAmt());
		glLineWidth(static_cast<GLfloat>(width));

		glBegin(GL_LINES);
		glVertex2f(begPosGl.x, begPosGl.y);
		glVertex2f(endPosGl.x, endPosGl.y);
		glEnd();
	}



	/* Polyline implementation */

	Polyline::Polyline(Window& window, const std::vector<Coordinates>& verteces,
	                   const size_t width, const Color& color)
		: window(window), verteces(verteces), width(width), color(color) {}


	Polyline::Polyline(Window& window, const size_t width, const Color& color)
		: window(window), verteces(), width(width), color(color) {}


	void Polyline::Draw() {
		if (verteces.size() >= 2) {
			window.SetActive();

			glColor3f(color.RedAmt(), color.GreenAmt(), color.BlueAmt());
			glLineWidth(static_cast<GLfloat>(width));

			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < verteces.size(); ++i) {
				GlCoordinates curGlCoords = WindowToGlCoords(window, verteces[i]);
				glVertex2f(curGlCoords.x, curGlCoords.y);
			}
			glEnd();
		}
	}


	void Polyline::AddVertex(const Coordinates& pos) {
		verteces.push_back(pos);
	}



	/* Rectangle implementation */

	Rectangle::Rectangle(Window& window, const Coordinates& pos, const Size& size, const Color& color)
		: window(window), pos(pos), size(size), color(color) {}


	void Rectangle::Draw() {
		GlCoordinates begPosGl = WindowToGlCoords(window, pos);
		GlCoordinates endPosGl = WindowToGlCoords(window, Coordinates(pos.x + size.width, pos.y + size.height));

		window.SetActive();

		glColor3f(color.RedAmt(), color.GreenAmt(), color.BlueAmt());

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

	Text::Text(Window& window, const char* string,
	           const Coordinates& pos, const size_t fontSize, const Color& color)
		: window(window), pos(pos), fontSize(fontSize), color(color) {

		assert(string != nullptr);

		if (0 == instanceCount) {
			const char bitmapFileName[] = "../Fonts/ASCII.bmp";
			const size_t charWidth = 16;
			const size_t charHeight = 16;

			InitFont(bitmapFileName, charWidth, charHeight);
		}

		contentLen = strlen(string);
		content = new char[contentLen + 1];
		strncpy(content, string, contentLen + 1);

		++instanceCount;
	}


	void Text::DrawChar(const char ch, const Coordinates& pos) {
		//GlCoordinates glPos = WindowToGlCoords(window, pos);
		//glRasterPos2f(glPos.x, glPos.y);

		Coordinates bmpCharPos = BmpCharPos(ch);

		const char* bmpCharPtr = bitmapImg->ImagePointer() + bmpCharPos.y * bitmapImg->Width() + bmpCharPos.x;
		/*glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmapImg->Width());

		glDrawPixels(charWidth, charHeight, GL_ALPHA, GL_UNSIGNED_BYTE, reinterpret_cast<const GLubyte*>(charPtr));

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);*/

		float scale = static_cast<float>(fontSize) / charWidth;

		glPointSize(1.0);
		glColor3f(color.RedAmt(), color.GreenAmt(), color.BlueAmt());

		glBegin(GL_POINTS);
		for (int curY = 0; curY < charHeight * scale; ++curY) {
			for (int curX = 0; curX < fontSize; ++curX) {

				int pxOffsetX = Round(curX / scale);
				int pxOffsetY = Round(curY / scale);
				if (pxOffsetX > charWidth - 1)
					pxOffsetX = charWidth - 1;
				if (pxOffsetY > charHeight - 1)
					pxOffsetY = charHeight - 1;

				char curPx = *(bmpCharPtr + pxOffsetY * bitmapImg->Width() + pxOffsetX);

				if (curPx != 0) {
					GlCoordinates curPos =
						WindowToGlCoords(window, Coordinates(pos.x + curX, pos.y + charHeight * scale - 1 - curY));

					glVertex2f(curPos.x, curPos.y);
				}

			}
		}
		glEnd();
	}


	void Text::Draw() {
		window.SetActive();

		Coordinates curPos(pos.x, pos.y);
		for (int i = 0; i < contentLen; ++i) {
			char curChar = content[i];

			DrawChar(curChar, curPos);
			curPos.x += fontSize;
		}
	}


	Coordinates Text::BmpCharPos(const unsigned char ch) const {
		size_t bmpWidth = bitmapImg->Width();
		size_t bmpHeight = bitmapImg->Height();

		size_t nColumns = bmpWidth / charWidth;
		size_t nRows = bmpHeight / charHeight;

		size_t chColumn = ch % nColumns + 1;
		size_t chRow = ch / nColumns + 1;

		Coordinates res = { 0, 0 };
		res.x = (chColumn - 1) * charWidth;
		res.y = bmpHeight - chRow * charHeight;

		return res;
	}


	Text::~Text() {
		assert(instanceCount >= 1);

		delete[] content;

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

	Button::Button(Window& window, const Coordinates& pos, const Size& size, const Color& color)
		: window(window), pos(pos), size(size), color(color) {

		window.AddLeftMouseUpListener(LeftMouseUpCallback, this);
		
		rectangle = new Rectangle(window, pos, size, color);
	}


	Text* Button::AddLabel(const char* string, const Coordinates& labelPos,
		const size_t fontSize, const Color& labelColor) {

		assert(string != nullptr);

		Coordinates globalPos(labelPos.x + pos.x, labelPos.y + pos.y);
		label = new Text(window, string, globalPos, fontSize, labelColor);

		return label;
	}


	Color Button::GetColor() const {
		return color;
	}


	void Button::AddLeftMouseUpListener(void(*Listener)(void*), void* addParam) {
		leftMouseUpListeners.push_back(std::pair<void (*)(void*), void*>(Listener, addParam));
	}


	void Button::Draw() {
		window.SetActive();

		rectangle->Draw();
		label->Draw();
	}


	Button::~Button() {
		delete rectangle;
		delete label;
	}



	/* Graph implementation */

	GraphProps::GraphProps(const int startX, const int rangeX, const int startY, const int rangeY)
		: startX(startX), rangeX(rangeX), startY(startY), rangeY(rangeY) {}


	Graph::Graph(Window& window, const GraphProps& props, const Coordinates& pos, const Size& size,
	             const Color& bgColor)
		: window(window), props(props), pos(pos), size(size), bgColor(bgColor) {

		const size_t axesWidth = 3;

		background = new Rectangle(window, pos, size, bgColor);

		InitGraphParts();
	}


	Graph::DiagramNode::DiagramNode(int column, int value) : column(column), value(value) {}

	int Graph::AddDiagram(const size_t lineWidth, const Color& color) {
		Polyline* polyline = new Polyline(window, lineWidth, color);

		diagrams.push_back(std::pair<std::vector<DiagramNode>, Polyline*>
		                   (std::vector<DiagramNode>(), polyline));

		return diagrams.size() - 1;
	}


	void Graph::AddData(const int diagramInd, int column, int value) {
		if (diagramInd < 0 || diagramInd >= diagrams.size())
			throw std::out_of_range("The diagramInd argument out of range.");

		int curVertexX = innerPos.x +
			(column - props.startX) / static_cast<float>(props.rangeX) * innerSize.width;

		int curVertexY = innerPos.y + innerSize.height -
			(value - props.startY) / static_cast<float>(props.rangeY) * innerSize.height;

		diagrams[diagramInd].first.push_back(DiagramNode(column, value));
		diagrams[diagramInd].second->AddVertex(Coordinates(curVertexX, curVertexY));
	}


	Graph::~Graph() {
		for (int i = 0; i < diagrams.size(); ++i)
			delete diagrams[i].second;

		DeleteArrayElements(labels);
		DeleteArrayElements(hatches);

		delete axes;
		delete background;
	}


	void Graph::InitGraphParts() {
		constexpr size_t maxIntStrLen = Log10Compile(INT_MAX) + 2;
		char tempBuf1[maxIntStrLen + 1] = "";
		char tempBuf2[maxIntStrLen + 1] = "";

		const int innerPadding = size.width / 15;
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

		innerSize.width = size.width - (innerPos.x - pos.x) - innerPadding;
		innerPos.y = pos.y + innerPadding;
		innerSize.height = size.height - props.fontSize - props.hatchSize - innerPadding;

		InitArrows(arrowFrontOffset, arrowSideOffset);

		InitLabels(rowLabelsOffset, columnLabelsOffset);
	}


	void Graph::InitArrows(const int arrowFrontOffset, const int arrowSideOffset) {

		Coordinates innerEndPos(innerPos.x + innerSize.width, innerPos.y + innerSize.height);

		axes = new Polyline(window, props.axesWidth, props.axesColor);
		axes->AddVertex(Coordinates(innerPos.x, innerPos.y));
		axes->AddVertex(Coordinates(innerPos.x - arrowSideOffset, innerPos.y));
		axes->AddVertex(Coordinates(innerPos.x, innerPos.y - arrowFrontOffset));
		axes->AddVertex(Coordinates(innerPos.x + arrowSideOffset, innerPos.y));
		axes->AddVertex(Coordinates(innerPos.x, innerPos.y));
		axes->AddVertex(Coordinates(innerPos.x, innerEndPos.y));

		axes->AddVertex(Coordinates(innerEndPos.x, innerEndPos.y));
		axes->AddVertex(Coordinates(innerEndPos.x, innerEndPos.y - arrowSideOffset));
		axes->AddVertex(Coordinates(innerEndPos.x + arrowFrontOffset, innerEndPos.y));
		axes->AddVertex(Coordinates(innerEndPos.x, innerEndPos.y + arrowSideOffset));
		axes->AddVertex(Coordinates(innerEndPos.x, innerEndPos.y));
	}


	void Graph::InitLabels(const int rowLabelsOffset, const int columnLabelsOffset) {
		constexpr size_t maxIntStrLen = Log10Compile(INT_MAX) + 2;
		char tempBuf[maxIntStrLen + 1] = "";

		int curY = props.fontSize / 2;
		while (curY < innerSize.height) {
			int curLabelVal = (curY - props.fontSize / 2) / static_cast<float>(innerSize.height) *
				props.rangeY + props.startY;

			int curYWindow = innerPos.y + innerSize.height - curY;

			Text* curLabel = new Text(window, itoa(curLabelVal, tempBuf, 10),
				Coordinates(pos.x, curYWindow), props.fontSize, props.fontColor);

			Line* curHatch = new Line(window, Coordinates(innerPos.x, curYWindow + props.fontSize / 2),
				Coordinates(innerPos.x - props.hatchSize, curYWindow + props.fontSize / 2),
				props.axesWidth, props.axesColor);

			labels.push_back(curLabel);
			hatches.push_back(curHatch);

			curY += rowLabelsOffset;
		}


		int curX = 0;
		while (curX < innerSize.width) {
			int curLabelVal = curX / static_cast<float>(innerSize.width) *
				props.rangeX + props.startX;

			int curXWindow = innerPos.x + curX;
			Text* curLabel = new Text(window, itoa(curLabelVal, tempBuf, 10),
				Coordinates(curXWindow, pos.y + size.height - props.fontSize),
				props.fontSize, props.fontColor);

			Line* curHatch = new Line(window, Coordinates(curXWindow, innerPos.y + innerSize.height),
				Coordinates(curXWindow, innerPos.y + innerSize.height + props.hatchSize),
				props.axesWidth, props.axesColor);

			labels.push_back(curLabel);
			hatches.push_back(curHatch);
			curX += columnLabelsOffset;
		}
	}


	void Graph::Draw() {
		window.SetActive();

		background->Draw();
		axes->Draw();

		for (int i = 0; i < diagrams.size(); ++i)
			diagrams[i].second->Draw();

		for (int i = 0; i < labels.size(); ++i)
			labels[i]->Draw();

		for (int i = 0; i < hatches.size(); ++i)
			hatches[i]->Draw();
	}

}