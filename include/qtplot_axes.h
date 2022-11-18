#include <QWidget>
#include <QLabel>

#include <vector>

namespace QtPlotType
{
	enum class Axis
	{
		X,
		Y,
		Z
	};
}

class QtPlotAxes: public QWidget
{
public:
	QtPlotAxes(QWidget* parent = nullptr);

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

	void setSegment(QtPlotType::Axis axis, double min_value, double max_value);

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	double x_min_value = 0.;
	double x_max_value = 1000.;

	double y_min_value = 0.;
	double y_max_value = 1000.;

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
	std::vector<QLabel*> x_lables;
	std::vector<QLabel*> y_lables;
	
	QLabel* xAxis_label;
	QLabel* yAxis_label;

	QBrush text_brush;
	QBrush axis_brush;

	QFontMetrics* fm_ptr;
};