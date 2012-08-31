/* bar.c - 2000/05/05 */
/*
 *  EasyTAG - Tag editor for MP3 and Ogg Vorbis files
 *  Copyright (C) 2000-2003  Jerome Couderc <easytag@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <config.h>

#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>

#include "bar.h"
#include "easytag.h"
#include "about.h"
#include "prefs.h"
#include "setting.h"
#include "browser.h"
#include "scan.h"
#include "cddb.h"
#include "log.h"
#include "misc.h"
#include "charset.h"
#include "ui_manager.h"

#include "gtk2_compat.h"

#ifdef MAC_INTEGRATION
#include <gtkmacintegration/gtkosxapplication.h>
#include <gtkmacintegration/gtk-mac-menu.h>
  #if GTK_CHECK_VERSION(2,90,7)
  #include <gdk/gdkkeysyms-compat.h>
  #else
  #include <gdk/gdkkeysyms.h>
  #endif
#endif

/***************
 * Declaration *
 ***************/
GtkWidget   *StatusBar = NULL;
static guint StatusbarTimerId = 0;
GList *ActionPairsList = NULL;

/**************
 * Prototypes *
 **************/
void Init_Menu_Bar (void);
void Menu_Sort_Action (GtkAction *action, gpointer data);

void     Statusbar_Start_Timer  (void);
gboolean Statusbar_Stop_Timer   (void);
void     Statusbar_Remove_Timer (void);


/*************
 * Functions *
 *************/

/*
 * Dynamic reimplementation of switch macros
 */
#define QCASE(string,callback) if (quark == g_quark_from_string((string))) { (callback)(); }
#define QCASE_DATA(string,callback,data) if (quark == g_quark_from_string((string))) { (callback)((data)); }

/*
 * Menu bar
 */
void Menu_Sort_Action (GtkAction *item, gpointer data)
{
    GtkWidget *TBViewMode;
    const gchar *action = gtk_action_get_name(item);
    GQuark quark = g_quark_from_string(action);

    QCASE_DATA(AM_SORT_ASCENDING_FILENAME,         ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_FILENAME);
    QCASE_DATA(AM_SORT_DESCENDING_FILENAME,        ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_FILENAME);
    QCASE_DATA(AM_SORT_ASCENDING_CREATION_DATE,    ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_CREATION_DATE);
    QCASE_DATA(AM_SORT_DESCENDING_CREATION_DATE,   ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_CREATION_DATE);
    QCASE_DATA(AM_SORT_ASCENDING_TRACK_NUMBER,     ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_TRACK_NUMBER);
    QCASE_DATA(AM_SORT_DESCENDING_TRACK_NUMBER,    ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_TRACK_NUMBER);
    QCASE_DATA(AM_SORT_ASCENDING_TITLE,            ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_TITLE);
    QCASE_DATA(AM_SORT_DESCENDING_TITLE,           ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_TITLE);
    QCASE_DATA(AM_SORT_ASCENDING_ARTIST,           ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_ARTIST);
    QCASE_DATA(AM_SORT_DESCENDING_ARTIST,          ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_ARTIST);
    QCASE_DATA(AM_SORT_ASCENDING_ALBUM_ARTIST,     ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_ALBUM_ARTIST);
    QCASE_DATA(AM_SORT_DESCENDING_ALBUM_ARTIST,    ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_ALBUM_ARTIST);
    QCASE_DATA(AM_SORT_ASCENDING_ALBUM,            ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_ALBUM);
    QCASE_DATA(AM_SORT_DESCENDING_ALBUM,           ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_ALBUM);
    QCASE_DATA(AM_SORT_ASCENDING_YEAR,             ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_YEAR);
    QCASE_DATA(AM_SORT_DESCENDING_YEAR,            ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_YEAR);
    QCASE_DATA(AM_SORT_ASCENDING_GENRE,            ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_GENRE);
    QCASE_DATA(AM_SORT_DESCENDING_GENRE,           ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_GENRE);
    QCASE_DATA(AM_SORT_ASCENDING_COMMENT,          ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_COMMENT);
    QCASE_DATA(AM_SORT_DESCENDING_COMMENT,         ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_COMMENT);
    QCASE_DATA(AM_SORT_ASCENDING_COMPOSER,         ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_COMPOSER);
    QCASE_DATA(AM_SORT_DESCENDING_COMPOSER,        ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_COMPOSER);
    QCASE_DATA(AM_SORT_ASCENDING_ORIG_ARTIST,      ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_ORIG_ARTIST);
    QCASE_DATA(AM_SORT_DESCENDING_ORIG_ARTIST,     ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_ORIG_ARTIST);
    QCASE_DATA(AM_SORT_ASCENDING_COPYRIGHT,        ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_COPYRIGHT);
    QCASE_DATA(AM_SORT_DESCENDING_COPYRIGHT,       ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_COPYRIGHT);
    QCASE_DATA(AM_SORT_ASCENDING_URL,              ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_URL);
    QCASE_DATA(AM_SORT_DESCENDING_URL,             ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_URL);
    QCASE_DATA(AM_SORT_ASCENDING_ENCODED_BY,       ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_ENCODED_BY);
    QCASE_DATA(AM_SORT_DESCENDING_ENCODED_BY,      ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_ENCODED_BY);
    QCASE_DATA(AM_SORT_ASCENDING_FILE_TYPE,        ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_FILE_TYPE);
    QCASE_DATA(AM_SORT_DESCENDING_FILE_TYPE,       ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_FILE_TYPE);
    QCASE_DATA(AM_SORT_ASCENDING_FILE_SIZE,        ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_FILE_SIZE);
    QCASE_DATA(AM_SORT_DESCENDING_FILE_SIZE,       ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_FILE_SIZE);
    QCASE_DATA(AM_SORT_ASCENDING_FILE_DURATION,    ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_FILE_DURATION);
    QCASE_DATA(AM_SORT_DESCENDING_FILE_DURATION,   ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_FILE_DURATION);
    QCASE_DATA(AM_SORT_ASCENDING_FILE_BITRATE,     ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_FILE_BITRATE);
    QCASE_DATA(AM_SORT_DESCENDING_FILE_BITRATE,    ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_FILE_BITRATE);
    QCASE_DATA(AM_SORT_ASCENDING_FILE_SAMPLERATE,  ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_ASCENDING_FILE_SAMPLERATE);
    QCASE_DATA(AM_SORT_DESCENDING_FILE_SAMPLERATE, ET_Sort_Displayed_File_List_And_Update_UI, SORTING_BY_DESCENDING_FILE_SAMPLERATE);
    QCASE_DATA(AM_INITIALIZE_TREE,                 Browser_Tree_Rebuild,                      NULL);

    if (quark == g_quark_from_string(AM_TREE_OR_ARTISTALBUM_VIEW))
    {
        // Toggle button to switch between Browser view and Artist / Album view
        TBViewMode = gtk_ui_manager_get_widget(UIManager, "/ToolBar/ViewModeToggle");
        gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(TBViewMode),
                                          !gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(TBViewMode)));
    }
}

