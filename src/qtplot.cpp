#include <qtplot.h>
#include <QPainter>
#include <vector>
#include <list>

typedef std::vector<double> double_vector;

void QtPlot::setData(const double* x_value_ptr, const double* y_value_ptr, size_t size)
{
	data.clear();
	plot_interval.setInterval(QtPlotType::Axis::X, *x_value_ptr, *x_value_ptr);
	plot_interval.setInterval(QtPlotType::Axis::Y, *y_value_ptr, *y_value_ptr);

	addData(x_value_ptr, y_value_ptr, size);
}

void QtPlot::setData(QtPlotType::Curve_list& value, size_t size)
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

void QtPlot::addData(const double* x_value_ptr, const double* y_value_ptr, size_t size)
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

void QtPlot::paintEvent(QPaintEvent* event)
{
	int height = this->height();
	int width = this->width();
	double dx = (double) width / (axes_interval[QtPlotType::Axis::X].to - axes_interval[QtPlotType::Axis::X].from);
	double dy = (double) height / (axes_interval[QtPlotType::Axis::Y].to - axes_interval[QtPlotType::Axis::Y].from);

	QPainter painter;
	painter.begin(this);

	int index = 0;
	for(auto iter = data.begin(); iter != data.end() ; ++iter) {
		if ( (*iter).x.size() < 2 ) { continue; }
			painter.setPen( QPen(colors[index], 3) );

		for (int i = 1; i < (*iter).x.size(); i += 1) {
			painter.drawLine( 	
								( (*iter).x[i - 1] - axes_interval[QtPlotType::Axis::X].from ) * dx,
								( axes_interval[QtPlotType::Axis::Y].to - (*iter).y[i - 1]) * dy,
								( (*iter).x[i] - axes_interval[QtPlotType::Axis::X].from ) * dx,
								( axes_interval[QtPlotType::Axis::Y].to - (*iter).y[i]) * dy
							);
		}
		++index;
	}

	painter.end();
}