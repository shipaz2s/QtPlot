#include <qtplot.h>

void QtPlot::setValue(double* y_value_ptr, size_t size)
{
	data.clear();
	x_min_value = 0;
	x_max_value = 0;
	y_min_value = *y_value_ptr;
	y_max_value = *y_value_ptr;

	addValue(y_value_ptr, size);
}

void QtPlot::setValue(double* x_value_ptr, double* y_value_ptr, size_t size)
{
	data.clear();
	x_min_value = *x_value_ptr;
	x_max_value = *x_value_ptr;
	y_min_value = *y_value_ptr;
	y_max_value = *y_value_ptr;

	addValue(x_value_ptr, y_value_ptr, size);
}

void QtPlot::setValue(curve_list& value, size_t size)
{
	x_min_value = value.front()[0].front();
	x_max_value = value.front()[0].front();
	y_min_value = value.front()[1].front();
	y_max_value = value.front()[1].front();
	data = value;
	find_min_max();
}

void QtPlot::addValue(double* y_value_ptr, size_t size)
{
	double_vector x_values;
	double_vector y_values;

	x_values.resize(size);
	for(int i = 0; i < size; ++i) {
		x_values[i] = i;
	}
	y_values.assign(y_value_ptr, y_value_ptr + size);

	if (x_min_value > x_values.front() ) {
		x_min_value = x_values.front();
	}

	if (x_max_value < x_values.back() ) {
		x_max_value = x_values.back();
	}

	if (y_min_value > y_values.front() ) {
		y_min_value = y_values.front();
	}

	if (y_max_value < y_values.back() ) {
		y_max_value = y_values.back();
	}

	curve new_curve(2);
	new_curve[0] = std::move(x_values);
	new_curve[1] = std::move(y_values);
	data.push_back(std::move(new_curve));
}

void QtPlot::addValue(double* x_value_ptr, double* y_value_ptr, size_t size)
{
	double_vector x_values;
	double_vector y_values;

	
	y_values.assign(x_value_ptr, x_value_ptr + size);
	y_values.assign(y_value_ptr, y_value_ptr + size);

	if (x_min_value > x_values.front() ) {
		x_min_value = x_values.front();
	}

	if (x_max_value < x_values.back() ) {
		x_max_value = x_values.back();
	}

	if (y_min_value > y_values.front() ) {
		y_min_value = y_values.front();
	}

	if (y_max_value < y_values.back() ) {
		y_max_value = y_values.back();
	}

	curve new_curve(2);
	new_curve[0] = std::move(x_values);
	new_curve[1] = std::move(y_values);
	data.push_back(std::move(new_curve));
}

void QtPlot::deleteCurve(int index)
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
}

void QtPlot::find_min_max()
{
	for (auto& cur_curve: data) {
		if ( x_min_value > cur_curve[0].front() ) {
			x_min_value = cur_curve[0].front();
		}
		if ( x_max_value < cur_curve[0].back() ) {
			x_max_value = cur_curve[0].back();
		}
		if ( y_min_value > cur_curve[1].front() ) {
			y_min_value = cur_curve[1].front();
		}
		if ( y_max_value < cur_curve[1].back() ) {
			y_max_value = cur_curve[1].back();
		}
	}
}

void QtPlot::paintEvent(QPaintEvent* event)
{
	
}