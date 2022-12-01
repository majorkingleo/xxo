#!/usr/bin/env bash

MINGWDIR=/usr/x86_64-w64-mingw32/sys-root/mingw
DLLS="Qt6Core.dll 
Qt6Gui.dll 
Qt6SvgWidgets.dll 
Qt6Widgets.dll 
libgcc_s_seh-1.dll 
libstdc++-6.dll 
libwinpthread-1.dll
Qt6Svg.dll
Qt6SvgWidgets.dll"

if ! test -d dist ; then
	mkdir dist
	( cd dist && ln -s ../xxo.exe . )
	( cd dist && ln -s ../*.svg . )
	( mkdir -p dist/plugins/platforms/ && cp -a ${MINGWDIR}/plugins/platforms/*.dll dist/plugins/platforms/ )
	for DLL in ${DLLS} ; do 
		cp -u ${MINGWDIR}/bin/${DLL} dist
	done
fi
export PKG_CONFIG_PATH=${MINGWDIR}/lib/pkgconfig/
make -f Makefile.win
