#pragma once

#include <string>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>



namespace GUI {
	class bad_init {
	public:

		bad_init();

		bad_init(const char* message);

		char* what();


	private:

		char* _what = nullptr;
	};


	enum class MouseButton {
		unknown, left, right, middle
	};


	enum class KeyboardKey {
		unknown,
		q, w, e, r, t, y, u, i, o, p, a, s, d, f, g, h, j, k, l, z = GLFW_KEY_Z, x, c, v, b, n, m,
		n0, n1, n2, n3, n4, n5, n6, n7, n8, n9,
		ctr_left = GLFW_KEY_LEFT_CONTROL, alt_left, shift_left, space, tab, enter, backspace, caps,
		ctrl_right, alt_right, shift_right,
		num_lock, num0, num1, num2, num3, num4, num5, num6, num7, num8, num9, num_div,
		num_mul, num_minus, num_plus, num_enter, num_comma,
		escape, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
		semicolon, quote, comma, period, slash, backslash, square_bracket_open, quare_bracket_close,
		acute, hyphen, equal,
		print_screen, scroll_lock, insert, del, home, end, page_up, page_down,
		arrow_up, arrow_down, arros_left, arrow_right,
	};


	struct Vector2 {
		Vector2() = default;

		Vector2(long long x, long long y);

		Vector2& operator=(const Vector2& other) = default;

		Vector2 operator+(const Vector2& right) const;

		Vector2 operator+() const;

		Vector2& operator+=(const Vector2& right);

		Vector2 operator-(const Vector2& right) const;
		
		Vector2 operator-() const;

		Vector2& operator-=(const Vector2& right);

		Vector2 operator*(const double right) const;

		friend Vector2 operator*(const double left, const Vector2& right);

		Vector2 operator/(const double right) const;

		long long x = 0;
		long long y = 0;
	};


	class Color {
	public:

		enum SpecialColors {
			black, white, grey, red, green, blue, yellow, magenta, cyan
		};

		Color();

		/*
		* Constructs Color. Arguments values must be betweeb 0.0 and 1.0
		*/
		Color(const float red, const float green, const float blue);

		/*
		* Constructs Color. Possible strings are "red", "green", "blue", "black", "white".
		*/
		Color(const SpecialColors color);

		Color(const Color& other) = default;

		Color& operator=(const Color& other) = default;

		Color operator+(const Color& right) const;

		Color& operator+=(const Color& right);

		Color operator-(const Color& right) const;

		Color& operator-=(const Color& right);

		Color operator*(const double right) const;

		friend Color operator*(const double left, const Color& right);

		Color operator/(const double right) const;

		/*
		* Returns specified color amount. Posiigble argument values are "red", "R", "green", "G", "blue", "B".
		*/
		float& operator[](const std::string& color);

		float operator[](const std::string& color) const;

		float& operator[](const char color);

		float operator[](const char color) const;


		float& Red();

		float& Green();

		float& Blue();

		float Red() const;

		float Green() const;

		float Blue() const;


	private:

		void Normalize();


		struct RGB_t {
			float red = 0.0, green = 0.0, blue = 0.0;
		} RGB;
	};


	class OSWindow;

	class Element {
	public:

		Element(OSWindow& osWindow);

		OSWindow& GetOSWindow() const;

	protected:

		OSWindow& osWindow;
	};
}