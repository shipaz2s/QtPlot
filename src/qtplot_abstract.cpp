#include <qtplot_abstract.h>

#include <QPainter>

typedef std::vector<double> double_vector;

void QtPlotAbstract::setData(const double* y_value_ptr, size_t size)
{
	data.clear();
	plot_interval.setInterval(QtPlotType::Axis::X, 0, 0);
	plot_interval.setInterval(QtPlotType::Axis::Y, *y_value_ptr, *y_value_ptr);

	addData(y_value_ptr, size);
}

void QtPlotAbstract::setData(const double* x_value_ptr, const double* y_value_ptr, size_t size)
{
	data.clear();
	plot_interval.setInterval(QtPlotType::Axis::X, *x_value_ptr, *x_value_ptr);
	plot_interval.setInterval(QtPlotType::Axis::Y, *y_value_ptr, *y_value_ptr);

	addData(x_value_ptr, y_value_ptr, size);
}

void QtPlotAbstract::setData(QtPlotType::Curve_list& value, size_t size)
{
	plot_interval.setInterval(QtPlotType::Axis::X, value.front().x.front(), value.front().x.front());
	plot_interval.setInterval(QtPlotType::Axis::Y, value.front().y.front(), value.front().y.front());
	data = value;
	find_interval();

	emit signalAxesHint(axes_interval);

	if (auto_plot) {
		repaint();
	}
}

void QtPlotAbstract::addData(const double* y_value_ptr, size_t size)
{
	double_vector x_values;
	double_vector y_values;

	x_values.resize(size);
	for(int i = 0; i < size; ++i) {
		x_values[i] = i;
	}
	y_values.assign(y_value_ptr, y_value_ptr + size);

	if (plot_interval[QtPlotType::Axis::X].from > x_values.front() ) {
		plot_interval[QtPlotType::Axis::X].from = x_values.front();
	}

	if (plot_interval[QtPlotType::Axis::X].to < x_values.back() ) {
		plot_interval[QtPlotType::Axis::X].to = x_values.back();
	}

	if (plot_interval[QtPlotType::Axis::Y].from > y_values.front() ) {
		plot_interval[QtPlotType::Axis::Y].from = y_values.front();
	}

	if (plot_interval[QtPlotType::Axis::Y].to < y_values.back() ) {
		plot_interval[QtPlotType::Axis::Y].to = y_values.back();
	}

	QtPlotType::Curve new_curve;
	new_curve.x = std::move(x_values);
	new_curve.y = std::move(y_values);
	data.push_back(std::move(new_curve));

	axes_interval = plot_interval;
	emit signalAxesHint(axes_interval);

	if (auto_plot) {
		repaint();
	}
}

void QtPlotAbstract::addData(const double* x_value_ptr, const double* y_value_ptr, size_t size)
{
	double_vector x_values;
	double_vector y_values;

	
	x_values.assign(x_value_ptr, x_value_ptr + size);
	y_values.assign(y_value_ptr, y_value_ptr + size);

	if (plot_interval[QtPlotType::Axis::X].from > x_values.front() ) {
		plot_interval[QtPlotType::Axis::X].from = x_values.front();
	}

	if (plot_interval[QtPlotType::Axis::X].to < x_values.back() ) {
		plot_interval[QtPlotType::Axis::X].to = x_values.back();
	}

	if (plot_interval[QtPlotType::Axis::Y].from > y_values.front() ) {
		plot_interval[QtPlotType::Axis::Y].from = y_values.front();
	}

	if (plot_interval[QtPlotType::Axis::Y].to < y_values.back() ) {
		plot_interval[QtPlotType::Axis::Y].to = y_values.back();
	}

	QtPlotType::Curve new_curve;
	new_curve.x = std::move(x_values);
	new_curve.y = std::move(y_values);
	data.push_back(std::move(new_curve));

	axes_interval = plot_interval;
	emit signalAxesHint(axes_interval);

	if (auto_plot) {
		repaint();
	}
}

void QtPlotAbstract::deleteCurve(int index)
{
	if (index < 0 || index >= data.size()) {
		return;
	}

	auto iter = data.begin();
	int i = 0;
	while (i < index) {
		if (iter == data.end()) {return;}
		++i;
		++iter;
	}

	data.erase(iter);
	find_interval();

	emit signalAxesHint(axes_interval);

	if (auto_plot) {
		repaint();
	}
}

void QtPlotAbstract::find_interval()
{
	for (auto& cur_curve: data) {
		if (plot_interval[QtPlotType::Axis::X].from > cur_curve.x.front() ) {
			plot_interval[QtPlotType::Axis::X].from = cur_curve.x.front();
		}

		if (plot_interval[QtPlotType::Axis::X].to < cur_curve.x.back() ) {
			plot_interval[QtPlotType::Axis::X].to = cur_curve.x.back();
		}

		if (plot_interval[QtPlotType::Axis::Y].from > cur_curve.y.front() ) {
			plot_interval[QtPlotType::Axis::Y].from = cur_curve.y.front();
		}

		if (plot_interval[QtPlotType::Axis::Y].to < cur_curve.y.back() ) {
			plot_interval[QtPlotType::Axis::Y].to = cur_curve.y.back();
		}
	}

	if (data.size() < 1) {
		plot_interval = QtPlotType::QtPlotInterval();
	}
	axes_interval = plot_interval;
}