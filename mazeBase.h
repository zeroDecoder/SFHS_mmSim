/*
 * mazeBase.h
 *
 *  Created on: Jun 11, 2014
 *      Author: Elis Pogace
 */

#ifndef MAZEBASE_H_
#define MAZEBASE_H_
#include<stdio.h>
#include<stdlib.h>
#include"mazeConst.h"
#include<iostream>

using namespace std;

struct baseMapNode
{
	int x, y;
	bool wallTop, wallBottom, wallLeft, wallRight;
	struct baseMapNode *top, *bottom, *left, *right;
};

struct mouseData
{
	int direction;
	int foundFinish;
	struct baseMapNode *posData;
};

void resetMouse(struct mouseData *mouse ,struct baseMapNode *start)
{
	mouse->direction = TOP;
	mouse->foundFinish = false;
	mouse->posData = start;
}

void moveMouseForward(struct mouseData *mouse)
{
	if(mouse->direction == LEFT && mouse->posData->wallLeft == false)
	{
		mouse->posData = mouse->posData->left;
	}
	else if(mouse->direction == RIGHT && mouse->posData->wallRight == false)
	{
		mouse->posData = mouse->posData->right;
	}
	else if(mouse->direction == TOP && mouse->posData->wallTop == false)
	{
		mouse->posData = mouse->posData->top;
	}
	else if(mouse->direction == BOTTOM && mouse->posData->wallBottom == false)
	{
		mouse->posData = mouse->posData->bottom;
	}
	else
	{
		cout << "Error: there is a wall there!" << endl;
	}
}
void turnMouseLeft(struct mouseData *mouse)
{
	if(mouse->direction ==TOP)
	{
		mouse->direction = LEFT;
	}
	else
	{
		mouse->direction -= 1;
	}
}

void turnMouseRight(struct mouseData *mouse)
{
	if(mouse->direction ==LEFT)
		{
			mouse->direction = TOP;
		}
		else
		{
			mouse->direction += 1;
		}
}

bool isWallLeft(struct mouseData *mouse)
{
	if(mouse->direction == LEFT)
		return mouse->posData->wallBottom;
	else if(mouse->direction == RIGHT)
		return mouse->posData->wallTop;
	else if(mouse->direction == TOP)
		return mouse->posData->wallLeft;
	else if(mouse->direction == BOTTOM)
		return mouse->posData->wallRight;
	else
		return 1;
}

bool isWallRight(struct mouseData *mouse)
{
	if(mouse->direction == LEFT)
		return mouse->posData->wallTop;
	else if(mouse->direction == RIGHT)
		return mouse->posData->wallBottom;
	else if(mouse->direction == TOP)
		return mouse->posData->wallRight;
	else if(mouse->direction == BOTTOM)
		return mouse->posData->wallLeft;
	else
		return 1;
}

bool isWallForward(struct mouseData *mouse)
{
	if(mouse->direction == LEFT)
		return mouse->posData->wallLeft;
	else if(mouse->direction == RIGHT)
		return mouse->posData->wallRight;
	else if(mouse->direction == TOP)
		return mouse->posData->wallTop;
	else if(mouse->direction == BOTTOM)
		return mouse->posData->wallBottom;
	else
		return 1;
}
int saveMaze2File(FILE *dest, struct baseMapNode startNode[][MAZE_HEIGHT])
{
	int i = 0 , j = 0, status;
	while(i <= MAZE_WIDTH -1 && j <= MAZE_HEIGHT )
	{
		status = fprintf(dest, "%d %d %d %d %d %d\n",startNode[i][j].x, startNode[i][j].y, startNode[i][j].wallTop, startNode[i][j].wallBottom,
			startNode[i][j].wallLeft, startNode[i][j].wallRight);
		
		if(++j >= MAZE_HEIGHT)
		{
			j = 0;
			i++;
		}
	}
	
	if(status >= 0)
	  cout << "File saved successfully";
	else
	  cout << "File could not be written properly";
	return status;
}