void Create_UI (GtkWidget **ppmenubar, GtkWidget **pptoolbar)
{
    GtkWidget *menubar;
    GtkWidget *toolbar;
    GtkWidget *toolbarwidget;

    /*
     * Structure :
     *  - name
     *  - stock_id
     *  - label
     *  - accelerator
     *  - tooltip
     *  - callback
     */
    GtkActionEntry ActionEntries[] =
    {

        /*
         * Main Menu Actions
         */
        { MENU_FILE,                          NULL,                      _("_File"),                         NULL, NULL,                               NULL},
        { MENU_FILE_SORT_TAG,                 GTK_STOCK_SORT_ASCENDING,  _("Sort list by tag"),              NULL, NULL,                               NULL },
        { MENU_FILE_SORT_PROP,                GTK_STOCK_SORT_ASCENDING,  _("Sort list by property"),         NULL, NULL,                               NULL },
        { AM_SORT_ASCENDING_FILENAME,         GTK_STOCK_SORT_ASCENDING,  _("Ascending by filename"),         NULL, _("Ascending by filename"),         G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_FILENAME,        GTK_STOCK_SORT_DESCENDING, _("Descending by filename"),        NULL, _("Descending by filename"),        G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_CREATION_DATE,    GTK_STOCK_SORT_ASCENDING,  _("Ascending by creation date"),    NULL, _("Ascending by creation date"),    G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_CREATION_DATE,   GTK_STOCK_SORT_DESCENDING, _("Descending by creation date"),   NULL, _("Descending by creation date"),   G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_TRACK_NUMBER,     GTK_STOCK_SORT_ASCENDING,  _("Ascending by track number"),     NULL, _("Ascending by track number"),     G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_TRACK_NUMBER,    GTK_STOCK_SORT_DESCENDING, _("Descending by track number"),    NULL, _("Descending by track number"),    G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_TITLE,            GTK_STOCK_SORT_ASCENDING,  _("Ascending by title"),            NULL, _("Ascending by title"),            G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_TITLE,           GTK_STOCK_SORT_DESCENDING, _("Descending by title"),           NULL, _("Descending by title"),           G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_ARTIST,           GTK_STOCK_SORT_ASCENDING,  _("Ascending by artist"),           NULL, _("Ascending by artist"),           G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_ARTIST,          GTK_STOCK_SORT_DESCENDING, _("Descending by artist"),          NULL, _("Descending by artist"),          G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_ALBUM_ARTIST,     GTK_STOCK_SORT_ASCENDING,  _("Ascending by album artist"),     NULL, _("Ascending by album artist"),   G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_ALBUM_ARTIST,    GTK_STOCK_SORT_DESCENDING, _("Descending by album artist"),    NULL, _("Descending by album artist"),   G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_ALBUM,            GTK_STOCK_SORT_ASCENDING,  _("Ascending by album"),            NULL, _("Ascending by album"),            G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_ALBUM,           GTK_STOCK_SORT_DESCENDING, _("Descending by album"),           NULL, _("Descending by album"),           G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_YEAR,             GTK_STOCK_SORT_ASCENDING,  _("Ascending by year"),             NULL, _("Ascending by year"),             G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_YEAR,            GTK_STOCK_SORT_DESCENDING, _("Descending by year"),            NULL, _("Descending by year"),            G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_GENRE,            GTK_STOCK_SORT_ASCENDING,  _("Ascending by genre"),            NULL, _("Ascending by genre"),            G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_GENRE,           GTK_STOCK_SORT_DESCENDING, _("Descending by genre"),           NULL, _("Descending by genre"),           G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_COMMENT,          GTK_STOCK_SORT_ASCENDING,  _("Ascending by comment"),          NULL, _("Ascending by comment"),          G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_COMMENT,         GTK_STOCK_SORT_DESCENDING, _("Descending by comment"),         NULL, _("Descending by comment"),         G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_COMPOSER,         GTK_STOCK_SORT_ASCENDING,  _("Ascending by composer"),         NULL, _("Ascending by composer"),         G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_COMPOSER,        GTK_STOCK_SORT_DESCENDING, _("Descending by composer"),        NULL, _("Descending by composer"),        G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_ORIG_ARTIST,      GTK_STOCK_SORT_ASCENDING,  _("Ascending by original artist"),  NULL, _("Ascending by original artist"),  G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_ORIG_ARTIST,     GTK_STOCK_SORT_DESCENDING, _("Descending by original artist"), NULL, _("Descending by original artist"), G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_COPYRIGHT,        GTK_STOCK_SORT_ASCENDING,  _("Ascending by copyright"),        NULL, _("Ascending by copyright"),        G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_COPYRIGHT,       GTK_STOCK_SORT_DESCENDING, _("Descending by copyright"),       NULL, _("Descending by copyright"),       G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_URL,              GTK_STOCK_SORT_ASCENDING,  _("Ascending by URL"),              NULL, _("Ascending by URL"),              G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_URL,             GTK_STOCK_SORT_DESCENDING, _("Descending by URL"),             NULL, _("Descending by URL"),             G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_ENCODED_BY,       GTK_STOCK_SORT_ASCENDING,  _("Ascending by encoder name"),     NULL, _("Ascending by encoder name"),     G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_ENCODED_BY,      GTK_STOCK_SORT_DESCENDING, _("Descending by encoder name"),    NULL, _("Descending by encoder name"),    G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_FILE_TYPE,        GTK_STOCK_SORT_ASCENDING,  _("Ascending by file type"),        NULL, _("Ascending by file type"),        G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_FILE_TYPE,       GTK_STOCK_SORT_DESCENDING, _("Descending by file type"),       NULL, _("Descending by file type"),       G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_FILE_SIZE,        GTK_STOCK_SORT_ASCENDING,  _("Ascending by file size"),        NULL, _("Ascending by file size"),        G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_FILE_SIZE,       GTK_STOCK_SORT_DESCENDING, _("Descending by file size"),       NULL, _("Descending by file size"),       G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_FILE_DURATION,    GTK_STOCK_SORT_ASCENDING,  _("Ascending by duration"),         NULL, _("Ascending by duration"),         G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_FILE_DURATION,   GTK_STOCK_SORT_DESCENDING, _("Descending by duration"),        NULL, _("Descending by duration"),        G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_FILE_BITRATE,     GTK_STOCK_SORT_ASCENDING,  _("Ascending by bitrate"),          NULL, _("Ascending by bitrate"),          G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_FILE_BITRATE,    GTK_STOCK_SORT_DESCENDING, _("Descending by bitrate"),         NULL, _("Descending by bitrate"),         G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_ASCENDING_FILE_SAMPLERATE,  GTK_STOCK_SORT_ASCENDING,  _("Ascending by samplerate"),       NULL, _("Ascending by samplerate"),       G_CALLBACK(Menu_Sort_Action) },
        { AM_SORT_DESCENDING_FILE_SAMPLERATE, GTK_STOCK_SORT_DESCENDING, _("Descending by samplerate"),      NULL, _("Descending by samplerate"),      G_CALLBACK(Menu_Sort_Action) },

#ifndef MAC_INTEGRATION
        { AM_OPEN_FILE_WITH,     GTK_STOCK_OPEN,             _("Open File(s) with ..."),      NULL,                _("Open File(s) with ..."),     G_CALLBACK(Browser_Open_Run_Program_List_Window) },
        { AM_SELECT_ALL_FILES,   "easytag-select-all",       _("Select All Files"),           "<Control>A",        _("Select All Files"),          G_CALLBACK(Action_Select_All_Files) },
        { AM_UNSELECT_ALL_FILES, "easytag-unselect-all",     _("Unselect All Files"),         "<Shift><Control>A", _("Unselect All Files"),        G_CALLBACK(Action_Unselect_All_Files) },
        { AM_INVERT_SELECTION,   "easytag-invert-selection", _("Invert Files Selection"),     "<Control>I",        _("Invert Files Selection"),    G_CALLBACK(Action_Invert_Files_Selection) },
        { AM_DELETE_FILE,        GTK_STOCK_DELETE,           _("Delete File(s)"),             NULL,                _("Delete File(s)"),            G_CALLBACK(Action_Delete_Selected_Files) },
        { AM_FIRST,              GTK_STOCK_GOTO_FIRST,       _("_First File"),                "<Control>Home",     _("First File"),                G_CALLBACK(Action_Select_First_File) },
        { AM_PREV,               GTK_STOCK_GO_BACK,          _("_Previous File"),             "Page_Up",           _("Previous File"),             G_CALLBACK(Action_Select_Prev_File) },
        { AM_NEXT,               GTK_STOCK_GO_FORWARD,       _("_Next File"),                 "Page_Down",         _("Next File"),                 G_CALLBACK(Action_Select_Next_File) },
        { AM_LAST,               GTK_STOCK_GOTO_LAST,        _("_Last File"),                 "<Control>End",      _("Last File"),                 G_CALLBACK(Action_Select_Last_File) },
        // XXX GTK1 version uses Ctrl+C for scanner, this doesnt work in GTK1 as its copy! in gtk2, behaviour is different
        // and binding Ctrl+C effectively stops the user copying text..
        { AM_SCAN,               "easytag-scan",             _("S_can File(s)"),              NULL,                _("Scan File(s)"),              G_CALLBACK(Action_Scan_Selected_Files) },
        { AM_REMOVE,             GTK_STOCK_CLEAR,            _("_Remove Tag(s)"),             "<Control>R",        _("Remove Tag(s)"),             G_CALLBACK(Action_Remove_Selected_Tags) },
        { AM_UNDO,               GTK_STOCK_UNDO,             _("_Undo Last File(s) Changes"), "<Control>Z",        _("Undo Last File(s) Changes"), G_CALLBACK(Action_Undo_Selected_Files) },
        { AM_REDO,               GTK_STOCK_REDO,             _("R_edo Last File(s) Changes"), "<Shift><Control>Z", _("Redo Last File(s) Changes"), G_CALLBACK(Action_Redo_Selected_File) },
        { AM_SAVE,               GTK_STOCK_SAVE,             _("_Save File(s)"),              "<Control>S",        _("Save File(s)"),              G_CALLBACK(Action_Save_Selected_Files) },
        { AM_SAVE_FORCED,        GTK_STOCK_SAVE,             _("_Force Saving File(s)"),      "<Shift><Control>S", _("Force Saving File(s)"),      G_CALLBACK(Action_Force_Saving_Selected_Files) },
        { AM_UNDO_HISTORY,       GTK_STOCK_UNDO,             _("Undo Last Changes"),          NULL,                _("Undo Last Changes"),         G_CALLBACK(Action_Undo_From_History_List) },
        { AM_REDO_HISTORY,       GTK_STOCK_REDO,             _("Redo Last Changes"),          NULL,                _("Redo Last Changes"),         G_CALLBACK(Action_Redo_From_History_List) },
        { AM_QUIT,               GTK_STOCK_QUIT,             _("_Quit"),                      "<Control>Q",        _("Quit"),                      G_CALLBACK(Quit_MainWindow) },

        { MENU_BROWSER,                NULL,                   _("_Browser"),                      NULL,                NULL,                               NULL },
        { AM_LOAD_HOME_DIR,            GTK_STOCK_HOME,         _("Go to _Home Directory"),         "<Alt>Home",         _("Go to Home Directory"),          G_CALLBACK(Browser_Load_Home_Directory) },
        { AM_LOAD_DESKTOP_DIR,         GTK_STOCK_JUMP_TO,      _("Go to Desktop Directory"),       NULL,         		_("Go to Desktop Directory"),       G_CALLBACK(Browser_Load_Desktop_Directory) },
        { AM_LOAD_DOCUMENTS_DIR,       GTK_STOCK_JUMP_TO,      _("Go to Documents Directory"),     NULL,         		_("Go to Documents Directory"),     G_CALLBACK(Browser_Load_Documents_Directory) },
        { AM_LOAD_DOWNLOADS_DIR,       GTK_STOCK_JUMP_TO,      _("Go to Downloads Directory"),     NULL,         		_("Go to Downloads Directory"),     G_CALLBACK(Browser_Load_Downloads_Directory) },
        { AM_LOAD_MUSIC_DIR,           GTK_STOCK_JUMP_TO,      _("Go to Music Directory"),         NULL,         		_("Go to Music Directory"),         G_CALLBACK(Browser_Load_Music_Directory) },
        { AM_LOAD_DEFAULT_DIR,         GTK_STOCK_JUMP_TO,      _("Go to _Default Directory"),      "<Control>D",        _("Go to Default Directory"),       G_CALLBACK(Browser_Load_Default_Directory) },
        { AM_SET_PATH_AS_DEFAULT,      GTK_STOCK_DIRECTORY,    _("Set _Current Path as Default"),  NULL,                _("Set Current Path as Default"),   G_CALLBACK(Set_Current_Path_As_Default) },
        { AM_TREE_OR_ARTISTALBUM_VIEW, "easytag-artist-album", _("Tree View | Artist-Album View"), NULL,                _("Tree View | Artist-Album View"), G_CALLBACK(Menu_Sort_Action) },
        { AM_RENAME_DIR,               GTK_STOCK_INDEX,        _("Rename Directory ..."),          "F2",                _("Rename Directory ..."),          G_CALLBACK(Browser_Open_Rename_Directory_Window) },
        { AM_RELOAD_DIRECTORY,         GTK_STOCK_REFRESH,      _("Reload Directory"),              "F5",                _("Reload Directory"),              G_CALLBACK(Browser_Reload_Directory) },
        { AM_BROWSE_DIRECTORY_WITH,    GTK_STOCK_EXECUTE,      _("Browse Directory with ..."),     NULL,                _("Browse Directory with ..."),     G_CALLBACK(Browser_Open_Run_Program_Tree_Window) },
        { AM_COLLAPSE_TREE,            NULL,                   _("_Collapse Tree"),                "<Control><Shift>C", _("_Collapse Tree"),                G_CALLBACK(Browser_Tree_Collapse) },
        { AM_INITIALIZE_TREE,          GTK_STOCK_REFRESH,      _("_Refresh Tree"),                 "<Control><Shift>R", _("_Refresh Tree"),                 G_CALLBACK(Browser_Tree_Rebuild) },

        { MENU_SCANNER,              NULL,                  _("S_canner"),                          NULL,         NULL,                                 NULL },
        { AM_SCANNER_FILL_TAG,       "easytag-scan",        _("_Fill Tag(s) ..."),                  NULL,         _("Fill Tag(s) ..."),                 G_CALLBACK(Scan_Use_Fill_Tag_Scanner) },
        { AM_SCANNER_RENAME_FILE,    "easytag-scan",        _("_Rename File(s) and Directory ..."), NULL,         _("Rename File(s) and Directory ..."),G_CALLBACK(Scan_Use_Rename_File_Scanner) },
        { AM_SCANNER_PROCESS_FIELDS, "easytag-scan",        _("_Process Field(s) ..."),             NULL,         _("Process Fields(s) ..."),           G_CALLBACK(Scan_Use_Process_Fields_Scanner) },

        { MENU_MISC,                NULL,                   _("_Misc"),                             NULL,         NULL,                                 NULL },
        { AM_SEARCH_FILE,           GTK_STOCK_FIND,         _("Search _File(s) ..."),               "<Control>F", _("Search File(s)..."),               G_CALLBACK(Open_Search_File_Window) },
        { AM_CDDB_SEARCH,           GTK_STOCK_CDROM,        _("CD Data _Base Search ..."),          "<Control>B", _("CDDB Search ..."),                 G_CALLBACK(Open_Cddb_Window) },
        { AM_FILENAME_FROM_TXT,     GTK_STOCK_OPEN,         _("Load Filenames from TXT ..."),       "<Alt>T",     _("Load Filenames from TXT ..."),     G_CALLBACK(Open_Load_Filename_Window) },
        { AM_WRITE_PLAYLIST,        GTK_STOCK_SAVE_AS,      _("Write Playlist ..."),                "<Alt>W",     _("Write Playlist ..."),              G_CALLBACK(Open_Write_Playlist_Window) },
        { AM_RUN_AUDIO_PLAYER,      "easytag-sound",        _("Run Audio Player"),                  "<Alt>X",     _("Run Audio Player"),                G_CALLBACK(Run_Audio_Player_Using_Selection) },

        { MENU_SETTINGS,            NULL,                   _("_Settings"),                         NULL,         NULL,                                 NULL },
        { AM_OPEN_OPTIONS_WINDOW,   GTK_STOCK_PREFERENCES,  _("_Preferences ..."),                  "<Alt>P",     _("Preferences ..."),                 G_CALLBACK(Open_OptionsWindow) },

        { MENU_HELP,                NULL,                   _("_Help"),                             NULL,         NULL,                                 NULL },
        { AM_OPEN_ABOUT_WINDOW,     GTK_STOCK_ABOUT,        _("_About"),                            NULL,         _("About"),                           G_CALLBACK(Show_About_Window) },

#else
		/* These are the changed shortcuts for the Mac Menubar, they only apply when GTK-Mac-Integration is present */
		{ AM_OPEN_FILE_WITH,     GTK_STOCK_OPEN,             "_Open",						  "<Meta>O",        _("Open File(s) with ..."),     G_CALLBACK(Browser_Open_Run_Program_List_Window) },		
        { AM_SELECT_ALL_FILES,   "easytag-select-all",       _("Select All Files"),           "<Meta>A",        _("Select All Files"),          G_CALLBACK(Action_Select_All_Files) },
        { AM_UNSELECT_ALL_FILES, "easytag-unselect-all",     _("Unselect All Files"),         "<Shift><Meta>A", _("Unselect All Files"),        G_CALLBACK(Action_Unselect_All_Files) },
        { AM_INVERT_SELECTION,   "easytag-invert-selection", _("Invert Files Selection"),     "<Meta>I",        _("Invert Files Selection"),    G_CALLBACK(Action_Invert_Files_Selection) },
        { AM_DELETE_FILE,        GTK_STOCK_DELETE,           _("Delete File(s)"),             NULL,             _("Delete File(s)"),            G_CALLBACK(Action_Delete_Selected_Files) },
        { AM_FIRST,              GTK_STOCK_GOTO_FIRST,       _("_First File"),                "<Meta>Home",     _("First File"),                G_CALLBACK(Action_Select_First_File) },
        { AM_PREV,               GTK_STOCK_GO_BACK,          _("_Previous File"),             "Page_Up",        _("Previous File"),             G_CALLBACK(Action_Select_Prev_File) },
        { AM_NEXT,               GTK_STOCK_GO_FORWARD,       _("_Next File"),                 "Page_Down",      _("Next File"),                 G_CALLBACK(Action_Select_Next_File) },
        { AM_LAST,               GTK_STOCK_GOTO_LAST,        _("_Last File"),                 "<Meta>End",      _("Last File"),                 G_CALLBACK(Action_Select_Last_File) },
        // XXX GTK1 version uses Ctrl+C for scanner, this doesnt work in GTK1 as its copy! in gtk2, behaviour is different
        // and binding Ctrl+C effectively stops the user copying text..
        { AM_SCAN,               "easytag-scan",             _("S_can File(s)"),              NULL,             _("Scan File(s)"),              G_CALLBACK(Action_Scan_Selected_Files) },
        { AM_REMOVE,             GTK_STOCK_CLEAR,            _("_Remove Tag(s)"),             "<Meta>R",        _("Remove Tag(s)"),             G_CALLBACK(Action_Remove_Selected_Tags) },
        { AM_UNDO,               GTK_STOCK_UNDO,             _("_Undo Last File(s) Changes"), "<Meta>Z",        _("Undo Last File(s) Changes"), G_CALLBACK(Action_Undo_Selected_Files) },
        { AM_REDO,               GTK_STOCK_REDO,             _("R_edo Last File(s) Changes"), "<Shift><Meta>Z", _("Redo Last File(s) Changes"), G_CALLBACK(Action_Redo_Selected_File) },
        { AM_SAVE,               GTK_STOCK_SAVE,             _("_Save File(s)"),              "<Meta>S",        _("Save File(s)"),              G_CALLBACK(Action_Save_Selected_Files) },
        { AM_SAVE_FORCED,        GTK_STOCK_SAVE,             _("_Force Saving File(s)"),      "<Shift><Meta>S", _("Force Saving File(s)"),      G_CALLBACK(Action_Force_Saving_Selected_Files) },
        { AM_UNDO_HISTORY,       GTK_STOCK_UNDO,             _("Undo Last Changes"),          NULL,             _("Undo Last Changes"),         G_CALLBACK(Action_Undo_From_History_List) },
        { AM_REDO_HISTORY,       GTK_STOCK_REDO,             _("Redo Last Changes"),          NULL,             _("Redo Last Changes"),         G_CALLBACK(Action_Redo_From_History_List) },
        { AM_QUIT,               GTK_STOCK_QUIT,             _("_Quit"),                      "<Meta>Q",        _("Quit"),                      G_CALLBACK(Quit_MainWindow) },

        { MENU_BROWSER,                NULL,                   _("_Browser"),                      NULL,                NULL,                               NULL },
        { AM_LOAD_HOME_DIR,            GTK_STOCK_HOME,         _("Go to _Home Directory"),         "<Alt>Home",      _("Go to Home Directory"),          G_CALLBACK(Browser_Load_Home_Directory) },
        { AM_LOAD_DESKTOP_DIR,         GTK_STOCK_JUMP_TO,      _("Go to Desktop Directory"),       NULL,        	 _("Go to Desktop Directory"),       G_CALLBACK(Browser_Load_Desktop_Directory) },
        { AM_LOAD_DOCUMENTS_DIR,       GTK_STOCK_JUMP_TO,      _("Go to Documents Directory"),     NULL,         	 _("Go to Documents Directory"),     G_CALLBACK(Browser_Load_Documents_Directory) },
        { AM_LOAD_DOWNLOADS_DIR,       GTK_STOCK_JUMP_TO,      _("Go to Downloads Directory"),     NULL,         	 _("Go to Downloads Directory"),     G_CALLBACK(Browser_Load_Downloads_Directory) },
        { AM_LOAD_MUSIC_DIR,           GTK_STOCK_JUMP_TO,      _("Go to Music Directory"),         NULL,         	 _("Go to Music Directory"),         G_CALLBACK(Browser_Load_Music_Directory) },
        { AM_LOAD_DEFAULT_DIR,         GTK_STOCK_JUMP_TO,      _("Go to _Default Directory"),      "<Meta>D",        _("Go to Default Directory"),       G_CALLBACK(Browser_Load_Default_Directory) },
        { AM_SET_PATH_AS_DEFAULT,      GTK_STOCK_DIRECTORY,    _("Set _Current Path as Default"),  NULL,             _("Set Current Path as Default"),   G_CALLBACK(Set_Current_Path_As_Default) },
        { AM_TREE_OR_ARTISTALBUM_VIEW, "easytag-artist-album", _("Tree View | Artist-Album View"), NULL,             _("Tree View | Artist-Album View"), G_CALLBACK(Menu_Sort_Action) },
        { AM_RENAME_DIR,               GTK_STOCK_INDEX,        _("Rename Directory ..."),          "F2",             _("Rename Directory ..."),          G_CALLBACK(Browser_Open_Rename_Directory_Window) },
        { AM_RELOAD_DIRECTORY,         GTK_STOCK_REFRESH,      _("Reload Directory"),              "F5",             _("Reload Directory"),              G_CALLBACK(Browser_Reload_Directory) },
        { AM_BROWSE_DIRECTORY_WITH,    GTK_STOCK_EXECUTE,      _("Browse Directory with ..."),     NULL,             _("Browse Directory with ..."),     G_CALLBACK(Browser_Open_Run_Program_Tree_Window) },
        { AM_COLLAPSE_TREE,            NULL,                   _("_Collapse Tree"),                "<Meta><Shift>C", _("_Collapse Tree"),                G_CALLBACK(Browser_Tree_Collapse) },
        { AM_INITIALIZE_TREE,          GTK_STOCK_REFRESH,      _("_Refresh Tree"),                 "<Meta><Shift>R", _("_Refresh Tree"),                 G_CALLBACK(Browser_Tree_Rebuild) },

        { MENU_SCANNER,              NULL,                  _("S_canner"),                          NULL,         NULL,                                 NULL },
        { AM_SCANNER_FILL_TAG,       "easytag-scan",        _("_Fill Tag(s) ..."),                  NULL,         _("Fill Tag(s) ..."),                 G_CALLBACK(Scan_Use_Fill_Tag_Scanner) },
        { AM_SCANNER_RENAME_FILE,    "easytag-scan",        _("_Rename File(s) and Directory ..."), NULL,         _("Rename File(s) and Directory ..."),G_CALLBACK(Scan_Use_Rename_File_Scanner) },
        { AM_SCANNER_PROCESS_FIELDS, "easytag-scan",        _("_Process Field(s) ..."),             NULL,         _("Process Fields(s) ..."),           G_CALLBACK(Scan_Use_Process_Fields_Scanner) },

        { MENU_MISC,                NULL,                   _("_Misc"),                             NULL,         NULL,                                 NULL },
        { AM_SEARCH_FILE,           GTK_STOCK_FIND,         _("Search _File(s) ..."),               "<Meta>F", 	   _("Search File(s)..."),               G_CALLBACK(Open_Search_File_Window) },
        { AM_CDDB_SEARCH,           GTK_STOCK_CDROM,        _("CD Data _Base Search ..."),          "<Meta>B", 	   _("CDDB Search ..."),                 G_CALLBACK(Open_Cddb_Window) },
        { AM_FILENAME_FROM_TXT,     GTK_STOCK_OPEN,         _("Load Filenames from TXT ..."),       "<Meta>T",     _("Load Filenames from TXT ..."),     G_CALLBACK(Open_Load_Filename_Window) },
        { AM_WRITE_PLAYLIST,        GTK_STOCK_SAVE_AS,      _("Write Playlist ..."),                "<Meta>W",     _("Write Playlist ..."),              G_CALLBACK(Open_Write_Playlist_Window) },
        { AM_RUN_AUDIO_PLAYER,      "easytag-sound",        _("Run Audio Player"),                  "<Meta>X",     _("Run Audio Player"),                G_CALLBACK(Run_Audio_Player_Using_Selection) },

        { MENU_SETTINGS,            NULL,                   _("_Settings"),                         NULL,         NULL,                                 NULL },
        { AM_OPEN_OPTIONS_WINDOW,   GTK_STOCK_PREFERENCES,  _("_Preferences ..."),                  "<Meta>,",     _("Preferences ..."),                 G_CALLBACK(Open_OptionsWindow) },

        { MENU_HELP,                NULL,                   _("_Help"),                             NULL,         NULL,                                 NULL },
        { AM_OPEN_ABOUT_WINDOW,     GTK_STOCK_ABOUT,        _("_About"),                            NULL,         _("About"),                           G_CALLBACK(Show_About_Window) },
#endif


        /*
         * Following items are on toolbar but not on menu
         */
        { AM_STOP, GTK_STOCK_STOP, _("Stop the current action"), NULL, _("Stop the current action"), G_CALLBACK(Action_Main_Stop_Button_Pressed) },


        /*
         * Popup menu's Actions
         */
        { POPUP_FILE,                   NULL,              _("_File Operations"),          NULL, NULL,                         NULL },
        { POPUP_SUBMENU_SCANNER,        "easytag-scan",    _("S_canner"),                  NULL, NULL,                         NULL },
        { POPUP_DIR_RUN_AUDIO,          "easytag-sound",   _("Run Audio Player"),          NULL, _("Run Audio Player"),        G_CALLBACK(Run_Audio_Player_Using_Directory) },
        { AM_ARTIST_RUN_AUDIO_PLAYER,   "easytag-sound",   _("Run Audio Player"),          NULL, _("Run Audio Player"),        G_CALLBACK(Run_Audio_Player_Using_Browser_Artist_List) },
        { AM_ALBUM_RUN_AUDIO_PLAYER,    "easytag-sound",   _("Run Audio Player"),          NULL, _("Run Audio Player"),        G_CALLBACK(Run_Audio_Player_Using_Browser_Album_List)  },
        { AM_CDDB_SEARCH_FILE,          GTK_STOCK_CDROM,   _("CDDB Search File(s)..."),    NULL, _("CDDB Search File(s)..."),  G_CALLBACK(Cddb_Popup_Menu_Search_Selected_File) },
        //{ AM_ARTIST_OPEN_FILE_WITH,     GTK_STOCK_OPEN,    _("Open File(s) with ..."),     NULL, _("Open File(s) with ..."),     G_CALLBACK(Browser_Open_Run_Program_List_Window??? Browser_Open_Run_Program_Tree_Window???) },
        //{ AM_ALBUM_OPEN_FILE_WITH,      GTK_STOCK_OPEN,    _("Open File(s) with ..."),     NULL, _("Open File(s) with ..."),     G_CALLBACK(Browser_Open_Run_Program_List_Window??? Browser_Open_Run_Program_Tree_Window???) },

        { AM_LOG_CLEAN,                 GTK_STOCK_CLEAR,   _("Clean Log"),                 NULL, _("Clean Log"),              G_CALLBACK(Log_Clean_Log_List) }

    };

    GtkToggleActionEntry ToggleActionEntries[] =
    {
        //{ AM_BROWSE_SUBDIR,      GTK_STOCK_INDEX,        _("Browse _Sub-directories"),                         NULL, _("Browse _Sub-directories"),                         NULL,                                    FALSE },
        { AM_BROWSE_SUBDIR,      NULL,                   _("Browse _Sub-directories"),                         NULL, _("Browse _Sub-directories"),                         NULL,                                    BROWSE_SUBDIR },
#ifndef WIN32 // No sense here for Win32, "hidden" means : starts with a '.'
        //{ AM_BROWSER_HIDDEN_DIR, NULL,                   _("Show Hidden Directories"),                         NULL, _("Show Hidden Directories"),                         G_CALLBACK(Browser_Tree_Rebuild),     FALSE },
        { AM_BROWSER_HIDDEN_DIR, NULL,                   _("Show Hidden Directories"),                         NULL, _("Show Hidden Directories"),                         G_CALLBACK(Browser_Tree_Rebuild),     BROWSE_HIDDEN_DIR },
#endif
        { AM_VIEWMODE_TOGGLE,    "easytag-artist-album", _("Show tree browser / Display by Artist and Album"), NULL, _("Show tree browser / Display by Artist and Album"), G_CALLBACK(Action_Select_Browser_Style), FALSE },
    };

    GError *error = NULL;
    guint num_menu_entries;
    guint num_toggle_entries;
    guint i;

    /* Calculate number of items into the menu */
    num_menu_entries = G_N_ELEMENTS(ActionEntries);
    num_toggle_entries = G_N_ELEMENTS(ToggleActionEntries);

    /* Populate quarks list with the entries */
    for(i = 0; i < num_menu_entries; i++)
    {
        Action_Pair* ActionPair = g_malloc0(sizeof(Action_Pair));
        ActionPair->action = ActionEntries[i].name;
        ActionPair->quark  = g_quark_from_string(ActionPair->action);
        ActionPairsList    = g_list_append(ActionPairsList, ActionPair);
    }

    for(i = 0; i < num_toggle_entries; i++)
    {
        Action_Pair* ActionPair = g_malloc0(sizeof(Action_Pair));
        ActionPair->action = ToggleActionEntries[i].name;
        ActionPair->quark  = g_quark_from_string(ActionPair->action);
        ActionPairsList    = g_list_append(ActionPairsList, ActionPair);
    }

    /* UI Management */
    ActionGroup = gtk_action_group_new("actions");
    gtk_action_group_add_actions(ActionGroup, ActionEntries, num_menu_entries, NULL);
    gtk_action_group_add_toggle_actions(ActionGroup, ToggleActionEntries, num_toggle_entries, NULL);

    UIManager = gtk_ui_manager_new();
    if (!gtk_ui_manager_add_ui_from_string(UIManager, ui_xml, -1, &error))
    {
        g_error(_("Could not merge UI, error was: %s\n"), error->message);
        g_error_free(error);
    }
    gtk_ui_manager_insert_action_group(UIManager, ActionGroup, 0);
    gtk_window_add_accel_group(GTK_WINDOW(MainWindow), gtk_ui_manager_get_accel_group(UIManager));

    /* Menu (not placed in a handlebar) */
    menubar = gtk_ui_manager_get_widget(UIManager, "/MenuBar");
    Init_Menu_Bar();
    gtk_widget_show_all(menubar);

    /* Toolbar is placed in a handlebar */
    toolbar = gtk_handle_box_new();
    toolbarwidget = gtk_ui_manager_get_widget(UIManager, "/ToolBar");
    gtk_container_add(GTK_CONTAINER(toolbar), toolbarwidget);
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbarwidget), GTK_TOOLBAR_ICONS);
    gtk_widget_show_all(toolbar);

    *pptoolbar = toolbar;
    *ppmenubar = menubar;

