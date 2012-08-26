/*
 * main.c
 *
 *  Created on: Jul 6, 2012
 *      Author: neutron
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include "mandelbrot.h"

#include <dasc.h>

#define MAT_DIM 500
#define GRID_SIZE   1
#define BLOCK_SIZE  1

	//*************************************
	// PUT THESE INTO THE PARAM STRUCT ASAP
	//*************************************
	double* pHostRstMat = NULL;
	dascResult ret = DASC_SUCCESS;
	double* pDevDstMat[4];
	//*************************************
	//*************************************

int main(int argc, char** argv) {
	int i, j;
	s32* pHostMat = NULL;
	s32* pDevMat[4];
	
	// create mandelbrot struct 
	DSPMandelbrot* m_oMandelbrot;

	//init a result matirx
	pHostRstMat = (s32*) malloc(GRID_SIZE * MAT_DIM * MAT_DIM * sizeof(double));
	if (pHostRstMat == NULL)
		return -1;

	memset(pHostRstMat, GRID_SIZE * MAT_DIM * MAT_DIM * sizeof(double), 0);
	
	//init dasc device
	ret = dascInitDev();
	if (ret != DASC_SUCCESS) {
		return -1;
	}

	//malloc device memory
	for (i = 0; i < GRID_SIZE; i++) {
		ret = dascMallocConfig((void**) (&pDevMat[i]),
				MAT_DIM * MAT_DIM * sizeof(double), 0, i);
		if (ret != DASC_SUCCESS) {
			return -1;
		}
	}

	//set up kernel size
	dascConfigKernel(1, GRID_SIZE, BLOCK_SIZE);
	
	//load the kernle into DSP
	ret = dascLoadKnl("./dasc_demo.hex", "./dasc_demo.ent");
	if (ret != DASC_SUCCESS) {
		return -1;
	}

	m_oMandelbrot -> pixel = (u32)(*(u32*)pDevMat[0]);
	
	// initialize counter
	m_iCounter = 0;

	// Initialize members
	m_oMandelbrot -> max = 4.0;
	m_oMandelbrot -> max_iterations = 1000;
	m_oMandelbrot -> iteration_target = 997;
	m_oMandelbrot -> min_x = -2.0;
	m_oMandelbrot -> max_x = 0.8;
	m_oMandelbrot -> min_y = -1.2;
	m_oMandelbrot -> max_y = 1.2;
	m_oMandelbrot -> xtemp = 0.0;
	m_oMandelbrot -> target_x = 0.0;
	m_oMandelbrot -> target_y = 0.0;
	m_oMandelbrot -> zoom = 0.0;
	m_oMandelbrot -> flag = 0;
	// rotation vars
	m_oMandelbrot -> theta = 0.0;
	m_oMandelbrot -> Xresize = ROW / 2;
	m_oMandelbrot -> Yresize = COL / 2;
	
	// Window variables
	//************************************************************************
	gtk_init (&argc, &argv);
	GdkVisual* m_oGdkvisual = gdk_visual_get_system();
	GdkImage* m_oGdkimage = gdk_image_new(GDK_IMAGE_NORMAL, m_oGdkvisual, ROW, COL);
	GtkWidget* m_oWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* m_oImage  = gtk_image_new_from_image( m_oGdkimage, NULL );

	// Window border
	gtk_window_set_title(GTK_CONTAINER(m_oWindow), m_oWindow);
	//************************************************************************

	// build the callback parameter struct to pass to the callback
	CbPms* m_oCallbackParams = malloc(sizeof *m_oCallbackParams);
	m_oCallbackParams -> oMndt = m_oMandelbrot;
	m_oCallbackParams -> oGmg = m_oGdkimage;

	gtk_container_add(GTK_CONTAINER (m_oWindow), m_oImage);
	gtk_widget_show_all(m_oWindow);

	// connect the callback to close the window
	g_signal_connect(GTK_OBJECT (m_oWindow), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);
	g_signal_connect(GTK_OBJECT (m_oWindow), "expose-event", G_CALLBACK(DSPexposeCallback), m_oCallbackParams);

	gtk_main();
	
	return 0;
}

// Callback for drawing to the window
gboolean DSPexposeCallback(GtkWidget* w, GdkEventExpose* e, CbPms* data) {

	if(m_iCounter == 1) {
		seconds = 5.0;
		while(seconds > 0) {
		g_print("\n\nInitial mandelbrot Rendering\n");
			g_print("Begining zoom in: %d seconds\n\n\n\n", (int)seconds);
			sleep(1);
			system("clear");
			seconds--;
		}
	}
	
	//lauch the kernel
	ret = dascLaunch();
	if (ret != DASC_SUCCESS) {
		return -1;
	}

	//copy back results from DSP
	for (i = 0; i < GRID_SIZE; i++) {
	dascMemcpy(pHostRstMat, pDevDstMat[i], MAT_DIM * MAT_DIM * sizeof(s32),
			dascMemcpyDeviceToHost);
	}
	
	// paint the pixels to the gdk buffer
	for(i = 0;i < ROW;i++) {
		for(j = 0;j < COL;j++) {
			gdk_image_put_pixel(data->oGmg, j, i, pHostRstMat[i*COL+j]);
		}
	}
	
	gdk_window_invalidate_rect(gtk_widget_get_window(w),(const GdkRectangle *)NULL,FALSE);
	m_iCounter++;

	return FALSE;
}

// destroy function for the window
void destroy(void) {
	gtk_main_quit();
}

