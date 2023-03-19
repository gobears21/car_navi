
/* mobile_s.c --- Display the moving object for which search was used */
/* compile: cc mobile_s.c -g -O2 -Wall -I/usr/include/freetype2 -lftgl -lglfw -lGLU -lGL -lX11 -lXrandr -lm -o mobile_s */

#include <stdio.h> 
#include <math.h>
#include <unistd.h> 
#include <GL/glfw.h>
#include <FTGL/ftgl.h>

#define CrossingNumber 100 
#define MaxName 50 
#define Radius_Marker 0.2 


#define ORIGIN_X -2.0 
#define ORIGIN_Y 3.0
#define REAL_SIZE_X 15.0 
#define REAL_SIZE_Y 15.0

typedef struct {
           double x, y;           /* Position x, y */
} Position;                       /* A structure that represents positions */
typedef struct {
           int id;               /* Crossing number */
           Position pos;         /* A structure that represents positions */
           double wait;          /* Average waiting time */
           char jname[MaxName];             /* Crossing name */
           char ename[MaxName];             /* Crossing name */
           int points;                      /* The number of crossing roads */
           int next[5];                     /* Crossing numbers of neighboring crossings */
           double distance;                 /* The distance to the reference crossing: add */
} Crossing;
typedef struct{
	int id;
}Road;

Crossing cross[CrossingNumber];
Road route[20];

 int map_read(char *filename)
{
/* Refer to ex2-1.c and complete */
          FILE *fp;
          int i, j;
          int crossing_number;                  /* The number of crossings */
          fp = fopen(filename, "r");
          if (fp == NULL) {
                      perror(filename);
                      return -1;
          }
          /* First, read the number of crossings */
          fscanf(fp, "%d", &crossing_number);
          for (i = 0; i < crossing_number; i++) {
                      /* Write a program that uses the function fprintf to output
                          the data of the structure to a file */
                      fscanf(fp, "%d,%lf,%lf,%lf,%[^,],%[^,],%d",
                                     &(cross[i].id), &(cross[i].pos.x), &(cross[i].pos.y),
                                     &(cross[i].wait), cross[i].jname,
                                     cross[i].ename, &(cross[i].points));
                      for(j=0; j < cross[i].points; j++) {
                                 fscanf(fp, ",%d", &(cross[i].next[j]));
                      }
  }
  fclose(fp);
  /* Return the number of crossings that has been read from the file */
  return crossing_number;
}




/* Modify so that the distance can be displayed */
void print_cross(int i)
{
           int j;
           printf("Crossing Number:%2d, Coordinates(%5.2lf,%5.2lf), Name: %s ( %s ),", cross[i].id, cross[i].pos.x, cross[i].pos.y, cross[i].jname,cross[i].ename);
           printf("\n         Distance to the destination %5.1lf Waiting time:%5.1lf, The number of neighboring crossing:%d ( ", cross[i].distance,cross[i].wait, cross[i].points); /* Modified here */

           for(j=0; j < cross[i].points; j++)             /* Repeat for the number of crossing roads */
                      printf("%d ", cross[i].next[j]);
           printf(")\n\n");
}
void print_cross_list(int num) /* Modified print_cross used in the exercise for search and sort to display the
number num = possible to limit the number of displayed items */
{
   int i;
   for(i=0;i < num; i++)
      print_cross(i);        /* Call the function print_cross for num times */
}


//-------------------------
/* Use search_cross used in search and sort as is */
int search_cross(int num) /* Search for exactly-matched data */
{
          int i;
          int f= -1;
          char input[200];
          printf("Input a crossing name (alphabetical) ->");
          scanf("%s",input);
          puts("");
          for(i=0;i<num;i++)
      {
                     if(strcmp(cross[i].ename,input)==0)
                     {
                        //print_cross(i);     /* Display */
                        f=i;                /* Retain the number found */
                     }
      }
if(f<0)
 printf("%s has not been found \n", input);
          return f; /* If found, return the data. Otherwise, return -1. */
}




