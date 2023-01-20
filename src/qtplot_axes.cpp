#include <qtplot_axes.h>

#include <QPainter>
#include <QLocale>

#include <QResizeEvent>
#include <QCoreApplication>


QtPlotAxes::QtPlotAxes(QWidget* parent) :
	QWidget(parent)
{
	setMinimumSize(320, 240);

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

	interval[QtPlotType::Axis::X].from = 0.;
	interval[QtPlotType::Axis::X].to = 1000.;
	interval[QtPlotType::Axis::Y].from = 0.;
	interval[QtPlotType::Axis::Y].to = 1000.;

	double dx = (interval[QtPlotType::Axis::X].to - interval[QtPlotType::Axis::X].from) / 10;
	double dy = (interval[QtPlotType::Axis::Y].to - interval[QtPlotType::Axis::Y].from) / 10;
	for (int i = 0; i < 11; ++i) {
		x_lables.push_back( new QLabel( locale().toString(interval[QtPlotType::Axis::X].from + dx * i, 'g'), this) );
		y_lables.push_back( new QLabel( locale().toString(interval[QtPlotType::Axis::Y].from + dy * i, 'g'), this) );
	}

	// to set variables used in resizeEvent (which are usually set in paint event), cause paintEvent is called after resizeEvent
	resize(640, 480);

	x_delta = (this->width() - 2 * horizontal_intend - label_width - hatch_length - xLabel_width)/ 10;
	y_delta = (this->height() - 2 * vertical_intend  - label_heigth - hatch_length - label_heigth) / 10;

	int x_start = label_width + horizontal_intend + hatch_length;
	int y_start = 480 - label_heigth - vertical_intend - hatch_length;
	int plot_width_hint = x_start + x_delta * 10;
	int plot_height_hint = y_start - y_delta * 10;

	plot_start_point = QPoint(x_start,
		plot_height_hint);
	plot_size = QSize(plot_width_hint - x_start,
		y_start - plot_height_hint);
}

QSize QtPlotAxes::minimumSizeHint() const {
	
	return QSize(320, 240);
}

QSize QtPlotAxes::sizeHint() const {
	
	return QSize(640, 480);
}

void QtPlotAxes::setSegment(QtPlotType::Axis axis, double min_value, double max_value)
{
	interval[axis].from = min_value;
	interval[axis].to = max_value;

	double delta = (interval[axis].to - interval[axis].from) / 10;

	switch (axis)
	{
	case QtPlotType::Axis::X :
		if (x_lables.size() != 11) {
			for (auto & lbl: x_lables) {
				lbl->deleteLater();
			}
			x_lables.resize(11);
		}

		for (int i = 0; i < 11; ++i) {
			x_lables[i]->setText( locale().toString(interval[axis].from + delta * i, 'g') );
			x_lables[i]->adjustSize();
		}

		break;

	case QtPlotType::Axis::Y :
		for (int i = 0; i < 11; ++i) {
			y_lables[i]->setText( locale().toString(interval[axis].from + delta * i, 'g') );
			y_lables[i]->adjustSize();
		}

		break;
	
	case QtPlotType::Axis::Z :

		break;

	default:
		for (int i = 0; i < 11; ++i) {
			x_lables[i]->setText( locale().toString(interval[axis].from + delta * i, 'g') );
		}
		
		break;
	}
}

void QtPlotAxes::setInterval(const QtPlotType::QtPlotInterval& new_interval)
{
	interval = new_interval;

	double delta_x = (interval[QtPlotType::Axis::X].to - interval[QtPlotType::Axis::X].from) / 10;
	double delta_y = (interval[QtPlotType::Axis::Y].to - interval[QtPlotType::Axis::Y].from) / 10;

	if (x_lables.size() != 11) {
		for (auto & lbl: x_lables) {
			lbl->deleteLater();
		}
		x_lables.resize(11, new QLabel(this));
	}

	for (int i = 0; i < 11; ++i) {
		x_lables[i]->setText( locale().toString(interval[QtPlotType::Axis::X].from + delta_x * i, 'g') );
		x_lables[i]->adjustSize();
	}

	for (int i = 0; i < 11; ++i) {
		y_lables[i]->setText( locale().toString(interval[QtPlotType::Axis::Y].from + delta_y * i, 'g') );
		y_lables[i]->adjustSize();
	}
}

void QtPlotAxes::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);

	painter.setBrush(axis_brush);
	
	int width = this->width();
	int height = this->height();

	int x_start = label_width + horizontal_intend + hatch_length;
	int x_end = width - horizontal_intend - xLabel_width;

	int y_start = height - label_heigth - vertical_intend - hatch_length;
	int y_end = label_heigth + vertical_intend;


	if (custom_axis) {
		x_delta = (this->width() - 2 * horizontal_intend - label_width - hatch_length - xLabel_width) / ( custom_lables.size() + 1 );
	} else {
		x_delta = (this->width() - 2 * horizontal_intend - label_width - hatch_length - xLabel_width) / 10;
	}

	if (x_delta >= min_delta) {
		int i;
		for (i = x_start; i <= x_end; i+= x_delta) {
			painter.drawLine(i, height - hatch_length - label_heigth - vertical_intend, i, height - label_heigth - vertical_intend);
		}
		plot_width_hint = i - x_delta;
	}

	if (y_delta >= min_delta) {
		int i;
		for (i = y_start; i >= y_end; i -= y_delta) {
			painter.drawLine(label_width + horizontal_intend, i, label_width + horizontal_intend + hatch_length, i);
		}
		plot_height_hint = i + y_delta;
	}

	painter.drawLine(	x_start,
						y_start,
						x_start,
						plot_height_hint
					);

	painter.drawLine(	x_start,
						y_start,
						plot_width_hint,
						y_start
					);

	painter.end();
}

