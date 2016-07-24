/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2004  SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : WIDGET_Multipage.c
Purpose     : Demonstrates the use of the MULTIPAGE widget
Requirements: WindowManager - (x)
              MemoryDevices - (x)
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include <stdio.h>
#include "GUI.h"
#include "DIALOG_DEFS.h"
#include "MULTIPAGE.h"
#include "GUI_Task.h"
#include "PARSE_Task.h"
#include "STEPPER_Intf.h"
#include "main.h"
#include "fatfs.h"
#include "string.h"

#define NUM_CHILD_NODES 3
#define NUM_CHILD_ITEMS 6
#define TREEVIEW_DEPTH  5

extern UART_HandleTypeDef huart1;
extern osThreadId PARSE_Handle;
extern osThreadId TS_Handle;
extern UART_HandleTypeDef huart1;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Recommended memory to run the sample with adequate performance
//
#define RECOMMENDED_MEMORY (1024L * 20)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static WM_HWIN hMultiPage, hDlgHome, hDlgSelect, hDlgConfig;
static WM_HWIN hFrameWin, hPopup, hKeypad;
static U8 filename[64];
static WM_MESSAGE msg;
static int cEdit = 0;
static int txtL = 0;
static char editT0[32], editT1[32], editT2[32], editT3[32], editT5[32] = "500", editT6[32], editT8[32] = "2";

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static void updateKeypad(int NCode, int numId){
	WM_HWIN hItem;
	
	switch(NCode){
		case WM_NOTIFICATION_CLICKED:
			
			hItem = WM_GetDialogItem(hKeypad, ID_EDIT_4);
		
			if(cEdit == ID_EDIT_0){
				editT0[txtL++] = 48 + (numId - 0x333);
				EDIT_SetText(hItem, (char *)editT0);
			}else if(cEdit == ID_EDIT_1){
				editT1[txtL++] = 48 + (numId - 0x333);
				EDIT_SetText(hItem, (char *)editT1);
			}else if(cEdit == ID_EDIT_2){
				editT2[txtL++] = 48 + (numId - 0x333);
				EDIT_SetText(hItem, (char *)editT2);
			}else if(cEdit == ID_EDIT_3){
				editT3[txtL++] = 48 + (numId - 0x333);
				EDIT_SetText(hItem, (char *)editT3);
			}else if(cEdit == ID_EDIT_5){
				editT5[txtL++] = 48 + (numId - 0x333);
				EDIT_SetText(hItem, (char *)editT5);
			}else if(cEdit == ID_EDIT_6){
				editT6[txtL++] = 48 + (numId - 0x333);
				EDIT_SetText(hItem, (char *)editT6);
			}else if(cEdit == ID_EDIT_8){
				editT8[txtL++] = 48 + (numId - 0x333);
				EDIT_SetText(hItem, (char *)editT8);
			}
			
			break;
		case WM_NOTIFICATION_USER:
			if(txtL >= 0){
				hItem = WM_GetDialogItem(hKeypad, ID_EDIT_4);
				if(cEdit == ID_EDIT_0){
					editT0[txtL] = 32;
					EDIT_SetText(hItem, (char *)editT0);
				}else if(cEdit == ID_EDIT_1){
					editT1[txtL] = 32;
					EDIT_SetText(hItem, (char *)editT1);
				}else if(cEdit == ID_EDIT_2){
					editT2[txtL] = 32;
					EDIT_SetText(hItem, (char *)editT2);
				}else if(cEdit == ID_EDIT_3){
					editT3[txtL] = 32;
					EDIT_SetText(hItem, (char *)editT3);
				}else if(cEdit == ID_EDIT_5){
					editT5[txtL] = 32;
					EDIT_SetText(hItem, (char *)editT5);
				}else if(cEdit == ID_EDIT_6){
					editT6[txtL] = 32;
					EDIT_SetText(hItem, (char *)editT6);
				}else if(cEdit == ID_EDIT_8){
					editT8[txtL] = 32;
					EDIT_SetText(hItem, (char *)editT8);
				}
				if(txtL > 0) txtL--;
			}
			break;
		default:
			break;
	}
}