#ifdef MAC_INTEGRATION
	/* If gtk-mac-integration is available we register with the osx menubar after the menubar has been completly constructed
	   note that because of the inclusion of GMenuItem in gtk > 3.4.0 may be conflicts have yet to be sorted out with
	   conflicting implementations of GMenuItem in gtk-mac-integration and gtk - the former will probably be deprecated in 
	   the future */
	
	// Change the Shortcut for the Prefs, because comma doesn't work else
	gtk_accel_map_change_entry("<Actions>/actions/Preferences", GDK_comma, 
							GDK_META_MASK, TRUE);
							
	GtkOSXApplication *theApp = g_object_new(GTK_TYPE_OSX_APPLICATION, NULL);				
	
	gtk_osxapplication_set_menu_bar (theApp, GTK_MENU_SHELL(menubar));
	
	GtkWidget *about_item = gtk_ui_manager_get_widget(UIManager, "/MenuBar/HelpMenu/About");
	gtk_osxapplication_insert_app_menu_item(theApp, about_item, 0);
	gtk_osxapplication_insert_app_menu_item (theApp, gtk_separator_menu_item_new(), 1);

	GtkWidget *preferences_item = gtk_ui_manager_get_widget(UIManager, "/MenuBar/SettingsMenu/Preferences");
	gtk_osxapplication_insert_app_menu_item(theApp, preferences_item, 2);
	
	GtkWidget *setting_item = gtk_ui_manager_get_widget(UIManager, "/MenuBar/SettingsMenu");
	gtk_widget_set_visible(setting_item, FALSE);
	
	GtkWidget *filequit_item = gtk_ui_manager_get_widget(UIManager, "/MenuBar/FileMenu/Quit");
	gtk_widget_set_visible(filequit_item, FALSE);

	// Hide the original menubar in the application window, because it is not needed anymore
	gtk_widget_hide (menubar);
	gtk_osxapplication_set_use_quartz_accelerators(theApp, TRUE);	
