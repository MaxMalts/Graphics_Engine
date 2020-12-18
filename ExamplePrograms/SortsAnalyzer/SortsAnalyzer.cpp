#include <iostream>
#include <vector>
#include "Sorts.h"
#include <GraphicsEngine.h>



class StatsFuncs {
public:

	static bool Comparator(const int val1, const int val2) {
		++comparatorCount;
		return val1 < val2;
	}


	static long long ComparatorCount() {
		return comparatorCount;
	}


	static void ResetComparator() {
		comparatorCount = 0;
	}



	static void Swap(int& val1, int& val2) {
		++swapCount;

		int temp = val1;
		val1 = val2;
		val2 = temp;
	}


	static long long SwapCount() {
		return swapCount;
	}


	static void ResetSwap() {
		swapCount = 0;
	}

private:
	static long long comparatorCount;
	static long long swapCount;
};
long long StatsFuncs::comparatorCount = 0;
long long StatsFuncs::swapCount = 0;



void FillRandom(std::vector<int>& arr, size_t count) {
	arr.clear();
	
	for (size_t i = 0; i < count; ++i) {
		arr.push_back(rand());
	}
}


template<typename T>
void PrintTable(std::vector<T>& arr) {
	assert(!arr.empty());

	for (int i = 0; i < arr.size(); ++i) {
		std::cout << i << "    " << arr[i] << '\n';
	}
}


struct Graphs {
	GUI::Graph* left = nullptr;
	GUI::Text* leftLabel = nullptr;

	GUI::Graph* right = nullptr;
	GUI::Text* rightLabel = nullptr;

	std::pair<GUI::Graph::Diagram*, GUI::Graph::Diagram*> bubbleDiagrams;
	std::pair<GUI::Graph::Diagram*, GUI::Graph::Diagram*> selectionDiagrams;
	std::pair<GUI::Graph::Diagram*, GUI::Graph::Diagram*> quickDiagrams;
	std::pair<GUI::Graph::Diagram*, GUI::Graph::Diagram*> mergeDiagrams;
};


struct Buttons {
	GUI::Button* bubble = nullptr;
	GUI::Button* selection = nullptr;
	GUI::Button* quick = nullptr;
	GUI::Button* merge = nullptr;
};


struct Interface {
	GUI::OSWindow* osWindow = nullptr;
	GUI::DesktopWindow* desktop = nullptr;
	Graphs graphs = {};
	Buttons buttons = {};
};


