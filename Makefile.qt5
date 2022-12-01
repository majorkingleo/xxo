all: xxo

#moc_%.cpp: %.h
#	moc $(DEFINES) $(INCPATH) $< -o $@

DEFINES=`pkg-config --cflags Qt5Gui Qt5Widgets Qt5Svg`

moc_KuhMainWindow.cpp: KuhMainWindow.h
	moc-qt5 $(DEFINES) $(INCPATH) $< -o $@
	
moc_XYButton.cpp: XYButton.h
	moc-qt5 $(DEFINES) $(INCPATH) $< -o $@	

xxo: main.cc KuhMainWindow.cc moc_KuhMainWindow.cpp KuhMainWindow.cc XYButton.cc moc_XYButton.cpp XYButton.h
	g++ -g -Wall -o xxo main.cc \
	KuhMainWindow.cc \
	moc_KuhMainWindow.cpp \
	XYButton.cc \
	moc_XYButton.cpp \
	$(DEFINES) \
	`pkg-config --libs Qt5Gui Qt5Widgets Qt5Svg`