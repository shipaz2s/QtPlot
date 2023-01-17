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

	zoomer = new QtPlotZoomer(this);
	zoomer->hide();

	connect(plot, &QtPlot::signalAxesHint, axes, &QtPlotAxes::slotSetInterval);

	QtPlotType::QtPlotInterval def_interval;
	def_interval[QtPlotType::Axis::X].from = 0.;
	def_interval[QtPlotType::Axis::X].to = 1000.;
	def_interval[QtPlotType::Axis::Y].from = 0.;
	def_interval[QtPlotType::Axis::Y].to = 1000.;

	setInterval(def_interval);
}

QSize QtPlotWidget::minimumSizeHint() const
{
	return QSize(320, 240);
}

QSize QtPlotWidget::sizeHint() const
{
	return QSize(640, 480);
}

void QtPlotWidget::slotDeleteMarker(QPointF& pnt)
{
	QtPlotMarker* finder = new QtPlotMarker(pnt, this);
	
	auto res = markers.find(finder);
	delete finder;

	if (res != markers.end()) {
		(*res)->deleteLater();
		markers.erase(res);
	}
}

void QtPlotWidget::slotDeleteMarkers()
{
	for (auto& marker: markers) {
		
		marker->deleteLater();
	}
	markers.clear();
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

	moveMarkers();
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
	if (zooming_in) {
		if (event->button() == Qt::LeftButton) {
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
	} else if (picking) {
		if (event->button() == Qt::LeftButton) {
			if ( event->position().x() >= plot_start_point.x() && event->position().x() <= plot_start_point.x() + plot_size.width() ) {
				if ( event->position().y() >= plot_start_point.y() && event->position().y() <= plot_start_point.y() + plot_size.height() ) {
					const QtPlotType::Curve_list* curves_ptr = plot->curves();
					auto interval = plot->getAxesInterval();

					qreal x_pnt = ( event->position().x() - plot_start_point.x() ) / plot_size.width();
					qreal y_pnt = ( plot_start_point.y() + plot_size.height() - event->position().y() ) / plot_size.height();

					x_pnt = interval[QtPlotType::Axis::X].from + (interval[QtPlotType::Axis::X].to - interval[QtPlotType::Axis::X].from) * x_pnt;
					y_pnt = interval[QtPlotType::Axis::Y].from + (interval[QtPlotType::Axis::Y].to - interval[QtPlotType::Axis::Y].from) * y_pnt;

					qreal distance;
					qreal distance_x;
					qreal distance_y;
					qreal min_distance;

					std::vector< std::pair<qreal, qreal> > nearest_points;
					for (auto & curve: *curves_ptr) {
						int pos = 0;
						for (auto x_val : curve.x) {
							if (x_val >= x_pnt) {
								distance_x = (curve.x[pos] - x_pnt);
								distance_y = (curve.y[pos] - y_pnt);
								min_distance = distance_x * distance_x + distance_y * distance_y;
								if (pos > 0) {
									distance_x = (curve.x[pos - 1] - x_pnt);
									distance_y = (curve.y[pos - 1] - y_pnt);
									distance = distance_x * distance_x + distance_y * distance_y;
									if (distance < min_distance) {
										--pos;
									}
								}
								break;
							}
							++pos;
						}
						nearest_points.push_back( std::make_pair(curve.x[pos], curve.y[pos]) );
					}

					if (nearest_points.size() == 0) {
						return;
					}
					int nearest_pos = 0;
					distance_x = (nearest_points[0].first - x_pnt);
					distance_y = (nearest_points[0].second - y_pnt);
					min_distance = distance_x * distance_x + distance_y * distance_y;
					
					for (int i = 1; i < nearest_points.size(); ++i) {
						distance_x = (nearest_points[i].first - x_pnt); 
						distance_y = (nearest_points[i].second - y_pnt);
						distance = distance_x * distance_x + distance_y * distance_y;

						if (distance < min_distance) {
							min_distance = distance;
							nearest_pos = i;
						}
					}

					qreal x_plot_pnt = nearest_points[nearest_pos].first;
					qreal y_plot_pnt = nearest_points[nearest_pos].second; 

					new_marker = new QtPlotMarker(QPointF(x_plot_pnt, y_plot_pnt), this);
					
					new_marker->raise();
					new_marker->show();
					auto res = markers.find(new_marker);
					if (res == markers.end()) {
						markers.insert(new_marker);
						new_marker->move(
							(x_plot_pnt - interval[QtPlotType::Axis::X].from) \
							/ (interval[QtPlotType::Axis::X].to - interval[QtPlotType::Axis::X].from) * plot_size.width() \
							+ plot_start_point.x() - new_marker->width() / 2,
							plot_start_point.y() + plot_size.height() - (y_plot_pnt - interval[QtPlotType::Axis::Y].from) \
							/ (interval[QtPlotType::Axis::Y].to - interval[QtPlotType::Axis::Y].from) * plot_size.height() - new_marker->getDiameter()
						);
						connect(new_marker, &QtPlotMarker::signalDeleteMarker, this, &QtPlotWidget::slotDeleteMarker);
						connect(new_marker, &QtPlotMarker::signalDeleteMarkers, this, &QtPlotWidget::slotDeleteMarkers);
						
					} else {
						delete new_marker;
					}
				}
			}
		}
	} else if (moving) {
		if (event->button() == Qt::LeftButton) {
			if ( event->position().x() >= plot_start_point.x() && event->position().x() <= plot_start_point.x() + plot_size.width() ) {
				if ( event->position().y() >= plot_start_point.y() && event->position().y() <= plot_start_point.y() + plot_size.height() ) {
					if ( !zooming ) {
						move_start_point = QPoint(event->position().x(), event->position().y());
						moving_started = true;
						plot->setCursor(Qt::ClosedHandCursor);
					}
				}
			}
		}
	}

}

void QtPlotWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (zooming_in) {
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
	} else if (moving_started) {
		auto interval = plot->getAxesInterval();
		qreal delta_x = (interval[QtPlotType::Axis::X].to - interval[QtPlotType::Axis::X].from) / plot_size.width();
		qreal delta_y = (interval[QtPlotType::Axis::Y].to - interval[QtPlotType::Axis::Y].from) / plot_size.height();

		qreal pix_delta_x = event->position().x() - move_start_point.x();
		qreal pix_delta_y = event->position().y() - move_start_point.y();

		QtPlotType::QtPlotInterval new_interval(interval);
		new_interval[QtPlotType::Axis::X].from = interval[QtPlotType::Axis::X].from - pix_delta_x * delta_x;
		new_interval[QtPlotType::Axis::X].to = interval[QtPlotType::Axis::X].to - pix_delta_x * delta_x;
		new_interval[QtPlotType::Axis::Y].from = interval[QtPlotType::Axis::Y].from + pix_delta_y * delta_y;
		new_interval[QtPlotType::Axis::Y].to = interval[QtPlotType::Axis::Y].to + pix_delta_y * delta_y;

		plot->setInterval(new_interval);
		axes->setInterval(new_interval);
		plot->repaint();
		axes->repaint();
		moveMarkers();

		move_start_point = QPoint( event->position().toPoint() );
	}
}

void QtPlotWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (zooming_out) {
		if (event->button() == Qt::RightButton) {
			plot->setInterval(axes_default_interval);
			zoom_intervals.clear();
			plot->repaint();
		}

		if (event->button() == Qt::LeftButton) {
			if (zoom_intervals.size() > 0) {
				plot->setInterval(zoom_intervals.back());
				zoom_intervals.pop_back();
				plot->repaint();
			}
		}

		moveMarkers();
	} else if (zooming_in) {
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
		moveMarkers();
	} else if (moving) {
		if (event->button() == Qt::LeftButton) {
			plot->setCursor( QCursor( QPixmap("resources/move.png") ) );
			moving_started = false;
		}
	}
}

void QtPlotWidget::moveMarkers()
{
	auto interval = plot->getAxesInterval();

	for (auto & marker: markers) {
		qreal x_point = marker->point().x();
		qreal y_point = marker->point().y();

		qreal new_x_point = (x_point - interval[QtPlotType::Axis::X].from) / \
			(interval[QtPlotType::Axis::X].to - interval[QtPlotType::Axis::X].from) \
			* plot_size.width() + plot_start_point.x() - new_marker->width() / 2;
		qreal new_y_point = plot_start_point.y() + (interval[QtPlotType::Axis::Y].to - y_point) / \
			(interval[QtPlotType::Axis::Y].to - interval[QtPlotType::Axis::Y].from) \
			* plot_size.height() - new_marker->getDiameter();

		marker->move(new_x_point, new_y_point);

		if (new_x_point + new_marker->width() / 2 >= plot_start_point.x() &&
			new_x_point + new_marker->width() / 2 <= plot_start_point.x() + plot_size.width() &&
			new_y_point + new_marker->getDiameter() >= plot_start_point.y() &&
			new_y_point + new_marker->getDiameter() <= plot_start_point.y() + plot_size.height() )
		{
			marker->show();		
		} else {
			marker->hide();
		}
	}
}