#pragma once

#include <QWidget>
#include <QSize>
#include <QPoint>


#include <qtplot.h>
#include <qtplot_axes.h>
#include <qtplot_types.h>

class QtPlotWidget : public QWidget
{
	Q_OBJECT

public:
	QtPlotWidget(QWidget* parent = nullptr);

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

	void setData(const double* y_value_ptr, size_t size) {plot->setData(y_value_ptr, size);};
	void setData(const double* x_value_ptr, const double* y_value_ptr, size_t size) {plot->setData(x_value_ptr, y_value_ptr, size);};
	void setData(QtPlotType::Curve_list& value, size_t size) {plot->setData(value, size);};

	void addData(const double* y_value_ptr, size_t size) {plot->addData(y_value_ptr, size);};
	void addData(const double* x_value_ptr, const double* y_value_ptr, size_t size) {plot->addData(x_value_ptr, y_value_ptr, size);};
	void setInterval(const QtPlotType::QtPlotInterval& new_interval) { plot->setInterval(new_interval); };
	void setInterval(const QtPlotType::Axis axis, const QtPlotType::QtPlotAxisInterval& new_interval) { plot->setInterval(axis, new_interval); };

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void changeEvent(QEvent* event) override;

private:
	QtPlot* plot;
	QtPlotAxes* axes;

	QPoint plot_start_point;
	QSize plot_size;
};