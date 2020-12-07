#pragma once

#include "Include\Common.h"
#include "Include\Application.h"
#include "Window.h"
#include "Include\Primitives\Line.h"
#include "Include\Primitives\Polyline.h"
#include "Include\Primitives\Rectangle.h"
#include "Include\Primitives\Text.h"



namespace GUI {
	struct GraphProps : public WindowProps {
	public:

		GraphProps() = default;

		GraphProps(const int startX, const int rangeX, const int startY, const int rangeY);

		int startX = 0, rangeX = 100, startY = 0, rangeY = 100;
		Color bgColor = Color(0.9, 0.9, 0.9);
		size_t fontSize = 16, axesWidth = 3, hatchSize = 10;
		Color axesColor = Color(0, 0, 0), fontColor = Color(0, 0, 0);
	};


	class Graph : public Window {
	public:

		class Diagram {
		public:

			Diagram(Graph& graph, OSWindow& window, const size_t width = 1, const Color& color = Color(0, 0, 0));

			void AddData(int column, int value);

			virtual void Draw();

		private:

			Graph& graph;

			struct DiagramData {
				DiagramData() = default;

				DiagramData(int column, int value);

				int column = 0, value = 0;
			};

			std::vector<DiagramData> data;

			Polyline polyline;
		};


		Graph(OSWindow& window, const GraphProps& props,
			const Vector2& pos, const Vector2& size);

		Graph(const Graph& other) = delete;


		Diagram* CreateDiagram(const size_t lineWidth = 1, const Color& color = Color(0, 0, 0));

		void Draw();

		~Graph();

	private:

		void InitGraphParts();

		void InitArrows(const int arrowBackOffset, const int arrowSideOffset);

		void InitLabels(const int rowLabelsOffset, const int columnLabelsOffset);


		GraphProps props;

		std::vector<Diagram*> diagrams;

		Rectangle* background = nullptr;
		Polyline* axes = nullptr;
		std::vector<Text*> labels;
		std::vector<Line*> hatches;

		Vector2 innerPos;    // Position without axes and labels
		Vector2 innerSize;    // Size of graph without axes and labels
	};
}