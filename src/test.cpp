#include <QApplication>

#include <qtplot_widget.h>

#include <vector>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QtPlotWidget wgt;
	wgt.show();

	// std::vector<double> x_values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	// std::vector<double> y_values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	// std::vector<double> x_values_2 = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	// std::vector<double> y_values_2 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

	std::vector<double> x_values = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	std::vector<double> y_values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<double> x_values_2 = {1, 3, 5, 7, 9, 11, 12, 13, 14, 15};
	std::vector<double> y_values_2 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

	wgt.setData(x_values.data(), y_values.data(), x_values.size());
	wgt.addData(x_values_2.data(), y_values_2.data(), x_values_2.size());
	// wgt.setInterval(
	// 	QtPlotType::QtPlotInterval(
	// 		QtPlotType::QtPlotAxisInterval(0., 15.),
	// 		QtPlotType::QtPlotAxisInterval(0., 15.),
	// 		QtPlotType::QtPlotAxisInterval()
	// 	)
	// );
	wgt.setInterval(QtPlotType::Axis::X, QtPlotType::QtPlotAxisInterval(-30., 30.) );
	wgt.setInterval(QtPlotType::Axis::Y, QtPlotType::QtPlotAxisInterval(-20., 20.) );

	// QtPlotAxes axes;
	// axes.show();

	return app.exec();
}