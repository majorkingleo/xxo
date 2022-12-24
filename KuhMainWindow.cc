/* KuhMainWindow.cc
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
#include "qstatusbar.h"
#include "qframe.h"
#include <iostream>
#include <set>

KuhMainWindow::Game::Game( XYButton::State symbol_own )
: symbols()
{
	reset( symbol_own );
}

void KuhMainWindow::Game::reset( XYButton::State symbol_own )
{
	if( symbols[Symbol_OWN] != symbol_own ) {
		who_starts = swapSymbol( who_starts );
	}

	symbols[Symbol_OWN] = symbol_own;

	if( symbols[Symbol_OWN] == XYButton::State::X ) {
		symbols[Symbol_OTHER] = XYButton::State::O;
	} else {
		symbols[Symbol_OTHER] = XYButton::State::X;
	}

	symbols[Symbol_WINNER] = XYButton::State::BLANK;
}

KuhMainWindow::KuhMainWindow(QWidget *parent)
: QMainWindow( parent ),
  buttons(),
  SIZE(3),
  rd(),
  game(),
  all_button_combinations(),
  all_buttons_linear(),
  turn(0)
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
	QMenu *menuGame = new QMenu(menuBar);
	menuGame->setTitle( "Game" );
	setMenuBar(menuBar);

	menuBar->addAction(menuGame->menuAction());

    menuGame->addAction(actionNewGame);
    menuGame->addAction(actionQuit);

    connect(actionQuit, SIGNAL (triggered()), QApplication::instance(), SLOT (quit()));
    connect(actionNewGame, SIGNAL (triggered()), this, SLOT (newGame()));


    QAction *actionItakeX = new QAction(this);
    actionItakeX->setObjectName(QString::fromUtf8("I take X"));
    actionItakeX->setText("I take X");
    actionItakeX->setCheckable( true );
    actionItakeX->setChecked(true);
    connect(actionItakeX, &QAction::triggered, this, &KuhMainWindow::takeX);

    QAction *actionItakeO = new QAction(this);
    actionItakeO->setObjectName(QString::fromUtf8("I take O"));
    actionItakeO->setText("I take O");
    actionItakeO->setCheckable( true );
    connect(actionItakeO, &QAction::triggered, this, &KuhMainWindow::takeO);


    QActionGroup *groupTake = new QActionGroup( this );
    groupTake->addAction( actionItakeX );
    groupTake->addAction( actionItakeO );
    groupTake->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);


    QMenu *menuOptions = new QMenu(menuBar);
    menuOptions->setTitle( "Options" );
    menuBar->addAction(menuOptions->menuAction());

    menuOptions->addAction( actionItakeX );
    menuOptions->addAction( actionItakeO );

    menuOptions->addSeparator();



    QAction *actionIstart = new QAction(this);
    actionIstart->setObjectName(QString::fromUtf8("I start"));
    actionIstart->setText("I start");
    actionIstart->setCheckable( true );
    actionIstart->setChecked(true);
    connect(actionIstart, &QAction::triggered, this, &KuhMainWindow::Istart);

    QAction *actionComuterStarts = new QAction(this);
    actionComuterStarts->setObjectName(QString::fromUtf8("Computer starts"));
    actionComuterStarts->setText("Comuter starts");
    actionComuterStarts->setCheckable( true );
    connect(actionComuterStarts, &QAction::triggered, this, &KuhMainWindow::ComputerStarts);


    QActionGroup *groupStart = new QActionGroup( this );
    groupStart->addAction( actionIstart );
    groupStart->addAction( actionComuterStarts );
    groupStart->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);

    menuOptions->addAction( actionIstart );
    menuOptions->addAction( actionComuterStarts );

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

    createStatusMessage();
}

KuhMainWindow::SCORE KuhMainWindow::findBestScore()
{
	SCORE my_score = find2InLine( game.symbols[Symbol_OWN] );
	SCORE other_score = find2InLine( game.symbols[Symbol_OTHER] );

	SCORE best_score = my_score;

	if( my_score.empty() && !other_score.empty() ) {
		best_score = other_score;
	}

	if( best_score.empty() ) {
		// game is already over
		return best_score;
	}

	if( best_score.rbegin()->first < my_score.rbegin()->first ) {
		best_score = my_score;
	}

	if( best_score.rbegin()->first < other_score.rbegin()->first ) {
		best_score = other_score;
	}

	/*
	std::cout << "Best Score: " << best_score.rbegin()->first
			  << " type: " << static_cast<int>((*best_score.rbegin()->second.begin())->getState())
			  << std::endl;
	*/

	return best_score;
}

