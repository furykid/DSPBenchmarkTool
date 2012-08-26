// Mandelbrot set using gtk
#include "mandelbrot.h"
#include "Callbacks.h"
#include "Enums.h"

int main(int argc, char** argv){

	// initialize counter and highest Framerate
	m_iCounter = 0;
	highestFramerate = 0;
	totalFPS = 0;

	// initialize the states of the enums
	MandelbrotState = Stopped;
	FirstPass = true;
	Platform = CPU;
	CreateLogFile = no;

	// DSP
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// create mandelbrot struct
	Mandelbrot* m_oMandelbrot = malloc(sizeof *m_oMandelbrot);
	//init the DSP
	DSPCanRun = init_DSP(pDevMat, m_oMandelbrot);

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// Initialize members
	InitMandelbrot(m_oMandelbrot);

	// Window variables
	//************************************************************************
	gtk_init (&argc, &argv);
	// Menu
	GtkWidget* m_oMenuBar = gtk_menu_bar_new();
	GtkWidget* m_oFileMenu = gtk_menu_new();
	GtkWidget* m_oFileItem = gtk_menu_item_new_with_label("File");
	GtkWidget* m_oDSPMenu = gtk_menu_new();
	GtkWidget* m_oDSPItem = gtk_menu_item_new_with_label("DSP");
	GtkWidget* m_oTestDSPConnection_item = gtk_menu_item_new_with_label("Test DSP connection");
	GtkWidget* m_oSaveResults_item = gtk_menu_item_new_with_label("Save results to file");
	//Image
	GdkVisual* m_oGdkvisual = gdk_visual_get_system();
	GdkImage* m_oGdkimage = gdk_image_new(GDK_IMAGE_NORMAL, m_oGdkvisual, ROW, COL);
	GtkWidget* m_oWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* m_oImage  = gtk_image_new_from_image( m_oGdkimage, NULL );
	//Buttons
	GtkWidget* m_oStartButton = gtk_button_new_with_label("Start Animation");
	GtkWidget* m_oStopButton = gtk_button_new_with_label("Stop Animation");
	GtkWidget* m_oCloseButton = gtk_button_new_with_label("Close");
	GtkWidget* m_oResetButton = gtk_button_new_with_label("Reset");
	GtkWidget* m_oTogglePlatformButton = gtk_button_new_with_label("Toggle Platform");
	// Labels
	GtkWidget* m_oTitleLabel = gtk_label_new("Mandelbrot Set Benchmarking Tool");
	GtkWidget* m_oPlatformLabel = gtk_label_new("CPU");
	GtkWidget* m_oFPSLabel = gtk_label_new("FPS");
	GtkWidget* m_oFPSValueLabel = gtk_label_new("");
	GtkWidget* m_oZoomIterationsLabel = gtk_label_new("Zoom iterations");
	GtkWidget* m_oZoomCountLabel = gtk_label_new("");
	GtkWidget* m_oHighestFPSCount = gtk_label_new("Highest Frame Rate");
	GtkWidget* m_oHighestFPSValue = gtk_label_new("");
	//************************************************************************

	// Main Window
	CreateMainWindow(m_oWindow, m_oMenuBar, m_oFileItem, m_oDSPItem);
	// Set up menu for the window
	CreateMenuForMainWindow(m_oDSPMenu, m_oTestDSPConnection_item, m_oFileMenu, m_oSaveResults_item, m_oFileItem, m_oDSPItem);
	// Main Table
	GtkWidget* m_oTable = gtk_table_new(4, 3, FALSE);
	GtkWidget* m_oControlsTable = gtk_table_new(7, 2, FALSE);
	CreateMainTableForMainWindow(m_oTable, m_oControlsTable);

	// Add Main table to the window
	gtk_container_add(GTK_CONTAINER(m_oWindow), m_oTable);
	// Add controls table to the main table
	gtk_table_attach_defaults(GTK_TABLE(m_oTable), m_oControlsTable, 1, 3, 2, 3);
	// Add menu bar
	gtk_table_attach_defaults(GTK_TABLE(m_oTable), m_oMenuBar, 0, 4, 0, 1);
	// Image
	gtk_table_attach_defaults(GTK_TABLE(m_oTable), m_oImage, 0, 1, 2, 3);

	// Control Buttons
	AttachControlButtons(m_oControlsTable, m_oStartButton, m_oStopButton, m_oTogglePlatformButton);
	// Labels
	AttachLabelsToWindow(m_oTable, m_oTitleLabel, m_oControlsTable, m_oPlatformLabel,
			m_oFPSLabel, m_oFPSValueLabel, m_oHighestFPSCount, m_oHighestFPSValue, m_oZoomIterationsLabel, m_oZoomCountLabel);
	// Main Buttons
	AttachMainButtons(m_oResetButton, m_oCloseButton, m_oTable);

	// build the callback parameter struct to pass to the callback
	CbPms* m_oCallbackParams = malloc(sizeof *m_oCallbackParams);
	ControlLabels* m_oControlLabels = malloc(sizeof *m_oControlLabels);
	m_oControlLabels -> FPSValueLabel = m_oFPSValueLabel;
	m_oControlLabels -> ZoomCountLabel = m_oZoomCountLabel;
	m_oControlLabels -> HighestFPSCountLabel = m_oHighestFPSValue;
	m_oCallbackParams -> oMndt = m_oMandelbrot;
	m_oCallbackParams -> oGmg = m_oGdkimage;
	m_oCallbackParams -> oLabels = m_oControlLabels;

	gtk_widget_show_all(m_oWindow);

	// POSSIBLY REFACTOR WHEN CLOSE TO FINISHED WITH THIS TO CLEAN UP MAIN
	// Expose event for mandelbrot set
	g_signal_connect(GTK_OBJECT (m_oImage), "expose-event", G_CALLBACK(exposeCallback), m_oCallbackParams);
	// buttons
	g_signal_connect(GTK_OBJECT (m_oWindow), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);
	g_signal_connect(GTK_OBJECT (m_oStartButton), "clicked", G_CALLBACK(startBenchmarkCallback), NULL);
	g_signal_connect(GTK_OBJECT (m_oStopButton), "clicked", G_CALLBACK(stopBenchmarkCallback), NULL);
	g_signal_connect(GTK_OBJECT (m_oCloseButton), "clicked", G_CALLBACK(destroy), NULL);
	g_signal_connect(GTK_OBJECT (m_oTogglePlatformButton), "clicked", G_CALLBACK(switchPlatformCallback), m_oPlatformLabel);
	g_signal_connect(GTK_OBJECT (m_oResetButton), "clicked", G_CALLBACK(resetCallback), m_oCallbackParams);
	// Menu items
	g_signal_connect(GTK_OBJECT (m_oSaveResults_item), "activate", G_CALLBACK(saveResultsCallback), NULL);
	g_signal_connect(GTK_OBJECT (m_oTestDSPConnection_item), "activate", G_CALLBACK(testDSPConnectionCallback), m_oCallbackParams);

	gtk_main();

	return(0);
}





