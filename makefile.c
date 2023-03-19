# # Sample of Makefile 
# Anything after '#' will be ignored as a comment # 
CC=cc #Specify a compiler, such as cc or gcc 
CFLAGS=-g -O2 -Wall #Specify include paths, compiler flags 
LDFLAGS= #Specify library paths 
LDLIBS=-lftgl -lglfw -lGLU -lGL -lX11 -lXrandr -lm 
#Specify libraries to be linked test2: test2.c #test2.c is required to create test2 $(CC) 
$@.c $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ #Substitute MACROS and compiles program. $@ means target name 
# Use TAB for this blank space, otherwise it won't work ~$ make cc test2.c -std=c99 -g -O2 -Wall -lftgl -lglfw -lGLU -lGL -lX11 lXrandr –lm
