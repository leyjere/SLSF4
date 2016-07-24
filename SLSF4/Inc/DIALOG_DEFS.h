#include "GUI.h"
#include "MULTIPAGE.h"
/*********************************************************************
*
*       Dialog resource
*
*  These tables contain the information required to create the dialogs.
*  It has been created manually, but could also be created by the GUIBuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, FRAMEWIN_CF_MOVEABLE},
  { TEXT_CreateIndirect, "File:", ID_TEXT_0, 55, 42, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Begin", ID_BUTTON_0, 65, 220, 100, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_1, 80, 43, 120, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Init Index:", ID_TEXT_13, 27, 72, 80, 20, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "Edit", ID_EDIT_6, 90, 70, 80, 20, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, "CW", ID_BUTTON_6, 100, 160, 60, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "CCW", ID_BUTTON_7, 165, 160, 60, 40, 0, 0x0, 0 },
	{ RADIO_CreateIndirect, "Rad", ID_RAD_0, 65, 110, 80, 0x0, 0x0, 0x2203},
	{ TEXT_CreateIndirect, "SRC", ID_TEXT_14, 30, 110, 40, 15, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "SNK", ID_TEXT_15, 30, 145, 40, 15, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "RLR", ID_TEXT_16, 30, 180, 40, 15, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Stepper Adjustment:", ID_TEXT_17, 115, 110, 105, 20, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "Edit", ID_EDIT_8, 110, 130, 105, 20, 0, 0x64, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] = {
  { WINDOW_CreateIndirect,    NULL,                ID_WINDOW_1,                   0,   76, 240, 264, FRAMEWIN_CF_MOVEABLE },
  { TREEVIEW_CreateIndirect,  NULL,                GUI_ID_TREEVIEW0,    0,   0, 240, 220 },
	{ BUTTON_CreateIndirect, "Select", 							 ID_BUTTON_1, 75, 224, 80, 40, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate3[] = {
  { WINDOW_CreateIndirect, NULL, ID_WINDOW_2, 0, 76, 240, 244, FRAMEWIN_CF_MOVEABLE },
  { TEXT_CreateIndirect, "X Dimension", ID_TEXT_4, 28, 24, 66, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Y Dimension", ID_TEXT_5, 26, 53, 62, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 104, 20, 80, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_1, 104, 50, 80, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_2, 104, 80, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "PWM", ID_TEXT_6, 59, 81, 39, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_3, 104, 110, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Delay Factor", ID_TEXT_8, 34, 111, 61, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Test", ID_BUTTON_4, 74, 200, 80, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Height", ID_TEXT_6, 59, 142, 39, 20, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "Edit", ID_EDIT_5, 104, 140, 80, 20, 0, 0x64, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogPrinting[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 40, 100, 160, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_BUTTON_2, 15, 64, 50, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Pause", ID_BUTTON_3, 83, 64, 50, 20, 0, 0x0, 0 },
  { PROGBAR_CreateIndirect, "Progbar", ID_PROGBAR_0, 34, 11, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Index:", ID_TEXT_11, 30, 41, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "0", ID_TEXT_12, 67, 41, 80, 20, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogAreaError[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_1, 40, 100, 160, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_5, 55, 60, 50, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Scan area too small!", ID_TEXT_10, 30, 20, 100, 30, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogKeypad[] = {
  { WINDOW_CreateIndirect, NULL, ID_WINDOW_3, 5, 65, 240, 250, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_4, 0, 0, 230, 50, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "1", ID_NUM_1, 0, 70, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2", ID_NUM_2, 80, 70, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3", ID_NUM_3, 160, 70, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4", ID_NUM_4, 0, 115, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5", ID_NUM_5, 80, 115, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6", ID_NUM_6, 160, 115, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "7", ID_NUM_7, 0, 160, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "8", ID_NUM_8, 80, 160, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "9", ID_NUM_9, 161, 160, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "0", ID_NUM_0, 80, 205, 70, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "BS", ID_NUM_BS, 0, 205, 70, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_NUM_OK, 160, 205, 70, 40, 0, 0x0, 0 },
};

