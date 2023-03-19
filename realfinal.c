#include <stdio.h> 
#include <math.h>
#include <unistd.h> 
#include <GL/glfw.h>
#include <FTGL/ftgl.h>

#define CrossingNumber 100 
#define MaxName 50 
#define Radius_Marker 0.2 
#define ORIGIN_X -0.5 
#define ORIGIN_Y 0
#define REAL_SIZE_X 15.0 
#define REAL_SIZE_Y 25.0

typedef struct {
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

typedef struct{
	int id;
}Road;

Crossing cross[CrossingNumber];
Road route[20];

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



int search_cross(int num) 
{
    int i;
    int f= -1;
    char input[200];
    printf(" in English (alphabetical) ->");
    scanf("%s",input);
    puts("");
    for(i=0;i<num;i++)
    {
        if(strcmp(cross[i].ename,input)==0)
        {                     
        	f=i;                
        }
    }
if(f<0)
	printf("%s has not been found \n", input);
return f; 
}



double distance(int a, int b)
{
    return hypot(cross[a].pos.x-cross[b].pos.x,cross[a].pos.y-cross[b].pos.y); 
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
    for(i=0;i < crossing_number;i++) 
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
        	printf("I am stuck(%d-%d). program terminated\n",c,path[i-1]);
        	return -1;
    	}
    	path[i]=c;
    	i++;
    	if(i==maxpath-1) 
    	{
        	printf("The path has become too long, terminated\n");
        	return -2;
    	}
    }
  	path[i]=-1;     
  	return 0;
}



void circle(double x, double y, double r)
{
    int const N = 12; 
    int i;

    glBegin(GL_LINE_LOOP); 
    for (i = 0; i < N; i++)
        glVertex2d(x + cos(2 * M_PI * i / N) * r, y + sin(2 * M_PI * i / N) * r); 
    glEnd();
}

#ifndef FONT_FILENAME
#define FONT_FILENAME "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf" 
#endif
FTGLfont *font; 


void outtextxy(double x, double y, char const *text) 
{
	double const scale = 0.01; 
    glPushMatrix();
    glTranslated(x, y, 0.0); 
    glScaled(scale, scale, scale);
    ftglRenderFont(font, text, FTGL_RENDER_ALL); 
    glPopMatrix();
}



int search_nearest(int id, int goal) {
    int i;
    int nearest = 0; 
    double min_distance = 1e100; 
    double distance;
    for (i = 0; i < cross[id].points; i++)
    {
        distance = hypot((cross[ cross[id].next[i] ].pos.x - cross[goal].pos.x), (cross[cross[id].next[i]].pos.y - cross[goal].pos.y));
        if(distance <= min_distance)
		{
        	min_distance = distance;
            nearest = cross[id].next[i];
        }
    }
    return nearest;
}



void map_show(int crossing_number) 
{
    int i, j;
    for (i = 0; i < crossing_number; i++) 
	{ 
        double x0 = cross[i].pos.x;
        double y0 = cross[i].pos.y;
        glColor3d(1.0, 0.5, 0.5);
        circle(x0, y0, 0.1);
        glColor3d(1.0, 1.0, 0.0);
        outtextxy(x0, y0, cross[i].ename);
        glColor3d(1.0, 1.0, 1.0); 
        glBegin(GL_LINES);
        for(j = 0; j < cross[i].points; j++) 
		{
            double x1 = cross[ cross[i].next[j] ].pos.x;
            double y1 = cross[ cross[i].next[j] ].pos.y; 
            glVertex2d(x0, y0);
            glVertex2d(x1, y1); 
        }
        glEnd(); 
    }
}



int main(void) {
    int crossing_number; 
    int vehicle_goal; 
    int vehicle_edgeFrom; 
    int vehicle_edgeTo; 
    int vehicle_stepOnEdge;
    double vehicle_x = 0.0, vehicle_y = 0.0;
	int goal,start;
    int path[20];
    int i;
	
	
	crossing_number = map_read("map2.dat");
    printf("loaded %d crossings\n",crossing_number);
    for(i=0;i<crossing_number;i++)
	{
		cross[i].distance=0;
	}
    printf("Please input your destination") ;
    goal=search_cross(crossing_number);
    if(goal<0)
    {
    	return 1; 
	}				       
    dijkstra(crossing_number,goal);
    printf("Please input your start place");
    start=search_cross(crossing_number);
    if(start<0)
    {
    	return 1;	
	}                  
    if(pickup_path(crossing_number,start,goal,path,20)<0)
    {
        return 1;	
	}
    i=0;
    while(path[i]>=0)
    {
        route[i+1].id=path[i]; 
        i++;
    }
    vehicle_goal=goal;
    glfwInit();
    glfwOpenWindow(1000, 800, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ORIGIN_X + REAL_SIZE_X * -0.5, ORIGIN_X + REAL_SIZE_X * 0.5,ORIGIN_Y + REAL_SIZE_Y * -0.5, ORIGIN_Y + REAL_SIZE_Y * 0.5, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 
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
    
#if 1
    vehicle_edgeFrom = 5; 
    vehicle_goal = 19; 
#else
    vehicle_edgeFrom = 5; 
    vehicle_goal = 71; 
#endif
    vehicle_edgeTo = vehicle_edgeFrom;
    vehicle_stepOnEdge = 0;


    for (;;) {
        int width, height;
        if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)) 
            break;
        glfwGetWindowSize(&width, &height);
        glViewport(0, 0, width, height); 
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT); 
        map_show(crossing_number); 
 		vehicle_edgeFrom=route[i].id;
        if (vehicle_edgeFrom != vehicle_goal) 
		{
            vehicle_edgeTo = route[i+1].id;
            double x0 = cross[vehicle_edgeFrom].pos.x; 
            double y0 = cross[vehicle_edgeFrom].pos.y; 
            double x1 = cross[vehicle_edgeTo].pos.x; 
            double y1 = cross[vehicle_edgeTo].pos.y; 
            double distance = hypot(x1 - x0, y1 - y0); 
            int steps = (int)(distance / 0.1);
            vehicle_stepOnEdge++;
            vehicle_x = x0 + (x1 - x0) / steps * vehicle_stepOnEdge; 
            vehicle_y = y0 + (y1 - y0) / steps * vehicle_stepOnEdge;
            if (vehicle_stepOnEdge >= steps) 
			{ 
                vehicle_edgeFrom = vehicle_edgeTo; 
                vehicle_stepOnEdge = 0; 
            }

        }
        glColor3d(1.0, 1.0, 1.0);
        circle(cross[vehicle_goal].pos.x, cross[vehicle_goal].pos.y, Radius_Marker);
        circle(vehicle_x, vehicle_y, Radius_Marker);
        glfwSwapBuffers(); 
        usleep(50 * 1000); 
    }
    glfwTerminate();
    return 0;
}

