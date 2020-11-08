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
};


struct Buttons {
	GUI::Button* bubble = nullptr;
	GUI::Button* selection = nullptr;
	GUI::Button* quick = nullptr;
	GUI::Button* merge = nullptr;
};


struct Interface {
	GUI::OSWindow* window = nullptr;
	Graphs graphs = {};
	Buttons buttons = {};
};


Interface DrawInterface(GUI::OSWindow* window, const int maxArraySize) {
	assert(window != nullptr);

	const int windowW = window->Width();
	const int windowH = window->Height();

	const int graphsMarginLeft = 0.05 * windowW;
	const int graphsMarginTop = 0.1 * windowH;
	const GUI::Vector2 graphsSize((windowW - 3 * graphsMarginLeft) / 2, 0.6 * windowH);
	const size_t graphLabelFontSize = 0.07 * graphsSize.x;

	const int buttonsMarginLeft = 0.03 * windowW;
	const int buttonsMarginTop = 0.1 * windowH;
	const GUI::Vector2 buttonsSize((windowW - 5 * buttonsMarginLeft) / 4, 0.15 * windowH);

	const GUI::Color bubbleColor("red");
	const GUI::Color selectionColor("green");
	const GUI::Color quickColor("blue");
	const GUI::Color mergeColor("yellow");

	const size_t buttonsFontSize = 0.1 * buttonsSize.x;
	const GUI::Vector2 buttonsLabelPos(10, (buttonsSize.y - buttonsFontSize) / 2);

	Interface interface = {window};

	GUI::GraphProps graphsProps(0, maxArraySize, 0, maxArraySize * maxArraySize / 2);
	interface.graphs.left =
		window->CreateGraph(graphsProps, GUI::Vector2(graphsMarginLeft, graphsMarginTop), graphsSize);
	interface.graphs.leftLabel =
		window->CreateText("Comparisons:", GUI::Vector2(graphsMarginLeft,
	                       graphsMarginTop - graphLabelFontSize), graphLabelFontSize);
	interface.graphs.right =
		window->CreateGraph(graphsProps, GUI::Vector2(2 * graphsMarginLeft + graphsSize.x,
		                    graphsMarginTop), graphsSize);
	interface.graphs.rightLabel =
		window->CreateText("Swaps:", GUI::Vector2(2 * graphsMarginLeft + graphsSize.x,
			graphsMarginTop - graphLabelFontSize), graphLabelFontSize);


	interface.buttons.bubble =
		window->CreateButton(GUI::Vector2(buttonsMarginLeft,
		                     graphsMarginTop + graphsSize.y + buttonsMarginTop), buttonsSize, bubbleColor);
	interface.buttons.bubble->AddLabel("bubble", buttonsLabelPos, buttonsFontSize);

	interface.buttons.selection =
		window->CreateButton(GUI::Vector2(2 * buttonsMarginLeft + buttonsSize.x,
		                     graphsMarginTop + graphsSize.y + buttonsMarginTop), buttonsSize, selectionColor);
	interface.buttons.selection->AddLabel("selection", buttonsLabelPos, buttonsFontSize);

	interface.buttons.quick =
		window->CreateButton(GUI::Vector2(3 * buttonsMarginLeft + 2 * buttonsSize.x,
		                     graphsMarginTop + graphsSize.y + buttonsMarginTop), buttonsSize, quickColor);
	interface.buttons.quick->AddLabel("quick", buttonsLabelPos, buttonsFontSize);

	interface.buttons.merge =
		window->CreateButton(GUI::Vector2(4 * buttonsMarginLeft + 3 * buttonsSize.x,
		                     graphsMarginTop + graphsSize.y + buttonsMarginTop), buttonsSize, mergeColor);
	interface.buttons.merge->AddLabel("merge", buttonsLabelPos, buttonsFontSize);

	window->Update();

	return interface;
}


void AnalyseSort(const Interface& interface, const int maxCount,
                 void (*Sort)(std::vector<int>&, bool (*)(const int, const int), void (*)(int&, int&)),
                 const GUI::Color& diagramColor = GUI::Color(0, 0, 0)) {

	assert(maxCount > 0);

	GUI::Graph::Diagram* comparesDiagram = interface.graphs.left->CreateDiagram(3, diagramColor);
	GUI::Graph::Diagram* swapsDiagram = interface.graphs.right->CreateDiagram(3, diagramColor);

	std::vector<long long> comparesByCount(maxCount, 0);
	std::vector<long long> swapsByCount(maxCount, 0);

	std::vector<int> arr;
	for (int curCount = 0; curCount < maxCount; ++curCount) {
		FillRandom(arr, curCount);

		Sort(arr, StatsFuncs::Comparator, StatsFuncs::Swap);

		//comparesByCount[curCount] = StatsFuncs::ComparatorCount();
		//swapsByCount[curCount] = StatsFuncs::SwapCount();
		comparesDiagram->AddData(curCount, StatsFuncs::ComparatorCount());
		swapsDiagram->AddData(curCount, StatsFuncs::SwapCount());
		interface.window->Update();

		StatsFuncs::ResetComparator();
		StatsFuncs::ResetSwap();
	}

	/*std::cout << "Compares:\n";
	PrintTable(comparesByCount);

	std::cout << "\nSwaps:\n";
	PrintTable(swapsByCount);*/
}


