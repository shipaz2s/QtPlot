#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <vector>
#include <list>

#include <qtplot_abstract.h>


class QtPlotHistogram : public QtPlotAbstract
{
	Q_OBJECT

public:
	QtPlotHistogram(QWidget* parent = nullptr) : QtPlotAbstract(parent) {};

protected:
	void paintEvent(QPaintEvent* event) override;
};