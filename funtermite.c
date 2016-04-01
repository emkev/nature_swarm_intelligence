
/* 2015.12.15 start , test to program the termite program from Gary . */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int width = 60 , height = 30 ;
int num = 4 , steps = 4 ;
double dense = 0.3 ;

/*
          ^ y
 [-1,1]  [0,1]  [1,1]
 [-1,0]  [0,0]  [1,0]  x->
 [-1,-1] [0,-1] [1,-1]

*/
int dir[8][2] = {
  { 0 , 1 } , { 1 , 1 } , { 1 , 0 } , { 1 , -1} , 
  { 0 , -1} , {-1 , -1} , {-1 , 0 } , {-1 , 1 }
} ; 

void newstep(int *a , int *b , int *d)
{
  /* "d" is a current direction by dir[*d][ ] .
     " (random() % 3) - 1 " is a trunning left or right of 45 degrees . 
     " *d + ((random() % 3) - 1) " is a turnning direction value 
     by dir[*d][ ] , then "+ 8 )" makes sure that all changing value is in
     the area when %8 later .
     Then , " % 8 " selects a new direction by dir[*d][ ] . 
   */
  *d = (*d + ((rand() % 3) - 1) + 8) % 8 ;

  /* move coordinate */
  *a = (*a + dir[*d][0] + width) % width ;
  *b = (*b + dir[*d][1] + height) % height ; 
}

int main(int argc , char **argv)
{
  int *tx , *ty , *td ;
  int i , j ;
  char **chips ;

  srand((unsigned)time(NULL));

  tx = malloc(sizeof(int) * num);
  ty = malloc(sizeof(int) * num);
  td = malloc(sizeof(int) * num);

  for(i = 0 ; i < num ; i++)
  {
    tx[i] = rand() % width ;
    ty[i] = rand() % height ;
    td[i] = rand() % 8 ;
  }

  chips = malloc(sizeof(char *) * width) ;
  for(i = 0 ; i < width ; i++)
  {
    chips[i] = malloc(sizeof(char) * height) ;
    for(j = 0 ; j < height ; j++)
    {
      /* note that chips array is almost equals to (width * heigth) . */
      chips[i][j] = (random_range(0.0 , 1.0) < dense) ? 1 : 0 ;
      if(chips[i][j])
        /* plot chips */
    }
  }

  /* Rules ( by me ) :

     A ant encounter a chip , then carry the chip . Then it always carry 
     and step randomly until it encounters another new chip .
     If a ant with a chip encounters another chip , then drops current chip
     and steps to the reverse direction , or gets a new one .
     If it happens nothing , a ant just steps randomly .

     In this process , all chips are moved together slowly . 
   */

  for(i = 0 ; i < steps ; i++)
  {
    for(j = 0 ; j < num ; j++)
    {
      /* for each termite , it has self's current direction , so when take 
         new step , that need to calculate
       */
      nx = tx[j] ;
      ny = ty[j] ;
      newstep(&nx , &ny , &td[j]) ;

      /* Rules are coming . */

      /* carry a chip (means that there is a chip 
         in current position(tx[j],ty[j]) ) 
         and move to a empty spot (means that there isn't a chip in the 
         moving position) . 

         By me : a ant with a chip always carry and step randomly until 
         encounter a new chip .
       */
      if(chips[tx[j]][ty[j]] && !chips[nx][ny])
      {
        /* erase the old spot . note that chips array is almost equals to 
           (width * height) , so locate a chip by using coodrinate of a ant .
         */
        chips[tx[j]][ty[j]] = 0 ;

        /* undraw this spot for that a ant move a chip away . */

        tx[j] = nx ;
        ty[j] = ny ;
        chips[tx[j]][ty[j]] = 1 ;

        /* draw a new sopt . */

      }
      /* the situation is that a ant carry a chip and there is another chip 
         in a moving position . Then reverse direction and step , 
         drop the chip , and perhaps get a new one . 
       */
      else if(chips[tx[j]][ty[j]] && chips[nx][ny])
      {
        /* Adding value "4" means that a reverse direction . */
        td[j] = (td[j] + 4) % 8 ;
        /* All chip marks are not changed , Just move . */
        tx[j] = (tx[j] + dir[td[j]][0] + width) % width ;
        ty[j] = (ty[j] + dir[td[j]][1] + height) % height ;
      }
      /* just move and do nothing else . */
      else
      {
        tx[j] = nx ;
        ty[j] = ny ;
      }

    } /* for(j = 0 ; j < num ; ... */

  } /* for(i = 0 ; i < steps ; ... */

  exit(0);

}