static void _cbDialogKeypad(WM_MESSAGE * pMsg) {
	int NCode;
	int Id;
	
  switch (pMsg->MsgId) {
		
		case WM_NOTIFY_PARENT:
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
		
			if(Id == ID_NUM_0 || Id == ID_NUM_1 || Id == ID_NUM_2 || Id == ID_NUM_3 || Id == ID_NUM_4 ||
				Id == ID_NUM_5 || Id == ID_NUM_6 || Id == ID_NUM_7 || Id == ID_NUM_8 || Id == ID_NUM_9){
				updateKeypad(NCode, Id);
			} else if(Id == ID_NUM_OK){
									
				if(cEdit == ID_EDIT_2){
					if(atoi(editT2) <= 100){
						EDIT_SetText(WM_GetDialogItem(hDlgConfig, cEdit), (char *)editT2);
						setPWM(atoi(editT2));
					} else {
						memset(editT2, 0, 32);
						editT2[0] = '1'; editT2[1] = '0'; editT2[2] = '0';
						EDIT_SetText(WM_GetDialogItem(hDlgConfig, cEdit), (char *)editT2);
						setPWM(atoi(editT2));
					}
				}else if(cEdit == ID_EDIT_3){
					EDIT_SetText(WM_GetDialogItem(hDlgConfig, cEdit), (char *)editT3);
					setDelay(atoi(editT3));
				}else if(cEdit == ID_EDIT_5){
					EDIT_SetText(WM_GetDialogItem(hDlgConfig, cEdit), (char *)editT5);
					int len;
					len = setHeight(atoi(editT5));
					sprintf(editT0, "%ld", (long)len*2);
					sprintf(editT1, "%ld", (long)len*2);
					EDIT_SetText(WM_GetDialogItem(hDlgConfig, ID_EDIT_0), (char *)editT0);
					EDIT_SetText(WM_GetDialogItem(hDlgConfig, ID_EDIT_1), (char *)editT1);
					EDIT_SetBkColor(WM_GetDialogItem(hDlgConfig, ID_EDIT_5), EDIT_CI_ENABLED, GUI_WHITE);
					EDIT_SetBkColor(WM_GetDialogItem(hDlgConfig, ID_EDIT_0), EDIT_CI_ENABLED, GUI_WHITE);
					EDIT_SetBkColor(WM_GetDialogItem(hDlgConfig, ID_EDIT_1), EDIT_CI_ENABLED, GUI_WHITE);
				}else if(cEdit == ID_EDIT_6){
					EDIT_SetText(WM_GetDialogItem(hDlgHome, cEdit), (char *)editT6);
					setFP((unsigned long)atoi(editT6));
				}else if(cEdit == ID_EDIT_8){
					EDIT_SetText(WM_GetDialogItem(hDlgHome, cEdit), (char *)editT8);
				}
				WM_BringToBottom(hKeypad);
				txtL = 0;
			} else if (Id == ID_NUM_BS){
					switch(NCode){
						case WM_NOTIFICATION_CLICKED:
							updateKeypad(WM_NOTIFICATION_USER, 0);
							break;
						default:
							break;
					}
			}
			break;
		case WM_INIT_DIALOG:
			break;
		default:
			WM_DefaultProc(pMsg);
  }
}

static void setupKeypad(){
	WM_HWIN hItem;

	if(!hKeypad){
		hKeypad = GUI_CreateDialogBox(_aDialogKeypad, GUI_COUNTOF(_aDialogKeypad), _cbDialogKeypad, NULL, 0, 0);
	} else {
		WM_BringToTop(hKeypad);
	}
	hItem = WM_GetDialogItem(hKeypad, ID_EDIT_4);
	if(cEdit == ID_EDIT_0){
		memset(editT0, 0, 32);
		EDIT_SetText(hItem, editT0);
	} else if (cEdit == ID_EDIT_1){
		memset(editT1, 0, 32);
		EDIT_SetText(hItem, editT1);
	} else if (cEdit == ID_EDIT_2){
		memset(editT2, 0, 32);
		EDIT_SetText(hItem, editT2);
	} else if (cEdit == ID_EDIT_3){
		memset(editT3, 0, 32);
		EDIT_SetText(hItem, editT3);
	}	else if (cEdit == ID_EDIT_5){
		memset(editT5, 0, 32);
		EDIT_SetText(hItem, editT5);
	}	else if (cEdit == ID_EDIT_6){
		memset(editT6, 0, 32);
		EDIT_SetText(hItem, editT6);
	} else if (cEdit == ID_EDIT_8){
		memset(editT8, 0, 32);
		EDIT_SetText(hItem, editT8);
	}
}


