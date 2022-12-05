#include <qtplot.h>
#include <QPainter>

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