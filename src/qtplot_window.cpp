#include <qtplot_window.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCursor>
#include <QIcon>
#include <QSpacerItem>

QtPlotWindow::QtPlotWindow(QWidget* parent, QtPlotType::Plot plt_type) :
	QWidget(parent)
{
	setLayout(new QVBoxLayout);

	QLayout* layout_ptr = this->layout();

	move_btn = new QPushButton(this);
	cursor_btn = new QPushButton(this);
	pick_btn = new QPushButton(this);
	zoom_in_btn = new QPushButton(this);
	zoom_out_btn = new QPushButton(this);

	move_btn->setIcon(QIcon("resources/move.png") );
	cursor_btn->setIcon(QIcon("resources/plain.png") );
	pick_btn->setIcon(QIcon("resources/pick.png") );
	zoom_in_btn->setIcon(QIcon("resources/zoom_in.png") );
	zoom_out_btn->setIcon(QIcon("resources/zoom_out.png") );

	cursor_btn->setCheckable(true);
	pick_btn->setCheckable(true);
	move_btn->setCheckable(true);
	zoom_in_btn->setCheckable(true);
	zoom_out_btn->setCheckable(true);

	mode_btns.push_back(cursor_btn);
	mode_btns.push_back(pick_btn);
	mode_btns.push_back(move_btn);
	mode_btns.push_back(zoom_in_btn);
	mode_btns.push_back(zoom_out_btn);

	plot_wgt = new QtPlotWidget(plt_type, this);

	QHBoxLayout* btns_layout = new QHBoxLayout;
	QSpacerItem* btns_spacer = new QSpacerItem(5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);
	btns_layout->addItem(btns_spacer);
	btns_layout->addWidget(cursor_btn);
	btns_layout->addWidget(pick_btn);
	btns_layout->addWidget(move_btn);
	btns_layout->addWidget(zoom_in_btn);
	btns_layout->addWidget(zoom_out_btn);

	layout_ptr->addItem(btns_layout);
	layout_ptr->addWidget(plot_wgt);


	connect(cursor_btn, &QPushButton::clicked, this, &QtPlotWindow::cursorBntToggled);
	connect(pick_btn, &QPushButton::clicked, this, &QtPlotWindow::pickBntToggled);
	connect(move_btn, &QPushButton::clicked, this, &QtPlotWindow::moveBntToggled);
	connect(zoom_in_btn, &QPushButton::clicked, this, &QtPlotWindow::zoomInBntToggled);
	connect(zoom_out_btn, &QPushButton::clicked, this, &QtPlotWindow::zoomOutBntToggled);
}

void QtPlotWindow::moveBntToggled(bool checked)
{
	if (move_btn->isChecked()) {
		switch (plot_wgt->type())
		{
		case QtPlotType::Plot::Histogram:
			this->setMouseTracking(true);
			for (auto& btn: mode_btns) {
				btn->setMouseTracking(true);
			}
			break;
		
		default:
			plot_wgt->getPlot()->setCursor( QCursor( QPixmap("resources/move.png") ) );
			break;
		}
	} else {
		plot_wgt->getPlot()->setCursor(Qt::ArrowCursor);
	}

	uncheckModeButtons();

	move_btn->setChecked(checked);
	if (checked) {
		plot_wgt->setStatus(QtPlotWidget::Status::moving);
	} else {
		plot_wgt->setStatus(QtPlotWidget::Status::watching);
	}
}

void QtPlotWindow::cursorBntToggled(bool checked)
{	
	uncheckModeButtons();

	cursor_btn->setChecked(checked);

	plot_wgt->getPlot()->setCursor(Qt::ArrowCursor);
	if (checked) {
		plot_wgt->setStatus(QtPlotWidget::Status::pointing);

		this->setMouseTracking(true);
		for (auto& btn: mode_btns) {
			btn->setMouseTracking(true);
		}
	} else {
		plot_wgt->setStatus(QtPlotWidget::Status::watching);
	}
}

void QtPlotWindow::pickBntToggled(bool checked)
{	
	if (pick_btn->isChecked()) {
		plot_wgt->getPlot()->setCursor(Qt::CrossCursor);
	} else {
		plot_wgt->getPlot()->setCursor(Qt::ArrowCursor);
	}

	uncheckModeButtons();

	pick_btn->setChecked(checked);
	if (checked) {
		plot_wgt->setStatus(QtPlotWidget::Status::markering);
	} else {
		plot_wgt->setStatus(QtPlotWidget::Status::watching);
	}
}

void QtPlotWindow::zoomInBntToggled(bool checked)
{
	if (zoom_in_btn->isChecked()) {
		switch (plot_wgt->type())
		{
		case QtPlotType::Plot::Histogram:
			this->setMouseTracking(true);
			for (auto& btn: mode_btns) {
				btn->setMouseTracking(true);
			}
			break;
		
		default:
			plot_wgt->getPlot()->setCursor( QCursor( QPixmap("resources/zoom_in.png") ) );
			break;
		}
	} else {
		plot_wgt->getPlot()->setCursor(Qt::ArrowCursor);
	}

	uncheckModeButtons();

	zoom_in_btn->setChecked(checked);
	if (checked) {
		plot_wgt->setStatus(QtPlotWidget::Status::zooming_in);
	} else {
		plot_wgt->setStatus(QtPlotWidget::Status::watching);
	}
}

void QtPlotWindow::zoomOutBntToggled(bool checked)
{
	if (zoom_out_btn->isChecked()) {
		switch (plot_wgt->type())
		{
		case QtPlotType::Plot::Histogram:
			this->setMouseTracking(true);
			for (auto& btn: mode_btns) {
				btn->setMouseTracking(true);
			}
			break;
		
		default:
			plot_wgt->getPlot()->setCursor( QCursor( QPixmap("resources/zoom_out.png") ) );
			break;
		}		
	} else {
		plot_wgt->getPlot()->setCursor(Qt::ArrowCursor);
	}

	uncheckModeButtons();

	zoom_out_btn->setChecked(checked);
	if (checked) {
		plot_wgt->setStatus(QtPlotWidget::Status::zooming_out);
	} else {
		plot_wgt->setStatus(QtPlotWidget::Status::watching);
	}
}