#pragma once

#include <QWidget>
#include <QOpenGLWidget>

#include <qtplot_abstract.h>


class QtPlot : public QtPlotAbstract
{
	Q_OBJECT

public:
	QtPlot(QWidget* parent = nullptr) :	QtPlotAbstract(parent) {};

	virtual void setData(const double* x_value_ptr, const double* y_value_ptr, size_t size) ;
	virtual void setData(QtPlotType::Curve_list& value, size_t size);

	virtual void addData(const double* x_value_ptr, const double* y_value_ptr, size_t size);
protected:
	void paintEvent(QPaintEvent* event) override;
};