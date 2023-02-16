#include "qtplot_histogram.h"

#include <QLocale>
#include <QPainter>

void QtPlotHistogram::paintEvent(QPaintEvent* event)
{
	int min_data_count = data.front().x.size();
	if (min_data_count == 0) {return;};
	for(auto iter = data.begin(); iter != data.end() ; ++iter) {
		if (iter->x.size() < min_data_count) {
			min_data_count = iter->x.size();
		}
	}

	int curves_count = data.size();

    int height = this->height();
	int width = this->width();
	double ddx = (double) width / min_data_count / curves_count ;
	double dx = ddx * curves_count;
	double dy = (double) height / (axes_interval[QtPlotType::Axis::Y].to - axes_interval[QtPlotType::Axis::Y].from);

    int zero_level = axes_interval[QtPlotType::Axis::Y].to * dy;
	// отсчет пикселей идет сверху.
	// пожтому ноль находится на уровне X пикселей,
	// где X = Количество пикселей между верхом графика и нулем
	// Количсетво пикселей в графике = to * dx - from * dx;
	// Количество пикселей от нуля до нижней точки = 0 * dx - from * dx;
	// => наш нулевой уровениь (to * dx - from * dx) - (0 * dx - from * dx)

	QPainter painter;
	painter.begin(this);

	int index = 0;
	
	int x0, y0;
	QFontMetrics fm(this->font());
	int text_height = fm.height();
	for(auto iter = data.begin(); iter != data.end() ; ++iter){
		for (int i = 0; i < min_data_count; i += 1) {
			painter.setPen( QPen( QColor(0,0,0), 1) );
			painter.setBrush( QBrush(colors[index]) );

			x0 = i * dx + index * ddx;
			int rect_w = ddx;

			y0 = ( axes_interval[QtPlotType::Axis::Y].to - (*iter).y[i] ) * dy;
			int rect_h = abs(zero_level - y0);

			QString txt = locale().toString( (*iter).y[i], 'g' );
			int text_width = fm.horizontalAdvance(txt);

			int txt_y = y0;
			if (y0 > zero_level) {
				y0 = zero_level;
			}

			painter.drawRect(
				x0,
				y0,
				rect_w,
				rect_h
			);

			auto bg_color = palette().color(backgroundRole());
			auto fg_color = palette().color(foregroundRole());
			bg_color.setAlpha(180);
			painter.setBrush( QBrush( bg_color ) );
			painter.setPen( QPen( QBrush( QColor(0,0,0, 0) ), 1 )  );

			painter.drawRect(x0 + rect_w / 2 - text_width / 2, txt_y - text_height - y_margin, text_width, text_height);
			painter.setPen( QPen( fg_color, 1 ) );
			painter.drawText(x0 + rect_w / 2 - text_width / 2, txt_y - y_margin, txt);
		}
		++index;
	}

	painter.end();
}