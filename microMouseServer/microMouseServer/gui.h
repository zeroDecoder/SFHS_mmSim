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

#include<string>

using namespace cv;

inline void drawGreenSquare(Mat *img, int x, int y)
{
	//draw square
}

void highlightFinish(Mat *img, struct baseMapNode maze[][MAZE_HEIGHT])
{

	for(int i=0; i<MAZE_HEIGHT; i++)
	{
		for(int j=0; j<MAZE_WIDTH; j++)
		{
			if(maze[i][j].right != NULL)
			{
				if(maze[i][j].right->top != NULL)
				{
					if(maze[i][j].right->top->left != NULL)
					{
						if(maze[i][j].right->top->left->bottom == &(maze[i][j]))
						{

							drawGreenSquare(img, i, j);
							drawGreenSquare(img, i, j+1);
							drawGreenSquare(img, i+1, j);
							drawGreenSquare(img, i+1, j+1);
						}
					}
				}

			}
		}
	}
}


void redrawMaze(Mat *img, struct baseMapNode startNode[][MAZE_HEIGHT], struct mouseData *mouse)
{
        redrawMaze(img, startNode);

		//draw mouse


        //display results
}

void redrawMaze(Mat *img, struct baseMapNode startNode[][MAZE_HEIGHT])
{
    //clear img


    //draw guide lines
    for(int i = MAZE_WIDTH; i; i--)
    {
        //draw vertical lines
    }
    for(int i = MAZE_HEIGHT; i; i--)
    {
       //draw horizontal lines

    }

    //Draw maze walls
    for(int i = 0; i < MAZE_WIDTH; i++)
    {
        for(int j = 0; j < MAZE_HEIGHT; j++)
        {
            //draw lines

        }
    }

    //highlight finish
    highlightFinish(img, startNode);

    //display results
    imshow(DISPLAY_WINDOW_NAME, *img );
}


#endif /* GUI_H_ */
