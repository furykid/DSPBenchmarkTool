/*
 * UIBuilder.h
 *
 *  Created on: Aug 24, 2012
 *      Author: mauron
 */
#include "mandelbrot.h"

#ifndef UIBUILDER_H_
#define UIBUILDER_H_

void CreateMainWindow(GtkWidget**, GtkWidget**, GtkWidget**, GtkWidget**);
void CreateMenuForMainWindow(GtkWidget*, GtkWidget**, GtkWidget*, GtkWidget**, GtkWidget*, GtkWidget*);
void CreateMainTableForMainWindow(GtkWidget**, GtkWidget**);
void AttachControlButtons(GtkWidget**, GtkWidget**, GtkWidget**, GtkWidget**);
void AttachLabelsToWindow(GtkWidget**, GtkWidget*, GtkWidget*, GtkWidget**, GtkWidget*, GtkWidget**, GtkWidget*, GtkWidget**, GtkWidget*, GtkWidget**);
void AttachMainButtons(GtkWidget**, GtkWidget**, GtkWidget**);

#endif /* UIBUILDER_H_ */
