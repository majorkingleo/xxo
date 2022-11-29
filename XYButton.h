/*
 * XYButton.h
 *
 *  Created on: 29.11.2022
 *      Author: martin
 */

#ifndef XYBUTTON_H_
#define XYBUTTON_H_

#include <qpushbutton.h>

class XYButton : public QPushButton
{
	Q_OBJECT

public:
	enum class State
	{
		BLANK,
		X,
		O
	};

private:
	State state;

public:
	XYButton();

	void setState( State state );
	State getState() const { return state; }

	bool isBlank() const {
		return state == State::BLANK;
	}

	void reset() {
		setState( State::BLANK );
	}

signals:
	void userPlayed();

private slots:
	void userClicked();

private:
	static void clearLayout(QLayout* layout, bool deleteWidgets = true);
};


#endif /* XYBUTTON_H_ */
