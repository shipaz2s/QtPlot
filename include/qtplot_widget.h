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
	enum class Status {
		watching,
		pointing,
		markering,
		moving,
		zooming_in,
		zooming_out
	};

public:
	QtPlotWidget(QtPlotType::Plot type = QtPlotType::Plot::Plot, QWidget* parent = nullptr);
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

	void setStatus(Status value) {
		status = value;

		if (status == Status::pointing) {
			this->setMouseTracking(true);
			plot->setMouseTracking(true);
			axes->setMouseTracking(true);
		} else {
			plot->setMouseTracking(true);
			this->setMouseTracking(false);
			axes->setMouseTracking(false);
		}
	}

	QtPlotAbstract* getPlot() {return plot;};
	QtPlotType::Plot type() {return plot_type;};

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

	QtPlotType::Plot plot_type;
	QtPlotAbstract* plot;
	QtPlotAxes* axes;

	QPoint plot_start_point;
	QSize plot_size;

	QtPlotZoomer* zoomer;
	QPoint zoomer_start_point;
	QtPlotMarker* new_marker;
	QPoint move_start_point;

	std::set<QtPlotMarker*, QtPlotMarkerComparator> markers;
	QLabel* pointing_lbl;

	bool zooming = false;
	bool moving_started = false;

	Status status = Status::watching;
	/*
	0 - default (watching)
	1 - pointing
	2 - setting markers
	3 - moving
	4 - zooming in
	5 - zooming out
	*/

	QtPlotType::QtPlotInterval axes_default_interval;
	std::list<QtPlotType::QtPlotInterval> zoom_intervals;
};