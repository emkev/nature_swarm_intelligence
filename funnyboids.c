
/* 20151125 am 0:20 start , 
   for be funny to code the Gary's boids.c
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int width = 60 , height = 30 ;
int steps = 400 ;

// number of boid 
int num = 20 ;

/* rcopy -- radius for copy vector ,
   rcent -- radius for centroid vector ,
   rvoid -- radius for avoidance vector , 
   rviso -- radius for visual avoidance vector , 
 */
double rcopy = 30 , rcent = 20 , rvoid = 2 , rviso = 2 ;

/* wcopy -- weight for copy vector , 
   wcent -- weight for centroid vector , 
   wvoid -- weight for avoidance vector , 
   wviso -- weight for visual avoidance vector , 
   wrand -- weight for random vector , 
 */
double wcopy = 0.2 , wcent = 0.4 , wvoid = 1.0 , wviso = 0.8 , wrand = 0.0 ;

double ddt = 0.95 , dt = 3.0 ;
/* min velocity */
double minv = 0.5 ; 

/* boids(x , y) positions , velocity vectors , new velocity vectors
 */
double *xp , *yp , *xv , *yv , *xnv , *ynv ;

/* some handy marcos ... , from Gary . 
 */
#define LEN(x , y) sqrt((x * x) + (y * y))
#define DIST(x1 , y1 , x2 , y2) LEN((x1 - x2) , (y1 - y2))
#define DOT(x1 , y1 , x2 , y2) (x1 * x2 + y1 * y2)
#define MAX(x , y) x >= y ? x : y

/* 20151130 18:55
   For one time , it presents different random numbers . 
   But it presents same number-set every time . 
*/
double random_range(double minnum , double maxnum)
{
  //int diff = 0 ;
  int ra = 0 ; 
  double randdiff = 0.0 , result = 0.0 ;
  double diff = 0.0;

  //srand((unsigned)time(NULL));
  diff = fabs(maxnum - minnum);
  //printf("diff = %d\n" , diff);
  //printf("rand = %d , rand %% 10 = %d\n" , rand() , rand() % 10);
  //printf("RAND_MAX = %d\n" , RAND_MAX);
  //printf("rand = %d\n" , rand());
  //ra = rand();
  //printf("ra = %d" , ra);
  randdiff = ((double)random() / (double)RAND_MAX) * ((double)diff);
  result = randdiff + (double)minnum;
  printf("result = %f\n" , result);

  return result;
}

/* normalize a vector . 
 */
void norm(double *x , double *y)
{
  double len;

  len = LEN(*x , *y);
  if(len != 0.0)
  {
    *x /= len;
    *y /= len;
  }
}

void *xmalloc(size_t bytes)
{
  void *ptr ;

  if( (ptr = malloc(bytes)) == NULL )
  {
    fprintf(stderr , "malloc failed to get %zu bytes . \n" , bytes);
    exit(1);
  }
  return(ptr);
}

void compute_new_heading(int which)
{
  int i , j , k ;
  int numcent = 0 ;

  double maxr ;
  double xa , ya , xb , yb , xc , yc , xd , yd , xt , yt ;
  double mindist , xpitemp , ypitemp , lentemp ;
  double mx = 0 , my = 0 ; 
  double xtemp , ytemp ;

  maxr = MAX(rviso , MAX(rcopy , MAX(rcent , rvoid)));

  xa = ya = xb = yb = xc = yc = xd = yd = 0 ;

  /* for every boid ... */
  for(i = 0 ; i < num ; i++)
  {
    /* 'which' is self , so do not include self . 
       'continue' means end current loop , continue to next loop .
     */
    if(i == which) continue;

    /* Relate to boid 'which' , by 9 direction desplaments ( include current
       position of boid 'i' ) , the most less distance to 'which' is 
       summarized to (mx , my) .
       This has included the situations ( 8 time displacements ) that 
       " left space connects to right space , up space connect to low 
       space " . For instance , 'i' in right edge is near to 'which' in left
       edge . 
       In the cases , when j = 0 and k = o , the coordinate xp[i]+j and 
       yp[i]+k is the current position 'i' 
       So , compute all 9 directions together , amazing !
     */

     mindist = 10e10;
     for(j = -width ; j <= width ; j += width )
     {
       for(k = -height ; k <= height ; k += height )
       {
         xpitemp = xp[i] + j ;
         ypitemp = yp[i] + k ;
         lentemp = LEN(xpitemp - xp[which] , ypitemp - yp[which]);
         if(lentemp < mindist)
	 {
           mindist = lentemp ;
           mx = xpitemp ;
           my = ypitemp ;
	 }
       }
     }
    
     /* If the distance is farther than any of the rule radius , than skip . */
     if(mindist > maxr) continue;

     /* Waiting for add the situation that boid(which) cannot see boid(i) .
      */

     /* FOUR-RULES :

        centering rule ( radius of 'center' , radius of 'avoidance' )
        accumulation of difference vector ( changing vector ) ; 

        copying rule ( radius of 'copy' , radius of 'avoidance')
        accumulation of boid(i)'s velocity ;

        avoidance rule ( radius of 'avoidance' )
        difference vector , then normalize , then accumulate ;
      */


     /* If the distance between the two boids is within the radius of 
        centering rule , but outside of the radius of the avoidance rule , 
        then attempt to center in on boid(i) .
      */
     if(mindist <= rcent && mindist > rvoid)
     {
       xa += mx - xp[which] ;
       ya += my - yp[which] ;
       numcent++ ;
     }

     /* If we are close enough to copy , but far enough to avoid , 
        then copy boid(i)'s velocity .
      */
     if(mindist <= rcopy && mindist > rvoid)
     {
       xb += xv[i] ;
       yb += yv[i] ;
     }

     /* If we are within collision range , then try to avoid boid(i) .  
      */
     if(mindist <= rvoid)
     {
       xtemp = xp[which] - mx ;
       ytemp = yp[which] - my ;

       /* Gary : make the length of the avoidance vector inversely 
          proportional to the distance between the two boids . 
          I : normalize the difference vector (xtemp , ytemp) , 
          then accumulate . Now I don't know why normalize .
	*/
       xtemp /= LEN(xtemp , ytemp) ;
       ytemp /= LEN(xtemp , ytemp) ;
       xc += xtemp ;
       yc += ytemp ;
     }

     /* about visual avoidance , waiting for finishing . 
      */

  }
  /* maybe , cancel centering when only one boid .  */
  if(numcent < 2)
    xa = ya = 0 ;

  /* in this , using &xa maybe is for tranferring value between functions . */
  if(LEN(xa , ya) > 1.0) norm(&xa , &ya);
  if(LEN(xb , yb) > 1.0) norm(&xb , &yb);
  if(LEN(xc , yc) > 1.0) norm(&xc , &yc);
  if(LEN(xd , yd) > 1.0) norm(&xd , &yd);

  /* Gary : Compute the composite trajectory based on all of the rules . 
     I : Compute all composite with rule-variables and rule-weights .  
   */
  xt = xa * wcent + xb * wcopy + xc * wvoid + xd * wviso ; 
  yt = ya * wcent + yb * wcopy + yc * wvoid + yd * wviso ;

  /* add some noise . 'wrand' is a weight by random , 
     for add some changing situations .  
   */
  if(wrand > 0)
  {
    xt += random_range(-1 , 1) * wrand ; 
    yt += random_range(-1 , 1) * wrand ; 
  }

  /* Compute new velocity by some velocity-time formulas . 
     my formulas : 
     a = dv / dt , dv = a dt   ,   v(new) = v(0) + at ; 
     dx = (v(0) + at) dt   ,   x = x(0) + v(0)t + (1/2)a(t^2) ; 
   */
  xnv[which] = xv[which] * ddt + xt * (1 - ddt) ;
  ynv[which] = yv[which] * ddt + yt * (1 - ddt) ;

  /* normalize if new velocity is too small . */
  lentemp = LEN(xnv[which] , ynv[which]) ;
  if(lentemp < minv)
  {
    xnv[which] = minv / lentemp ;
    ynv[which] = minv / lentemp ;
  }

}

