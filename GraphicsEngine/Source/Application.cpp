#include <stdexcept>
#include <assert.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Include\Common.h"
#include "Include\Application.h"
#include "Include\Windows\Desktop.h"



namespace GUI {

	/* Application */

	Application::Application() {
		if (!glfwInit()) {
			throw bad_init("Error while initializing Application.");
		}
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
		if (window == nullptr) {
			return;
		}

		std::unordered_set<OSWindow*>::iterator windowIter = osWindows.find(window);
		if (osWindows.end() == windowIter) {
			throw std::invalid_argument("Passed window not found.");
		}

		delete *windowIter;
		osWindows.erase(windowIter);
	}

	void Application::CloseWindow(OSWindow& window) {
		CloseWindow(&window);
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



	/* OSWindow */

	OSWindow::OSWindow(Application& application, const int width, const int height,
	                   const char* name, const Color& desktopColor)
		: application(application), width(width), height(height) {

		assert(width >= 0);
		assert(height >= 0);
		assert(name != nullptr);

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
		glfwWindow = glfwCreateWindow(width, height, name, NULL, NULL);
		if (glfwWindow == nullptr) {
			glfwTerminate();
			throw bad_init("Error occurred while creating the window");
		}
		glfwSetWindowUserPointer(glfwWindow, this);

		SetActive();
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);

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


	bool OSWindow::MouseButtonPressed(const MouseButton button) {
		int glfwButton = 0;
		switch (button) {
		case MouseButton::left:
			glfwButton = GLFW_MOUSE_BUTTON_LEFT;
			break;

		case MouseButton::right:
			glfwButton = GLFW_MOUSE_BUTTON_RIGHT;
			break;

		case MouseButton::middle:
			glfwButton = GLFW_MOUSE_BUTTON_MIDDLE;
			break;

		default:
			return false;
		}

		return (glfwGetMouseButton(glfwWindow, glfwButton) == GLFW_PRESS);
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


	void OSWindow::StartEvent(Event& event) {
		desktop->HandleEvent(event);
	}


	OSWindow::~OSWindow() {
		delete desktop;
		glfwDestroyWindow(glfwWindow);
	}
}