/*
  /// The script finishes around here ////
  /// From here, create and write your own script ////
*/
/* Provide the "distance" between the crossings a and b */
double distance(int a, int b)
{
           return hypot(cross[a].pos.x-cross[b].pos.x, /* If this formula is changed */
                          cross[a].pos.y-cross[b].pos.y); /* evaluation will be changed */
}

void dijkstra(int crossing_number,int target)
{
          int i,j,n;
          double d;
          double min_distance;   /* As usual, a variable to search for "the smallest" distance */
          int min_cross = 0;
          int done[CrossingNumber];                   /* Set Confirmed:1 or Not confirmed:0 to this flag */
          for(i=0;i < crossing_number;i++)            /* Initialization */
     {
                     cross[i].distance=1e100;         /* Set an unrealistically large value as the initial value */
                     done[i]=0;                       /* All crossings have not been confirmed */
     }
          /* Set 0 for the reference crossing */
          cross[target].distance=0;
          for(i=0;i < crossing_number;i++) /* This should end after repeating crossing_number times */
     {
          /* Select a crossing that has the smallest distance from unconfirmed crossings --> refer to
          the description of search */
                     min_distance=1e100;
                     for(j=0;j < crossing_number;j++)
                     {
                                  /*
                                  * Write a program so that it sets the crossing number of a crossing that has
                                  * the smallest distance value to min_cross.
                                  * Note that only the crossings which done is 0 are processed here.
                                  */
	  if((done[j]==0)&&(cross[j].distance < min_distance))
	    {
	      min_distance=cross[j].distance;
	      min_cross=j;
	    }
                     }
                     /* The crossing min_cross can be confirmed */
                     done[min_cross]=1; /* Confirmed */
                     /* Calculate the distance based on the confirmed crossing */
                     for(j=0;j < cross[min_cross].points;j++)
                     {
                                n=cross[min_cross].next[j];          /* Replace (only) to make it short */
                                /* Evaluation metrics "distance" */
                                d=distance(min_cross,n)+cross[min_cross].distance;
                                /* Compare with the current temporary value. If shorter, update */
                                if(cross[n].distance > d)
                                           cross[n].distance = d;
                     }
     }
}

//-------------
/* Set the shortest path */
int pickup_path(int crossing_number,int start,int goal,int path[],int maxpath)
{
          int c=start;            /* The current crossing */
          int i,j,n;
          double min_distance;
          int min_cross = 0;
          path[0]=start;
          i=1;
                                    /* Set “start” to the current value */
          c=start;
          while(c!=goal)
                 {
                     /* Search for the crossing to go after the crossing c                */
                     /* Write a program that finds the smallest                            */
                     /* "distance to a neighboring crossing + the shortest distance among
                          the neighboring crossings".
                       *->The resulted crossing number should be set to min_cross.
                       */
      min_distance=1e100;
      for(j=0;j<cross[c].points;j++)
	{
	  n=cross[c].next[j];
	  if(min_distance > cross[n].distance+distance(c,n))
	    {
	      min_distance=cross[n].distance+distance(c,n);
	      min_cross=n;
	    }
	}


            c=min_cross;                   /* The next crossing to go to */
            if((i > 1)&&(c==path[i-2])) /* Is it the same as 2 crossings before? */
            {
                       printf("I am stuck(%d-%d). I give up this path search(;_;)\n",
                                     c,path[i-1]);
                       return -1;
            }
            path[i]=c;
            i++;
            if(i==maxpath-1) /* Too long */
            {
                       printf("The path has become too long. I give up this path search\n");
                       return -2;
            }
       }
  path[i]=-1;     /* This marks the end */
  return 0;
}


/* Draw a circle */
void circle(double x, double y, double r)
{
    int const N = 12; /* Divide the circumference by 12 and draw in line segments */
    int i;

    glBegin(GL_LINE_LOOP); 
    for (i = 0; i < N; i++)
        glVertex2d(x + cos(2 * M_PI * i / N) * r, y + sin(2 * M_PI * i / N) * r); 
    glEnd();
}

