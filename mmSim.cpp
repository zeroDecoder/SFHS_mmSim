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

#define win32 1

#include <dirent.h>
#include<iostream>
#include <string>



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
#ifdef win32
			DIR *local = opendir(".");
			struct dirent *dircontents = readdir(local);
			int selection;
			std::ostringstream buff ("");

			do{
				buff << telldir(local) << ": " << dircontents->d_name; //setup output string
				cvPrint(&guiText,buff.str());
				cout << buff.str() <<endl;
				buff.str("");//clear string buffer for next loop
				buff.clear();
				dircontents = readdir(local);
			}while(dircontents != NULL);

			do {
				cout << endl << "Select file to load: " << endl;
				selection = atoi(cvIn(&guiText).c_str());
				rewinddir(local);
				seekdir(local, selection - 1);
				dircontents = readdir(local);

				if(!cvGetWindowHandle(DISPLAY_WINDOW_NAME)) break; //if the user closed the window break
			} while (dircontents == NULL || dircontents->d_type != 0);
			cvPrint(&guiText, "");
			mazeFile = fopen(dircontents->d_name, "r");
			if(mazeFile == NULL)
			{
				exit(101);
			}
			readMazeFromFile(mazeFile, start);
			cout << "Maze loaded" << endl;

			fclose(mazeFile);
			redrawMaze(&image, start);
#else
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
#endif
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
				if(waitKey(90) == 'q')
				{
					break;
				}
			}
			cout << "The AI says the mouse has finished the maze!"<<endl;
		}

	}
	return 0;
}
