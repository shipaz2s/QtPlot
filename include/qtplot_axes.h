#pragma once

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QSize>

#include <vector>
#include <list>

#include <qtplot_types.h>
#include <QStringList>

#include <memory>

class QtPlotAxes: public QWidget
{
	Q_OBJECT

	enum class zoomMode
	{
		reset,
		in,
		out,
		right,
		left
	};

public:
	QtPlotAxes(QWidget* parent = nullptr);

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

	void setSegment(QtPlotType::Axis axis, double min_value, double max_value);
	void setInterval(const QtPlotType::QtPlotInterval& new_interval);
	QtPlotType::QtPlotInterval& getInterval() {return interval;};

	QPoint getPlotStartPoint() {return plot_start_point;};
	QSize getPlotSize() {return plot_size;};
	void updateLabels();

	void setCustomAxis(bool);
	void setXLabels(const QStringList&);
	void zoom(zoomMode mode);
	/*
	0 - to default
	1 - zoom in
	2 - zoom out
	3 - move right
	4 - move left
	*/

public slots:
	void slotSetInterval(const QtPlotType::QtPlotInterval& new_interval) {
		setInterval(new_interval);
		updateLabels();
	}

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	bool custom_axis = false;
	QStringList custom_lables;
	QStringList default_custom_lables;
	int start_lbl_index = 0; 

	QtPlotType::QtPlotInterval interval;

	int min_delta = 5;
	int x_delta = 5;
	int y_delta = 5;

	int hatch_length = 20;
	
	int plot_width_hint = 0;
	int plot_height_hint = 0;

	int horizontal_intend = 10;
	int vertical_intend = 5;

	int label_width;
	int xLabel_width;
	int yLabel_width;
	int label_heigth;
	QList<QLabel*> x_lables;
	std::vector<QLabel*> y_lables;
	
	QLabel* xAxis_label;
	QLabel* yAxis_label;

	QBrush text_brush;
	QBrush axis_brush;

	QFontMetrics* fm_ptr;

	QPoint plot_start_point;
	QSize plot_size;
};