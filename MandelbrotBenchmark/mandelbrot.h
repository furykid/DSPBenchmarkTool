#ifndef MANDELBROT_H_INCLUDED
#define MANDELBROT_H_INCLUDED

#include <fcntl.h>
#include <math.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dasc.h>
#include <omp.h>

// Image size
#define ROW 500
#define COL 500

// variables for the expose callback
double start_time, end_time, globalStart_time, globalEnd_time;
double globalElapsedSeconds, seconds, framerate, totalFPS, averageFrameRate, highestFramerate;
gchar *m_sSecondsString;
gchar *m_sZoomCountString;
gchar *m_sHighestFPSCountString;
int m_iCounter;
int i, j;
gboolean m_bShouldZoom;

// DSP variables
double* pHostRstMat;
double* pDevDstMat[4];
s32* pDevMat[4];
dascResult ret;

#define MAT_DIM 500
#define GRID_SIZE   1
#define BLOCK_SIZE  1

#define G_GUINT32_FORMAT "u"

typedef struct
{
	uint CPUpixel[ROW][COL];
	uint DSPpixel;

	uint upper_color;
	uint mid_color;
	uint lower_color;
	uint color;
	uint iteration;
	uint iteration_target;
	uint max_iterations;
	uint max;
  	uint flag;

	double x;
	double y;
	double xO;
	double yO;
	double xtemp;
	double min_x;
	double min_y;
	double max_x;
	double max_y;
	double target_x;
	double target_y;
	double zoom;
	double p;

	// for rotation
	double sine;
	double cosine;
	double theta;
	int xRot;
	int yRot;
	int Xresize;
	int Yresize;
} Mandelbrot;

typedef struct
{
	GtkWidget* FPSValueLabel;
	GtkWidget* ZoomCountLabel;
	GtkWidget* HighestFPSCountLabel;
} ControlLabels;

typedef struct
{
	// We need these 2 parameters to send for the callback
	GdkImage* oGmg;
	Mandelbrot* oMndt;
	ControlLabels* oLabels;
	GtkWindow* oParentWindow;
} CbPms;

// Initialize Mandelbrot members
void InitMandelbrot(Mandelbrot*);

// function to do the calculations
void calculate_mandelbrot();

// zoom function
double zoomIn(double);

#endif

