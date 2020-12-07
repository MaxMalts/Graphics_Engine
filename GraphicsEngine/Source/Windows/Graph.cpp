#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "Include\Extra.h"
#include "Include\Common.h"
#include "Include\Application.h"

#include "Include\Primitives\Line.h"
#include "Include\Primitives\Polyline.h"
#include "Include\Primitives\Rectangle.h"
#include "Include\Primitives\Text.h"

#include "Include\Windows\Graph.h"



namespace GUI {

	GraphProps::GraphProps(const int startX, const int rangeX, const int startY, const int rangeY)
		: startX(startX), rangeX(rangeX), startY(startY), rangeY(rangeY) {}


	Graph::Diagram::Diagram(Graph& graph, OSWindow& window, const size_t width, const Color& color)
		: graph(graph), polyline(window, PolylineProps(width, color)) {}


	void Graph::Diagram::AddData(int column, int value) {
		int curVertexX = graph.innerPos.x +
			(column - graph.props.startX) / static_cast<float>(graph.props.rangeX) * graph.innerSize.x;

		int curVertexY = graph.innerPos.y + graph.innerSize.y -
			(value - graph.props.startY) / static_cast<float>(graph.props.rangeY) * graph.innerSize.y;

		data.push_back(DiagramData(column, value));
		polyline.AddVertex(Vector2(curVertexX, curVertexY));
	}


	void Graph::Diagram::Draw() {
		polyline.Draw();
	}


	Graph::Graph(OSWindow& osWindow, const GraphProps& props, const Vector2& pos, const Vector2& size)
		: Window(osWindow, pos, size), props(props) {

		const size_t axesWidth = 3;

		background = new Rectangle(osWindow, RectangleProps(pos, size, props.bgColor));

		InitGraphParts();
	}


	Graph::Diagram::DiagramData::DiagramData(int column, int value) : column(column), value(value) {}

	Graph::Diagram* Graph::CreateDiagram(const size_t lineWidth, const Color& color) {
		Diagram* newDiag = new Diagram(*this, osWindow, lineWidth, color);

		diagrams.push_back(newDiag);

		return newDiag;
	}


	void Graph::InitGraphParts() {
		constexpr size_t maxIntStrLen = Log10Compile(INT_MAX) + 2;
		char tempBuf1[maxIntStrLen + 1] = "";
		char tempBuf2[maxIntStrLen + 1] = "";

		const int innerPadding = size.x / 15;
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

		innerSize.x = size.x - (innerPos.x - pos.x) - innerPadding;
		innerPos.y = pos.y + innerPadding;
		innerSize.y = size.y - props.fontSize - props.hatchSize - innerPadding;

		InitArrows(arrowFrontOffset, arrowSideOffset);

		InitLabels(rowLabelsOffset, columnLabelsOffset);
	}


	void Graph::InitArrows(const int arrowFrontOffset, const int arrowSideOffset) {

		Vector2 innerEndPos(innerPos.x + innerSize.x, innerPos.y + innerSize.y);

		axes = new Polyline(osWindow, PolylineProps(props.axesWidth, props.axesColor));
		axes->AddVertex(Vector2(innerPos.x, innerPos.y));
		axes->AddVertex(Vector2(innerPos.x - arrowSideOffset, innerPos.y));
		axes->AddVertex(Vector2(innerPos.x, innerPos.y - arrowFrontOffset));
		axes->AddVertex(Vector2(innerPos.x + arrowSideOffset, innerPos.y));
		axes->AddVertex(Vector2(innerPos.x, innerPos.y));
		axes->AddVertex(Vector2(innerPos.x, innerEndPos.y));

		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y));
		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y - arrowSideOffset));
		axes->AddVertex(Vector2(innerEndPos.x + arrowFrontOffset, innerEndPos.y));
		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y + arrowSideOffset));
		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y));
	}


	void Graph::InitLabels(const int rowLabelsOffset, const int columnLabelsOffset) {
		constexpr size_t maxIntStrLen = Log10Compile(INT_MAX) + 2;
		char tempBuf[maxIntStrLen + 1] = "";

		int curY = props.fontSize / 2;
		while (curY < innerSize.y) {
			int curLabelVal = (curY - props.fontSize / 2) / static_cast<float>(innerSize.y) *
				props.rangeY + props.startY;

			int curYWindow = innerPos.y + innerSize.y - curY;

			Text* curLabel = new Text(osWindow, TextProps(itoa(curLabelVal, tempBuf, 10),
				Vector2(pos.x, curYWindow), props.fontSize, props.fontColor));

			Line* curHatch = new Line(osWindow, LineProps(Vector2(innerPos.x, curYWindow + props.fontSize / 2),
				Vector2(innerPos.x - props.hatchSize, curYWindow + props.fontSize / 2),
				props.axesWidth, props.axesColor));

			labels.push_back(curLabel);
			hatches.push_back(curHatch);

			curY += rowLabelsOffset;
		}


		int curX = 0;
		while (curX < innerSize.x) {
			int curLabelVal = curX / static_cast<float>(innerSize.x) *
				props.rangeX + props.startX;

			int curXWindow = innerPos.x + curX;
			Text* curLabel = new Text(osWindow, TextProps(itoa(curLabelVal, tempBuf, 10),
				Vector2(curXWindow, pos.y + size.y - props.fontSize),
				props.fontSize, props.fontColor));

			Line* curHatch = new Line(osWindow, LineProps(Vector2(curXWindow, innerPos.y + innerSize.y),
				Vector2(curXWindow, innerPos.y + innerSize.y + props.hatchSize),
				props.axesWidth, props.axesColor));

			labels.push_back(curLabel);
			hatches.push_back(curHatch);
			curX += columnLabelsOffset;
		}
	}


	void Graph::Draw() {
		osWindow.SetActive();

		background->Draw();
		axes->Draw();

		for (int i = 0; i < diagrams.size(); ++i)
			diagrams[i]->Draw();

		for (int i = 0; i < labels.size(); ++i)
			labels[i]->Draw();

		for (int i = 0; i < hatches.size(); ++i)
			hatches[i]->Draw();
	}


	Graph::~Graph() {
		delete axes;
		delete background;
		DeleteArrayElements(labels);
		DeleteArrayElements(hatches);

		DeleteArrayElements(diagrams);
	}
}