#ifndef FONT_FILENAME
/* The file name of the fonts that are available on the training server */
#define FONT_FILENAME "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf" 
#endif
FTGLfont *font; /* A pointer that points to the read fonts */

/* Draw strings */
void outtextxy(double x, double y, char const *text) {
    double const scale = 0.01; 
    glPushMatrix();
    glTranslated(x, y, 0.0); 
    glScaled(scale, scale, scale);
    ftglRenderFont(font, text, FTGL_RENDER_ALL); 
    glPopMatrix();
}




/* Search for the nearest crossing to the crossing goal among the neighboring crossings to the crossing id */
int search_nearest(int id, int goal) {
    int i;
    int nearest = 0; /* nearest represents the ID of the nearest crossing */
    double min_distance = 1e100; /* Initialize with a large numerical value temporarily */ 
    double distance;
    for (i = 0; i < cross[id].points; i++)
    {

        distance = hypot((cross[ cross[id].next[i] ].pos.x - cross[goal].pos.x), 
                (cross[cross[id].next[i]].pos.y - cross[goal].pos.y));

        if(distance <= min_distance){
            min_distance = distance;
            nearest = cross[id].next[i];
        }
    }
    return nearest;
}



/* Display road networks */
void map_show(int crossing_number) {
    int i, j;

    for (i = 0; i < crossing_number; i++) { /* A loop for each crossing */ 
        double x0 = cross[i].pos.x;
        double y0 = cross[i].pos.y;

        /* Draw a circle that represents a crossing */
        glColor3d(1.0, 0.5, 0.5);
        circle(x0, y0, 0.1);

        /* Draw the name of the crossing */
        glColor3d(1.0, 1.0, 0.0);
        outtextxy(x0, y0, cross[i].ename);

        /* Draw roads that start from the crossing */
        glColor3d(1.0, 1.0, 1.0); 
        glBegin(GL_LINES);

        for(j = 0; j < cross[i].points; j++) {
            double x1 = cross[ cross[i].next[j] ].pos.x;
            double y1 = cross[ cross[i].next[j] ].pos.y; 
            glVertex2d(x0, y0);
            glVertex2d(x1, y1); 
        }

        glEnd(); 
    }
}


















