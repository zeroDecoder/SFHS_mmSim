//============================================================================
// Name        : mmSim.cpp
// Author      : Elis Pogace
// Version     : 1.0
// Created on  : Jun 11, 2014
// Description : Micro mouse simulation software for SFHS robotics students
//============================================================================

#include"mazeBase.h"
#include"mazeConst.h"
#include"gui.h"
#include"studentAi.h"
#include"time.h"
#include"string.h"

#include <dirent.h>
#include<iostream>
#include <string>



using namespace cv;


int main()
{

	int userInput;

	struct baseMapNode start[MAZE_WIDTH][MAZE_HEIGHT];
	struct mouseData mouse;
	FILE *mazeFile = NULL;

	//new maze construction
	newMazeArea(start, MAZE_WIDTH, MAZE_HEIGHT);


	//create game board
	Mat image(MAZE_WIDTH*PX_PER_UNIT, MAZE_HEIGHT*PX_PER_UNIT, CV_8UC3, Scalar(0,0,0));
	Mat guiText(MAZE_WIDTH*PX_PER_UNIT, MAZE_HEIGHT*PX_PER_UNIT, CV_8UC3, Scalar(0,0,0));

	namedWindow( DISPLAY_WINDOW_NAME, WINDOW_NORMAL);
	redrawMaze(&image, start);

	//create mouse call back
	struct callbackWrapper progData;
	progData.img = &image;
	progData.startNode = start;
	setMouseCallback(DISPLAY_WINDOW_NAME, mouseCallBackFunc, (void*)&progData);

	//user instructions
	std::cout << "Press 'q' to quit" << endl;
	std::cout << "Press 'l' to load maze" <<endl;
	std::cout << "Press 's' to save maze"<<endl;
	std::cout << "Press 'r' to reset the maze area"<<endl;
	std::cout << "Press 'm' to have the mouse start to solve the maze"<<endl;
	std::cout << "Left click to add a wall"<<endl;
	std::cout << "Right click to remove a wall"<<endl;

	while((userInput =waitKey(1))!= 'q')
	{

		if (!cvGetWindowHandle(DISPLAY_WINDOW_NAME))//if window is closed clean up and exit
		{
			break;
		}
		else if (userInput == 's')
		{
			string input;

			//save file
			cout << "Enter filename to save to (without extension): ";
			cvPrint(&guiText,"Enter filename to save to (without extension): " );
			input = cvIn(&guiText);
			input.append(".maz");

			mazeFile = fopen(input.c_str(), "w");
			if(mazeFile == NULL)
			{
				exit(100);
			}
			saveMaze2File(mazeFile, start);
			fclose(mazeFile);
			redrawMaze(&image, start);
		}
		else if(userInput == 'l')
		{
			DIR *local = opendir(".");
			struct dirent *dircontents = readdir(local);
			int selection;
			int fileNum =0;
			vector<std::string> fileNames;
			std::ostringstream buff ("");

			fileNames.clear();
			do{
				if(strstr(dircontents->d_name, ".maz")) //check to see if it is a maze file
				{
					//print selection number and filename
					buff << fileNum+1 << ": " << dircontents->d_name;
					fileNames.push_back(dircontents->d_name);
					cvPrint(&guiText,buff.str().c_str());
					fileNum++;

					//clear string buffer for next loop
					buff.str("");
					buff.clear();
				}
				dircontents = readdir(local);
			}while(dircontents != NULL);

			do {
				cout << endl << "Select file to load: " << endl;
				selection = atoi(cvIn(&guiText).c_str())-1;
				if(selection >= 0 && selection < fileNum) //check if user input is within range
					mazeFile = fopen(fileNames.at(selection).c_str(), "r");
				else
					mazeFile = NULL;

				if(!cvGetWindowHandle(DISPLAY_WINDOW_NAME)) break; //if the user closed the window break
			} while (mazeFile == NULL);
			cvPrint(&guiText, "");

			readMazeFromFile(mazeFile, start);

			cout << "Maze loaded" << endl;

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
			while(mouse.foundFinish != true && waitKey(100) == -1)
			{
				studentAi(&mouse);
				redrawMaze(&image, start, &mouse);
			}
			cout << "The AI says the mouse has finished the maze!"<<endl;
		}

	}
	return 0;
}
