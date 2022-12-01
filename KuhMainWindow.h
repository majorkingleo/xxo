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

protected:
	 BUTTON_ROWS_AND_COLS buttons;
	 const unsigned SIZE;
	 std::random_device rd;  //Will be used to obtain a seed for the random number engine
	 XYButton::State own_symbol;
	 XYButton::State other_symbol;
	 BUTTON_ROWS_AND_COLS all_button_combinations;
	 BUTTON_ROW all_buttons_linear;

public:
	 explicit KuhMainWindow(QWidget *parent = 0);

private slots:
	void userPlayed();
	void newGame();


private:
	void playRandomOf( BUTTON_ROW & buttons );
	SCORE find2InLine( XYButton::State symbol  );
	SCORE find2InLine( BUTTON_ROW & row, XYButton::State symbol );
	BUTTON_ROWS_AND_COLS getAllCombinationOfRows();
};



#endif /* KUHMAINWINDOW_H_ */
