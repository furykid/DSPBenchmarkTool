#include "mandelbrot.h"
#include "Callbacks.h"
#include "Enums.h"

gboolean exposeCallback(GtkWidget* w, GdkEventExpose* e, CbPms* data) {

	if(Platform == CPU) {

		start_time = 0;
		end_time = 0;
		start_time = clock();

		// Send out the data for processing
		calculate_mandelbrot(data->oMndt);

		// paint the pixels to the gdk buffer
		for(i = 0;i < ROW;i++) {
			for(j = 0;j < COL;j++) {
				gdk_image_put_pixel(data->oGmg, j, i, data->oMndt->CPUpixel[i][j]);
			}
		}

		end_time = clock();
		PrintTimeDataToWindow(data);
	}

	// Send the data out to the DSP
	else if(DSPCanRun == True) {

		start_time = 0;
		end_time = 0;
		start_time = clock();

		//lauch the kernel
		ret = dascLaunch();
		if (ret != DASC_SUCCESS) {
			ShowErrorWindow(CbPms *data);
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
		end_time = clock();
		PrintTimeDataToWindow(data);
	}

	gdk_window_invalidate_rect(gtk_widget_get_window(w),(const GdkRectangle *)NULL,FALSE);

	if(MandelbrotState == Running) {
		m_iCounter++;
	}

	return FALSE;
}

void destroy(void) {
	gtk_main_quit();
}

void startBenchmarkCallback() {
	MandelbrotState = Running;
	highestFramerate = 0;
	globalElapsedSeconds = 0;
	globalStart_time = clock();
}

void stopBenchmarkCallback() {
	MandelbrotState = Stopped;

	globalEnd_time = clock();
	globalElapsedSeconds = (globalEnd_time - globalStart_time) / CLOCKS_PER_SEC;
	averageFrameRate = totalFPS / m_iCounter;

	char *oPlatform;
	if(Platform == CPU) {
		oPlatform = "CPU";
	}
	else {
		oPlatform = "DSP";
	}

	if(CreateLogFile == yes) {
		time_t now;
		time(&now);

		FILE *oLogFile = fopen("Mandelbrot_Benchmark_Log.txt", "a");
		fprintf(oLogFile, "\nBenchmark Results @ %.24s.\n------------------------------------------------\n", ctime(&now));
		fprintf(oLogFile, "-> Platform used: %.4s\n", oPlatform);
		fprintf(oLogFile, "-> Elapsed time of test: %0.2f seconds", globalElapsedSeconds);
		fprintf(oLogFile, "\n-> Highest Framerate: %0.2f Frames per second\n-> Average Framerate: %0.2f Frames per second", highestFramerate, averageFrameRate);
		fprintf(oLogFile, "\n-> Zoom Iterations: %d time(s)\n\n\n\n", m_iCounter);
		fclose(oLogFile);

		// prevent multiple writes
		CreateLogFile = no;
	}
}

void switchPlatformCallback(GtkWidget *widget, gpointer oLabel) {
	if(Platform == CPU) {
		Platform = DSP;
		if(DSPCanRun == False) {
			gtk_label_set_text(oLabel, "Not Available");
		}
		else {
			gtk_label_set_text(oLabel, "DSP");
		}
	}
	else {
		Platform = CPU;
		gtk_label_set_text(oLabel, "CPU");
	}
}

void resetCallback(GtkWidget *oWidget, CbPms *oData) {
	MandelbrotState = Stopped;
	FirstPass = true;
	m_bShouldZoom = true;
	highestFramerate = 0;
	globalElapsedSeconds = 0;
	seconds = 0;
	m_iCounter = 0;
	averageFrameRate = 0;
	highestFramerate = 0;
	framerate = 0;
	totalFPS = 0;
	InitMandelbrot(oData->oMndt);

	m_sZoomCountString = g_strdup_printf ("%u", m_iCounter);
	gtk_label_set_text(oData->oLabels->ZoomCountLabel, m_sZoomCountString);
	g_free (m_sZoomCountString);

	m_sSecondsString = g_strdup_printf ("%0.4f", framerate);
	gtk_label_set_text(oData->oLabels->FPSValueLabel, m_sSecondsString);
	g_free (m_sSecondsString);

	m_sHighestFPSCountString = g_strdup_printf ("%0.4f", highestFramerate);
	gtk_label_set_text(oData->oLabels->HighestFPSCountLabel, m_sHighestFPSCountString);
	g_free (m_sHighestFPSCountString);
}

void testDSPConnectionCallback(CbPms *oData) {
	// Test the system
	DSPCanRun = init_DSP(pDevMat, oData->oMndt);

	if(DSPCanRun == False) {

		GtkWidget *oDialog = gtk_message_dialog_new (NULL,
				GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "The DSP is not ready.");

		g_signal_connect_swapped (oDialog, "response",
				G_CALLBACK (gtk_widget_destroy), oDialog);

		gtk_widget_show_all(oDialog);
	}
	else {
		GtkWidget *oDialog = gtk_message_dialog_new (NULL,
				GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "The DSP running and ready.");

		g_signal_connect_swapped (oDialog, "response",
				G_CALLBACK (gtk_widget_destroy), oDialog);

		gtk_widget_show_all(oDialog);
	}
}

void saveResultsCallback() {

	CreateLogFile = yes;

	GtkWidget *oDialog = gtk_message_dialog_new (NULL,
			GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "When the benchmark is stopped, a log file will be created.");

	g_signal_connect_swapped (oDialog, "response",
			G_CALLBACK (gtk_widget_destroy), oDialog);

	gtk_widget_show_all(oDialog);
}

void PrintTimeDataToWindow(CbPms* data) {

	if(MandelbrotState == Running) {

		seconds = (end_time - start_time) / CLOCKS_PER_SEC;
		framerate = 1 / seconds;
		totalFPS += framerate;
		if(framerate > highestFramerate) {
			highestFramerate = framerate;
		}

		m_sZoomCountString = g_strdup_printf ("%u", m_iCounter);
		gtk_label_set_text(data->oLabels->ZoomCountLabel, m_sZoomCountString);
		g_free (m_sZoomCountString);

		m_sSecondsString = g_strdup_printf ("%0.4f", framerate);
		gtk_label_set_text(data->oLabels->FPSValueLabel, m_sSecondsString);
		g_free (m_sSecondsString);

		m_sHighestFPSCountString = g_strdup_printf ("%0.4f", highestFramerate);
		gtk_label_set_text(data->oLabels->HighestFPSCountLabel, m_sHighestFPSCountString);
		g_free (m_sHighestFPSCountString);
	}
	else {
		gtk_label_set_text(data->oLabels->FPSValueLabel, "0");
	}
}

void ShowErrorWindow(CbPms *oData) {
	GtkWidget *oDialog = gtk_message_dialog_new (NULL,
			GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "There was an error communicating with the DSP.");

	g_signal_connect_swapped (oDialog, "response",
			G_CALLBACK (gtk_widget_destroy), oDialog);

	gtk_widget_show_all(oDialog);
}




