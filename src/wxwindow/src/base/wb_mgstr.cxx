/*
 * File:      wb_mgstr.cc
 * Purpose:     simple language support
 * Author:      Julian Smart
 * Created:     1993
 * Updated:     August 1994
 * RCS_ID:      $Id: wb_mgstr.cxx,v 1.1.1.1 1997/12/22 16:11:56 mflatt Exp $
 * Copyright:   (c) 1993, AIAI, University of Edinburgh
 */

// #include "wx.h" // Uncomment this line for Borland precomp. headers to work

/* static const char sccsid[] = "@(#)wb_mgstr.cc	1.2 5/9/94"; */

#if defined(_MSC_VER)
# include "wx.h"
#else

#ifdef __GNUG__
#pragma implementation
#endif

#include "common.h"
#include "wx_setup.h"
#include "wx_utils.h"
#include "wx_main.h"

#endif

#include "ctype.h"

static const char *msg_english[] =
{
/* wxSTR_ERROR               */ "Error",
/* wxSTR_WARNING             */ "Warning",

/* wxSTR_YES                 */ "Yes",
/* wxSTR_NO                  */ "No",

/* wxSTR_BUTTON_OK           */ "OK",
/* wxSTR_BUTTON_CANCEL       */ "Cancel",
/* wxSTR_BUTTON_REVERT       */ "Revert",
/* wxSTR_BUTTON_UPDATE       */ "Update",
/* wxSTR_CONSTRAINT_VIOLATION*/ "Constraint violation",
/* wxSTR_VIOLATION_FLOAT     */ "%s should be in range %.2f to %.2f",
/* wxSTR_VIOLATION_LONG      */ "%s should be in range %.0f to %.0f",
/* wxSTR_MENU_HELP           */ "Help",
/* wxSTR_BUTTON_HELP         */ "Help",

/* wxSTR_FILE_SELECTOR       */ "File selector",
/* wxSTR_OVERWRITE_FILE      */ "Overwrite existing file %s?",
/* wxSTR_LOAD_FILE           */ "Load %s file",
/* wxSTR_SAVE_FILE           */ "Save %s file",
/* wxSTR_ENTER_FILENAME      */ "Enter %s file name",
/* wxSTR_LABEL_FILENAME      */ "Name",
/* wxSTR_LABEL_PATH          */ "Path",
/* wxSTR_LABEL_FILES         */ "Files",
/* wxSTR_LABEL_DIRS          */ "Directories",

/* wxSTR_HELP_TIMEOUT        */ "Connection to wxHelp timed out in %d seconds",
 NULL
};

static const char *msg_german[] =
{
/* wxSTR_ERROR               */ "Fehler",
/* wxSTR_WARNING             */ "Warnung",

/* wxSTR_YES                 */ "Ja",
/* wxSTR_NO                  */ "Nein",

/* wxSTR_BUTTON_OK           */ "OK",
/* wxSTR_BUTTON_CANCEL       */ "Abbrechen",
/* wxSTR_BUTTON_REVERT       */ "Reset",
/* wxSTR_BUTTON_UPDATE       */ "Aktualisieren",
/* wxSTR_CONSTRAINT_VIOLATION*/ "Wertebereich Verletzung",
/* wxSTR_VIOLATION_FLOAT     */ "%s liegt nicht im Bereich von %.2f bis %.2f",
/* wxSTR_VIOLATION_LONG      */ "%s liegt nicht im Bereich von %.0f bis %.0f",
/* wxSTR_MENU_HELP           */ "Hilfe",
/* wxSTR_BUTTON_HELP         */ "Hilfe",

/* wxSTR_FILE_SELECTOR       */ "Datai Auswahl",
/* wxSTR_OVERWRITE_FILE      */ "‹berschriebe datai %s?",
/* wxSTR_LOAD_FILE           */ "Lese %s-Datai",
/* wxSTR_SAVE_FILE           */ "Speiche %s-Datai",
/* wxSTR_ENTER_FILENAME      */ "%s Datainame Auswahl",
/* wxSTR_LABEL_FILENAME      */ "Name",
/* wxSTR_LABEL_PATH          */ "Pfad",
/* wxSTR_LABEL_FILES         */ "Dataien",
/* wxSTR_LABEL_DIRS          */ "Verzeichnis",

/* wxSTR_HELP_TIMEOUT        */ "wxHelp Verbindungsaufbau 'timeout' nach %d sec.",
 NULL
};

// To Do:
//      (0) Use the language of the platform NOT default
//          English
//      (1) extend support to other 8859-1 languages
//      (2) Use UTF for string encodings
//      (3) Add in sub-font mechanism for Unicode
//      (4) Add in support for non 8859 languages

// Match a string INDEPENDENT OF CASE
Bool String_Match (char *str1, char *str2)
{
  if (str1 == NULL || str2 == NULL)
    return FALSE;
  if (str1 == str2)
    return TRUE;

  int len1 = strlen (str1);
  int len2 = strlen (str2);
  int i;
  
  // Search for str1 in str2
  // Slow .... but acceptable for short strings
  for (i = 0; i <= len2 - len1; i++) {
    int j;
    for (j = 0; j < len2; j++) {
      if (toupper(str2[i + j]) != toupper(str1[j]))
	break;
    }
    if (j >= len2)
      return TRUE;
  }
  
  return FALSE;
}


void 
wxSetLanguage (wxlanguage_t language)
{
  switch (language)
    {
    case wxLANGUAGE_DEFAULT:
#if defined(wx_msw) && USE_RESOURCES
      char *lang;
      if (wxGetResource ("intl", "sLanguage", (char **)&lang, "WIN.INI") == FALSE) /* MATTHEW: BC */
	lang = copystring ("eng");
      if (String_Match (lang, "eng"))
	wxSetLanguage (wxLANGUAGE_ENGLISH);
      else if (String_Match (lang, "ger"))
	wxSetLanguage (wxLANGUAGE_GERMAN);
      else
	wxSetLanguage (wxLANGUAGE_ENGLISH);
      delete[]lang;
#else
      // Default is English
      wxSetLanguage (wxLANGUAGE_ENGLISH);
#endif
      break;
    case wxLANGUAGE_GERMAN:
      wx_msg_str = (char **) &(msg_german[0]);
      break;
    case wxLANGUAGE_ENGLISH:
    default:
      wx_msg_str = (char **) &(msg_english[0]);
      break;
    }
}
