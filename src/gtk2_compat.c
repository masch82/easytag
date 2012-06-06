#include "gtk2_compat.h"
#if !GTK_CHECK_VERSION(3,0,0)

GtkWidget *gtk_box_new(GtkOrientation orientation, gint padding)
{
	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		return gtk_hbox_new(FALSE, padding);
	return gtk_vbox_new(FALSE, padding);
}

GtkWidget *gtk_paned_new(GtkOrientation orientation)
{
	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		return gtk_hpaned_new();
	return gtk_vpaned_new();
}

GtkWidget *gtk_separator_new(GtkOrientation orientation)
{
	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		return gtk_hseparator_new();
	return gtk_vseparator_new();
}

GtkWidget *gtk_button_box_new(GtkOrientation orientation)
{
	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		return gtk_hbutton_box_new();
	return gtk_vbutton_box_new();
}

#endif

#if !GTK_CHECK_VERSION(2,18,0)

void gtk_widget_set_can_default(GtkWidget *widget, gboolean can_default)
{
	if (can_default)
		GTK_WIDGET_SET_FLAGS(widget, GTK_CAN_DEFAULT);
	else
		GTK_WIDGET_UNSET_FLAGS(widget, GTK_CAN_DEFAULT);
}

void gtk_widget_set_can_focus(GtkWidget *widget, gboolean can_focus)
{
	if (can_focus)
		GTK_WIDGET_SET_FLAGS(widget, GTK_CAN_FOCUS);
	else
		GTK_WIDGET_UNSET_FLAGS(widget, GTK_CAN_FOCUS);
}

#endif