#endif
}


/*
 * Initialize some items of the main menu
 */
void Init_Menu_Bar (void)
{
    
    CheckMenuItemBrowseSubdirMainMenu = gtk_ui_manager_get_widget(UIManager, "/MenuBar/BrowserMenu/BrowseSubdir");
    if (CheckMenuItemBrowseSubdirMainMenu)
    {
        // Link to update BROWSE_SUBDIR when changed
        g_signal_connect(G_OBJECT(CheckMenuItemBrowseSubdirMainMenu),"toggled",
            G_CALLBACK(Check_Menu_Item_Toggled_Browse_Subdir),NULL);
    }

    CheckMenuItemBrowseHiddenDirMainMenu = gtk_ui_manager_get_widget(UIManager, "/MenuBar/BrowserMenu/BrowseHiddenDir");
    if (CheckMenuItemBrowseHiddenDirMainMenu)
    {
        // Link to update BROWSE_HIDDEN_DIR when changed
        g_signal_connect(G_OBJECT(CheckMenuItemBrowseHiddenDirMainMenu),"toggled",
            G_CALLBACK(Check_Menu_Item_Toggled_Browse_Hidden_Dir),NULL);
    }

    /* If entry not implemented */
    //{GtkWidget *widget = gtk_item_factory_get_widget_by_action(ItemFactory,FILENAME_FROM_TXT);
    //if (widget) gtk_widget_set_sensitive(widget,FALSE);}
}

