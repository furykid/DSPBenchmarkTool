/*
 * Callbacks.h
 *
 *  Created on: Aug 24, 2012
 *      Author: mauron
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

// Callback functions
gboolean exposeCallback(GtkWidget*, GdkEventExpose*, CbPms*);
gboolean DSPexposeCallback(GtkWidget*, GdkEventExpose*, CbPms*);
void startBenchmarkCallback(void);
void stopBenchmarkCallback(void);
void switchPlatformCallback(GtkWidget*, gpointer);
void resetCallback(GtkWidget*, CbPms*);
void saveResultsCallback(void);
void testDSPConnectionCallback(CbPms*);
void PrintTimeDataToWindow(CbPms*);
void destroy(void);
void ShowErrorWindow(CbPms*);

#endif /* CALLBACKS_H_ */
