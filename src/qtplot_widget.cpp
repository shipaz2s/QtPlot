#include "qtplot_widget.h"

#include <QEvent>
#include <QMouseEvent>

QtPlotWidget::QtPlotWidget(QWidget* parent) :
	QWidget(parent)
{
	setMinimumSize(320, 240);
	axes = new QtPlotAxes(this);
	plot = new QtPlot(this);
	plot_start_point = axes->getPlotStartPoint();
	plot_size = axes->getPlotSize();

	// QPalette pal = QPalette();

	// // set black background
	// // Qt::black / "#000000" / "black"
	// pal.setColor(QPalette::Window, Qt::black);

	// plot->setAutoFillBackground(true); 
	// plot->setPalette(pal);

	// plot->lower();
	// plot->raise();

	zoomer = new QtPlotZoomer(this);
	zoomer->hide();

	connect(plot, &QtPlot::signalAxesHint, axes, &QtPlotAxes::slotSetInterval);
}

QSize QtPlotWidget::minimumSizeHint() const
{
	return QSize(320, 240);
}

QSize QtPlotWidget::sizeHint() const
{
	return QSize(640, 480);
}

void QtPlotWidget::paintEvent(QPaintEvent* event)
{
}

void QtPlotWidget::resizeEvent(QResizeEvent* event)
{
	int height = this->height();
	int width = this->width();

	axes->resize(width, height);
	
	plot_start_point = axes->getPlotStartPoint();
	plot_size = axes->getPlotSize();

	plot->move(plot_start_point);
	plot->resize(plot_size);
}

void QtPlotWidget::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::WindowStateChange)
	{
		axes->updateLabels();
	}
}

void QtPlotWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if ( event->position().x() >= plot_start_point.x() && event->position().x() <= plot_start_point.x() + plot_size.width() ) {
			if ( event->position().y() >= plot_start_point.y() && event->position().y() <= plot_start_point.y() + plot_size.height() ) {
				if ( !zooming ) {
					zoomer_start_point = QPoint(event->position().x(), event->position().y());
					zooming = true;
					zoomer->move(zoomer_start_point);
					zoomer->show();
				}
			}
		}
	}
}

void QtPlotWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (zooming) {
		if ( event->position().x() >= plot_start_point.x() && event->position().x() <= plot_start_point.x() + plot_size.width() ) {
			if ( event->position().y() >= plot_start_point.y() && event->position().y() <= plot_start_point.y() + plot_size.height() ) {

				if (event->position().y() <= zoomer_start_point.y() && event->position().x() <= zoomer_start_point.x()) {
					zoomer->move(event->position().x(), event->position().y());
				} else if (event->position().y() <= zoomer_start_point.y()) {
					zoomer->move(zoomer_start_point.x(), event->position().y());
				} else if (event->position().x() <= zoomer_start_point.x()) {
					zoomer->move(event->position().x(), zoomer_start_point.y());
				}
			} else {
								
				if (event->position().x() <= zoomer_start_point.x()) {
					if ( event->position().y() <= plot_start_point.y() ) {
						zoomer->move( event->position().x(), plot_start_point.y() );
					}
					else {
						zoomer->move( event->position().x(), zoomer_start_point.y() );
					}
				}
			}
		} else if ( event->position().y() >= plot_start_point.y() && event->position().y() <= plot_start_point.y() + plot_size.height() ) {

			if ( event->position().y() <= zoomer_start_point.y() ) {

				if ( event->position().x() <= plot_start_point.x() ) {
					zoomer->move( plot_start_point.x(), event->position().y() );
				}
				else {
					zoomer->move( zoomer_start_point.x(), event->position().y() );
				}
			}
		}

		int new_width;
		int new_height;

		if ( event->position().x() < plot_start_point.x() ) {
			new_width = zoomer_start_point.x() - plot_start_point.x();
		} else if ( event->position().x() > plot_start_point.x() + plot_size.width() ) {
			new_width = plot_start_point.x() + plot_size.width() - zoomer_start_point.x();
		} else {
			new_width = abs(zoomer_start_point.x() - event->position().x());
		}
		
		if ( event->position().y() < plot_start_point.y() ) {
			new_height = zoomer_start_point.y() - plot_start_point.y();
		} else if ( event->position().y() > plot_start_point.y() + plot_size.height() ) {
			new_height = plot_start_point.y() + plot_size.height() - zoomer_start_point.y();
		} else {
			new_height = abs(zoomer_start_point.y() - event->position().y());
		}

		zoomer->resize( QSize( new_width, new_height ) );
	}
}

void QtPlotWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		plot->setInterval(axes_default_interval);
		zoom_intervals.clear();
		plot->repaint();
	}

	if (event->button() == Qt::MiddleButton) {
		if (zoom_intervals.size() > 0) {
			plot->setInterval(zoom_intervals.back());
			zoom_intervals.pop_back();
			plot->repaint();
		}
	}

	if (event->button() == Qt::LeftButton) {
		if (zooming) {
			zooming = false;
			zoomer->hide();
			zoomer->resize(0, 0);

			QPoint zoomer_end_point = QPoint(event->position().x(), event->position().y());
			if (zoomer_end_point.x() < plot_start_point.x()) {
				zoomer_end_point.setX( plot_start_point.x() );
			} else	if (zoomer_end_point.x() > plot_start_point.x() + plot_size.width() ) {
				zoomer_end_point.setX( plot_start_point.x() + plot_size.width() );
			}

			if (zoomer_end_point.y() < plot_start_point.y()) {
				zoomer_end_point.setY( plot_start_point.y() );
			} else if (zoomer_end_point.y() > plot_start_point.y() + plot_size.height() ) {
				zoomer_end_point.setY( plot_start_point.y() + plot_size.height() );
			}
			
			auto plot_axes_interval = plot->getAxesInterval();
			zoom_intervals.push_back(plot_axes_interval);
			
			double dx = (plot_axes_interval[QtPlotType::Axis::X].to - plot_axes_interval[QtPlotType::Axis::X].from) / plot_size.width();
			double dy = (plot_axes_interval[QtPlotType::Axis::Y].to - plot_axes_interval[QtPlotType::Axis::Y].from) / plot_size.height();

			double x_start = ( zoomer_start_point.x() - plot_start_point.x() ) * dx + plot_axes_interval[QtPlotType::Axis::X].from;
			double x_end = ( zoomer_end_point.x() - plot_start_point.x() ) * dx + plot_axes_interval[QtPlotType::Axis::X].from;
			if (x_start >= x_end) {
				std::swap(x_start, x_end);
			}
			double y_start = ( plot_size.height() - zoomer_start_point.y() + plot_start_point.y() )* dy + plot_axes_interval[QtPlotType::Axis::Y].from;
			double y_end = ( plot_size.height() - zoomer_end_point.y() + plot_start_point.y() )* dy + plot_axes_interval[QtPlotType::Axis::Y].from;
			if (y_start >= y_end) {
				std::swap(y_end, y_start);
			}

			plot->setInterval(QtPlotType::Axis::X, QtPlotType::QtPlotAxisInterval(x_start, x_end));
			plot->setInterval(QtPlotType::Axis::Y, QtPlotType::QtPlotAxisInterval(y_start, y_end));

			plot->repaint();
		}
	}
}