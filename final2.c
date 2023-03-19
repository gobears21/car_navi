/* mobile_s.c --- Display the moving object for which search was used */
/* compile: cc final2.c -g -O2 -Wall -I/usr/include/freetype2 -lftgl -lglfw -lGLU -lGL -lX11 -lXrandr -lm -o final2 */

#include <stdio.h> 
#include <math.h>
#include <unistd.h> 
#include <GL/glfw.h>
#include <FTGL/ftgl.h>

#define CrossingNumber 100 /* The number of crossings = 100 */
#define MaxName 50 /* The maximum number of characters = 50 (half-width) */
#define Radius_Marker 0.2 /* The radius of a marker */

/* Definition of coordinate conversion macro */
#define ORIGIN_X -2.0 
#define ORIGIN_Y 3.0
#define REAL_SIZE_X 15.0 
#define REAL_SIZE_Y 15.0

/* Definition of data structure */
typedef struct {
    double x, y; /* Position x, y */
} Position; /* A structure that represents positions */

typedef struct {
    int id; /* Crossing number */
    Position pos; /* A structure that represents positions */
    double wait; /* Average waiting time */
    char jname[MaxName]; /* Crossing name (Japanese) */
    char ename[MaxName]; /* Crossing name (Alphabetical name) */ 
    int points; /* The number of crossing roads */
    int next[5]; /* Crossing numbers of neighboring crossings */ 
} Crossing;

/* Definition of variables in which data will be stored */
Crossing cross[CrossingNumber];

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
        /* Calculate the distance between the i-th neighboring crossing of the 
           crossing id and the crossing goal, then assign the value to distance. 
           Use hypot(x, y) to calculate the distance.
           If the calculated distance is shorter than the previously set shortest 
           distance ( min_distance )...

           Complete the program. */

        distance = hypot((cross[ cross[id].next[i] ].pos.x - cross[goal].pos.x), (cross[cross[id].next[i]].pos.y - cross[goal].pos.y));

        if(distance <= min_distance)
	  {
            min_distance = distance;
            nearest = cross[id].next[i];
        }
    }
    return nearest;
}

/* Read the file */
int map_read(char *filename) {
    FILE *fp; 
    int i, j;
    int crossing_number; /* The number of crossings */

    fp = fopen(filename, "r"); 
    if (fp == NULL) {
        perror(filename); 
        return -1;
    }

    /* First, read the number of crossings */
    fscanf(fp, "%d", &crossing_number);

    for (i = 0; i < crossing_number; i++) {
        /* Write a program that uses the function fprintf to output the data of the structure to the file */
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


    /* Initialize the graphic environment and open a window */
    glfwInit();
    glfwOpenWindow(1500,1200, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

    /* Based on (ORIGIN_X, ORIGIN_Y), project the space within the range of REAL_SIZE_X * REAL_SIZE_Y to the viewport */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ORIGIN_X + REAL_SIZE_X * -0.5, ORIGIN_X + REAL_SIZE_X * 0.5,
            ORIGIN_Y + REAL_SIZE_Y * -0.5, ORIGIN_Y + REAL_SIZE_Y * 0.5, 
            -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); /* Do not perform other coordinate conversion */

    /* Read and configure the font used for drawing strings */
    font = ftglCreateExtrudeFont(FONT_FILENAME); 
    if (font == NULL) {
        perror(FONT_FILENAME);
        fprintf(stderr, "could not load font\n");
        exit (1); 
    }

    ftglSetFontFaceSize(font, 24, 24);
    ftglSetFontDepth(font, 0.01); 
    ftglSetFontOutset(font, 0, 0.1);
    ftglSetFontCharMap(font, ft_encoding_unicode);

    /* Read the map file */
    crossing_number = map_read("map2.dat");
    if (crossing_number < 0) {
        fprintf(stderr, "couldn't read map file\n");
        exit (1); 
    }





    
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

        /* Move the moving object forward and calculate the coordinate */
        if (vehicle_edgeFrom != vehicle_goal) {
            /* Since the goal has not been reached, move forward the position of the moving object */
            /* Create based on the previous exercise.
               The number of variables and their names have been changed, but the basic flow is the
               same.
               Complete the program.
               */

            vehicle_edgeTo = search_nearest(vehicle_edgeFrom, vehicle_goal);

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
