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
#include <set>

KuhMainWindow::KuhMainWindow(QWidget *parent)
: QMainWindow( parent ),
  buttons(),
  SIZE(3),
  rd(),
  own_symbol( XYButton::State::O ),
  other_symbol( XYButton::State::X ),
  all_button_combinations(),
  all_buttons_linear()
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

    		XYButton* button = buttons[row][col] = new XYButton();
    		all_buttons_linear.push_back( button );

    		gridLayout->addWidget( button, row, col );
    		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    		connect(button, SIGNAL (userPlayed()), this, SLOT (userPlayed()));
    	}
    }

    all_button_combinations = getAllCombinationOfRows();
}

void KuhMainWindow::userPlayed()
{
	SCORE my_score = find2InLine( own_symbol );
	SCORE other_score = find2InLine( other_symbol );

	SCORE best_score = my_score;

	if( my_score.empty() && !other_score.empty() ) {
		best_score = other_score;
	}

	if( best_score.empty() ) {
		playRandomOf( all_buttons_linear );
		return;
	}

	if( best_score.rbegin()->first < my_score.rbegin()->first ) {
		best_score = my_score;
	}

	if( best_score.rbegin()->first < other_score.rbegin()->first ) {
		best_score = other_score;
	}

	playRandomOf( best_score.rbegin()->second );
}


void KuhMainWindow::playRandomOf( BUTTON_ROW & buttons )
{
	BUTTON_ROW empty_buttons;

	for( auto button : buttons ) {
		if( button->isBlank() ) {
			empty_buttons.push_back( button );
		}
	}

	if( empty_buttons.empty() ) {
		return;
	}

	auto rng = std::default_random_engine { rd() };
	std::shuffle(std::begin(empty_buttons), std::end(empty_buttons), rng);

	(*empty_buttons.begin())->setState( own_symbol );
}

void KuhMainWindow::newGame()
{
	for( auto & row : buttons ) {
		for( auto & button : row ) {
			button->reset();
		}
	}
}

KuhMainWindow::BUTTON_ROWS_AND_COLS KuhMainWindow::getAllCombinationOfRows()
{
	BUTTON_ROWS_AND_COLS all_rows;

	// vertically
	for( auto & row : buttons ) {
		all_rows.push_back( row );
	}

	// horizontal
	for( unsigned col = 0; col < SIZE; col++ ) {
		BUTTON_ROW button_row;
		for( unsigned row = 0; row < SIZE; row++ ) {
			button_row.push_back( buttons[row][col] );
		}
		all_rows.push_back( button_row );
	}

	// cross
	{
		BUTTON_ROW button_row;

		for( unsigned col = 0, row = 0; col < SIZE && row < SIZE; col++, row++ ) {
			button_row.push_back( buttons[row][col] );
		}
		all_rows.push_back( button_row );
	}

	{
		BUTTON_ROW button_row;
		for( int col = SIZE-1, row = 0; col >= 0 && row < static_cast<int>(SIZE); col--, row++ ) {
			button_row.push_back( buttons[row][col] );
		}
		all_rows.push_back( button_row );
	}

	return all_rows;
}

KuhMainWindow::SCORE KuhMainWindow::find2InLine( XYButton::State symbol )
{
	SCORE score;

	for( auto & row : all_button_combinations ) {
		auto score4row = find2InLine( row, symbol );
		score.insert( score4row.begin(), score4row.end() );
	}

	return score;
}

KuhMainWindow::SCORE KuhMainWindow::find2InLine( BUTTON_ROW & row,  XYButton::State symbol )
{
	std::set<decltype(symbol)> symbols;
	unsigned symbol_count = 0;

	for( auto * button : row ) {
		auto button_symbol = button->getState();

		if( button_symbol != XYButton::State::BLANK ) {
			symbols.insert( button_symbol );

			if( symbol == button_symbol ) {
				symbol_count++;
			}
		}
	}

	if( symbols.size() != 1 ) {
		return {};
	}

	return { std::make_pair(symbol_count, row) };
}