void QtPlotAxes::resizeEvent(QResizeEvent* event)
{
	updateLabels();
}

void QtPlotAxes::updateLabels()
{
	int height = this->height();
	int width = this->width();

	int x_start = label_width + horizontal_intend + hatch_length;
	int y_start = height - label_heigth - vertical_intend - hatch_length - label_heigth / 2;


	if (custom_axis) {
		x_delta = (this->width() - 2 * horizontal_intend - label_width - hatch_length - xLabel_width) / ( custom_lables.size() + 1 );
	} else {
		x_delta = (this->width() - 2 * horizontal_intend - label_width - hatch_length - xLabel_width) / 10;
	}
	y_delta = (height - 2 * vertical_intend  - label_heigth - hatch_length - label_heigth) / 10;

	xAxis_label->move(width - xLabel_width, y_start);
	yAxis_label->move(x_start - yLabel_width / 2, 0);



	int x_add_length = 0;
	int last_x_pos = INT_MIN;
	int x_pos;

	int y_add_length = 0;
	int last_y_pos = INT_MAX;
	int y_pos;

	if (custom_axis) {
		auto x_iter = x_lables.begin();
		for (int i = 0; i < custom_lables.size(); ++i) {
			x_add_length = fm_ptr->horizontalAdvance((*x_iter)->text()) / 2;
			x_pos = x_start + (i + 1) * x_delta - x_add_length;
			(*x_iter)->move(x_pos, height - label_heigth);

			if (last_x_pos + horizontal_intend >=  x_pos) {
				(*x_iter)->setVisible(false);
			} else {
				(*x_iter)->setVisible(true);
				last_x_pos = x_start + (i + 1) * x_delta + x_add_length;
			}
			++x_iter;
		}
	} else {
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
		}
	}

	for (int i = 0; i < 11; ++i) {
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

	plot_start_point = QPoint(x_start, height - label_heigth - vertical_intend - hatch_length - 10 * y_delta);

	if (custom_axis) {
		plot_size = QSize( (custom_lables.size() + 1) * x_delta, 10 * y_delta);
	} else {
		plot_size = QSize(10 * x_delta, 10 * y_delta);
	}
}

void QtPlotAxes::setCustomAxis(bool value)
{
	custom_axis = value;
}


void QtPlotAxes::setXLabels(const QStringList& x_list)
{
	default_custom_lables = x_list;
	custom_lables = default_custom_lables;
}

void QtPlotAxes::zoom(zoomMode mode)
{

	if (!custom_axis) {
		return;
	}

	switch (mode)
	{
	case zoomMode::reset:

		custom_lables = default_custom_lables;
		for (auto & lbl: x_lables) {
			lbl->deleteLater();
		}
		x_lables.resize(custom_lables.size(), new QLabel(this));

		for (int i = 0; i < custom_lables.size(); ++i) {
			x_lables[i]->setText( custom_lables[i] );
			x_lables[i]->adjustSize();
		}
		break;
	
	case zoomMode::in:
		
		if (custom_lables.size() > 1) {
			custom_lables.pop_back();
			x_lables.back()->deleteLater();
			x_lables.pop_back();
		}
		break;

	case zoomMode::out:

		if (custom_lables.size() < default_custom_lables.size()) {
			if (start_lbl_index + custom_lables.size() < default_custom_lables.size()) {
				custom_lables.push_back(default_custom_lables[start_lbl_index + custom_lables.size()]);
				x_lables.push_back(new QLabel(custom_lables.back(), this));
			} else {
				custom_lables.push_front(default_custom_lables[start_lbl_index - 1]);
				x_lables.push_front(new QLabel(custom_lables.front(), this));
			}
		}
		break;

	case zoomMode::right:
		if (start_lbl_index + custom_lables.size() < default_custom_lables.size()) {
				custom_lables.push_back(default_custom_lables[start_lbl_index + custom_lables.size()]);
				x_lables.push_back(new QLabel(custom_lables.back(), this));
				custom_lables.pop_front();
				x_lables.pop_front();
				++start_lbl_index;
		}
		break;

	case zoomMode::left:
		if (start_lbl_index > 0) {
				custom_lables.push_front(default_custom_lables[--start_lbl_index]);
				x_lables.push_front(new QLabel(custom_lables.back(), this));
				custom_lables.pop_back();
				x_lables.pop_back();
		}
		break;

	default:
		break;
	}
}