#pragma once

#include <QWidget>
#include <QSize>
#include <QPoint>


#include <qtplot.h>
#include <qtplot_axes.h>
#include <qtplot_types.h>
#include <qtplot_zoomer.h>
#include <qtplot_marker.h>

#include <set>

class QtPlotWidget : public QWidget
{
	Q_OBJECT

public:
	QtPlotWidget(QWidget* parent = nullptr);

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

	void setData(const double* y_value_ptr, size_t size) {
		plot->setData(y_value_ptr, size);
		axes_default_interval = plot->getAxesInterval();
	};
	void setData(const double* x_value_ptr, const double* y_value_ptr, size_t size) {
		plot->setData(x_value_ptr, y_value_ptr, size);
		axes_default_interval = plot->getAxesInterval();
	};
	void setData(QtPlotType::Curve_list& value, size_t size) {
		plot->setData(value, size);
		axes_default_interval = plot->getAxesInterval();
	};

	void addData(const double* y_value_ptr, size_t size) {
		plot->addData(y_value_ptr, size);
		axes_default_interval = plot->getAxesInterval();
	};
	void addData(const double* x_value_ptr, const double* y_value_ptr, size_t size) {
		plot->addData(x_value_ptr, y_value_ptr, size);
		axes_default_interval = plot->getAxesInterval();
	};
	
	void setInterval(const QtPlotType::QtPlotInterval& new_interval) { 
		plot->setInterval(new_interval);
		axes_default_interval = new_interval;
		zoom_intervals.clear();
	};
	void setInterval(const QtPlotType::Axis axis, const QtPlotType::QtPlotAxisInterval& new_interval) {
		plot->setInterval(axis, new_interval);
		axes_default_interval[axis] = new_interval;
		zoom_intervals.clear();
	};

	void setZoomIn(bool value) {zooming_in = value;};
	void setZoomOut(bool value) {zooming_out = value;};
	void setPicking(bool value) {picking = value;};
	void setMoving(bool value) {moving = value;};

public slots:
	void slotDeleteMarker(QPointF&);
	void slotDeleteMarkers();

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void changeEvent(QEvent* event) override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	void moveMarkers();

	QtPlot* plot;
	QtPlotAxes* axes;

	QPoint plot_start_point;
	QSize plot_size;

	QtPlotZoomer* zoomer;
	QPoint zoomer_start_point;
	QtPlotMarker* new_marker;

	std::set<QtPlotMarker*, QtPlotMarkerComparator> markers;

	bool zooming = false;
	bool picking = false;
	bool moving = false;
	bool zooming_out = false;
	bool zooming_in = false;

	QtPlotType::QtPlotInterval axes_default_interval;
	std::list<QtPlotType::QtPlotInterval> zoom_intervals;
};