#include <QApplication>

#include <qtplot_widget.h>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QtPlotWidget wgt;
	wgt.show();

	// QtPlotAxes axes;
	// axes.show();

	return app.exec();
}