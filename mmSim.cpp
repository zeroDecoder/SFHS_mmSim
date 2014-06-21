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
#include <string>
#include <dirent.h>

using namespace cv;

static int filter(const struct dirent* input)
{
  const char* ext = string(".maz").c_str();
  
  if (input->d_name[0] == '.')
    return 0;
  else
  {
    size_t j = (strlen(input->d_name)-4);
    for (int i = 0; j < (strlen(input->d_name)); i++, j++)
    {
      if (ext[i] != (input->d_name[j]))
	return 0;
    }
    return 1;
  }
}

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
	std::cout << "Press 's' to save maze"<<endl;
	std::cout << "Press 'r' to reset the maze area"<<endl;
	std::cout << "Press 'm' to have the mouse start to solve the maze"<<endl;
	std::cout << "Left click to add a wall"<<endl;
	std::cout << "Right click to remove a wall"<<endl;


	while((userInput =waitKey(1))!= 'q')
	{
		if(userInput == 's')
		{
			string input; const char* c_input;
			
			//save file
			std::cout << "Enter filename to save to (without extension): ";
			cin >> input; 
			input.append(".maz");
			c_input = input.c_str();
			
			mazeFile = fopen(c_input, "w");
			if(mazeFile == NULL)
			{
				exit(100);
			}
			fclose(mazeFile);
		}
		else if(userInput == 'l')
		{
		  struct dirent **dircontents;
		  string choices[20];
		  int dirnum, selection;
		  
		  cout << endl << "Select file to load: " << endl;
		  
		  dirnum = scandir("./", &dircontents, filter, alphasort);
		  if (dirnum >= 0)
		  {
		    for (int i = 0; i < dirnum; i++)
		    {
		      choices[i].assign(dircontents[i]->d_name);
		      cout << i << ") " << choices[i] << endl;
		    }
		  }
		  
		  cout << endl;
		  
		  //load file
		  cin >> selection;
		  
		  if (selection <= dirnum && selection <= 20)
		  {
		    mazeFile = fopen(choices[selection].c_str(), "r");
		    if(mazeFile != NULL)
		    {
		      readMazeFromFile(mazeFile, start);
		      cout << "Maze loaded" << endl;
		    }
		    fclose(mazeFile);
		    redrawMaze(&image, start);
		  }
		  
		  else
		    cout << "Invalid selection, try another action" << endl;
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