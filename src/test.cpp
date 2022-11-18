#include <QApplication>

#include <qtplot_axes.h>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QtPlotAxes axes;
	axes.show();

	return app.exec();
}