void KuhMainWindow::userPlayed()
{
	turn++;

	SCORE best_score = findBestScore();

	if( best_score.empty() ) {
		playRandomOf( all_buttons_linear );
		createStatusMessage();
		return;
	}

	if( best_score.rbegin()->first == SIZE ) {
		game.symbols[Symbol_WINNER] = (*best_score.rbegin()->second.begin())->getState();
	} else {
		playRandomOf( best_score.rbegin()->second );
		best_score = findBestScore();

		if( best_score.empty() ) {
			// game is already over
			return;
		}

		if( best_score.rbegin()->first == SIZE ) {
			game.symbols[Symbol_WINNER] = (*best_score.rbegin()->second.begin())->getState();
		}
	}

	createStatusMessage();
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

	(*empty_buttons.begin())->setState( game.symbols[Symbol_OWN] );
	turn++;
}

void KuhMainWindow::newGame(  XYButton::State symbol_own )
{
	std::for_each( all_buttons_linear.begin(), all_buttons_linear.end(),
			[symbol_own]( auto button ) {
				button->reset();
				button->setComputerSymbol( symbol_own );
			}
	);

	turn = 0;
	game.reset( symbol_own );

	QMessageLogger().debug( "game.who_starts: %d symbol_own: %d", game.who_starts, symbol_own );
	if( game.who_starts == symbol_own ) {
		userPlayed();
	}

	createStatusMessage();
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

	std::string buf;
	for( auto * button : row ) {
		switch( button->getState() )
		{
			case XYButton::State::BLANK: buf += " "; break;
			case XYButton::State::X: 	 buf += "X"; break;
			case XYButton::State::O: 	 buf += "O"; break;
		}
	}


	//std::cout << "symbol_count:" << symbol_count << " " << buf << std::endl;
	QMessageLogger().debug( "Symbol count: %d %s", symbol_count, buf.c_str() );

	if( symbols.size() != 1 ) {
		return {};
	}

	return { std::make_pair(symbol_count, row) };
}


void KuhMainWindow::createStatusMessage()
{
	if( game.symbols[Symbol_WINNER] == XYButton::State::BLANK ) {

		if( countBlankButtons() == 0 ) {
			statusBar()->showMessage( QString::fromUtf8("Tie! Let's try it again!" ) );
			endGame();
			return;
		}

		if( turn == 0 ) {
			statusBar()->showMessage( QString::fromUtf8("You start, it's your turn." ) );
		} else if( turn % 2 == 0 ) {
			statusBar()->showMessage( QString::fromUtf8("It's your turn." ) );
		}

		return;
	}

	if( game.symbols[Symbol_WINNER] == game.symbols[Symbol_OWN] ) {
		statusBar()->showMessage( QString::fromUtf8("I'm the winner!!" ) );
	} else {
		statusBar()->showMessage( QString::fromUtf8("Congratulations, you win!" ) );
	}

	endGame();
}

void KuhMainWindow::takeX()
{
	QMessageLogger().debug( "takeX" );
	newGame( XYButton::State::O );
}

void KuhMainWindow::takeO()
{
	QMessageLogger().debug( "takeO" );
	newGame( XYButton::State::X );
}


void KuhMainWindow::Istart()
{
	QMessageLogger().debug( "Istart" );
	game.setWhoStarts( game.getUserSymbol() );
	newGame();
}

void KuhMainWindow::ComputerStarts()
{
	QMessageLogger().debug( "ComputerStarts" );
	game.setWhoStarts( game.getComputerSymbol() );
	newGame();
}