int main(void) {
    int crossing_number; /* The number of crossings */
    int vehicle_goal; /* The final destination point of the moving object */
    int vehicle_edgeFrom; /* The position of the moving object on the path 
                             (it represents that this is the n-th road) */
    int vehicle_edgeTo; /* The position of the moving object on the path 
                           (it represents that this is the n-th road) */
    int vehicle_stepOnEdge; /* The position of the moving object on the road 
                               (it represents that this is the n-th step) */
    double vehicle_x = 0.0, vehicle_y = 0.0; /* The coordinate of the moving object */
	int goal,start;
    int path[20];
    int i;
	
	
	crossing_number = map_read("map2.dat");
    printf("loaded %d crossings\n",crossing_number);
    for(i=0;i<crossing_number;i++)cross[i].distance=0;                              
    printf("Determine the destination point.") ;
    goal=search_cross(crossing_number);
    if(goal<0)
        return 1;  
					       
    dijkstra(crossing_number,goal);
            //print_cross_list(crossing_number);
    printf("Determine the current position.") ;
    start=search_cross(crossing_number);
    if(start<0)
                       return 1;
    if(pickup_path(crossing_number,start,goal,path,20)<0)
        return 1;
    printf("Path has been confirmed\n");
    i=0;
    while(path[i]>=0)
    {
 //     printf("%2d %5.1lf %s\n",i+1,cross[path[i]].distance,cross[path[i]].ename);
        route[i+1].id=path[i]; 
        i++;
    }


    /* Initialize the graphic environment and open a window */
    glfwInit();
    glfwOpenWindow(1000, 800, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

    /* Based on (ORIGIN_X, ORIGIN_Y), project the space within the range of REAL_SIZE_X * REAL_SIZE_Y to the viewport */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ORIGIN_X + REAL_SIZE_X * -0.5, ORIGIN_X + REAL_SIZE_X * 0.5,ORIGIN_Y + REAL_SIZE_Y * -0.5, ORIGIN_Y + REAL_SIZE_Y * 0.5, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); /* Do not perform other coordinate conversion */



    /* Read and configure the font used for drawing strings */
    font = ftglCreateExtrudeFont(FONT_FILENAME); 
    if (font == NULL) 
	{
        perror(FONT_FILENAME);
        fprintf(stderr, "could not load font\n");
        exit (1); 
    }

    ftglSetFontFaceSize(font, 24, 24);
    ftglSetFontDepth(font, 0.01); 
    ftglSetFontOutset(font, 0, 0.1);
    ftglSetFontCharMap(font, ft_encoding_unicode);


    /* Initialize the state of the moving object */
#if 1
    vehicle_edgeFrom = 5; /* From Izumi Ohashi */
    vehicle_goal = 19; /* To the Road Museum */ 
#else
    vehicle_edgeFrom = 5; /* From Izumi Ohashi */ 
    vehicle_goal = 71; /* To Yagiyama Zoological Park */
#endif
    vehicle_edgeTo = vehicle_edgeFrom;
    vehicle_stepOnEdge = 0;




    for (;;) {
        int width, height;
		
        /* End when Esc is pressed or the window is closed */
        if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)) 
            break;

        glfwGetWindowSize(&width, &height); /* Get the size of the current window */
        glViewport(0, 0, width, height); /* Set the whole window as a viewport */
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT); /* Fill the back buffer with black */

        map_show(crossing_number); /* Display road networks */


 		vehicle_edgeFrom=route[i].id;
 		vehicle_goal=goal;
        /* Move the moving object forward and calculate the coordinate */
        if (vehicle_edgeFrom != vehicle_goal) {
            /* Since the goal has not been reached, move forward the position of the moving object */
            /* Create based on the previous exercise.
               The number of variables and their names have been changed, but the basic flow is the
               same.
               Complete the program.
               */

            vehicle_edgeTo = route[i+1].id;

            /* Set (x0, y0), (x1, y1) for the current and next crossings */
            double x0 = cross[vehicle_edgeFrom].pos.x; /* The last crossing passed, x */
            double y0 = cross[vehicle_edgeFrom].pos.y; /* The last crossing passed, y */
            double x1 = cross[vehicle_edgeTo].pos.x; /* The next crossing to pass, x */
            double y1 = cross[vehicle_edgeTo].pos.y; /* The next crossing to pass, y */

            double distance = hypot(x1 - x0, y1 - y0); /* Direct distance */
            int steps = (int)(distance / 0.1);

            /* Move forward on the road and update the coordinate */
            vehicle_stepOnEdge++;

            vehicle_x = x0 + (x1 - x0) / steps * vehicle_stepOnEdge; 
            vehicle_y = y0 + (y1 - y0) / steps * vehicle_stepOnEdge;

            if (vehicle_stepOnEdge >= steps) { /* After reaching the crossing, enter the next road */
                vehicle_edgeFrom = vehicle_edgeTo; /* Increment the index that represents the current crossing on the path */
                vehicle_stepOnEdge = 0; /* Reset the index that represents that it is the n-th step on the edge */
            }

        }

        /* Display the goal and the moving object */
        glColor3d(1.0, 1.0, 1.0);

        circle(cross[vehicle_goal].pos.x, cross[vehicle_goal].pos.y, Radius_Marker);
        circle(vehicle_x, vehicle_y, Radius_Marker);

        glfwSwapBuffers(); /* Swap the front buffer with the back buffer */
        usleep(50 * 1000); /* Wait for about 50 millisecond */ 
    }

    glfwTerminate();

    return 0;
}

