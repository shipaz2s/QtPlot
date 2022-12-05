#pragma once

#include <QWidget>

class QtPlotZoomer : public QFrame
{
	Q_OBJECT

public:
	QtPlotZoomer(QWidget* parent = nullptr) :
		QFrame(parent)
	{
		QPalette pal = QPalette();
		// pal.setColor(QPalette::Window, QColor (255, 255, 255, 0) );
		pal.setColor(QPalette::Window, QColor (255, 255, 255, 0) );
		// pal.setColor(QPalette::Window, Qt::black);
		
		this->setAutoFillBackground(true); 
		this->setPalette(pal);
		this->setStyleSheet("border: 1px solid black");
		this->resize(0, 0);
	}
};