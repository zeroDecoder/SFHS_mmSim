//============================================================================
// Name        : mmSim.cpp
// Author      : Elis Pogace
// Version     : 0.1
// Created on  : Jun 11, 2014
// Description : Micro mouse simulation software for SFHS robotics students
//============================================================================

#include"mazeBase.h"
#include"mazeConst.h"
#include"gui.h"
#include"studentAi.h"

#include<iostream>


using namespace cv;

int main()
{
	int userInput;

	struct baseMapNode start[MAZE_WIDTH][MAZE_HEIGHT];
	struct mouseData mouse;
	FILE *mazeFile;

	//new maze construction
	newMazeArea(start, MAZE_WIDTH, MAZE_HEIGHT);


	//create game board
	Mat image(MAZE_WIDTH*PX_PER_UNIT, MAZE_HEIGHT*PX_PER_UNIT, CV_8UC3, Scalar(0,0,0));
	redrawMaze(&image, start);

	//create mouse call back
	struct callbackWrapper progData;
	progData.img = &image;
	progData.startNode = start;
	setMouseCallback(DISPLAY_WINDOW_NAME, mouseCallBackFunc, (void*)&progData);

	//user instructions
	std::cout << "Press 'q' to quit" << endl;
	std::cout << "Press 'l' to load maze" <<endl;
	std::cout << "Press 's' to save maze, NOTE: overwrites current save"<<endl;
	std::cout << "Press 'r' to reset the maze area"<<endl;
	std::cout << "Press 'm' to have the mouse start to solve the maze"<<endl;
	std::cout << "Left click to add a wall"<<endl;
	std::cout << "Right click to remove a wall"<<endl;


	while((userInput =waitKey(1))!= 'q')
	{
		if(userInput == 's')
		{
			//save file
			mazeFile = fopen(SAVE_FILE, "w");
			if(mazeFile == NULL)
			{
				exit(100);
			}
			saveMaze2File(mazeFile, start);
			fclose(mazeFile);
		}
		else if(userInput == 'l')
		{
			//load file
			mazeFile = fopen(SAVE_FILE, "r");
			if(mazeFile != NULL)
			{
				readMazeFromFile(mazeFile, start);
			}
			fclose(mazeFile);
			redrawMaze(&image, start);
		}
		else if(userInput == 'r')
		{
			//reset maze area
			newMazeArea(start, MAZE_WIDTH, MAZE_HEIGHT);
			redrawMaze(&image, start);
		}
		else if(userInput == 'm')
		{
			resetMouse(&mouse, &(start[0][0]));
			while(mouse.foundFinish != true)
			{
				studentAi(&mouse);
				redrawMaze(&image, start, &mouse);
				if(waitKey(500) == 'q')
				{
					break;
				}
			}
			cout << "The AI says the mouse has finished the maze!"<<endl;
		}
	}

	return 0;
}
