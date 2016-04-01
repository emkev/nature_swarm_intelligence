
/* 2015.12.17 start */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int width = 60 , height = 30 ;
int num = 1 , steps = 4 ;
int states = 2 ;
double dense = 0.0 ;
char *rule = "10" ;

int random_range_int(int minnum , int maxnum)
{
  int diff = 0 , randdiff = 0 , result = 0 ;

  diff = abs(maxnum - minnum);
  randdiff = rand() % diff ;
  /* randdiff = (rand() / RAND_MAX) * diff ;  */
  result = randdiff + minnum ;

  return result;
  
}

double random_range_double(double minnum , double maxnum)
{
  double diff = 0.0 , randdiff = 0.0 , result = 0.0 ;

  diff = fabs(maxnum - minnum);
  randdiff = ((double)rand() / (double)RAND_MAX) * ((double)diff) ;
  result = randdiff + (double)minnum ;

  return result;
}

int main(int argc , char ** argv)
{
  int i , j ;
  int *vx , *vy , *vd , nx , ny ;
  int old ;
  char **grid ;

  srand((unsigned)time(NULL));
  states = strlen(rule);

  vx = malloc(sizeof(int) * num);
  vy = malloc(sizeof(int) * num);
  vd = malloc(sizeof(int) * num);

  for(i = 0 ; i < num ; i++)
  {
    vx[i] = random_range_int(0 , width);
    vy[i] = random_range_int(0 , height);
    vd[i] = random_range_int(0 , 4);
  }

  grid = malloc(sizeof(char *) * width);
  for(i = 0 ; i < width ; i++)
  {
    grid[i] = malloc(sizeof(char) * height);
    for(j = 0 ; j < height ; j++)
    {
      if(dense > 0)
        grid[i][j]=(random_range_double(0.0 , 1.0)<dense) ? rand()%states : 0;
      else
        grid[i][j] = 0;

      /* draw point */

    } /* for(j = 0 ; j < height ; ... */
  } /* for(i = 0 ; i < width ; ... */

  for(i = 0 ; i < steps ; i++)
  {
    for(j = 0 ; j < num ; j++)
    {
      /* */
      nx = vx[j];
      ny = vy[j];

      if(vd[j] == 0) ny++;
      else if(vd[j] == 1) nx++;
      else if(vd[j] == 2) ny--;
      else nx--;

      /* wrap around coordinates . 
         Gary's amazing expression ! ! !
         If my turn , I'm afraid that it will use a lot of "if ... else" .
       */
      nx = (nx < 0) ? (width - 1) : (nx > (width - 1)) ? 0 : nx ;
      ny = (ny < 0) ? (height - 1) : (ny > (height - 1)) ? 0 : ny ;
      /* above expression is amazing ! Must repeat again ! Funny , Funny ! */

      old = grid[nx][ny];
      grid[nx][ny] = (grid[nx][ny] + 1) % states ;
      /* draw point (nx , ny) */

      vx[j] = nx;
      vy[j] = ny;

      /* ... int states = 2 ... char *rule = "10" ...
         ... states = strlen(rule) ...
         grid[][] is "states" int value , "old" is "states" int value .
         All variables about "states" are NUMBER of "rule" char array .
         rule[old] is fact rule from rule array .
       */

      //printf("rule[old] - 0 = f%" , rule[old] - '0');
      vd[j] += (rule[old] - '0') ? (4 + 1) : (4 - 1) ;
      vd[j] %= 4 ;

    } /* for(j = 0 ; j < num ; ... */
  } /* for(i = 0 ; i < steps ; ... */

  exit(0);
}
