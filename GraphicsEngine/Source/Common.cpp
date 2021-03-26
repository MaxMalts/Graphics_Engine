#include <string>
#include <unordered_map>
#include <stdexcept>
#include <assert.h>
#include "Include\Common.h"



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



	/* Vector2 */

	Vector2::Vector2(long long x, long long y) : x(x), y(y) {}


	Vector2 Vector2::operator+(const Vector2& right) const {
		return Vector2(x + right.x, y + right.y);
	}

    Vector2 Vector2::operator+() const {
		return *this;
    }

    Vector2& Vector2::operator+=(const Vector2& right) {
		x += right.x;
		y += right.y;

		return *this;
    }

	Vector2 Vector2::operator-(const Vector2& right) const {
		return Vector2(x - right.x, y - right.y);
	}

	Vector2 Vector2::operator-() const {
		return GUI::Vector2(-x, -y);
	}

	Vector2& Vector2::operator-=(const Vector2& right) {
		x -= right.x;
		y -= right.y;

		return *this;
	}

	Vector2 Vector2::operator*(const double right) const {
		return Vector2(x * right, y * right);
	}

	Vector2 operator*(const double left, const Vector2& right) {
		return Vector2(right.x * left, right.y * left);

	}

	Vector2 Vector2::operator/(const double right) const {
		return Vector2(x / right, y / right);
	}



	/* Color */

	Color::Color() = default;

	Color::Color(const float red, const float green, const float blue) : RGB{ red, green, blue } {
		Normalize();
	}


	Color::Color(const SpecialColors color) {
		const std::unordered_map<SpecialColors, const RGB_t> colorsMap {
			{black, RGB_t{0.0, 0.0, 0.0}},
			{white, RGB_t{1.0, 1.0, 1.0}},
			{grey, RGB_t{0.5, 0.5, 0.5}},
			{red, RGB_t{1.0, 0.0, 0.0}},
			{green, RGB_t{0.0, 1.0, 0.0}},
			{blue, RGB_t{0.0, 0.0, 1.0}},
			{yellow, RGB_t{1.0, 1.0, 0.0}},
			{magenta, RGB_t{1.0, 0.0, 1.0}},
			{cyan, RGB_t{0.0, 1.0, 1.0}},
		};

		bool colorFound = false;
		for (auto& curMap : colorsMap) {
			if (color == curMap.first) {
				RGB = curMap.second;
				colorFound = true;
				break;
			}
		}

		if (!colorFound) {
			throw std::invalid_argument("Invalid value of color argument");
		}
	}


	Color Color::operator+(const Color& right) const {
		return Color(RGB.red + right.RGB.red, RGB.green + right.RGB.green, RGB.blue + right.RGB.blue);
	}

	Color& Color::operator+=(const Color& right) {
		RGB.red += right.RGB.red;
		RGB.green += right.RGB.green;
		RGB.blue += right.RGB.blue;
		Normalize();

		return *this;
	}

	Color Color::operator-(const Color& right) const {
		return Color(RGB.red - right.RGB.red, RGB.green - right.RGB.green, RGB.blue - right.RGB.blue);
	}

	Color& Color::operator-=(const Color& right) {
		RGB.red -= right.RGB.red;
		RGB.green -= right.RGB.green;
		RGB.blue -= right.RGB.blue;
		Normalize();

		return *this;
	}

	Color Color::operator*(const double right) const {
		return Color(RGB.red * right, RGB.green * right, RGB.blue * right);
	}

	Color operator*(const double left, const Color& right) {
		return Color(right.RGB.red * left, right.RGB.green * left, right.RGB.blue * left);

	}

	Color Color::operator/(const double right) const {
		return Color(RGB.red / right, RGB.green / right, RGB.blue / right);
	}


	float Color::operator[](const std::string& color) const {
		if (color.compare("red") == 0) {
			return RGB.red;

		} else if (color.compare("green") == 0) {
			return RGB.green;

		} else if (color.compare("blue") == 0) {
			return RGB.blue;

		} else {
			throw std::invalid_argument("Invalid value of color argument");
		}
	}

	float& Color::operator[](const std::string& color) {
		if (color.compare("red") == 0) {
			return RGB.red;

		} else if (color.compare("green") == 0) {
			return RGB.green;

		} else if (color.compare("blue") == 0) {
			return RGB.blue;

		} else {
			throw std::invalid_argument("Invalid value of color argument");
		}
	}

	float Color::operator[](const char color) const {
		switch (color) {
		case 'r':
		case 'R':
			return RGB.red;

		case 'g':
		case'G':
			return RGB.green;

		case 'b':
		case'B':
			return RGB.blue;

		default:
			throw std::invalid_argument("Invalid value of color argument");
		}
	}

	float& Color::operator[](const char color) {
		switch (color) {
		case 'r':
		case 'R':
			return RGB.red;

		case 'g':
		case'G':
			return RGB.green;

		case 'b':
		case'B':
			return RGB.blue;

		default:
			throw std::invalid_argument("Invalid value of color argument");
		}
	}


	float& Color::Red() {
		return RGB.red;
	}

	float& Color::Green() {
		return RGB.green;
	}

	float& Color::Blue() {
		return RGB.blue;
	}


	float Color::Red() const {
		return RGB.red;
	}
	
	float Color::Green() const {
		return RGB.green;
	}

	float Color::Blue() const {
		return RGB.blue;
	}


	void Color::Normalize() {
		if (RGB.red > 1.0) {
			RGB.red = 1.0;
		} else if (RGB.red < 0.0) {
			RGB.red = 0.0;
		}

		if (RGB.green > 1.0) {
			RGB.green = 1.0;
		} else if (RGB.green < 0.0) {
			RGB.green = 0.0;
		}

		if (RGB.blue > 1.0) {
			RGB.blue = 1.0;
		} else if (RGB.blue < 0.0) {
			RGB.blue = 0.0;
		}
	}



	/* Element */

	Element::Element(OSWindow& osWindow) : osWindow(osWindow) {}


	OSWindow& Element::GetOSWindow() const {
		return osWindow;
	}

}