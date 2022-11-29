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

	 std::vector<std::vector<XYButton*>> buttons;
	 const unsigned SIZE;
	 std::random_device rd;  //Will be used to obtain a seed for the random number engine

public:
	 explicit KuhMainWindow(QWidget *parent = 0);

private slots:
	void userPlayed();
	void newGame();


private:
	void playRandom();
};



#endif /* KUHMAINWINDOW_H_ */
