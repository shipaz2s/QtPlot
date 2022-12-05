#pragma once

#include <QWidget>
// #include <QOpenGLWidget>
#include <vector>
#include <list>

#include <qtplot_types.h>

class QtPlotAbstract : public QWidget
{
	Q_OBJECT

public:
	QtPlotAbstract(QWidget* parent = nullptr) :
		QWidget(parent)
	{
		colors.resize(10);
		colors[0]  = Qt::blue;
		colors[1]  = Qt::darkRed;
		colors[2]  = Qt::darkGreen;
		colors[3]  = Qt::darkYellow;
		colors[4]  = Qt::darkCyan;
		colors[5]  = Qt::darkGray;		
	};

	bool isAutoPlot() {return auto_plot;};
	void setAutoPlot(bool value) {auto_plot = value;};

	void setData(const double* y_value_ptr, size_t size);
	void setData(const double* x_value_ptr, const double* y_value_ptr, size_t size);
	void setData(QtPlotType::Curve_list& value, size_t size);

	void addData(const double* y_value_ptr, size_t size);
	void addData(const double* x_value_ptr, const double* y_value_ptr, size_t size);

	QtPlotType::QtPlotInterval& getAxesInterval() {return axes_interval;};

	size_t curveCount() {return data.size();};
	void clear() {data.clear();};
	void deleteCurve(int index);

	void setInterval(const QtPlotType::QtPlotInterval& new_interval) {
		axes_interval = new_interval;
		emit signalAxesHint(axes_interval);
	}

	void setInterval(const QtPlotType::Axis axis, const QtPlotType::QtPlotAxisInterval& new_interval) {
		axes_interval[axis] = new_interval;
		emit signalAxesHint(axes_interval);
	}

signals:
	void signalAxesHint(const QtPlotType::QtPlotInterval&);

protected:
	void paintEvent(QPaintEvent* event) override = 0;

protected:
	void find_interval();

	QtPlotType::Curve_list data;
	std::vector< Qt::GlobalColor > colors;
	bool auto_plot = false;

	QtPlotType::QtPlotInterval plot_interval;
	QtPlotType::QtPlotInterval axes_interval;
};