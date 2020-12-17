#pragma once



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

		Vector2 operator+(const Vector2& right) const;

		Vector2 operator-(const Vector2& right) const;

		int x = 0;
		int y = 0;
	};


	class Color {
	public:

		enum SpecialColors {
			black, red, green, blue, yellow, magenta, cyan, white
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

	class Element {
	public:

		Element(OSWindow& osWindow);

		OSWindow& GetOSWindow() const;

	protected:

		OSWindow& osWindow;
	};
}