void ButtonsListener(void* voidArgs) {
	assert(voidArgs != nullptr);

	auto args = reinterpret_cast<std::tuple<Interface, const int, GUI::Button*>*>(voidArgs);

	const Interface& interface = std::get<0>(*args);
	const int maxCount = std::get<1>(*args);
	GUI::Button* button = std::get<2>(*args);
	assert(button != nullptr);

	if (button == interface.buttons.bubble) {
		AnalyseSort(interface, maxCount, BubbleSort, button->GetColor());

	} else if (button == interface.buttons.selection) {
		AnalyseSort(interface, maxCount, SelectionSort, button->GetColor());

	} else if (button == interface.buttons.quick) {
		AnalyseSort(interface, maxCount, QuickSort, button->GetColor());

	} else if (button == interface.buttons.merge) {
		AnalyseSort(interface, maxCount, MergeSort, button->GetColor());
	}
}


void AddButtonsListeners(const Interface& interface, const int maxArraySize) {
	static std::tuple<const Interface, const int, GUI::Button*> bubbleArgs(interface, maxArraySize,
	                                                                       interface.buttons.bubble);
	static std::tuple<const Interface, const int, GUI::Button*> selectionArgs(interface, maxArraySize,
	                                                                          interface.buttons.selection);
	static std::tuple<const Interface, const int, GUI::Button*> quickArgs(interface, maxArraySize,
	                                                                      interface.buttons.quick);
	static std::tuple<const Interface, const int, GUI::Button*> mergeArgs(interface, maxArraySize,
	                                                                      interface.buttons.merge);


	interface.buttons.bubble->AddLeftMouseUpListener(ButtonsListener, &bubbleArgs);
	interface.buttons.selection->AddLeftMouseUpListener(ButtonsListener, &selectionArgs);
	interface.buttons.quick->AddLeftMouseUpListener(ButtonsListener, &quickArgs);
	interface.buttons.merge->AddLeftMouseUpListener(ButtonsListener, &mergeArgs);
}


void CloseWindow(void* voidWindow) {
	GUI::OSWindow* window = static_cast<GUI::OSWindow*>(voidWindow);
	window->GetApplication().CloseWindow(window);
}


int main() {
	const int maxArraySize = 50;

	const int windowW = 1280;
	const int windowH = 720;

	GUI::Application app;
	GUI::OSWindow* window = app.CreateWindow(1280, 720, "Sorts Analyzer", GUI::Color("white"));

	Interface interface = DrawInterface(window, maxArraySize);

	AddButtonsListeners(interface, maxArraySize);

	window->AddWindowCloseListener(CloseWindow, window);

	double prevTime = glfwGetTime();
	while (app.WindowsOpened() > 0) {
		window->Update();
		app.ProcessEvents();
		double curTime = glfwGetTime();
		std::cout << 1 / (curTime - prevTime) << '\n';
		prevTime = curTime;
	}
	
	//GUI::Application app;
	//GUI::Window* window = app.CreateWindow(1080, 720, "Hello", GUI::Color("green"));

	//GUI::Line* line = window->CreateLine(GUI::Coordinates(10, 10), GUI::Coordinates(500, 500), 5, GUI::Color("red"));
	//line->Draw();

	//window->CreateText("Test text", GUI::Coordinates(50, 100), 32, GUI::Color("green"));

	//GUI::Button* button = window->CreateButton(GUI::Coordinates(50, 50));
	//button->AddLabel("test", GUI::Coordinates(10, 10), 30, GUI::Color("red"));

	//GUI::GraphProps props(-100, 100, -50, 100);
	//props.axesWidth = 3;
	//GUI::Graph* graph = window->CreateGraph(props, GUI::Coordinates(10, 100), GUI::Size(500, 500));
	//int diag = graph->AddDiagram(3, GUI::Color("red"));
	//graph->AddData(diag, 18 - 100, 22 - 50);
	//graph->AddData(diag, 0 - 100, 0 - 50);

	//int diag1 = graph->AddDiagram(3, GUI::Color("green"));
	//graph->AddData(diag1, 40 - 100, 69 - 50);
	//graph->AddData(diag1, 1 - 100, 1 - 50);
	//graph->AddData(diag, 100 - 100, 100 - 50);

	//window->Draw();

	//window->AddWindowCloseListener(CloseWindow, window);
	////button->AddLeftMouseUpListener(TestFunc, window);

	//while (app.WindowsOpened() > 0) {
	//	app.ProcessEvents();
	//}

	return 0;
}