/*
 * Callback to update state of check button to browse subdir into menu
 */
void Check_Menu_Item_Toggled_Browse_Subdir (GtkWidget *checkmenuitem)
{
    BROWSE_SUBDIR = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(checkmenuitem));
    Check_Menu_Item_Update_Browse_Subdir();
}
void Check_Menu_Item_Update_Browse_Subdir (void)
{
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(CheckMenuItemBrowseSubdirMainMenu),BROWSE_SUBDIR);
}

/*
 * Callback to update state of check button to show hiddendirectories into menu
 */
void Check_Menu_Item_Toggled_Browse_Hidden_Dir (GtkWidget *checkmenuitem)
{
    BROWSE_HIDDEN_DIR = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(checkmenuitem));
    Check_Menu_Item_Update_Browse_Hidden_Dir();

    // Reload directory, in case we have changed BROWSE_HIDDEN_DIR
    //Browser_Tree_Rebuild(NULL); // Commented, as already done in GtkToggleActionEntry for AM_BROWSER_HIDDEN_DIR
}
void Check_Menu_Item_Update_Browse_Hidden_Dir (void)
{
#ifndef WIN32
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(CheckMenuItemBrowseHiddenDirMainMenu),BROWSE_HIDDEN_DIR);
#endif
}









/*
 * Status bar functions
 */
