#include<stdio.h> 
#include<math.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>








#define CrossingNumber 100
#define MaxName 50 









typedef struct {
    double x, y; 
} Position; 

typedef struct jietouti{
    int id; 
    Position pos; 
    double wait;
    char jname[MaxName]; 
    char ename[MaxName]; 
    int points; 
    int next[5];
    double distance;
} Crossing;

typedef struct{
  int sid;
  int eid;
  int path;
}Idjiegouti;

typedef struct{
	double distance;
	double totaldis;
}Connected_distance;

typedef struct{
	int id;
}Stepid;













Crossing cross[CrossingNumber];
Idjiegouti jiegouti1[CrossingNumber];
Connected_distance connected[5];
Stepid step[CrossingNumber];







int read_map(char *filename, char *mode)
{
  int crossing_number,i,j;
  FILE *fp;
  fp=fopen(filename,mode); 
  if (fp == NULL) 
    {
      perror(filename);
      return -1;
    }
  
    fscanf(fp, "%d", &crossing_number);
   
    for (i = 0; i < crossing_number; i++)
      {
        fscanf(fp, "%d,%lf,%lf,%lf,%[^,],%[^,],%d",&(cross[i].id), &(cross[i].pos.x), &(cross[i].pos.y), &(cross[i].wait), cross[i].jname,cross[i].ename, &(cross[i].points));
	

        for(j=0; j < cross[i].points; j++)
	  { 
            fscanf(fp, ",%d", &(cross[i].next[j]));
	    
        }

    } 
    fclose(fp);
    printf("%d\n",crossing_number);
    return crossing_number;
}







/*******************************************************


void search_shortest_path(int startid, int endid)
{
	int a,i,j;
	int id; //id of the next connected point
	a=startid;
	printf("your start id is %d endid is %d\n",startid, endid);
	for(i=0;a!=endid;i++)
	{
		
		for(j=0;j<cross[i].points;j++)
		{
			id=cross[a].next[j];
			connected[j].distance=hypot(cross[a].pos.x-cross[id].pos.x,cross[a].pos.y-cross[id].pos.y);
			connected[j].totaldis=connected[j].distance+cross[id].distance;
		}
		
		for(j=0;j<cross[a].points-1;j++)
		{
			if(connected[j].totaldis<connected[j+1].totaldis)
			step[i+1].id=cross[a].next[j];
			else
			step[i+1].id=cross[a].next[j+1];
		}
		
		if (cross[a].points==1)
		{
		step[i+1].id=cross[a].next[0];
	    }
	    a=step[i+1].id;	
	    printf("path %d is %d\n",i+1,a);
		
	}
	
      
    
}

  
**************************************************************/


      
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
					
  
































void match_input_data(char *start,char *end, int a)
{
  int i,fs,fe,crossing_number,startid,endid;
  crossing_number=a;
  fs=0;
  fe=0;



  if(strcmp(start,end)==0)
    puts("you are on your destination");
  
  for(i=0;i<crossing_number;i++)
    {
      if(strcmp(cross[i].ename,start)==0)
	{
	  startid=cross[i].id;
	  fs=1;
        }
    }
  for(i=0;i<crossing_number;i++)
    {
      if(strcmp(cross[i].ename,end)==0)
	{
	  endid=cross[i].id;
	  fe=1;
	}
    }
  if(fs==0 || fe==0)
    puts("your input is not been found");
  else
 //   printf("\nyour journey from %s to %s\n",start,end);
  jiegouti1[1].sid=startid;
  jiegouti1[1].eid=endid;
    
}






void distance_calculation(int startid,int endid, int crossingnumber)
{
  int i;
  double x,y;
  
  for(i=0;i<crossingnumber;i++)
    {
      x=abs(cross[startid].pos.x-cross[i].pos.x);
      y=abs(cross[endid].pos.y-cross[i].pos.y);
      cross[i].distance=hypot(x,y);
    }
}
      







int main() 
{
    FILE *fp; 
    int i,j,k,sid,eid;
    int crossing_number;
    char startname[MaxName]; 
    char endname[MaxName];
      
    
    crossing_number=read_map("map2.dat","r");






    printf("please input your data, for example if your journey from A to B, please input as A to B\n\n");
    scanf("%s to %s",startname,endname);
    
    match_input_data(startname,endname,crossing_number);
    distance_calculation(jiegouti1[1].sid,jiegouti1[1].eid,crossing_number);
    sid=jiegouti1[1].sid;
    eid=jiegouti1[1].eid;
    search_shortest_path(sid,eid);












    



    /*******************************************************************test section below ******************************************************************************    

    printf("crossngnumber=%d\n",crossing_number);
    for (i = 0; i < crossing_number; i++)
    {
    printf("%d\n",i);
    printf("%d,%lf,%lf,%lf,%s,%s,%d,%lf\n",(cross[i].id), (cross[i].pos.x), (cross[i].pos.y), (cross[i].wait),cross[i].jname,cross[i].ename, (cross[i].points),cross[i].distance);
    for(j=0; j < cross[i].points; j++)
    {
    printf("%d\n",j);
    printf("%d\n",cross[i].next[j]);
    }
    }
    
        for(i=0;i<10;i++)
      {
	printf("%d\n",jiegouti1[i].path);
      }
    
    
    
    
    **************************************************************************************  test section above  **********************************************************************************/




    
    return 0;
    
}

