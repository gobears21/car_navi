
#include <stdio.h>
#include <string.h>
#include <math.h>
#define CrossingNumber 100
#define MaxName 50


 
typedef struct 
{
    double x, y;          
} Position;  
                     
typedef struct {
    int id;               
    Position pos;        
    double wait;          
    char jname[MaxName];             
    char ename[MaxName];             
    int points;                      
    int next[5];                    
    double distance;                 
} Crossing;


Crossing cross[CrossingNumber];


 int map_read(char *filename)
{
    FILE *fp;
    int i, j;
    int crossing_number;                  
    fp = fopen(filename, "r");
    if (fp == NULL) 
	{
        perror(filename);
        return -1;
    }
    fscanf(fp, "%d", &crossing_number);
    for (i = 0; i < crossing_number; i++) 
	{
        fscanf(fp, "%d,%lf,%lf,%lf,%[^,],%[^,],%d",&(cross[i].id), &(cross[i].pos.x), &(cross[i].pos.y),&(cross[i].wait), cross[i].jname,cross[i].ename, &(cross[i].points));
        for(j=0; j < cross[i].points; j++)		
		{
            fscanf(fp, ",%d", &(cross[i].next[j]));
        }
    }
    fclose(fp);
    return crossing_number;
}





void print_cross(int i)
{
           int j;
           printf("Crossing Number:%2d, Coordinates(%5.2lf,%5.2lf), Name: %s ( %s ),", cross[i].id, cross[i].pos.x, cross[i].pos.y, cross[i].jname,cross[i].ename);
           printf("\n         Distance to the destination %5.1lf Waiting time:%5.1lf, The number of neighboring crossing:%d ( ", cross[i].distance,cross[i].wait, cross[i].points); 
           for(j=0; j < cross[i].points; j++)
		   {
		   	    printf("%d ", cross[i].next[j]);
		    }             
           printf(")\n\n");
}



/*
void print_cross_list(int num) 
{
   int i;
   for(i=0;i < num; i++)
   {
   		print_cross(i); 
   }
            
}

*/


int search_cross(int num) 
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
            //print_cross(i);     
            f=i;                
        }
    }
    
    if(f<0)
    	printf("%s has not been found \n", input);
    return f; 
}



double distance(int a, int b)
{
    return hypot(cross[a].pos.x-cross[b].pos.x, 
    cross[a].pos.y-cross[b].pos.y); 
}




void dijkstra(int crossing_number,int target)
{
    int i,j,n;
    double d;
    double min_distance;   
    int min_cross = 0;
    int done[CrossingNumber];                   
    for(i=0;i < crossing_number;i++)            
    {
        cross[i].distance=1e100;         
        done[i]=0;                       
    }
          
    cross[target].distance=0;
    for(i=0;i < crossing_number;i++) /* This should end after repeating crossing_number times */
    {
        min_distance=1e100;
        for(j=0;j < crossing_number;j++)
        {
	  		if((done[j]==0)&&(cross[j].distance < min_distance))
	    	{
	      		min_distance=cross[j].distance;
	      		min_cross=j;
	    	}
        }                   
        done[min_cross]=1; 
        for(j=0;j < cross[min_cross].points;j++)
        {
            n=cross[min_cross].next[j];                                          
            d=distance(min_cross,n)+cross[min_cross].distance;                                
            if(cross[n].distance > d)
                cross[n].distance = d;
        }
    }
}







int pickup_path(int crossing_number,int start,int goal,int path[],int maxpath)
{
    int c=start;            
    int i,j,n;
    double min_distance;
    int min_cross = 0;
    path[0]=start;
    i=1;                                   
    c=start;
    while(c!=goal)
    {
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


        c=min_cross;                  
        if((i > 1)&&(c==path[i-2])) 
        {
            printf("I am stuck(%d-%d). I give up this path search(;_;)\n",c,path[i-1]);
            return -1;
        }
        path[i]=c;
        i++;
        if(i==maxpath-1) 
        {
            printf("The path has become too long. I give up this path search\n");
            return -2;
        }
    }
  path[i]=-1;     
  return 0;
}













int main(void)
{
    int crossing_number; 
    int goal,start;
    int path[20];
    int i;
    crossing_number = map_read("map2.dat");
    printf("loaded %d crossings\n",crossing_number);
    for(i=0;i<crossing_number;i++)
    cross[i].distance=0;                   
    printf("Determine the destination point.") ;           
    goal=search_cross(crossing_number);          
    if(goal<0)
    	return 1;  		       
    dijkstra(crossing_number,goal);
            
    print_cross_list(crossing_number);
    printf("Determine the current position.") ;
            
            
    start=search_cross(crossing_number);
            
    if(start<0)
    {
        return 1;
    }
    if(pickup_path(crossing_number,start,goal,path,20)<0)
    {
        return 1;
    }
    printf("Path has been confirmed\n");
    i=0;
            
    while(path[i]>=0)
    {
        printf("%2d %5.1lf %s\n",i+1,cross[path[i]].distance,cross[path[i]].ename);
        i++;
    }
  return 0;
}
