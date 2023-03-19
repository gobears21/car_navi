/* ex011201.c --- The first step of the Dijkstra’s algorithm */
#include <stdio.h>
#include <string.h>
#include <math.h>
#define CrossingNumber 100 /* The number of crossings = 100 */
#define MaxName              50 /* The maximum number of characters = 50 (half-width) */
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



int main(int crossing_number)
{
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
 //                      printf("%2d %5.1lf %s\n",i+1,cross[path[i]].distance,cross[path[i]].ename);
                       route[i+1].id=path[i]; 
                       i++;
            }

  return 0;
}