void DrawInterface(GUI::DesktopWindow* desktop, const int maxArraySize, Interface& interface) {
	assert(desktop != nullptr);

	const int windowW = desktop->Width();
	const int windowH = desktop->Height();

	const int graphsMarginLeft = 0.05 * windowW;
	const int graphsMarginTop = 0.1 * windowH;
	const GUI::Vector2 graphsSize((windowW - 3 * graphsMarginLeft) / 2, 0.6 * windowH);
	const size_t graphLabelFontSize = 0.07 * graphsSize.x;

	const int buttonsMarginLeft = 0.03 * windowW;
	const int buttonsMarginTop = 0.1 * windowH;
	const GUI::Vector2 buttonsSize((windowW - 5 * buttonsMarginLeft) / 4, 0.15 * windowH);

	const GUI::Color bubbleColor(GUI::Color::red);
	const GUI::Color selectionColor(GUI::Color::green);
	const GUI::Color quickColor(GUI::Color::blue);
	const GUI::Color mergeColor(GUI::Color::yellow);

	const size_t buttonsFontSize = 0.1 * buttonsSize.x;
	const GUI::Vector2 buttonsLabelPos(10, (buttonsSize.y - buttonsFontSize) / 2);

	interface.desktop = desktop;


	/* Creating graphs */

	GUI::GraphProps graphsProps(0, maxArraySize, 0, maxArraySize * maxArraySize / 2);
	interface.graphs.left = dynamic_cast<GUI::Graph*>(
			desktop->CreateWindow(GUI::Window::graph, graphsProps, GUI::Vector2(graphsMarginLeft,
				graphsMarginTop), graphsSize)
		);
	interface.graphs.leftLabel = dynamic_cast<GUI::Text*>(
			desktop->CreatePrimitive(GUI::Primitive::text, GUI::TextProps("Comparisons", GUI::Vector2(graphsMarginLeft,
				graphsMarginTop - graphLabelFontSize), graphLabelFontSize))
		);

	interface.graphs.right = dynamic_cast<GUI::Graph*>(
			desktop->CreateWindow(GUI::Window::graph, graphsProps, GUI::Vector2(2 * graphsMarginLeft + graphsSize.x,
				graphsMarginTop), graphsSize)
		);
	interface.graphs.rightLabel = dynamic_cast<GUI::Text*>(
			desktop->CreatePrimitive(GUI::Primitive::text, GUI::TextProps("Swaps:", GUI::Vector2(2 * graphsMarginLeft + graphsSize.x,
				graphsMarginTop - graphLabelFontSize), graphLabelFontSize))
		);


	/* Creating buttons */

	interface.buttons.bubble = dynamic_cast<GUI::Button*>(
			desktop->CreateWindow(GUI::Window::button, GUI::ButtonProps(bubbleColor), GUI::Vector2(buttonsMarginLeft,
				graphsMarginTop + graphsSize.y + buttonsMarginTop), buttonsSize)
		);
	interface.buttons.bubble->AddLabel("bubble", GUI::FontProps(buttonsFontSize));

	interface.buttons.selection = dynamic_cast<GUI::Button*>(
			desktop->CreateWindow(GUI::Window::button, GUI::ButtonProps(selectionColor),
				GUI::Vector2(2 * buttonsMarginLeft + buttonsSize.x, graphsMarginTop + graphsSize.y + buttonsMarginTop),
				buttonsSize)
		);
	interface.buttons.selection->AddLabel("selection", GUI::FontProps(buttonsFontSize));

	interface.buttons.quick = dynamic_cast<GUI::Button*>(
			desktop->CreateWindow(GUI::Window::button, GUI::ButtonProps(quickColor),
				GUI::Vector2(3 * buttonsMarginLeft + 2 * buttonsSize.x, graphsMarginTop + graphsSize.y + buttonsMarginTop),
				buttonsSize)
		);
	interface.buttons.quick->AddLabel("quick", GUI::FontProps(buttonsFontSize));

	interface.buttons.merge = dynamic_cast<GUI::Button*>(
			desktop->CreateWindow(GUI::Window::button, GUI::ButtonProps(mergeColor),
				GUI::Vector2(4 * buttonsMarginLeft + 3 * buttonsSize.x, graphsMarginTop + graphsSize.y + buttonsMarginTop),
				buttonsSize)
		);
	interface.buttons.merge->AddLabel("merge", GUI::FontProps(buttonsFontSize));
}


void AnalyseSort(const Interface& interface, const int maxCount,
                 void (*Sort)(std::vector<int>&, bool (*)(const int, const int), void (*)(int&, int&)),
                 GUI::Graph::Diagram* comparesDiagram, GUI::Graph::Diagram* swapsDiagram) {

	assert(maxCount > 0);

	std::vector<long long> comparesByCount(maxCount, 0);
	std::vector<long long> swapsByCount(maxCount, 0);

	std::vector<int> arr;
	for (int curCount = 0; curCount < maxCount; ++curCount) {
		FillRandom(arr, curCount);

		Sort(arr, StatsFuncs::Comparator, StatsFuncs::Swap);

		comparesDiagram->AddData(curCount, StatsFuncs::ComparatorCount());
		swapsDiagram->AddData(curCount, StatsFuncs::SwapCount());
		interface.osWindow->Update();

		StatsFuncs::ResetComparator();
		StatsFuncs::ResetSwap();
	}
}


