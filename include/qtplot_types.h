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

	class QtPlotInterval
	{
	public:
		QtPlotInterval() {
			intervals.resize(3);
		}

		void setInterval(Axis axis, double from, double to) {
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

			intervals[interval_num] = std::move( std::vector<double>(2) );
			intervals[interval_num][0] = from;
			intervals[interval_num][1] = to;
		}

	private:

		std::vector< std::vector<double> > intervals;

	};
	
}



