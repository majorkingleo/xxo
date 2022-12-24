/*
 * XYButton.cc
 *
 *  Created on: 29.11.2022
 *      Author: martin
 */
#include "XYButton.h"
#include "qboxlayout.h"
#include "qsvgwidget.h"
#include <iostream>

XYButton::XYButton()
: QPushButton(),
  state( State::BLANK ),
  userSymbol( State::X )
{
	setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
	setLayout( new QHBoxLayout(this) );

	connect(this, SIGNAL (pressed()), this, SLOT (userClicked()));
}

void XYButton::setState( State state_ )
{
	if( state == state_ ) {
		return;
	}

	state = state_;

	clearLayout( layout(), true );

	QString icon_name;

	switch( state )
	{
		case State::BLANK:  return;
		case State::X:		icon_name = QString::fromUtf8("Red_X_Freehand.svg"); break;
		case State::O: 		icon_name = QString::fromUtf8("moon-hand-drawn-circle-svgrepo-com.svg"); break;
	}

	if( icon_name.isEmpty() ) {
		return;
	}

	layout()->addWidget( new QSvgWidget(icon_name,this) );
}

void XYButton::clearLayout(QLayout* layout, bool deleteWidgets )
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        QWidget* widget;
        if (  (deleteWidgets)
              && (widget = item->widget())  ) {
            delete widget;
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout, deleteWidgets);
        }
        delete item;
    }
}

void XYButton::userClicked()
{
	if( state != State::BLANK ) {
		// std::cout << "State is not blank its: " << static_cast<int>(state) << std::endl;
		return;
	}

	setState( userSymbol );

	emit userPlayed();
}


