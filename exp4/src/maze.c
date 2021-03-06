#include <stdlib.h>
#include "HAL_LCD.h"
#include "maze.h"


volatile uint8_t start_i = 6 , start_j = 6; //Predefinidos
volatile uint8_t goal_i = 6 , goal_j = 95; //Predefinidos

uint8_t maze[64][102];

//extern volatile uint8_t screen[8][102];

void setMaze(){

  for(uint8_t i = 0; i<64;i++){
      for(uint8_t j = 0; j<102;j++){
        maze[i][j] = LCD_getPixel(64-i,j);
        //LCD_printf("%u",maze[i][j]);
      }
  }

  //Solo guardar informacion de los muros
  maze[start_i][start_j] = 0;
  maze[start_i+1][start_j] = 0;
  maze[start_i][start_j+1] = 0;
  maze[start_i+1][start_j+1] = 0;
  
  LCD_setPixel(start_i+1,start_j,0);
  LCD_setPixel(start_i,start_j+1,0);
  LCD_setPixel(start_i+1,start_j+1,0);

  maze[goal_i][goal_j] = 0;
  maze[goal_i+1][goal_j] = 0;
  maze[goal_i][goal_j+1] = 0;
  maze[goal_i+1][goal_j+1] = 0;

  maze[goal_i-1][goal_j-1] = 0;
  maze[goal_i-2][goal_j-2] = 0;

  maze[goal_i+2][goal_j+2] = 0;
  maze[goal_i+3][goal_j+3] = 0;

  maze[goal_i-1][goal_j+2] = 0;
  maze[goal_i-2][goal_j+3] = 0;

  maze[goal_i+2][goal_j-1] = 0;
  maze[goal_i+3][goal_j-2] = 0;
}

uint8_t checkGoal(){
  return start_i == goal_i && start_j == goal_j;

}


void moveLeft(){
/*  if(start_j-1>=0 && start_i+1<64){
  if(maze[start_i][start_j-1]==0 && maze[start_i+1][start_j-1]==0){
    start_j--;
    LCD_setPixel(start_i  , start_j+2,0);
    LCD_setPixel(start_i+1, start_j+2,0);
    LCD_setPixel(start_i, start_j,1);
    LCD_setPixel(start_i+1, start_j,1);
  }}*/
  if(start_j-1<0) return;
  if(maze[start_i][start_j-1]==0){
    start_j--;
    LCD_setPixel(start_i, start_j,1);
    LCD_setPixel(start_i, start_j+1,0);
    
  }
}

void moveRight(){
/*  if(start_j+2<102 && start_i+1<64){
  if(maze[start_i][start_j+2]==0 && maze[start_i+1][start_j+2]==0 ){
    start_j++;
    LCD_setPixel(start_i  , start_j-1,0);
    LCD_setPixel(start_i+1, start_j-1,0);
    LCD_setPixel(start_i, start_j+1,1);
    LCD_setPixel(start_i+1, start_j+1,1);
  }}*/  
  if(start_j+1>=102) return;
  if(maze[start_i][start_j+1]==0){
    start_j++;
    LCD_setPixel(start_i, start_j,1);
    LCD_setPixel(start_i, start_j-1,0);
    
  }
}

void moveUp(){
/*  if(start_j+1<102 && start_i-1>=0){
  if(maze[start_i-1][start_j]==0 && maze[start_i-1][start_j+1]==0){
    start_i--;
    LCD_setPixel(start_i+2, start_j,0);
    LCD_setPixel(start_i+2, start_j+1,0);
    LCD_setPixel(start_i, start_j,1);
    LCD_setPixel(start_i, start_j+1,1);
  }}  */
  if(start_i-1<0) return;
  if(maze[start_i-1][start_j]==0){
    start_i--;
    LCD_setPixel(start_i, start_j,1);
    LCD_setPixel(start_i+1, start_j,0);
    
  }

}

void moveDown(){
  /*if(start_j+1<102 && start_i+2<64){
  if(maze[start_i+2][start_j]==0 && maze[start_i+2][start_j+1]==0){
    start_i++;
    LCD_setPixel(start_i-1, start_j,0);
    LCD_setPixel(start_i-1, start_j+1,0);
    LCD_setPixel(start_i, start_j,1);
    LCD_setPixel(start_i, start_j+1,1);
  }}*/
  if(start_i+1>=64) return;
  if(maze[start_i+1][start_j]==0){
    start_i++;
    LCD_setPixel(start_i, start_j,1);
    LCD_setPixel(start_i-1, start_j,0);
    
  }  
}

