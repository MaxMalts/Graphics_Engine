#include <stdlib.h>
#include <string.h>
#include <unordered_set>
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


	Graph::Diagram::Diagram(Graph& graph, const size_t width, const Color& color)
		: graph(graph), polyline(graph, PolylineProps(width, color)) {}


	void Graph::Diagram::AddData(int column, int value) {
		int curVertexX = graph.innerPos.x +
			(column - graph.props.startX) / static_cast<float>(graph.props.rangeX) * graph.innerSize.x;

		int curVertexY = graph.innerPos.y + graph.innerSize.y -
			(value - graph.props.startY) / static_cast<float>(graph.props.rangeY) * graph.innerSize.y;

		data.push_back(DiagramData(column, value));
		polyline.AddVertex(Vector2(curVertexX, curVertexY) - graph.AbsPosition());
	}


	void Graph::Diagram::Draw() {
		polyline.Draw();
	}


	Graph::Graph(OSWindow& osWindow, Window* parent, const GraphProps& props, const Vector2& pos, const Vector2& size)
		: Window(osWindow, parent, pos, size), props(props) {

		background = new Rectangle(*this, RectangleProps(Vector2(0, 0), size, props.bgColor));

		InitGraphParts();
	}


	Graph::Diagram::DiagramData::DiagramData(int column, int value) : column(column), value(value) {}

	Graph::Diagram* Graph::CreateDiagram(const size_t lineWidth, const Color& color) {

		Diagram* newDiag = new Diagram(*this, lineWidth, color);

		diagrams.push_back(newDiag);

		return newDiag;
	}

	void Graph::RemoveDiargam(Diagram* diagram) {
		if (diagram == nullptr) {
			return;
		}

		std::list<Diagram*>::iterator diagIter = std::find(diagrams.begin(), diagrams.end(), diagram);
		if (diagIter != diagrams.end()) {
			delete diagram;
			diagrams.erase(diagIter);
		}
	}


	void Graph::InitGraphParts() {
		constexpr size_t maxIntStrLen = Log10Compile(INT_MAX) + 2;
		char tempBuf1[maxIntStrLen + 1] = "";
		char tempBuf2[maxIntStrLen + 1] = "";

		const int innerPadding = Width() / 15;
		const int arrowFrontOffset = innerPadding / 1.3;
		const int arrowSideOffset = arrowFrontOffset / 3;

		const int hatchSize = 3;

		const int rowLabelsOffset = props.fontSize + 20;
		const int columnLabelsOffset = std::max(strlen(itoa(props.startX, tempBuf1, 10)),
			strlen(itoa(props.startX + props.rangeX, tempBuf2, 10))) *
			props.fontSize + props.fontSize * 2;

		innerPos.x = AbsPosition().x + std::max(strlen(itoa(props.startY, tempBuf1, 10)),
			strlen(itoa(props.startY + props.rangeY, tempBuf2, 10))) * props.fontSize +
			props.hatchSize;

		innerSize.x = Width() - (innerPos.x - AbsPosition().x) - innerPadding;
		innerPos.y = AbsPosition().y + innerPadding;
		innerSize.y = Height() - props.fontSize - props.hatchSize - innerPadding;

		InitArrows(arrowFrontOffset, arrowSideOffset);

		InitLabels(rowLabelsOffset, columnLabelsOffset);
	}


	void Graph::InitArrows(const int arrowFrontOffset, const int arrowSideOffset) {

		Vector2 innerEndPos(innerPos.x + innerSize.x, innerPos.y + innerSize.y);

		axes = new Polyline(*this, PolylineProps(props.axesWidth, props.axesColor));
		axes->AddVertex(Vector2(innerPos.x, innerPos.y) - AbsPosition());
		axes->AddVertex(Vector2(innerPos.x - arrowSideOffset, innerPos.y) - AbsPosition());
		axes->AddVertex(Vector2(innerPos.x, innerPos.y - arrowFrontOffset) - AbsPosition());
		axes->AddVertex(Vector2(innerPos.x + arrowSideOffset, innerPos.y) - AbsPosition());
		axes->AddVertex(Vector2(innerPos.x, innerPos.y) - AbsPosition());
		axes->AddVertex(Vector2(innerPos.x, innerEndPos.y) - AbsPosition());

		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y) - AbsPosition());
		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y - arrowSideOffset) - AbsPosition());
		axes->AddVertex(Vector2(innerEndPos.x + arrowFrontOffset, innerEndPos.y) - AbsPosition());
		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y + arrowSideOffset) - AbsPosition());
		axes->AddVertex(Vector2(innerEndPos.x, innerEndPos.y) - AbsPosition());
	}


	void Graph::InitLabels(const int rowLabelsOffset, const int columnLabelsOffset) {
		constexpr size_t maxIntStrLen = Log10Compile(INT_MAX) + 2;
		char tempBuf[maxIntStrLen + 1] = "";

		int curY = props.fontSize / 2;
		while (curY < innerSize.y) {
			int curLabelVal = (curY - props.fontSize / 2) / static_cast<float>(innerSize.y) *
				props.rangeY + props.startY;

			int curYWindow = innerPos.y + innerSize.y - curY;

			Text* curLabel = new Text(*this,
				TextProps(itoa(curLabelVal, tempBuf, 10),
				Vector2(AbsPosition().x, curYWindow) - AbsPosition(),
				FontProps(props.fontSize, props.fontColor)));

			Line* curHatch = new Line(*this,
				LineProps(Vector2(innerPos.x, curYWindow + props.fontSize / 2) - AbsPosition(),
				Vector2(innerPos.x - props.hatchSize, curYWindow + props.fontSize / 2) - AbsPosition(),
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
			Text* curLabel = new Text(*this, TextProps(itoa(curLabelVal, tempBuf, 10),
				Vector2(curXWindow, AbsPosition().y + Height() - props.fontSize) - AbsPosition(),
				FontProps(props.fontSize, props.fontColor)));

			Line* curHatch = new Line(*this, LineProps(Vector2(curXWindow, innerPos.y + innerSize.y) - AbsPosition(),
				Vector2(curXWindow, innerPos.y + innerSize.y + props.hatchSize) - AbsPosition(),
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

		for (auto curDiagram : diagrams) {
			curDiagram->Draw();
		}

		for (int i = 0; i < labels.size(); ++i) {
			labels[i]->Draw();
		}

		for (int i = 0; i < hatches.size(); ++i) {
			hatches[i]->Draw();
		}

		DrawInsides();
	}


	Graph::~Graph() {
		delete axes;
		delete background;
		DeleteConatinerElements(labels);
		DeleteConatinerElements(hatches);

		DeleteConatinerElements(diagrams);
	}
}