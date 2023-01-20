#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPointF>
#include <QMenu>
#include <QAction>
#include <QLocale>

class QtPlotMarker : public QWidget
{
	Q_OBJECT

public:
	QtPlotMarker(QPointF pnt = QPointF(), QWidget* parent = nullptr, int new_width = 3, QColor new_color = QColor(Qt::blue)) :
		line_width(new_width),
		color(new_color),
		marker_point(pnt),
		QWidget(parent)
	{
		menu = new QMenu(this);
		QAction* ac_delete = menu->addAction("Удалить текущий маркер");
		QAction* ac_delete_all = menu->addAction("Удалить все маркеры");

		// connect(ac_delete, &QAction::triggered, this, &QtPlotMarker::slotDeleteMarker);
		// connect(ac_delete_all, &QAction::triggered, this, &QtPlotMarker::slotDeleteMarkers);

		connect(ac_delete, &QAction::triggered, this, [this] () {emit signalDeleteMarker(marker_point);} );
		connect(ac_delete_all, &QAction::triggered, this, [this] () {emit signalDeleteMarkers();} );

		label_text = locale().toString(pnt.x(), 'g') + " " + locale().toString(pnt.y(), 'g');

		QFontMetrics fm_ptr(this->font());

		label_width = fm_ptr.horizontalAdvance(label_text);
		label_heigth = fm_ptr.height();

		resize(label_width, diameter + line_width * 2 + 5 + label_heigth);
	};
	void setWidth(int new_width) {line_width = new_width;};
	void setColor(QColor& new_color) {color = new_color;};

	int lineWidth() {return line_width;};
	int getDiameter() {return diameter;}; 

	QPointF point() {return marker_point;};

signals:
	void signalDeleteMarker(QPointF &);
	void signalDeleteMarkers();

protected:
	void paintEvent(QPaintEvent* event) override
	{
		QPainter painter;
		painter.begin(this);
		
		painter.setBrush( QBrush( QColor(255,255,255, 0) ) );
		painter.setPen( QPen( QBrush(color), line_width ) );

		painter.drawEllipse(QPoint(width() / 2, diameter), diameter / 2, diameter / 2);

		auto bg_color = palette().color(backgroundRole());
		auto fg_color = palette().color(foregroundRole());
		bg_color.setAlpha(180);
		painter.setBrush( QBrush( bg_color ) );
		painter.setPen( QPen( QBrush( QColor(0,0,0, 0) ), line_width )  );

		painter.drawRect(0, diameter + 10 + line_width * 2, label_width, label_heigth);
		painter.setPen( QPen( fg_color, line_width ) );
		painter.drawText(0, diameter + line_width * 2 + 5 + label_heigth, label_text);

		painter.end();
	}

	void contextMenuEvent( QContextMenuEvent* e )
	{
		menu->exec(e->globalPos());
	}


private:
	int line_width;
	QColor color;

	QPointF marker_point;
	QMenu* menu;
	int diameter = 10;

	QString label_text;

	int label_width;
	int label_heigth;
};

struct QtPlotMarkerComparator {
	bool operator() (QtPlotMarker* p1, QtPlotMarker* p2) const {
		QPointF pnt1 = p1->point();
		QPointF pnt2 = p2->point();
		if (pnt1.x() < pnt2.x()) {
			return true;
		} else if ( pnt1.x() == pnt2.x() ) {
			if (pnt1.y() < pnt2.y()) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	};
};