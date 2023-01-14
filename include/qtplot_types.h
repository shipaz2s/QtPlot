#pragma once

#include <vector>

namespace QtPlotType
{
	enum class Plot
	{
		Plot,
		WaterFall,
		Histogram
	};

	enum class Axis
	{
		X,
		Y,
		Z
	};

	struct QtPlotAxisInterval
	{
		double from = 0;
		double to = 0;
		QtPlotAxisInterval(double from_value = 0., double to_value = 0.) : from(from_value), to(to_value){};
	};

	class QtPlotInterval
	{
	public:
		QtPlotInterval() {
			intervals.resize(3);
		}

		QtPlotInterval(
						const QtPlotAxisInterval& x_interval,
						const QtPlotAxisInterval& y_interval,
						const QtPlotAxisInterval& z_interval
					) {
			intervals.resize(3);
			intervals[0] = x_interval;
			intervals[1] = y_interval;
			intervals[2] = z_interval;
		}

		void setInterval(Axis axis, const QtPlotAxisInterval& interval) {
			int interval_num;
			switch (axis)
			{
			case Axis::X:
				interval_num = 0;
				break;
			case Axis::Y:
				interval_num = 1;
				break;
			case Axis::Z:
				interval_num = 2;
				break;
			
			default:
				interval_num = 0;
				break;
			}

			intervals[interval_num] = interval;
		}

		void setInterval(Axis axis, const double from, const double to) {
			setInterval(axis, QtPlotAxisInterval(from, to));
		}

		void setIntervalFrom(Axis axis, double value) {
			int interval_num;
			switch (axis)
			{
			case Axis::X:
				interval_num = 0;
				break;
			case Axis::Y:
				interval_num = 1;
				break;
			case Axis::Z:
				interval_num = 2;
				break;
			
			default:
				interval_num = 0;
				break;
			}
			intervals[interval_num].from = value;
		}

		void setIntervalTo(Axis axis, double value) {
			int interval_num;
			switch (axis)
			{
			case Axis::X:
				interval_num = 0;
				break;
			case Axis::Y:
				interval_num = 1;
				break;
			case Axis::Z:
				interval_num = 2;
				break;
			
			default:
				interval_num = 0;
				break;
			}
			intervals[interval_num].to = value;
		}

		QtPlotAxisInterval& operator [] (Axis axis) {
			int interval_num;
			switch (axis)
			{
			case Axis::X:
				interval_num = 0;
				break;
			case Axis::Y:
				interval_num = 1;
				break;
			case Axis::Z:
				interval_num = 2;
				break;
			
			default:
				interval_num = 0;
				break;
			}
			return intervals[interval_num];
		}

	private:

		std::vector< QtPlotAxisInterval > intervals;

	};

	struct Curve {
		std::vector<qreal> x;
		std::vector<qreal> y;
	};
	typedef std::list<Curve> Curve_list;
}