GtkWidget *Create_Status_Bar (void)
{
    StatusBar = gtk_statusbar_new();
    /* Specify a size to avoid statubar resizing if the message is too long */
    gtk_widget_set_size_request(StatusBar, 200, -1);
    /* Create serie */
    StatusBarContext = gtk_statusbar_get_context_id(GTK_STATUSBAR(StatusBar),"Messages");

    Statusbar_Message(_("Ready to start..."),TRUE);

    gtk_widget_show(StatusBar);
    return StatusBar;
}

gboolean Statusbar_Stop_Timer (void)
{
    gtk_statusbar_pop(GTK_STATUSBAR(StatusBar),StatusBarContext);
    return FALSE;    /* Stop the timer */
}

void Statusbar_Start_Timer (void)
{
    Statusbar_Remove_Timer();
    StatusbarTimerId = g_timeout_add(4000,(GSourceFunc)Statusbar_Stop_Timer,NULL);
}

void Statusbar_Remove_Timer (void)
{
    if (StatusbarTimerId)
    {
        g_source_remove(StatusbarTimerId);
        StatusbarTimerId = 0;
    }
}

/*
 * Send a message to the status bar
 *  - with_timer: if TRUE, the message will be displayed during 4s
 *                if FALSE, the message will be displayed up to the next posted message
 */
void Statusbar_Message (gchar *message, gint with_timer)
{
    gchar *msg_temp;
    if (!StatusBar) return;

    /* Validate UTF8 */
    /*if (!g_utf8_validate(message, -1, NULL))
        msg_temp = convert_to_utf8(message);
    else
        msg_temp = g_strdup(message);*/
    msg_temp = Try_To_Validate_Utf8_String(message);
    
    /* Remove a running timer */
    Statusbar_Remove_Timer();

    /* Pop last message */
    gtk_statusbar_pop(GTK_STATUSBAR(StatusBar),StatusBarContext);

    /* Push the given message */
    gtk_statusbar_push(GTK_STATUSBAR(StatusBar),StatusBarContext,msg_temp);

    g_free(msg_temp);

    while (gtk_events_pending())
        gtk_main_iteration();

    if (with_timer)
        Statusbar_Start_Timer();
}







/*
 * Progress bar
 */
GtkWidget *Create_Progress_Bar (void)
{
    ProgressBar = gtk_progress_bar_new();

    gtk_widget_show(ProgressBar);
    return ProgressBar;
}
