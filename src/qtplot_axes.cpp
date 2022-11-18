#include <qtplot_axes.h>
#include <QPainter>
#include <QLocale>

#include <QResizeEvent>
#include <QCoreApplication>


QtPlotAxes::QtPlotAxes(QWidget* parent) :
	QWidget(parent)
{
	setMinimumSize(300, 200);

	text_brush.setColor(palette().color(QPalette::Text));
	axis_brush.setColor(palette().color(QPalette::Text));
	
	QString test_text = locale().toString(11234567.890123, 'g');

	fm_ptr = new QFontMetrics(this->font());

	label_width = fm_ptr->horizontalAdvance(test_text);
	label_heigth = fm_ptr->height();

	xAxis_label = new QLabel("X", this);
	yAxis_label = new QLabel("Y", this);
	xAxis_label->raise();
	yAxis_label->raise();

	xLabel_width = fm_ptr->horizontalAdvance(xAxis_label->text());
	yLabel_width = fm_ptr->horizontalAdvance(yAxis_label->text());

	x_lables.reserve(10);
	y_lables.reserve(10);

	double dx = (x_max_value - x_min_value) / 10;
	double dy = (y_max_value - y_min_value) / 10;
	for (int i = 0; i < 11; ++i) {
		x_lables.push_back( new QLabel( locale().toString(x_min_value + dx * i, 'g'), this) );
		y_lables.push_back( new QLabel( locale().toString(y_min_value + dy * i, 'g'), this) );
	}

	// to set variables used in resizeEvent (which are usually set in paint event), cause paintEvent is called after resizeEvent
	resize(640, 480);
	x_delta = (this->width() - 2 * horizontal_intend - label_width - hatch_length - xLabel_width)/ 10;
	y_delta = (this->height() - 2 * vertical_intend  - label_heigth - hatch_length - label_heigth) / 10;
}

QSize QtPlotAxes::minimumSizeHint() const {
	
	return QSize(320, 240);
}

QSize QtPlotAxes::sizeHint() const {
	
	return QSize(640, 480);
}

void QtPlotAxes::setSegment(QtPlotType::Axis axis, double min_value, double max_value)
{
	switch (axis)
	{
	case QtPlotType::Axis::X :
		x_min_value = min_value;
		y_max_value = max_value;
		break;

	case QtPlotType::Axis::Y :
		x_min_value = min_value;
		y_max_value = max_value;
		break;
	
	default:
		break;
	}
}

void QtPlotAxes::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);

	painter.setBrush(axis_brush);
	
	int width = this->width();
	int height = this->height();

	x_delta = (width - 2 * horizontal_intend - label_width - hatch_length - xLabel_width)/ 10;

	if (x_delta >= min_delta) {
		int i;
		for (i = label_width + horizontal_intend + hatch_length; i <= width - horizontal_intend - xLabel_width; i+= x_delta) {
			painter.drawLine(i, height - hatch_length - label_heigth - vertical_intend, i, height - label_heigth - vertical_intend);
		}
		plot_width_hint = i - x_delta;
	}

	y_delta = (height - 2 * vertical_intend  - label_heigth - hatch_length - label_heigth) / 10;

	if (y_delta >= min_delta) {
		int i;
		for (i = height - label_heigth - vertical_intend - hatch_length; i >= label_heigth + vertical_intend; i -= y_delta) {
			painter.drawLine(label_width + horizontal_intend, i, label_width + horizontal_intend + hatch_length, i);
		}
		plot_height_hint = i + y_delta;
	}

	painter.drawLine(	label_width + horizontal_intend + hatch_length,
						height - label_heigth - vertical_intend - hatch_length,
						label_width + horizontal_intend + hatch_length,
						plot_height_hint
					);

	painter.drawLine(	label_width + horizontal_intend + hatch_length,
						height - label_heigth - vertical_intend - hatch_length,
						plot_width_hint,
						height - label_heigth - vertical_intend - hatch_length
					);

	painter.end();
}

void QtPlotAxes::resizeEvent(QResizeEvent* event)
{
	int height = this->height();
	int width = this->width();

	xAxis_label->move(width - xLabel_width, height - label_heigth - vertical_intend - hatch_length - label_heigth / 2);
	yAxis_label->move(label_width + horizontal_intend + hatch_length - yLabel_width / 2, 0);

	int x_start = label_width + horizontal_intend + hatch_length;
	int y_start = height - label_heigth - vertical_intend - hatch_length - label_heigth / 2;


	int x_add_length = 0;
	int last_x_pos = INT_MIN;
	int x_pos;

	int y_add_length = 0;
	int last_y_pos = INT_MAX;
	int y_pos;
	for (int i = 0; i < 11; ++i) {
		x_add_length = fm_ptr->horizontalAdvance(x_lables[i]->text()) / 2;
		x_pos = x_start + i * x_delta - x_add_length;
		x_lables[i]->move(x_pos, height - label_heigth);

		if (last_x_pos + horizontal_intend >=  x_pos) {
			x_lables[i]->setVisible(false);
		} else {
			x_lables[i]->setVisible(true);
			last_x_pos = x_start + i * x_delta + x_add_length;
		}

		y_add_length = fm_ptr->horizontalAdvance(y_lables[i]->text());
		y_pos = y_start - i * y_delta;
		y_lables[i]->move(label_width - y_add_length, y_pos);

		if (last_y_pos - vertical_intend - label_heigth <=  y_pos) {
			y_lables[i]->setVisible(false);
		} else {
			y_lables[i]->setVisible(true);
			last_y_pos = y_start - i * y_delta;
		}
	}
}