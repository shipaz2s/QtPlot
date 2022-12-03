#include "qtplot_widget.h"

QtPlotWidget::QtPlotWidget(QWidget* parent) :
	QWidget(parent)
{
	axes = new QtPlotAxes(this);
	plot = new QtPlot(this);
}