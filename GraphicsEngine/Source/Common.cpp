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

	Vector2::Vector2(int x, int y) : x(x), y(y) {}


	Vector2 Vector2::operator+(const Vector2& right) const {
		return Vector2(x + right.x, y + right.y);
	}

	Vector2 Vector2::operator-(const Vector2& right) const {
		return Vector2(x - right.x, y - right.y);
	}



	/* Color */

	Color::Color() = default;

	Color::Color(const float red, const float green, const float blue) : RGB{ red, green, blue } {
		if (red > 1.0 || red < 0.0)
			throw std::invalid_argument("Invalid value of red argument");

		else if (green > 1.0 || green < 0.0)
			throw std::invalid_argument("Invalid value of green argument");

		else if (blue > 1.0 || blue < 0.0)
			throw std::invalid_argument("Invalid value of blue argument");
	}


	Color::Color(const SpecialColors color) {
		const std::unordered_map<SpecialColors, const RGB_t> colorsMap {
			{black, RGB_t{0.0, 0.0, 0.0}},
			{red, RGB_t{1.0, 0.0, 0.0}},
			{green, RGB_t{0.0, 1.0, 0.0}},
			{blue, RGB_t{0.0, 0.0, 1.0}},
			{yellow, RGB_t{1.0, 1.0, 0.0}},
			{magenta, RGB_t{1.0, 0.0, 1.0}},
			{cyan, RGB_t{0.0, 1.0, 1.0}},
			{white, RGB_t{1.0, 1.0, 1.0}},
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


	Color::Color(const Color & other) {
		RGB = other.RGB;
	}


	Color& Color::operator=(const Color & other) {
		RGB = other.RGB;

		return *this;
	}


	float Color::operator[](const char* color) const {
		if (!strcmp(color, "red") || !strcmp(color, "R")) {
			return RGB.red;

		} else if (!strcmp(color, "green") || !strcmp(color, "G")) {
			return RGB.green;

		} else if (!strcmp(color, "blue") || !strcmp(color, "B")) {
			return RGB.blue;

		} else {
			throw std::invalid_argument("Invalid value of color argument");
		}
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



	/* Element */

	Element::Element(OSWindow& osWindow) : osWindow(osWindow) {}


	OSWindow& Element::GetOSWindow() const {
		return osWindow;
	}

}