/*
 * KuhMainWindow.cc
 *
 *  Created on: 26.11.2022
 *      Author: martin
 */
#include "KuhMainWindow.h"
#include "qaction.h"
#include "qmenu.h"
#include "qmenubar.h"
#include "qapplication.h"
#include "qgridlayout.h"
#include "qframe.h"
#include <iostream>

KuhMainWindow::KuhMainWindow(QWidget *parent)
: QMainWindow( parent ),
  buttons(),
  SIZE(3),
  rd()
{

    QAction *actionNewGame;
    QAction *actionQuit;

    actionNewGame = new QAction(this);
    actionNewGame->setObjectName(QString::fromUtf8("actionNewGame"));
    actionNewGame->setText("New Game");

    actionQuit = new QAction(this);
    actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
    actionQuit->setText("Quit");

	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *menuTest = new QMenu(menuBar);
	menuTest->setTitle( "Game" );
	setMenuBar(menuBar);

	menuBar->addAction(menuTest->menuAction());

    menuTest->addAction(actionNewGame);
    menuTest->addAction(actionQuit);

    connect(actionQuit, SIGNAL (triggered()), QApplication::instance(), SLOT (quit()));
    connect(actionNewGame, SIGNAL (triggered()), this, SLOT (newGame()));

    auto *f = new QFrame();
    setCentralWidget(f);

    // Grid layout with 3 buttons
    QGridLayout *gridLayout = new QGridLayout(f);

    for( unsigned row = 0; row < SIZE; row++ ) {
    	buttons.resize(SIZE);

    	for( unsigned col = 0; col < SIZE; col++ ) {
    		buttons[row].resize(SIZE);

    		auto button = buttons[row][col] = new XYButton();
    		gridLayout->addWidget( button, row, col );
    		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    		connect(button, SIGNAL (userPlayed()), this, SLOT (userPlayed()));
    	}
    }
}

void KuhMainWindow::userPlayed()
{
	playRandom();
}


void KuhMainWindow::playRandom()
{
	std::vector<XYButton*> empty_buttons;

	for( auto & row : buttons ) {
		for( auto & button : row ) {
			if( button->isBlank() ) {
				empty_buttons.push_back( button );
			}
		}
	}

	if( empty_buttons.empty() ) {
		return;
	}

	auto rng = std::default_random_engine { rd() };
	std::shuffle(std::begin(empty_buttons), std::end(empty_buttons), rng);

	(*empty_buttons.begin())->setState( XYButton::State::O );
}

void KuhMainWindow::newGame()
{
	for( auto & row : buttons ) {
		for( auto & button : row ) {
			button->reset();
		}
	}
}