void readMazeFromFile(FILE *maze, struct baseMapNode startNode[][MAZE_HEIGHT])
{
	/*
	 * Error codes
	 *
	 * exiting error codes:
	 * *201: file formating error
	 * *202: file not found
	 * *203: unexpected end of file
	 * *204: maze file is wider than max maze size
	 * *205: maze file is taller than max maze size
	 */
	int largestX =0, largestY=0, lineCount = 0;
	int x, y, wallTop, wallBottom, wallLeft,wallRight;

	if(maze == NULL)
	{
		exit(202); //file pointer is invalid
	}

	while(!feof(maze))
	{
		lineCount++;

		fscanf(maze, "%d %d %d %d %d %d\n", &x, &y, &wallTop, &wallBottom, &wallLeft, &wallRight);

		//check formating
		if(x<0 || y<0 || wallTop<0 || wallBottom<0 || wallLeft<0 || wallRight<0 ||
		   wallTop>1 || wallBottom>1 || wallLeft>1 || wallRight>1)
		{
			exit(201); //file formating error
		}

		//check x boundary
		if(x > largestX)
		{
			if(x > MAZE_WIDTH)
			{
				exit(204); //maze file is larger than this maze allows
			}
			else
			{
				largestX = x;
			}
		}


		//check y boundary
		if(y > largestY)
		{
			if(y > MAZE_HEIGHT)
			{
				exit(205); //maze file is larger than this maze allows
			}
			else
			{
				largestY = y;
			}
		}

		//load data into maze
		if(wallLeft)
		{
			startNode[x-1][y-1].wallLeft = true;
			startNode[x-1][y-1].left = NULL;
		}
		else
		{
			startNode[x-1][y-1].wallLeft = false;
			startNode[x-1][y-1].left = &startNode[x-2][y-1];
		}

		if(wallRight)
		{
			startNode[x-1][y-1].wallRight = true;
			startNode[x-1][y-1].right = NULL;
		}
		else
		{
			startNode[x-1][y-1].wallRight = false;
			startNode[x-1][y-1].right = &startNode[x][y-1];
		}

		if(wallTop)
		{
			startNode[x-1][y-1].wallTop = true;
			startNode[x-1][y-1].top = NULL;
		}
		else
		{
			startNode[x-1][y-1].wallTop = false;
			startNode[x-1][y-1].top = &startNode[x-1][y];
		}

		if(wallBottom)
		{
			startNode[x-1][y-1].wallBottom = true;
			startNode[x-1][y-1].bottom = NULL;
		}
		else
		{
			startNode[x-1][y-1].wallBottom = false;
			startNode[x-1][y-1].bottom = &startNode[x-1][y-2];
		}

	}

	if(lineCount < MAZE_HEIGHT*MAZE_WIDTH)
	{
		exit(203); //unexpected end of file
	}
}

void newMazeArea(struct baseMapNode startNode[][MAZE_HEIGHT], int width, int height)
{
	int xMover = width;
	int yMover = height;

	while(xMover-- != 0)
	{
		while(yMover-- != 0 )
		{
			startNode[xMover][yMover].x = xMover+1;
			startNode[xMover][yMover].y = yMover+1;

			if(xMover == width-1) //special condition left extent of maze
			{
				startNode[xMover][yMover].wallRight = true;
				startNode[xMover][yMover].wallLeft = false;

				startNode[xMover][yMover].right = NULL;
				startNode[xMover][yMover].left = &startNode[xMover-1][yMover];
			}
			else if(xMover == 0) //special condition right extent of maze
			{
				startNode[xMover][yMover].wallRight = false;
				startNode[xMover][yMover].wallLeft = true;

				startNode[xMover][yMover].right = &startNode[xMover+1][yMover];
				startNode[xMover][yMover].left = NULL;
			}
			else
			{
				startNode[xMover][yMover].wallRight = false;
				startNode[xMover][yMover].wallLeft = false;

				startNode[xMover][yMover].right = &startNode[xMover+1][yMover];
				startNode[xMover][yMover].left = &startNode[xMover-1][yMover];
			}

			if(yMover == height-1) //special condition top of maze
			{
				startNode[xMover][yMover].wallTop = true;
				startNode[xMover][yMover].wallBottom = false;

				startNode[xMover][yMover].top = NULL;
				startNode[xMover][yMover].bottom = &startNode[xMover][yMover-1];
			}
			else if(yMover == 0) //special condition bottom of maze
			{
				startNode[xMover][yMover].wallTop = false;
				startNode[xMover][yMover].wallBottom = true;

				startNode[xMover][yMover].top = &startNode[xMover][yMover+1];
				startNode[xMover][yMover].bottom = NULL;
			}
			else
			{
				startNode[xMover][yMover].wallTop = false;
				startNode[xMover][yMover].wallBottom = false;

				startNode[xMover][yMover].top = &startNode[xMover][yMover+1];
				startNode[xMover][yMover].bottom = &startNode[xMover][yMover-1];
			}
		}
	yMover = height;
	}
}
#endif /* MAZEBASE_H_ */
