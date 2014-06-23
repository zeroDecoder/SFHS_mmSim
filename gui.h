/*
 * gui.h
 *
 *  Created on: Jun 12, 2014
 *      Author: Elis Pogace
 */

#ifndef GUI_H_
#define GUI_H_

#include "mazeConst.h"
#include "mazeBase.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<string>

using namespace cv;

struct callbackWrapper
{
	struct baseMapNode (*startNode)[MAZE_HEIGHT];
	Mat *img;
};

void clearUiInputArea(Mat *img)
{
	rectangle(*img,Point(0,MAZE_HEIGHT_PX-PX_PER_UNIT), Point(MAZE_WIDTH_PX,MAZE_HEIGHT_PX), CV_RGB(0,0,0),CV_FILLED);
}

void clearUiPrintArea(Mat *img)
{
	rectangle(*img,Point(0,0), Point(MAZE_WIDTH_PX,MAZE_HEIGHT_PX-PX_PER_UNIT), CV_RGB(0,0,0),CV_FILLED);
}

int cvPrint(Mat *img, std::string text)
{
	static int lineNum =0;
	if(!text.compare(""))
	{
		lineNum =0;
		clearUiPrintArea(img);
	}
	else if(lineNum++ < GUI_MAXLINE) //if there's space print the line and increment the line number
	{
		putText(*img, text.c_str(), Point(0,lineNum*PX_PER_UNIT), FONT_HERSHEY_SIMPLEX, 1,
					CV_RGB(0,0,255), 2);
	}
	else
	{
		return -1; //if the line wasn't printed return -1
	}
	return lineNum;
}

std::string cvIn(Mat *img)
{
	char inputKey;
	std::string inputString ("");

	while((inputKey = waitKey(100)) != '\r' && cvGetWindowHandle(DISPLAY_WINDOW_NAME))
	{
		if (inputKey != -1)
		{
			if (inputKey == '\b')
			{
				if (inputString.size() > 0)
					inputString.resize(inputString.size() - 1);
			} else
				inputString += inputKey;
		}
		//redraw text
		clearUiInputArea(img);
		putText(*img, inputString, UI_INPUT_LOCATION, FONT_HERSHEY_SIMPLEX, 1,
				CV_RGB(0,0,255), 2);
		imshow(DISPLAY_WINDOW_NAME, *img);
	}
	return inputString;
}