void ButtonsListener(GUI::Event& event, void* voidArgs) {
	assert(voidArgs != nullptr);

	auto args = reinterpret_cast<std::pair<Interface&, const int>*>(voidArgs);

	Interface& interface = args->first;
	const int maxCount = args->second;
	GUI::Button* button = dynamic_cast<GUI::Button*>(event.GetTarget());
	assert(button != nullptr);

	if (button == interface.buttons.bubble) {
		interface.graphs.left->RemoveDiargam(interface.graphs.bubbleDiagrams.first);
		interface.graphs.right->RemoveDiargam(interface.graphs.bubbleDiagrams.second);
		interface.graphs.bubbleDiagrams.first = interface.graphs.left->CreateDiagram(3, button->GetColor());
		interface.graphs.bubbleDiagrams.second = interface.graphs.right->CreateDiagram(3, button->GetColor());
		AnalyseSort(interface, maxCount, BubbleSort,
		            interface.graphs.bubbleDiagrams.first, interface.graphs.bubbleDiagrams.second);

	} else if (button == interface.buttons.selection) {
		interface.graphs.left->RemoveDiargam(interface.graphs.selectionDiagrams.first);
		interface.graphs.right->RemoveDiargam(interface.graphs.selectionDiagrams.second);
		interface.graphs.selectionDiagrams.first = interface.graphs.left->CreateDiagram(3, button->GetColor());
		interface.graphs.selectionDiagrams.second = interface.graphs.right->CreateDiagram(3, button->GetColor());
		AnalyseSort(interface, maxCount, SelectionSort,
		            interface.graphs.selectionDiagrams.first, interface.graphs.selectionDiagrams.second);

	} else if (button == interface.buttons.quick) {
		interface.graphs.left->RemoveDiargam(interface.graphs.quickDiagrams.first);
		interface.graphs.right->RemoveDiargam(interface.graphs.quickDiagrams.second);
		interface.graphs.quickDiagrams.first = interface.graphs.left->CreateDiagram(3, button->GetColor());
		interface.graphs.quickDiagrams.second = interface.graphs.right->CreateDiagram(3, button->GetColor());
		AnalyseSort(interface, maxCount, QuickSort,
		            interface.graphs.quickDiagrams.first, interface.graphs.quickDiagrams.second);

	} else if (button == interface.buttons.merge) {
		interface.graphs.left->RemoveDiargam(interface.graphs.mergeDiagrams.first);
		interface.graphs.right->RemoveDiargam(interface.graphs.mergeDiagrams.second);
		interface.graphs.mergeDiagrams.first = interface.graphs.left->CreateDiagram(3, button->GetColor());
		interface.graphs.mergeDiagrams.second = interface.graphs.right->CreateDiagram(3, button->GetColor());
		AnalyseSort(interface, maxCount, MergeSort,
		            interface.graphs.mergeDiagrams.first, interface.graphs.mergeDiagrams.second);
	}
}


void AddButtonsListeners(Interface& interface, const int maxArraySize) {
	static std::tuple<const Interface, const int, GUI::Button*> bubbleArgs(interface, maxArraySize,
	                                                                       interface.buttons.bubble);
	static std::tuple<const Interface, const int, GUI::Button*> selectionArgs(interface, maxArraySize,
	                                                                          interface.buttons.selection);
	static std::tuple<const Interface, const int, GUI::Button*> quickArgs(interface, maxArraySize,
	                                                                      interface.buttons.quick);
	static std::tuple<const Interface, const int, GUI::Button*> mergeArgs(interface, maxArraySize,
	                                                                      interface.buttons.merge);

	
	static std::pair<Interface&, const int> additArgs(interface, maxArraySize);

	interface.buttons.bubble->AddEventListener(GUI::Event::mouse_up, ButtonsListener, &additArgs);
	interface.buttons.selection->AddEventListener(GUI::Event::mouse_up, ButtonsListener, &additArgs);
	interface.buttons.quick->AddEventListener(GUI::Event::mouse_up, ButtonsListener, &additArgs);
	interface.buttons.merge->AddEventListener(GUI::Event::mouse_up, ButtonsListener, &additArgs);
}


int main() {
	const int maxArraySize = 100;

	const int windowW = 1280;
	const int windowH = 720;

	GUI::Text::SetFontFile("../Fonts/Ascii.bmp");

	GUI::Application app;
	GUI::OSWindow* osWindow = app.CreateWindow(1280, 720, "Sorts Analyzer", GUI::Color(GUI::Color::white));
	GUI::DesktopWindow* desktop = osWindow->GetDesktop();

	Interface interface = { osWindow };
	DrawInterface(desktop, maxArraySize, interface);

	AddButtonsListeners(interface, maxArraySize);

	desktop->AddEventListener(GUI::Event::window_close, GUI::OSWindowCloseListener);

	double prevTime = glfwGetTime();
	while (app.WindowsOpened() > 0) {
		osWindow->Update();
		app.ProcessEvents();
		double curTime = glfwGetTime();
		//std::cout << 1 / (curTime - prevTime) << '\n';
		prevTime = curTime;
	}

	return 0;
}