static void _cbDialog3(WM_MESSAGE * pMsg) {
	int NCode;
	int Id;
	static int init;
	
  switch (pMsg->MsgId) {

	case WM_NOTIFY_PARENT:
		
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
	
		switch(Id){
				case ID_EDIT_0:
					switch(NCode){
						case WM_NOTIFICATION_CLICKED:
							cEdit = ID_EDIT_0;
							break;
						default:
							break;
					}
					break;
				case ID_EDIT_1:
					switch(NCode){
						case WM_NOTIFICATION_CLICKED:
							cEdit = ID_EDIT_1;
							break;
						default:
							break;
					}
					break;
				case ID_EDIT_2:
					switch(NCode){
						case WM_NOTIFICATION_CLICKED:
							cEdit = ID_EDIT_2;
							setupKeypad();
							break;
						default:
							break;
					}
					break;
				case ID_EDIT_3:
					switch(NCode){
						case WM_NOTIFICATION_CLICKED:
							cEdit = ID_EDIT_3;
							setupKeypad();
							break;
						default:
							break;
					}
					break;
				case ID_EDIT_5:
					switch(NCode){
						case WM_NOTIFICATION_CLICKED:
							cEdit = ID_EDIT_5;
							setupKeypad();
							break;
						default:
							break;
					}
					break;
				case ID_BUTTON_4:
					switch(NCode){
						case WM_NOTIFICATION_CLICKED:
							xTaskCreate(TEST_Task, "Thread 4", 512, NULL, tskIDLE_PRIORITY+1, 0);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		break;
	case WM_POST_PAINT:
		if(!init){
			EDIT_SetText(WM_GetDialogItem(hDlgConfig, ID_EDIT_0), "576");
			EDIT_SetText(WM_GetDialogItem(hDlgConfig, ID_EDIT_1), "576");
			EDIT_SetText(WM_GetDialogItem(hDlgConfig, ID_EDIT_2), "50");
			EDIT_SetText(WM_GetDialogItem(hDlgConfig, ID_EDIT_3), "10");
			EDIT_SetText(WM_GetDialogItem(hDlgConfig, ID_EDIT_5), "500");
			EDIT_SetText(WM_GetDialogItem(hDlgHome, ID_EDIT_6), "0");
			EDIT_SetText(WM_GetDialogItem(hDlgHome, ID_EDIT_8), "2");
			init = 1;
		}
		break;
  default:
    WM_DefaultProc(pMsg);
  }
}


static void _cbDialog2(WM_MESSAGE * pMsg) {
  WM_HWIN hItem, hTree;
  WM_HWIN hDlg;
	int NCode;
	int Id;

  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id){
			case ID_BUTTON_1:
				switch(NCode){
					case WM_NOTIFICATION_CLICKED:
						hTree = WM_GetDialogItem(hDlg, GUI_ID_TREEVIEW0);
						hItem = TREEVIEW_GetSel(hTree);
						//Temp solution until we learn how to get parent names for the sake of a full path
						TREEVIEW_ITEM_GetText(hItem, (unsigned char *)filename, 64);
						openFile((char *)filename);
					
						GUI_Delay(100);
					
						MULTIPAGE_SelectPage(hMultiPage, 0);
					
						msg.MsgId = WM_USER;
						msg.hWinSrc = pMsg->hWin;
						msg.hWin = hDlgHome;
						msg.Data.p = &filename;
						WM_SendMessage(WM_HBKWIN, &msg);
					
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
				}
			default:
				break;
		}
	
		break;
  case WM_INIT_DIALOG:
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

static void _cbDialogAreaError(WM_MESSAGE * pMsg) {

	int NCode;
	int Id;

  switch (pMsg->MsgId) {
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id){
			case ID_BUTTON_5:
				switch(NCode){
					case WM_NOTIFICATION_CLICKED:
						GUI_EndDialog(pMsg->hWin, 0);
						MULTIPAGE_SelectPage(hMultiPage, 2);
						EDIT_SetBkColor(WM_GetDialogItem(hDlgConfig, ID_EDIT_0), EDIT_CI_ENABLED, GUI_YELLOW);
						EDIT_SetBkColor(WM_GetDialogItem(hDlgConfig, ID_EDIT_1), EDIT_CI_ENABLED, GUI_YELLOW);
						EDIT_SetBkColor(WM_GetDialogItem(hDlgConfig, ID_EDIT_5), EDIT_CI_ENABLED, GUI_RED);
						GUI_Exec();
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
				}
			default:
				break;
		}
	
		break;
  case WM_INIT_DIALOG:
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}


void areaError(){
	GUI_EndDialog(hPopup, 0);
	hPopup = GUI_CreateDialogBox(_aDialogAreaError, GUI_COUNTOF(_aDialogAreaError), _cbDialogAreaError, NULL, 0, 0);
	vTaskDelete(PARSE_Handle);
}

void updateProgbar(unsigned long fp, unsigned long fsize){
	char num[8];
	PROGBAR_SetValue(WM_GetDialogItem(hPopup, ID_PROGBAR_0), (fp/(float)fsize)*100);
	sprintf(num, "%ld", (long)fp);
	TEXT_SetText(WM_GetDialogItem(hPopup, ID_TEXT_12), num);
	GUI_Exec();
}

static void _cbPrintDialog(WM_MESSAGE * pMsg){
	static int pause = 0;
  WM_HWIN hDlg;
	int NCode;
	int Id;

  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id){
			case ID_BUTTON_2:	//Cancel
				switch(NCode){
					case WM_NOTIFICATION_CLICKED:
						vTaskDelete(PARSE_Handle);
						GUI_EndDialog(pMsg->hWin, 0);
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
				}
				break;
			case ID_BUTTON_3:	//Pause
				switch(NCode){
					case WM_NOTIFICATION_CLICKED:
						if(pause){
							vTaskSuspend(PARSE_Handle);
							BUTTON_SetText(WM_GetDialogItem(hDlg, ID_BUTTON_3), "Resume");
						} else {
							vTaskResume(PARSE_Handle);
							BUTTON_SetText(WM_GetDialogItem(hDlg, ID_BUTTON_3), "Pause");
						}
						pause = ~pause;
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
				}
				break;
			default:
				break;
		}
	
		break;
  case WM_INIT_DIALOG:
    break;
  default:
    WM_DefaultProc(pMsg);
  }
	
	
}

