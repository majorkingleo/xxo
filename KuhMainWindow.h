/*
 * kuhMainWindow.h
 *
 *  Created on: 26.11.2022
 *      Author: martin
 */

#ifndef KUHMAINWINDOW_H_
#define KUHMAINWINDOW_H_

#include <qmainwindow.h>
#include "XYButton.h"
#include <vector>
#include <random>

class KuhMainWindow : public QMainWindow
{
	 Q_OBJECT
private:


	 typedef std::vector<XYButton*>  		BUTTON_ROW;
	 typedef std::vector<BUTTON_ROW> 		BUTTON_ROWS_AND_COLS;
	 typedef std::map<unsigned,BUTTON_ROW>  SCORE;

	 enum Symbol
	 {
		 Symbol_OWN = 0,
		 Symbol_OTHER,
		 Symbol_WINNER
	 };

	 static const unsigned Symbol_LAST = static_cast<unsigned>(Symbol_WINNER);

	 class Game
	 {
	 public:
		 std::array<XYButton::State,Symbol_LAST+1> symbols;

	 public:
		 Game( XYButton::State symbol_own = XYButton::State::O );

		 void reset( XYButton::State symbol_own );
	 };

protected:
	 BUTTON_ROWS_AND_COLS buttons;
	 const unsigned SIZE;
	 std::random_device rd;  // Will be used to obtain a seed for the random number engine
	 Game game;
	 BUTTON_ROWS_AND_COLS all_button_combinations;
	 BUTTON_ROW all_buttons_linear;
	 unsigned turn;

public:
	 explicit KuhMainWindow(QWidget *parent = 0);

private slots:
	void userPlayed();
	void newGame();


private:
	void playRandomOf( BUTTON_ROW & buttons );
	SCORE find2InLine( XYButton::State symbol  );
	SCORE find2InLine( BUTTON_ROW & row, XYButton::State symbol );
	SCORE findBestScore();
	BUTTON_ROWS_AND_COLS getAllCombinationOfRows();

	void createStatusMessage();

	void endGame() {
		std::for_each( all_buttons_linear.begin(), all_buttons_linear.end(),
				[]( auto button ) {
					button->setDisabled(true);
				}
		);
	}

	BUTTON_ROW::size_type countBlankButtons() {
		return std::count_if( all_buttons_linear.begin(),  all_buttons_linear.end(),
							  []( auto button ){
									return button->isBlank();
							   }
							);
	}
};



#endif /* KUHMAINWINDOW_H_ */
