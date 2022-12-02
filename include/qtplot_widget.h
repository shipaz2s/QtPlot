#pragma once

#include <QWidget>

#include "qtplot.h"
#include "qtplot_axes.h"

namespace QtPlotType
{
	enum class Plot
	{
		Plot,
		WaterFall,
		Histogram
	};
}

class QtPlotWidget : public QWidget
{
	Q_OBJECT

public:
	QtPlotWidget(QWidget* parent);

	QtPlot* plot;
	QtPlotAxes* axes;
};