static void _cbDialog1(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int NCode, Id;
	static enum stepper_motor motor = STEPPER_PWDR_SRC;
	
  switch (pMsg->MsgId) {
		
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		
		switch(Id){
			case ID_BUTTON_0:	//Begin
				switch(NCode){
					case WM_NOTIFICATION_CLICKED:
							xTaskCreate(PARSE_Task, "Thread 1", 1024, NULL, tskIDLE_PRIORITY+1, &PARSE_Handle);
							hPopup = GUI_CreateDialogBox(_aDialogPrinting, GUI_COUNTOF(_aDialogPrinting), _cbPrintDialog, NULL, 0, 0);
							WM_MakeModal(hPopup);
							//vTaskResume(PARSE_Handle);
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
				}
				break;
			case ID_RAD_0:
				switch(NCode){
					case WM_NOTIFICATION_VALUE_CHANGED:
						hItem = WM_GetDialogItem(pMsg->hWin, ID_RAD_0);
						Id = RADIO_GetValue(hItem);
					
						if(Id == 0) motor = STEPPER_PWDR_SRC;
						else if(Id == 1) motor = STEPPER_PWDR_SINK;
						else if(Id == 2) motor = STEPPER_ROLLER;
					
						break;
				}
				break;
			case ID_BUTTON_6:	//FWD
				switch(NCode){
					case WM_NOTIFICATION_CLICKED:
						
						if(motor != STEPPER_ROLLER){
							translate(motor, atof(editT8), 2, STEPPER_CW);
						} else {
							translate(motor, atof(editT8), 8, STEPPER_CW);
						}

						break;
					case WM_NOTIFICATION_RELEASED:
						break;
				}
				break;
			case ID_BUTTON_7:	//BWD
				switch(NCode){
					case WM_NOTIFICATION_CLICKED:
						
						if(motor != STEPPER_ROLLER){
							translate(motor, atof(editT8), 2, STEPPER_CCW);
						} else {
							translate(motor, atof(editT8), 8, STEPPER_CCW);
						}
						
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
				}
				break;
			case ID_EDIT_6:
				switch(NCode){
					case WM_NOTIFICATION_CLICKED:
						cEdit = ID_EDIT_6;
						setupKeypad();
						break;
					default:
						break;
				}
				break;
			case ID_EDIT_8:
				switch(NCode){
					case WM_NOTIFICATION_CLICKED:
						cEdit = ID_EDIT_8;
						setupKeypad();
						break;
					default:
						break;
				}
				break;
		}
  default:
    WM_DefaultProc(pMsg);
  }

}

