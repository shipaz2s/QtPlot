#pragma once

#include <qtplot_widget.h>

#include <QPushButton>
#include <vector>


class QtPlotWindow : public QWidget
{
	Q_OBJECT

public:
	QtPlotWindow(QWidget* parent = nullptr);

private slots:
	void moveBntToggled(bool);
	void cursorBntToggled(bool);
	void pickBntToggled(bool);
	void zoomInBntToggled(bool);
	void zoomOutBntToggled(bool);

private:
	void uncheckModeButtons() {
		for (auto& btn: mode_btns) {
			btn->setChecked(false);

			plot_wgt->setZoomIn(false);
			plot_wgt->setZoomOut(false);
			plot_wgt->setPicking(false);
			plot_wgt->setMoving(false);
		}
	}

private:
	QtPlotWidget* plot_wgt;

	QPushButton* move_btn;
	QPushButton* cursor_btn;
	QPushButton* pick_btn;
	QPushButton* zoom_in_btn;
	QPushButton* zoom_out_btn;

	std::vector<QPushButton*> mode_btns;
};