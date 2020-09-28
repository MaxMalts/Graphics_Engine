#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <gl/glew.h> 
#include <GLFW/glfw3.h>
#include "GraphicsApi.h"


template<typename Arr>
void DeleteArrayElements(Arr arr) {
	for (int i = 0; i < arr.size(); ++i) {
		delete arr[i];
	}
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



	/* Structures */

	Coordinates::Coordinates() = default;

	Coordinates::Coordinates(int x, int y) : x(x), y(y) {}


	Size::Size() = default;

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
		if (!strcmp(color, "red")) {
			RGB[0] = 1.0; RGB[1] = 0.0; RGB[2] = 0.0;

		} else if (!strcmp(color, "green")) {
			RGB[0] = 0.0; RGB[1] = 1.0; RGB[2] = 0.0;

		} else if (!strcmp(color, "blue")) {
			RGB[0] = 0.0; RGB[1] = 0.0; RGB[2] = 1.0;

		} else if (!strcmp(color, "black")) {
			RGB[0] = 0.0; RGB[1] = 0.0; RGB[2] = 0.0;

		} else if (!strcmp(color, "white")) {
			RGB[0] = 1.0; RGB[1] = 1.0; RGB[2] = 1.0;

		} else {
			throw std::invalid_argument("Invalid value of color argument");
		}
	}


	Color::Color(const Color& other) {
		memcpy(RGB, other.RGB, sizeof(RGB));
	}


	float Color::operator[](const char* color) const {
		if (!strcmp(color, "red") || !strcmp(color, "R"))
			return RGB[0];

		else if (!strcmp(color, "green") || !strcmp(color, "G"))
			return RGB[1];

		else if (!strcmp(color, "blue") || !strcmp(color, "B"))
			return RGB[2];

		else
			throw std::invalid_argument("Invalid value of color argument");
	}


	float Color::operator[](const size_t index) const {
		if (index >= sizeof(RGB) / sizeof(RGB[0]))
			throw std::invalid_argument("Invalid index argument");

		return RGB[index];
	}


	float Color::RedAmt() const {
		return RGB[0];
	}


	float Color::GreenAmt() const {
		return RGB[1];
	}


	float Color::BlueAmt() const {
		return RGB[2];
	}



	/* Line implementation */

	Line::Line(const Window& window, const Coordinates& begPos, const Coordinates& endPos,
	           const size_t width, const Color& color)
		: window(window), firstPoint(begPos), secondPoint(endPos), width(width), color(color) {

		GlCoordinates begPosGl = WindowToGlCoords(window, begPos);
		GlCoordinates endPosGl = WindowToGlCoords(window, endPos);

		
		window.SetActive();

		glColor3f(color.RedAmt(), color.GreenAmt(), color.BlueAmt());
		glLineWidth(static_cast<GLfloat>(width));

		glBegin(GL_LINES);
		glVertex2f(begPosGl.x, begPosGl.y);
		glVertex2f(endPosGl.x, endPosGl.y);
		glEnd();
	}



	/* Polyline implementation */

	Polyline::Polyline(const Window& window, const std::vector<Coordinates>& verteces,
	                   const size_t width, const Color& color) 
		: window(window), verteces(verteces), width(width), color(color) {

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


	Polyline::Polyline(const Window& window, const size_t width, const Color& color)
		: window(window), verteces(), width(width), color(color) {}


	void Polyline::AddVertex(const Coordinates& pos) {

		if (verteces.size() >= 1) {
			GlCoordinates lastPosGl = WindowToGlCoords(window, verteces[verteces.size() - 1]);
			GlCoordinates posGl = WindowToGlCoords(window, pos);

			window.SetActive();
			
			glColor3f(color.RedAmt(), color.GreenAmt(), color.BlueAmt());
			glLineWidth(static_cast<GLfloat>(width));

			glBegin(GL_LINES);
			glVertex2f(lastPosGl.x, lastPosGl.y);
			glVertex2f(posGl.x, posGl.y);
			glEnd();
		}

		verteces.push_back(pos);
	}



	/* Rectangle implementation */

	Rectangle::Rectangle(const Window& window, const Coordinates& pos, const Size& size, const Color& color)
		: window(window), pos(pos), size(size), color(color) {

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

	Text::Text(const Window& window, const char* string,
	           const Coordinates& pos, const size_t fontSize, const Color& color) 
		: window(window), pos(pos), fontSize(fontSize), color(color) {
		
		if (0 == instanceCount) {
			const char bitmapFileName[] = "Fonts/ASCII.bmp";
			const size_t charWidth = 16;
			const size_t charHeight = 16;

			InitFont(bitmapFileName, charWidth, charHeight);
		}

		content = new char[strlen(string) + 1];
		strcpy(content, string);

		Draw();

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

				int pxOffsetX = round(curX / scale);
				int pxOffsetY = round(curY / scale);
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

		//glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0.0, 0.0, 0.0);


		Coordinates curPos(pos.x, pos.y);
		for (int i = 0; i < strlen(content); ++i) {
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

		if (instanceCount == 1) {
			DestroyFont();
		}

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

	Button::Button(const Window& window, const Coordinates& pos, const Size& size, const Color& color)
		: window(window), pos(pos), size(size), color(color) {

		window.SetActive();

		Rectangle* newRect = new Rectangle(window, pos, size, color);
		rectangle = newRect;
	}


	Button::~Button() {
		delete rectangle;
	}



	/* Window Implementation */

	Window::Window(const int width, const int height, const char* name, const Color& backgroundColor)
		: width(width), height(height), backgroundColor(backgroundColor) {

		assert(width >= 0);
		assert(height >= 0);
		assert(name != nullptr);

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
		window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (!window) {
			glfwTerminate();
			throw bad_init("Error occurred while creating the window");
		}

		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);

		SetActive();

		glClearColor(backgroundColor.RedAmt(), backgroundColor.GreenAmt(), backgroundColor.BlueAmt(), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}


	size_t Window::Width() const {
		return width;
	}


	size_t Window::Height() const {
		return height;
	}


	Rectangle* Window::CreateRectangle(const Coordinates& pos, const Size& size, const Color& color) {
		SetActive();

		Rectangle* newRect = new Rectangle(*this, pos, size, color);
		rectangles.push_back(newRect);

		return newRect;
	}


	Button* Window::CreateButton(const Coordinates& pos, const Size& size, const Color& color) {
		SetActive();

		Button* newButton = new Button(*this, pos, size, color);
		buttons.push_back(newButton);

		return newButton;
	}


	Line* Window::CreateLine(const Coordinates& begPos, const Coordinates& endPos,
	                         const size_t width, const Color& color) {

		SetActive();

		Line* newLine = new Line(*this, begPos, endPos, width, color);
		lines.push_back(newLine);

		return newLine;
	}


	Polyline* Window::CreatePolyline(const std::vector<Coordinates>& verteces,
                                     const size_t width, const Color& color) {

		SetActive();

		Polyline* newPolyline = new Polyline(*this, verteces, width, color);
		polylines.push_back(newPolyline);

		return newPolyline;
	}


	Polyline* Window::CreatePolyline(const size_t width, const Color& color) {
		SetActive();

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


	void Window::DrawChanges() {
		glfwSwapBuffers(window);
	}


	void Window::SetActive() const {
		assert(window != nullptr);

		if (glfwGetCurrentContext() != window) {
			glfwMakeContextCurrent(window);
		}
	}


	Window::~Window() {
		SetActive();

		DeleteArrayElements(lines);
		DeleteArrayElements(polylines);
		DeleteArrayElements(rectangles);
		DeleteArrayElements(texts);
		DeleteArrayElements(buttons);

		glfwDestroyWindow(window);
	}



	/* Application implementation */

	Application::Application() {
		if (!glfwInit())
			throw bad_init("Error while initializing Application)");
	}


	Window* Application::CreateWindow(const int width, const int height, const char* name, const Color& backgroundColor) {
		assert(width >= 0);
		assert(height >= 0);
		assert(name != nullptr);

		Window* newWindow = new Window(width, height, name, backgroundColor);
		windows.push_back(newWindow);

		return newWindow;
	}


	void Application::WaitEvents() const {
		glfwWaitEvents();
	}


	void Application::PollEvents() const {
		glfwPollEvents();
	}


	Application::~Application() {
		DeleteArrayElements(windows);
		glfwTerminate();
	}
}