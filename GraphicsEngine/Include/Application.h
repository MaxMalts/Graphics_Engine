#pragma once

#include <unordered_set>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Common.h"
#include "Event.h"
#include "Windows\Desktop.h"



namespace GUI {
	class OSWindow;
	class DesktopWindow;

	class Application {
	public:

		Application();

		Application(const Application& other) = delete;

		OSWindow* CreateWindow(const int width = 640, const int height = 420, const char* name = "Window",
		                       const Color& backgroundColor = Color(0, 0, 0));

		size_t WindowsOpened() const;

		void CloseWindow(OSWindow* window);

		void CloseWindow(OSWindow& window);

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


		void Update();

		void SetActive() const;


		Vector2 CursorPos() const;

		bool MouseButtonPressed(const MouseButton button);

		void StartEvent(Event& event);


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
				window->StartEvent(closeEvent);
			}
		}


		static void MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
			assert(glfwWindow != nullptr);

			OSWindow* window = static_cast<OSWindow*>(glfwGetWindowUserPointer(glfwWindow));
			assert(window != nullptr);

			MouseButtonProps mouseProps{ window->CursorPos() };
			switch (button) {
			case GLFW_MOUSE_BUTTON_LEFT:
				mouseProps.button = MouseButton::left;
				break;

			case GLFW_MOUSE_BUTTON_RIGHT:
				mouseProps.button = MouseButton::right;
				break;

			case GLFW_MOUSE_BUTTON_MIDDLE:
				mouseProps.button = MouseButton::middle;
				break;

			default:
				mouseProps.button = MouseButton::unknown;
			}

			Event mouseEvent((GLFW_RELEASE == action ? Event::mouse_up : Event::mouse_down), mouseProps);

			if (!mouseEvent.Stopped()) {
				window->StartEvent(mouseEvent);
			}
		}


		static void MouseMoveCallback(GLFWwindow* glfwWindow, double xpos, double ypos) {
			assert(glfwWindow != nullptr);

			OSWindow* window = static_cast<OSWindow*>(glfwGetWindowUserPointer(glfwWindow));
			assert(window != nullptr);

			MouseMoveProps mousePosProps{ window->CursorPos() };

			Event mouseEvent(Event::mouse_move, mousePosProps);

			if (!mouseEvent.Stopped()) {
				window->StartEvent(mouseEvent);
			}
		}


		static void KeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
			assert(glfwWindow != nullptr);

			OSWindow* window = static_cast<OSWindow*>(glfwGetWindowUserPointer(glfwWindow));
			assert(window != nullptr);

			KeyProps keyProps{ static_cast<KeyboardKey>(key) };

			Event keyEvent((GLFW_RELEASE == action ? Event::key_up :
				(GLFW_PRESS == action ? Event::key_down : Event::key_repeat)),
				keyProps);

			if (!keyEvent.Stopped()) {
				window->StartEvent(keyEvent);
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
				window->StartEvent(scrollEvent);
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