/*********************************************************************
*
*       _cbBkWindow
*
*  Function description
*    Callback routine of the background window.
*    It shows the sample title and draws the background.
*/
static void _cbBkWindow(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;

  switch (pMsg->MsgId) {
		
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    break;
	case WM_USER:
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
		TEXT_SetText(hItem, (char *)filename);
		break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

static int _FillNode(WM_HWIN hTree, TREEVIEW_ITEM_Handle hNode, char path[]){
	
	TREEVIEW_ITEM_Handle hItem;
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	int file_count = 0;
	char *fn;   /* This function assumes non-Unicode configuration */
#if _USE_LFN
	static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
	fno.lfname = lfn;
	fno.lfsize = sizeof lfn;
#endif
	
		res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        i = strlen(path);
			
        for (;;) {
					
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
							
								char new_path[128];
								strcpy(new_path, path);
								strcat(new_path, "/");
								strcat(new_path, fn);
								// Need to file paths
								hItem = TREEVIEW_ITEM_Create(1, fn, 0);
								if (hItem == 0) {
									return 1;    // Error
								}
								
								TREEVIEW_AttachItem(hTree, hItem, hNode, TREEVIEW_INSERT_FIRST_CHILD);
								
								_FillNode(hTree, hItem, new_path);
								
                path[i] = 0;
								
            } else {                                      /* It is a file. */
							
							hItem = TREEVIEW_ITEM_Create(0, fn, 0);
							if (hItem == 0) {
								return 1;    // Error
							}

							TREEVIEW_AttachItem(hTree, hItem, hNode, TREEVIEW_INSERT_FIRST_CHILD);
							
							file_count++;

            }
        }
        f_closedir(&dir);
    }

    return 0;
	
}

static int _InitTreeview(WM_HWIN hTree) {
  TREEVIEW_ITEM_Handle hNode;
  int r;

  hNode = TREEVIEW_InsertItem(hTree, TREEVIEW_ITEM_TYPE_NODE, 0, 0, "SD Root");
	
	char * path;
	getPath(path);
	
  r = _FillNode(hTree, hNode, "");
	
  if (r) {
    //
    // Error message
    //
    WM_DeleteWindow(hTree);
    GUI_MessageBox("Error", "Not enough memory available!", 0);
    return 1;      // Error
  }
  TREEVIEW_SetAutoScrollH(hTree, 1);
  TREEVIEW_SetAutoScrollV(hTree, 1);
  TREEVIEW_ITEM_Expand(hNode);
  return 0;
}
/*********************************************************************
*
*       MainTask
*/
void CreateGUI(void) {
	WM_HWIN hItem;

  //
  // Check if recommended memory for the sample is available
  //
  if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY) {
    GUI_ErrorOut("Not enough memory available."); 
    return;
  }
	
  WM_SetCallback(WM_HBKWIN, _cbBkWindow);
  //
  // Create the frame window
  //
  hFrameWin = FRAMEWIN_Create("SLSF4", NULL, WM_CF_SHOW, 0, 0, 240, 320);
  FRAMEWIN_SetClientColor(hFrameWin, GUI_BLACK);
  FRAMEWIN_SetActive(hFrameWin, 1);
  FRAMEWIN_SetMoveable(hFrameWin, 0);
  //
  // Create the MULTIPAGE widget
  //
  hMultiPage = MULTIPAGE_CreateEx(0, 0, 240, 320, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, 0);
	MULTIPAGE_SetTabHeight(hMultiPage, 30);
  GUI_Delay(100);
  //
  // Create and attach the MULTIPAGE dialog windows
  //
  hDlgHome = GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), _cbDialog1, WM_HBKWIN, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDlgHome, "Home");
	MULTIPAGE_SetTabWidth(hMultiPage, 75, 0);
  GUI_Delay(100);
	
  hDlgSelect = GUI_CreateDialogBox(_aDialogCreate2, GUI_COUNTOF(_aDialogCreate2), _cbDialog2,       WM_HBKWIN, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDlgSelect, "Select");
	MULTIPAGE_SetTabWidth(hMultiPage, 75, 1);
	hItem = WM_GetDialogItem(hDlgSelect, GUI_ID_TREEVIEW0);
	_InitTreeview(hItem);
  GUI_Delay(100);
	
  hDlgConfig = GUI_CreateDialogBox(_aDialogCreate3, GUI_COUNTOF(_aDialogCreate3), _cbDialog3,       WM_HBKWIN, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDlgConfig, "Config");
	MULTIPAGE_SetTabWidth(hMultiPage, 75, 2);
  GUI_Delay(100);
  //
  // Demonstrate the use of MULTIPAGE_SetAlign
  //
  MULTIPAGE_SetAlign(hMultiPage, MULTIPAGE_ALIGN_LEFT);
	MULTIPAGE_SelectPage(hMultiPage, 2);
  GUI_Delay(100);
	
	//vTaskResume(PARSE_Handle);
	vTaskResume(TS_Handle);
}

void GUI_Task(void * pvParams){
	CreateGUI();
	while(1){
		GUI_Delay(10);
		vTaskSuspend(NULL);
	}
}
