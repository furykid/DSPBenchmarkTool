// Library for building the user interface
#include "UIBuilder.h"

void CreateMainWindow(GtkWidget **m_oWindow, GtkWidget **m_oMenuBar, GtkWidget **m_oFileItem, GtkWidget **m_oDSPItem)
{
	// Main Window
	gtk_window_set_title(GTK_WINDOW(m_oWindow), "Benchmark Suite");
	gtk_window_set_default_size(GTK_WINDOW(m_oWindow), 290, 200);
	gtk_window_set_position(GTK_WINDOW(m_oWindow), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(m_oWindow), 5);
	gtk_menu_bar_append(GTK_MENU_BAR(m_oMenuBar), m_oFileItem);
	gtk_menu_bar_append(GTK_MENU_BAR(m_oMenuBar), m_oDSPItem);
}

void CreateMenuForMainWindow(GtkWidget *m_oDSPMenu, GtkWidget **m_oTestDSPConnection_item,
		GtkWidget *m_oFileMenu, GtkWidget **m_oSaveResults_item, GtkWidget *m_oFileItem, GtkWidget *m_oDSPItem)
{
	// Set up menu for the window
	gtk_menu_append(GTK_MENU (m_oDSPMenu), m_oTestDSPConnection_item);
	gtk_menu_append(GTK_MENU (m_oFileMenu), m_oSaveResults_item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(m_oFileItem), m_oFileMenu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(m_oDSPItem), m_oDSPMenu);
}

void CreateMainTableForMainWindow(GtkWidget **m_oTable, GtkWidget **m_oControlsTable)
{
	gtk_table_set_row_spacings(GTK_TABLE(m_oTable), 5);
	gtk_table_set_col_spacings(GTK_TABLE(m_oTable), 5);
	gtk_table_set_row_spacings(GTK_TABLE(m_oControlsTable), 1);
	gtk_table_set_col_spacings(GTK_TABLE(m_oControlsTable), 1);
}

void AttachControlButtons(GtkWidget **m_oControlsTable, GtkWidget **m_oStartButton,
		GtkWidget **m_oStopButton, GtkWidget **m_oTogglePlatformButton)
{
	// Control Buttons
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oStartButton, 0, 1, 0, 1, FALSE, FALSE, 25, 5);
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oStopButton, 1, 2, 0, 1, FALSE, FALSE, 5, 5);
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oTogglePlatformButton, 0, 1, 6, 7, FALSE, FALSE, 5, 25);
}

void AttachLabelsToWindow(GtkWidget **m_oTable, GtkWidget *m_oTitleLabel,
		GtkWidget *m_oControlsTable, GtkWidget **m_oPlatformLabel,
		GtkWidget *m_oFPSLabel, GtkWidget **m_oFPSValueLabel,
		GtkWidget *m_oHighestFPSCount, GtkWidget **m_oHighestFPSValue,
		GtkWidget *m_oZoomIterationsLabel, GtkWidget **m_oZoomCountLabel)
{
	// Attach Labels
	gtk_table_attach_defaults(GTK_TABLE(m_oTable), m_oTitleLabel, 0, 1, 1, 2);
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oPlatformLabel, 1, 2, 6, 7, FALSE, FALSE, 5, 200);
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oFPSLabel, 0, 1, 2, 3, FALSE, FALSE, 0, 25);
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oFPSValueLabel, 1, 2, 2, 3, FALSE, FALSE, 0, 25);
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oHighestFPSCount, 0, 1, 3, 4, FALSE, FALSE, 0, 20);
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oHighestFPSValue, 1, 2, 3, 4, FALSE, FALSE, 0, 20);
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oZoomIterationsLabel, 0, 1, 4, 5, FALSE, FALSE, 0, 0);
	gtk_table_attach(GTK_TABLE(m_oControlsTable), m_oZoomCountLabel, 1, 2, 4, 5, FALSE, FALSE, 0, 0);
}

void AttachMainButtons(GtkWidget **m_oResetButton, GtkWidget **m_oCloseButton, GtkWidget **m_oTable)
{
	// Main Buttons
	gtk_widget_set_usize(m_oResetButton, 100, 30);
	gtk_widget_set_usize(m_oCloseButton, 100, 30);
	gtk_table_attach(GTK_TABLE(m_oTable), m_oCloseButton, 3, 4, 3, 4, FALSE, FALSE, 5, 5);
	gtk_table_attach(GTK_TABLE(m_oTable), m_oResetButton, 2, 3, 3, 4, FALSE, FALSE, 5, 5);
}
