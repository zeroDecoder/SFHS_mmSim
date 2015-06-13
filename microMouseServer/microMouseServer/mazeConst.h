/*
 * mazeConst.h
 *
 *  Created on: Jun 11, 2014
 *      Author: Elis Pogace
 */

#ifndef MAZECONST_H_
#define MAZECONST_H_
#define PX_PER_UNIT 34
#define MAZE_HEIGHT 20
#define MAZE_WIDTH 20
#define MAZE_HEIGHT_PX PX_PER_UNIT*MAZE_HEIGHT
#define MAZE_WIDTH_PX PX_PER_UNIT*MAZE_WIDTH
#define MDELAY 120

#define WALL_THICKNESS_PX 2

#define MOUSE_OUTLINE_THICKNESS 9
#define MOUSE_RADIUS 7

enum mDirection
{
    dRIGHT,
    dDOWN,
    dLEFT,
    dUP
};


#endif /* MAZECONST_H_ */
