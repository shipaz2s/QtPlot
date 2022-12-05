#include "qtplot_widget.h"

#include <QEvent>

QtPlotWidget::QtPlotWidget(QWidget* parent) :
	QWidget(parent)
{
	setMinimumSize(320, 240);
	axes = new QtPlotAxes(this);
	plot = new QtPlot(this);
	plot_start_point = axes->getPlotStartPoint();
	plot_size = axes->getPlotSize();

	QPalette pal = QPalette();

	// set black background
	// Qt::black / "#000000" / "black"
	pal.setColor(QPalette::Window, Qt::black);

	plot->setAutoFillBackground(true); 
	plot->setPalette(pal);

	plot->lower();
	// plot->raise();

	axes->setSegment(QtPlotType::Axis::X, 10, 50);
	axes->setSegment(QtPlotType::Axis::Y, -20, 140);
}

QSize QtPlotWidget::minimumSizeHint() const
{
	return QSize(320, 240);
}

QSize QtPlotWidget::sizeHint() const
{
	return QSize(640, 480);
}

void QtPlotWidget::paintEvent(QPaintEvent* event)
{
}

void QtPlotWidget::resizeEvent(QResizeEvent* event)
{
	int height = this->height();
	int width = this->width();

	axes->resize(width, height);
	
	plot_start_point = axes->getPlotStartPoint();
	plot_size = axes->getPlotSize();

	plot->move(plot_start_point);
	plot->resize(plot_size);
}

void QtPlotWidget::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::WindowStateChange)
	{
		axes->updateLabels();
	}
}