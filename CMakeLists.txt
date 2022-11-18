project(QtPlot LANGUAGES CXX)
cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets OpenGL OpenGLWidgets REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(sources
	src/qtplot.cpp
	src/qtplot_histogram.cpp
	src/qtplot_waterfall.cpp
	src/qtplot_axes.cpp
	src/qtplot_widget.cpp
)

set(headers
	include/qtplot.h
	include/qtplot_histogram.h
	include/qtplot_waterfall.h
	include/qtplot_axes.h
	include/qtplot_widget.h
)

add_library(qtplot SHARED ${headers} ${sources})
target_include_directories(qtplot PRIVATE ${Qt6Widgets_INCLUDE_DIRS})
target_link_libraries(qtplot PRIVATE
	Qt::Widgets
	Qt::OpenGL
	Qt::OpenGLWidgets
)

add_executable(test src/test.cpp)
target_include_directories(test PRIVATE ${Qt6Widgets_INCLUDE_DIRS})
target_link_libraries(test PRIVATE
	Qt::Widgets
	Qt::OpenGL
	Qt::OpenGLWidgets
	qtplot
)