void redrawMaze(Mat *img, struct baseMapNode startNode[][MAZE_HEIGHT], struct mouseData *mouse)
{
		//setup window if it doesn't exit
		if(!cvGetWindowHandle(DISPLAY_WINDOW_NAME))
		{
			namedWindow( DISPLAY_WINDOW_NAME, CV_WINDOW_NORMAL );
		}
		//clear img
		img->setTo(Scalar(0,0,0));

		//draw guide lines
		for(int i = MAZE_WIDTH; i; i--)
		{
			line(*img,
					Point(i*PX_PER_UNIT,0),
					Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT),
					CV_RGB(25,25,25));
		}
		for(int i = MAZE_HEIGHT; i; i--)
		{
			line(*img,
					Point(0,i*PX_PER_UNIT),
					Point(MAZE_HEIGHT*PX_PER_UNIT,i*PX_PER_UNIT),
					CV_RGB(25,25,25));

		}

		//Draw maze walls
		for(int i = 0; i < MAZE_WIDTH; i++)
		{
			for(int j = 0; j < MAZE_HEIGHT; j++)
			{
				if(startNode[i][j].wallBottom)
				{
					line(*img,
						Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT),
						Point((i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT),
						CV_RGB(255,255,255),
						WALL_THICKNESS_PX);
				}
				if(startNode[i][j].wallTop)
				{
					line(*img,
						Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
						Point((i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
						CV_RGB(255,255,255),
						WALL_THICKNESS_PX);
				}
				if(startNode[i][j].wallLeft)
				{
					line(*img,
						Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT),
						Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
						CV_RGB(255,255,255),
						WALL_THICKNESS_PX);
				}
				if(startNode[i][j].wallRight)
				{
					line(*img,
						Point((i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT),
						Point((i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
						CV_RGB(255,255,255),
						WALL_THICKNESS_PX);
				}

			}
		}

		//draw mouse
		Point tmp[3] = {Point(mouse->posData->x*PX_PER_UNIT-PX_PER_UNIT/2,MAZE_HEIGHT_PX-mouse->posData->y*PX_PER_UNIT+PX_PER_UNIT/2),
							Point(mouse->posData->x*PX_PER_UNIT-PX_PER_UNIT/4,MAZE_HEIGHT_PX-mouse->posData->y*PX_PER_UNIT+PX_PER_UNIT/8),
							Point(mouse->posData->x*PX_PER_UNIT-3*PX_PER_UNIT/4,MAZE_HEIGHT_PX-mouse->posData->y*PX_PER_UNIT+PX_PER_UNIT/8)};
		if(mouse->direction == RIGHT)
		{
			tmp[1] = Point(mouse->posData->x*PX_PER_UNIT-PX_PER_UNIT/8,MAZE_HEIGHT_PX-mouse->posData->y*PX_PER_UNIT+PX_PER_UNIT/4);
			tmp[2] = Point(mouse->posData->x*PX_PER_UNIT-PX_PER_UNIT/8,MAZE_HEIGHT_PX-mouse->posData->y*PX_PER_UNIT+3*PX_PER_UNIT/4);
		}
		else if(mouse->direction == BOTTOM)
		{
			tmp[1] = Point(mouse->posData->x*PX_PER_UNIT-PX_PER_UNIT/4,MAZE_HEIGHT_PX-mouse->posData->y*PX_PER_UNIT+7*PX_PER_UNIT/8);
			tmp[2] = Point(mouse->posData->x*PX_PER_UNIT-3*PX_PER_UNIT/4,MAZE_HEIGHT_PX-mouse->posData->y*PX_PER_UNIT+7*PX_PER_UNIT/8);
		}
		else if(mouse->direction == LEFT)
		{
			tmp[1] = Point(mouse->posData->x*PX_PER_UNIT-7*PX_PER_UNIT/8,MAZE_HEIGHT_PX-mouse->posData->y*PX_PER_UNIT+PX_PER_UNIT/4);
			tmp[2] = Point(mouse->posData->x*PX_PER_UNIT-7*PX_PER_UNIT/8,MAZE_HEIGHT_PX-mouse->posData->y*PX_PER_UNIT+3*PX_PER_UNIT/4);
		}
		circle(*img,
				Point((mouse->posData->x)*PX_PER_UNIT - PX_PER_UNIT/2,MAZE_HEIGHT_PX-((mouse->posData->y)*PX_PER_UNIT - PX_PER_UNIT/2)),
				MOUSE_RADIUS,
				CV_RGB(255,255,0),
				MOUSE_OUTLINE_THICKNESS);
		fillConvexPoly(*img, tmp, 3, Scalar (0, 0, 0));

		//display results
		imshow(DISPLAY_WINDOW_NAME, *img );
}

void redrawMaze(Mat *img, struct baseMapNode startNode[][MAZE_HEIGHT])
{
		//setup window if it doesn't exit
		if(!cvGetWindowHandle(DISPLAY_WINDOW_NAME))
		{
			namedWindow( DISPLAY_WINDOW_NAME, CV_WINDOW_NORMAL );
		}
		//clear img
		img->setTo(Scalar(0,0,0));

		//draw guide lines
		for(int i = MAZE_WIDTH; i; i--)
		{
			line(*img,
					Point(i*PX_PER_UNIT,0),
					Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT),
					CV_RGB(25,25,25));
		}
		for(int i = MAZE_HEIGHT; i; i--)
		{
			line(*img,
					Point(0,i*PX_PER_UNIT),
					Point(MAZE_HEIGHT*PX_PER_UNIT,i*PX_PER_UNIT),
					CV_RGB(25,25,25));

		}

		//Draw maze walls
		for(int i = 0; i < MAZE_WIDTH; i++)
		{
			for(int j = 0; j < MAZE_HEIGHT; j++)
			{
				if(startNode[i][j].wallBottom)
				{
					line(*img,
						Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT),
						Point((i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT),
						CV_RGB(255,255,255),
						WALL_THICKNESS_PX);
				}
				if(startNode[i][j].wallTop)
				{
					line(*img,
						Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
						Point((i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
						CV_RGB(255,255,255),
						WALL_THICKNESS_PX);
				}
				if(startNode[i][j].wallLeft)
				{
					line(*img,
						Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT),
						Point(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
						CV_RGB(255,255,255),
						WALL_THICKNESS_PX);
				}
				if(startNode[i][j].wallRight)
				{
					line(*img,
						Point((i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT),
						Point((i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
						CV_RGB(255,255,255),
						WALL_THICKNESS_PX);
				}

			}
		}
		//display results
		imshow(DISPLAY_WINDOW_NAME, *img );
}

void mouseCallBackFunc(int event, int x, int y, int flags, void* ptr)
{
	callbackWrapper * data = (callbackWrapper*)ptr;

	//left click adds a wall
	if(event == EVENT_LBUTTONDOWN)
	{
		//check if click intent is for a side wall
		if((x%PX_PER_UNIT < PX_PER_UNIT/3  || x%PX_PER_UNIT > 2*PX_PER_UNIT/3 )&& y%PX_PER_UNIT > PX_PER_UNIT/3  && y%PX_PER_UNIT < 2*PX_PER_UNIT/3)
		{
			//
			if(x%PX_PER_UNIT>=PX_PER_UNIT/2)
			{
				if((int)(x/PX_PER_UNIT) != MAZE_WIDTH)
				{
					data->startNode[(int)(x/PX_PER_UNIT)+1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallLeft = true;
					data->startNode[(int)(x/PX_PER_UNIT)+1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].left = NULL;
				}
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallRight = true;
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].right = NULL;


			}
			else
			{
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallLeft = true;
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].left = NULL;

				if(x/PX_PER_UNIT !=0)
				{
					data->startNode[(int)(x/PX_PER_UNIT)-1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallRight = true;
					data->startNode[(int)(x/PX_PER_UNIT)-1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].right = NULL;
				}
			}
		}
		//check if click intent is for a top or bottom wall
		if((y%PX_PER_UNIT < PX_PER_UNIT/3  || y%PX_PER_UNIT > 2*PX_PER_UNIT/3 )&& x%PX_PER_UNIT > PX_PER_UNIT/3  && x%PX_PER_UNIT < 2*PX_PER_UNIT/3)
		{

			if(y%PX_PER_UNIT>=PX_PER_UNIT/2 || y%PX_PER_UNIT == 0)
			{
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallBottom = true;
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].bottom = NULL;

				if((int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT) != 0)
				{
					data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)-1].wallTop = true;
					data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)-1].top = NULL;
				}
			}
			else
			{
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallTop = true;
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].top = NULL;

				if((MAZE_HEIGHT_PX-y)/PX_PER_UNIT != MAZE_HEIGHT)
				{
					data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)+1].wallBottom = true;
					data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)+1].bottom = NULL;
				}
			}

		}
		redrawMaze(data->img,data->startNode);
	}

	//Right click removes a wall
	if(event == EVENT_RBUTTONDOWN)
	{
		//check if click intent is for a side wall
		if((x%PX_PER_UNIT < PX_PER_UNIT/3  || x%PX_PER_UNIT > 2*PX_PER_UNIT/3 )&& y%PX_PER_UNIT > PX_PER_UNIT/3  && y%PX_PER_UNIT < 2*PX_PER_UNIT/3)
		{
			//
			if(x%PX_PER_UNIT>=PX_PER_UNIT/2)
			{
				if((int)(x/PX_PER_UNIT) != MAZE_WIDTH)
				{
					data->startNode[(int)(x/PX_PER_UNIT)+1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallLeft = false;
					data->startNode[(int)(x/PX_PER_UNIT)+1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].left = &(data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)]);
				}
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallRight = false;
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].right = &(data->startNode[(int)(x/PX_PER_UNIT)+1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)]);
			}
			else
			{
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallLeft = false;
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].left = &(data->startNode[(int)(x/PX_PER_UNIT)-1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)]);

				if(x/PX_PER_UNIT !=0)
				{
					data->startNode[(int)(x/PX_PER_UNIT)-1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallRight = false;
					data->startNode[(int)(x/PX_PER_UNIT)-1][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].right = &(data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)]);
				}
			}
		}
		//check if click intent is for a top or bottom wall
		if((y%PX_PER_UNIT < PX_PER_UNIT/3  || y%PX_PER_UNIT > 2*PX_PER_UNIT/3 )&& x%PX_PER_UNIT > PX_PER_UNIT/3  && x%PX_PER_UNIT < 2*PX_PER_UNIT/3)
		{

			if(y%PX_PER_UNIT>=PX_PER_UNIT/2 || y%PX_PER_UNIT == 0)
			{
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallBottom = false;
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].bottom = &(data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)-1]);

				if((int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT) != 0)
				{
					data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)-1].wallTop = false;
					data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)-1].top = &(data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)]);
				}
			}
			else
			{
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].wallTop = false;
				data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)].top = &(data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)+1]);

				if((MAZE_HEIGHT_PX-y)/PX_PER_UNIT != MAZE_HEIGHT)
				{
					data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)+1].wallBottom = false;
					data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)+1].bottom = &(data->startNode[(int)(x/PX_PER_UNIT)][(int)((MAZE_HEIGHT_PX-y)/PX_PER_UNIT)]);
				}
			}
		}
		redrawMaze(data->img,data->startNode);
	}

}



#endif /* GUI_H_ */
