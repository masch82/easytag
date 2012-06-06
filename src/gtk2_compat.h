#ifndef GTK2_COMPAT_H
#define GTK2_COMPAT_H

#include <gtk/gtk.h>

#if !GTK_CHECK_VERSION(3,0,0)

#define GTK_SCROLLABLE(x) GTK_TREE_VIEW(x)
#define gtk_scrollable_get_vadjustment(x) gtk_tree_view_get_vadjustment(x)

GtkWidget *gtk_box_new(GtkOrientation orientation, gint padding);
GtkWidget *gtk_paned_new(GtkOrientation orientation);
GtkWidget *gtk_separator_new(GtkOrientation orientation);
GtkWidget *gtk_button_box_new(GtkOrientation orientation);

#define gtk_widget_get_allocated_height(x) GTK_WIDGET(x)->allocation.height
#define gtk_widget_get_allocated_width(x) GTK_WIDGET(x)->allocation.width

#if GTK_CHECK_VERSION(2,24,0)
#define gtk_combo_box_text_append(x, y, z) \
	gtk_combo_box_text_append_text(x, z)
#define gtk_combo_box_text_prepend(x, y, z) \
	gtk_combo_box_text_prepend_text(x, z)
#endif

#endif

#if !GTK_CHECK_VERSION(2,24,0)

#define GTK_COMBO_BOX_TEXT(x) GTK_COMBO_BOX(x)
#define gtk_combo_box_text_new() gtk_combo_box_new_text()
#define gtk_combo_box_text_append(x, y, z) \
	gtk_combo_box_append_text(x, z)
#define gtk_combo_box_text_prepend(x, y, z) \
	gtk_combo_box_prepend_text(x, z)
#define gtk_combo_box_text_new_with_entry() \
	gtk_combo_box_entry_new_text()
#define gtk_combo_box_new_with_model_and_entry(x) \
	gtk_combo_box_entry_new_with_model(x, MISC_COMBO_TEXT)
#define gtk_combo_box_new_with_entry() \
	gtk_combo_box_entry_new()
#define gtk_combo_box_set_entry_text_column(x, y)
#define gtk_combo_box_text_new_with_entry() gtk_combo_box_entry_new_text()

#endif

#if !GTK_CHECK_VERSION(2,22,0)
#define GDK_KEY_c GDK_c
#define GDK_KEY_C GDK_C
#define GDK_KEY_Delete GDK_Delete
#define GDK_KEY_e GDK_e
#define GDK_KEY_E GDK_E
#define GDK_KEY_Escape GDK_Escape
#define GDK_KEY_i GDK_i
#define GDK_KEY_I GDK_I
#define GDK_KEY_KP_Enter GDK_KP_Enter
#define GDK_KEY_Return GDK_Return
#define GDK_KEY_t GDK_t
#define GDK_KEY_T GDK_T
#endif

#if !GTK_CHECK_VERSION(2,18,0)

#define gtk_widget_is_toplevel(x) GTK_WIDGET_TOPLEVEL(x)
void gtk_widget_set_can_default(GtkWidget *widget, gboolean can_default);
void gtk_widget_set_can_focus(GtkWidget *widget, gboolean can_default);

#endif

#if !GTK_CHECK_VERSION(2,14,0)

#define gtk_dialog_get_content_area(x) GTK_DIALOG(x)->vbox
#define gtk_dialog_get_action_area(x) GTK_DIALOG(x)->action_area
#define gtk_adjustment_get_page_size(x) GTK_ADJUSTMENT(x)->page_size
#define gtk_adjustment_get_value(x) GTK_ADJUSTMENT(x)->value
#define gtk_adjustment_get_upper(x) GTK_ADJUSTMENT(x)->upper
#define gtk_selection_data_get_data(x) (x)->data
#define gtk_widget_get_window(x) GTK_WIDGET(x)->window

#endif


#endif
