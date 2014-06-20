/*
 * studentAi.h
 *
 *  Created on: Jun 12, 2014
 *      Author: zeros
 */

#ifndef STUDENAI_H_
#define STUDENAI_H_

#include"mazeBase.h"

void studentAi(struct mouseData *mouse)
{

	//example student written AI
	static bool lastLeft = false;
	static bool lastRight = false;

	if(!isWallLeft(mouse))
	{
		turnMouseLeft(mouse);
		moveMouseForward(mouse);
	}
	else if(!isWallForward(mouse))
	{
		if(!isWallLeft(mouse))
		{
			lastLeft = true;
		}
		else
		{
			lastLeft = false;
		}
		if(!isWallRight(mouse))
		{
			lastRight = true;
		}
		else
		{
			lastRight = false;
		}

		moveMouseForward(mouse);

		if(!isWallLeft(mouse) && lastLeft)
		{
			mouse->foundFinish = true;
		}
		else
		{
			lastLeft = false;
		}
		if(!isWallRight(mouse) && lastRight)
		{
			mouse->foundFinish = true;
		}
		else
		{
			lastRight = false;
		}

	}
	else if(!isWallLeft(mouse))
	{
		turnMouseLeft(mouse);
		moveMouseForward(mouse);
	}
	else if(!isWallRight(mouse))
	{
		turnMouseRight(mouse);
		moveMouseForward(mouse);
	}
	else
	{
		while(isWallForward(mouse))
		{
			turnMouseLeft(mouse);
		}
	}
}

#endif /* STUDENAI_H_ */
