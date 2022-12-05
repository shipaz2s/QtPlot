#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <vector>
#include <list>

typedef std::vector<double> double_vector;
typedef std::vector<double_vector> curve;
typedef std::list<curve> curve_list;


class QtPlot : public QWidget
{
	Q_OBJECT

public:
	QtPlot(QWidget* parent = nullptr) :
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

	void setValue(double* y_value_ptr, size_t size);
	void setValue(double* x_value_ptr, double* y_value_ptr, size_t size);
	void setValue(curve_list& value, size_t size);

	void addValue(double* y_value_ptr, size_t size);
	void addValue(double* x_value_ptr, double* y_value_ptr, size_t size);

	size_t curveCount() {return data.size();};
	void clear() {data.clear();};
	void deleteCurve(int index);

signals:
	void signalAxisHint();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	void find_min_max();

	curve_list data;
	std::vector< Qt::GlobalColor > colors;
	bool auto_plot = false;

	double x_min_value;
	double x_max_value;
	double y_min_value;
	double y_max_value;
};

class QtPlotGL : public QOpenGLWidget
{

};