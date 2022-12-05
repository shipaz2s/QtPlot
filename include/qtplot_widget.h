#pragma once

#include <QWidget>
#include <QSize>
#include <QPoint>


#include <qtplot.h>
#include <qtplot_axes.h>
#include <qtplot_types.h>

class QtPlotWidget : public QWidget
{
	Q_OBJECT

public:
	QtPlotWidget(QWidget* parent = nullptr);

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void changeEvent(QEvent* event) override;

private:
	QtPlot* plot;
	QtPlotAxes* axes;

	QPoint plot_start_point;
	QSize plot_size;
};