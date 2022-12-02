#pragma once

#include <QWidget>
#include <QOpenGLWidget>

class QtPlot : public QWidget
{
	Q_OBJECT

public:
	QtPlot(QWidget* parent = nullptr) :
		QWidget(parent)
	{};

};

class QtPlotGL : public QOpenGLWidget
{

};