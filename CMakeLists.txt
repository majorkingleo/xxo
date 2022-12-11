cmake_minimum_required(VERSION 3.15)
project(xxo VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS SvgWidgets)
qt_standard_project_setup()

add_executable(xxo 
	KuhMainWindow.cc 
	KuhMainWindow.h
	main.cc 
	XYButton.cc	
	XYButton.h	)

target_link_libraries(xxo PRIVATE Qt6::Core)
target_link_libraries(xxo PRIVATE Qt6::Widgets)
target_link_libraries(xxo PRIVATE Qt6::SvgWidgets)
