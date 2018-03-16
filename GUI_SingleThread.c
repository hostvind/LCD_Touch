
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "GUI.h"
#include "dialog.h"
WM_HWIN hWin;
PROGBAR_Handle hPrB;
#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

/*----------------------------------------------------------------------------
 *      GUIThread: GUI Thread for Single-Task Execution Model
 *---------------------------------------------------------------------------*/
 
void GUIThread (void const *argument);              // thread function
osThreadId tid_GUIThread;                           // thread id
osThreadDef (GUIThread, osPriorityIdle, 1, 2048);   // thread object

static void wincallback(WM_MESSAGE* pMsg){
GUI_RECT Rect;
  switch (pMsg->MsgId)
    {
    	case WM_PAINT:
            WM_GetInsideRect(&Rect);
            GUI_SetBkColor(GUI_MAGENTA);
            GUI_SetColor(GUI_YELLOW);
            GUI_ClearRectEx(&Rect);
            GUI_DrawRectEx(&Rect);
            GUI_SetFont(&GUI_Font8x16);
            GUI_DispStringHCenterAt("ADC Channel 0", 220,20);
            GUI_DrawLine(20,35,360,35);        
    		break;
    	default:
        WM_DefaultProc(pMsg);
    }  
}

int Init_GUIThread (void) {

  tid_GUIThread = osThreadCreate (osThread(GUIThread), NULL);
  if (!tid_GUIThread) return(-1);
  
  return(0);
}

void GUIThread (void const *argument) {

  GUI_Init();                     /* Initialize the Graphics Component */
hWin=FRAMEWIN_Create("Main Window",wincallback,WM_CF_SHOW,0,0,480,272);
hPrB=PROGBAR_CreateEx(    110,180,160,25,hWin,WM_CF_SHOW,0,0);
  /* Add GUI setup code here */

  while (1) {
    
    /* All GUI related activities might only be called from here */
#ifdef RTE_Graphics_Touchscreen
        GUI_TOUCH_Exec();
#endif
    PROGBAR_SetValue(hPrB, 35);
    GUI_Exec();                   /* Execute all GUI jobs ... Return 0 if nothing was done. */
    GUI_X_ExecIdle();             /* Nothing left to do for the moment ... Idle processing */
  }
}

