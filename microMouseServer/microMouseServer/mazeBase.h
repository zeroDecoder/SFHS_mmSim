/*
 * mazeBase.h
 *
 *  Created on: Jun 11, 2014
 *      Author: Elis Pogace
 */

#ifndef MAZEBASE_H_
#define MAZEBASE_H_
#include"mazeConst.h"

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

#endif /* MAZEBASE_H_ */