/* 2015.12.11 , be stupid to plot a boids space view by printf() . */
void stupid_printf()
{
  int h , w , i ;
  int point ;

  for(h = 0 ; h < height ; h++)
  {
    for(w = 0 ; w < width ; w++)
    {
      point = 0 ;

      /* check if the location exists a boid .  */
      for(i = 0 ; i < num ; i++)
      {
        if(((int)yp[i] == h) && ((int)xp[i] == w))
          point = 1 ; 
      }

      /* if exists a boid , then plot a "*" , or plot a space . */
      if(point == 1) 
        printf("*");
      else 
        printf(" ");

    } /* for(w = 0 ; ... */

    printf("\n");

  } /* for(h = 0 ; ... */

}

int main(int argc , char **argv)
{
  double rannum ;
  
  /* int xp , yp ; 
     Before , define this local variable cause error : 
     incompatible pointer to integer conversion assigning to 'int' from 
     'void *'
     xp = malloc(sizeof(double) * num);
        ^
  */
  int i , j ;

  srand((unsigned)time(NULL));
  rannum = random_range(10 , 100);

  //printf("rannum = %f\n" , rannum);

  //printf("len = %f\n" , LEN(30 , 40));
  //printf("dist = %f\n" , DIST(2 , 4 , 6 , 9));

  xp  = malloc(sizeof(double) * num);
  yp  = malloc(sizeof(double) * num);
  xv  = malloc(sizeof(double) * num);
  yv  = malloc(sizeof(double) * num);
  xnv = malloc(sizeof(double) * num);
  ynv = malloc(sizeof(double) * num);

  for(i = 0 ; i < num ; i++)
  {
    xp[i] = rand() % width;
    yp[i] = rand() % height;
    xv[i] = random_range(-1.0 , 1.0);
    yv[i] = random_range(-1.0 , 1.0);

    norm(&xv[i] , &yv[i]);
  }

  for(i = 0 ; i < steps ; i++)
  {
    for(j = 0 ; j < num ; j++)
    {
      /* for each boid , compute the distances of all other boids 
         relate to it , ... 
       */
      compute_new_heading(j);
    }

    for(j = 0 ; j < num ; j++)
    {
      /* undraw the boid . */

      /* update velocity and position . 
         my formulas :
         x = x(0) + v(0)t + (1/2)a(t^2)
         Gary's formulas :
         x(new) = x(0) + v(new) * dt
       */
      xv[j] = xnv[j] ;
      yv[i] = ynv[i] ;
      xp[j] += xv[j] * dt ;
      yp[j] += yv[i] * dt ;

      /* Wrap around the screen coordinates . */
      if(xp[j] < 0 ) 
        xp[j] += width ;
      else if(xp[j] >= width)
        xp[j] -= width ;

      if(yp[j] < 0)
        yp[j] += height ;
      else if(yp[j] >= height - 1)
        yp[j] -= height ;

      /* Redraw the boid . */

      //printf("xp = %f , yp = %f\n" , xp[j] , yp[j]);

    } /* for(j = 0 ; j < num ; ... */

    /* a stupid output by printf() . */
    stupid_printf();

  } /* for(i = 0 ; i < steps ; ... */

  exit(0);
}
