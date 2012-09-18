/* osx_util.c */
/*    Copyright (c) 2012, Marco Schuh
      All rights reserved.

   Redistribution and use in source and binary forms, with or without 
   modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution.
   3. Neither the name of the <ORGANIZATION> nor the names of its contributors 
      may be used to endorse or promote products derived from this software 
      without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
   POSSIBILITY OF SUCH DAMAGE.
*/

#include <glib.h>
#include <glib/gstdio.h>

#include "osx_util.h"

#ifdef MAC_INTEGRATION
#	include <gtkmacintegration/gtkosxapplication.h>
#	include "browser.h"
#endif


#ifdef MAC_INTEGRATION
gboolean is_running_from_osx_appBundle() {
	gchar *bundle_id = quartz_application_get_bundle_id();
	if (bundle_id != NULL) {
		g_free(bundle_id);
		return TRUE;
	} else {
		g_free(bundle_id);
		return FALSE;
	}
}

/*
 * Callback function to be executed on the main gtk loop
 */
static gboolean
osx_open_file_idle_callback (gpointer user_data)
{
	gchar *path = (gchar *) user_data;	
	gchar *path2 = NULL;
	gboolean fileIsValid = FALSE;
	g_print("OSX open file/folder :  %s \n", path);
	
	/* Since files are only passed via (Finder) drag'n'drop
	 * and then the dock validates the drag contents with the
	 * supported file types in the applications plist, these should
	 * always be correct paths,
	 * however it can not hurt to validate
	 */
	if (g_file_test(path, G_FILE_TEST_EXISTS )) {
		if (g_file_test(path, G_FILE_TEST_IS_REGULAR )) {
			// the path is a file - extract dir name and continue
			path2 = g_path_get_dirname(path);
			g_free(path);
			path = g_strdup(path2);
		} 
		if (g_file_test(path, G_FILE_TEST_IS_DIR )) {
			// the path is a directory - all well do nothing and continue
			fileIsValid = TRUE;
		} else {
			// The file path is neither a dir nor a regular file, so we can't handle it
		}
	} else {
		// If the file doesn't exist just return and ignore
	}
	
	if (fileIsValid)
		Browser_Tree_Select_Dir(path);
	
	g_free(path);
	g_free(path2);

   /* return FALSE to be automatically removed from the list of event sources and will not be called again */
   return FALSE;
}

/*
 * Mediator function between the Apple-App-RunLoop and the GTK-RunLoop
 */
gboolean osx_open_file_cb (GtkOSXApplication *app, gchar *path, gpointer user_data)
{
	gchar *test = g_strdup(path);
	
	/* Schedule the open File action to be performed on the gtk main loop
	 * thread, when it's available again and idle
	 * We cannot call UI actions here directly, this would force the Apple Event loop
	 * and thus the application to crash
	 */
	gdk_threads_add_idle(osx_open_file_idle_callback, (gpointer) test);
	
	/* return true to indicate we handled the NSOpenApplication */
	return TRUE;
}

#endif