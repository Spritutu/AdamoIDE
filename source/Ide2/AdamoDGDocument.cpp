// AdamoDGDocument.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "adamotemplates.h"
#include "AdamoDGDocument.h"
#include "adamocomm.inl"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGDocument

IMPLEMENT_DYNCREATE(CAdamoDGDocument, CDocument)

CAdamoDGDocument::CAdamoDGDocument()
{
}

BOOL CAdamoDGDocument::OnNewDocument()
{
    SetTitle (LOADSTRING(IDS_ADAMODGDOCUMENT_1));
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CAdamoDGDocument::~CAdamoDGDocument()
{
}


BEGIN_MESSAGE_MAP(CAdamoDGDocument, CDocument)
	//{{AFX_MSG_MAP(CAdamoDGDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGDocument diagnostics

#ifdef _DEBUG
void CAdamoDGDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CAdamoDGDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGDocument serialization

void CAdamoDGDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGDocument commands

void CAdamoDGDocument::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
    CShowErrorsTemplate* tmpl=(CShowErrorsTemplate*) GetDocTemplate ();
    if (tmpl)
        tmpl->SetActiveView (NULL);
	CDocument::OnCloseDocument();
}

/*
** UpdateTree : aggiorna l'albero con i record del file comm.log
*/
int CAdamoDGTraceCommFile::UpdateTree (CTreeView* tv)
{
    CAdamoCommHeader ach;
    CString str;
    time_t t;
    int nType, nImage;
    char pBuffer[COMMBUF];

    /* apriamo il file */
    ((CIdeApp*)AfxGetApp ())->BeginWaitCursor ();
    if (Open (GetPathName (), CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))   {
        /* il file evidentemente esiste, posizioniamoci sull'ultimo record */
        Seek (m_f, CFile::begin);
        /* comiciamo a leggere e riempiamo l'albero, il primo dato e' l'ora e la data */
        while (true)   {
            if (Read (&t, sizeof (time_t))!=sizeof (time_t))
                break;
            if (Read (&ach, sizeof (CAdamoCommHeader))!=sizeof (CAdamoCommHeader))
                break;
            if (Read (&pBuffer, ach.GetLength())!=ach.GetLength())
                break;
            m_fp[m_n]=m_f;
            m_f=m_f+sizeof (time_t)+sizeof (CAdamoCommHeader)+ach.GetLength();
            str=FormatItem (&t, &ach, pBuffer);
            GetCommandType (&ach, &nType);
            str.TrimRight();
            if (FiltersOk (ach.GetCmd(), nType))   {
                /* abbiamo passtao i filtri, possiamo aggiungerli all'albero */
                GetCommandImage (nType, &nImage);
                HTREEITEM ht=tv->GetTreeCtrl().InsertItem (str, nImage, nImage);
                tv->GetTreeCtrl().SetItemData (ht, m_fp[m_n]);
            }
            m_n++;
            /* abbiamo un numero massimo di record caricabili */
            if (m_n>=NMAX_TRACECOMMRECORDS)
                break;
        }
        /* chiudiamo il file */
        Close ();
    }
    ((CIdeApp*)AfxGetApp ())->EndWaitCursor ();
    return 0;
}

/*
** UpdateTree : aggiorna l'albero con i record del file comm.log
*/
int CAdamoDGTraceCommFile::ExplodeTree (CTreeView* tv, HTREEITEM ht)
{
    CAdamoCommHeader ach;
    int n=0, nImage;
    bool bNeAvanza=false;
    char pBuffer[COMMBUF], sz[256], *p;

    GetCommandImage (TYPE_DATA, &nImage);
    /* comiciamo con l'andare a prendere il buffer da scrivere */
    long l=(long)tv->GetTreeCtrl ().GetItemData (ht);
    /* apriamo il file */
    if (Open (GetPathName (), CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))   {
        /* il file evidentemente esiste, posizioniamoci sull'ultimo record */
        Seek (l+sizeof (time_t), CFile::begin);
        /* leggiamo i record a cui siamo interessati */
        Read (&ach, sizeof (CAdamoCommHeader));
        Read (&pBuffer, ach.GetLength());
        /* ora esplodiamo l'albero fino a quando il buffer non e' finito */
        p=sz;
        while (n<ach.GetLength())   {
            bNeAvanza=true;
            sprintf (p, "%02x.", (BYTE)pBuffer[n]);
            n++; p+=3;
            if (n%42==0)   {
                p--; *p='\0'; p=sz;
                tv->GetTreeCtrl ().InsertItem (sz, nImage, nImage, ht);
                bNeAvanza=false;
            }
        }
        if (bNeAvanza)   {
            p--; *p='\0'; p=sz;
            tv->GetTreeCtrl ().InsertItem (sz, nImage, nImage, ht);
        }
    }
    return 0;
}

/*
** GetPathName :
*/
CString CAdamoDGTraceCommFile::GetPathName ()
{
    CProject* pPrj=GETWS()->GetActiveProject();
    CString str;

    if (pPrj)
        str.Format ("%s\\%s\\%s.log", pPrj->GetProjectDir(), DIRLOGS, TRACECOMMFILE);
    return str;
}

/*
** FormatItem :
*/
CString CAdamoDGTraceCommFile::FormatItem (time_t* t, CAdamoCommHeader* ach, char* pBuffer)
{
    CString str;
    CString strHour=COleDateTime(*t).Format ("%H:%M:%S    ");
    CString strCmd;
    GetCommandString (ach->GetCmd (), str);
    strCmd.Format ("%-48s", str);
    strCmd.MakeUpper();
    CString strID;
    strID.Format ("%-16d", ach->GetID ());
    CString strLenght;
    strLenght.Format ("%-16d", ach->GetLength ());
    CString strChannel;
    strChannel.Format ("%-16d", ach->GetChannel ());
    CString strFrame;
    strFrame.Format ("%-16d", ach->GetFrameNumber ());
    str="   "+strChannel+strCmd+strHour+strID+strFrame+strLenght;
    return str;
}

/*
** GetCommandType :
*/
int CAdamoDGTraceCommFile::GetCommandType (CAdamoCommHeader* ach, int* nType)
{
    if (ach->GetFrameNumber ()==REQUEST_FRAME_RECEIVE)
        *nType=TYPE_ANSWER_REQUEST;
    else   {
        switch (ach->GetCmd ())   {    
            case CMD_SENDPROGRAM :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_SENDCONF :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_STARTPROGRAM :
                *nType=TYPE_COMMAND;
                break;
            case CMD_STOPPROGRAM :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GETADAMOSTATUS :
                *nType=TYPE_REQUEST;
                break;
            case CMD_QUIT :
                *nType=TYPE_COMMAND;
                break;
            case CMD_STARTDEBUG :
                *nType=TYPE_COMMAND;
                break;
            case CMD_STOPDEBUG :
                *nType=TYPE_COMMAND;
                break;
            case CMD_SENDDEBUGDATA :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_CLEARDEBUGDATA :
                *nType=TYPE_COMMAND;
                break;
            case CMD_INITDEBUGDATA :
                *nType=TYPE_COMMAND;
                break;
            case CMD_DEBUGCMD :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GETDEBUGDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETSTACKDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETLOCALDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETGLOBALDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETTASKDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_STARTOSC :
                *nType=TYPE_COMMAND;
                break;
            case CMD_STOPOSC :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GETOSCDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETOSCSTATUS :
                *nType=TYPE_REQUEST;
                break;
            case CMD_SENDOSCEVENTS :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_SENDOSCDATA :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_DELETESLOT :
                *nType=TYPE_COMMAND;
                break;
            case CMD_SETSLOTSTATUS :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GETGLBDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_SETGLBDATA :
                *nType=TYPE_COMMAND;
                break;
            case CMD_SENDMSGTOMB :
                *nType=TYPE_COMMAND;
                break;
            case CMD_EVENTOKMSGTOMB :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GETDEVICEDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_SETDEVICEDATA :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GETAXISDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETADAMOAXIS :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETAXISINFO :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GOTOPOS :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GOTOINC :
                *nType=TYPE_COMMAND;
                break;
            case CMD_LINEAR :
                *nType=TYPE_COMMAND;
                break;
            case CMD_LINEARINC :
                *nType=TYPE_COMMAND;
                break;
            case CMD_STOP :
                *nType=TYPE_COMMAND;
                break;
            case CMD_OPENLOOP :
                *nType=TYPE_COMMAND;
                break;
            case CMD_CLOSELOOP :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GETPARCNT :
                *nType=TYPE_REQUEST;
                break;
            case CMD_SENDCONFGEN :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_SENDSTRINGS :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_SENDCC :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_EXISTSYMBOL :
                *nType=TYPE_REQUEST;
                break;
            case CMD_SENDTEMPORARY :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_GETSLOTNUMBER :
                *nType=TYPE_REQUEST;
                break;
            case CMD_SETRESDATA :
                *nType=TYPE_COMMAND;
                break;
            case CMD_CLEARCCDATA :
                *nType=TYPE_COMMAND;
                break;
            case CMD_SENDGPROGRAM :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_STARTGPROGRAM :
                *nType=TYPE_COMMAND;
                break;
            case CMD_STOPGPROGRAM :
                *nType=TYPE_COMMAND;
                break;
            case CMD_CLEARGPROGRAM :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GETGSLOTINFO :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETGLINEINFO :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETGSOFTWAREINFO :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETGCARDINFO :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETGDEVICEINFO :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETTABLEKEYS :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETTABLEVALUES :
                *nType=TYPE_REQUEST;
                break;
            case CMD_SETTABLEVALUES :
                *nType=TYPE_COMMAND;
                break;
            case CMD_STARTCANSCAN :
                *nType=TYPE_COMMAND;
                break;
            case CMD_SETCANMODE :
                *nType=TYPE_COMMAND;
                break;
            case CMD_GETCANSCANSTATUS :
                *nType=TYPE_REQUEST;
                break;
            case CMD_GETCANSCANDATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_READ_WRITE_SDO :
                *nType=TYPE_COMMAND;
                break;
            case CMD_READ_WRITE_SDO_STATUS :
                *nType=TYPE_REQUEST;
                break;
            case CMD_READ_WRITE_SDO_DATA :
                *nType=TYPE_REQUEST;
                break;
            case CMD_READ_DEBUG_INFO :
                *nType=TYPE_REQUEST;
                break;
            case CMD_WRITE_DEBUG_INFO :
                *nType=TYPE_COMMAND;
                break;
            case CMD_READ_MGA023_PARAMETER :
                *nType=TYPE_REQUEST;
                break;
            case CMD_WRITE_MGA023_PARAMETER :
                *nType=TYPE_COMMAND;
                break;
            case CMD_SENDCONFHW :
                *nType=TYPE_DOWNLOAD;
                break;
            case CMD_READ_MGA023_STATUS :
                *nType=TYPE_REQUEST;
                break;
            case CMD_READ_MGA023_STATIONS :
                *nType=TYPE_REQUEST;
                break;
            case CMD_READ_MGA023_ALARM :
                *nType=TYPE_REQUEST;
                break;
            case CMD_MGA023_READY_ON :
                *nType=TYPE_COMMAND;
                break;
            case CMD_MGA023_SERVO_ON :
                *nType=TYPE_COMMAND;
                break;
            case CMD_MGA023_RESET_ALARM :
                *nType=TYPE_COMMAND;
                break;
            case CMD_MGA023_WRITE_ABS_POS :
                *nType=TYPE_COMMAND;
                break;
            case CMD_MGA023_READ_ABS_POS :
                *nType=TYPE_REQUEST;
                break;
            case CMD_MGA023_SET_ABS_POS :
                *nType=TYPE_COMMAND;
                break;
            case CMD_MGA023_INIT_SSCNET :
                *nType=TYPE_COMMAND;
                break;
            case CMD_STARTCANINITIALIZATION :
                *nType=TYPE_COMMAND;
                break;
            case CMDASYNC_CLOSE :
                *nType=TYPE_RECEIVE;
                break;
            case CMDASYNC_MBMESSAGE :
                *nType=TYPE_RECEIVE;
                break;
            case CMDASYNC_ERROR :
                *nType=TYPE_RECEIVE;
                break;
            case CMDASYNC_BREAK :
                *nType=TYPE_RECEIVE;
                break;
            case CMDASYNC_START :
                *nType=TYPE_RECEIVE;
                break;
            case CMDASYNC_STOP  :
                *nType=TYPE_RECEIVE;
                break;
            case CMDASYNC_STRING :
                *nType=TYPE_RECEIVE;
                break;
            case CMDASYNC_CONNECT :
                *nType=TYPE_RECEIVE;
                break;
            case CMDASYNC_DISCONNECT :
                *nType=TYPE_RECEIVE;
                break;
        }
    }
    return 0;
}

/*
** GetCommandString :
*/
int CAdamoDGTraceCommFile::GetCommandString (int nCmd, CString& strCmd)
{
    switch (nCmd)   {
        case CMD_SENDPROGRAM :
            strCmd="SEND_PROGRAM";
            break;
        case CMD_SENDCONF :
            strCmd="SEND_CONFIGURATION";
            break;
        case CMD_STARTPROGRAM :
            strCmd="START_PROGRAM";
            break;
        case CMD_STOPPROGRAM :
            strCmd="STOP_PROGRAM";
            break;
        case CMD_GETADAMOSTATUS :
            strCmd="GET_STATUS";
            break;
        case CMD_QUIT :
            strCmd="QUIT";
            break;
        case CMD_STARTDEBUG :
            strCmd="START_DEBUG";
            break;
        case CMD_STOPDEBUG :
            strCmd="STOP_DEBUG";
            break;
        case CMD_SENDDEBUGDATA :
            strCmd="SEND_DEBUG_DATA";
            break;
        case CMD_CLEARDEBUGDATA :
            strCmd="CLEAR_DEBUG_DATA";
            break;
        case CMD_INITDEBUGDATA :
            strCmd="INIT_DEBUG_DATA";
            break;
        case CMD_DEBUGCMD :
            strCmd="SEND_DEBUG_COMMAND";
            break;
        case CMD_GETDEBUGDATA :
            strCmd="GET_DEBUG_DATA";
            break;
        case CMD_GETSTACKDATA :
            strCmd="GET_STACK_DATA";
            break;
        case CMD_GETLOCALDATA :
            strCmd="GET_LOCAL_DATA";
            break;
        case CMD_GETGLOBALDATA :
            strCmd="GET_EXPRESSION";
            break;
        case CMD_GETTASKDATA :
            strCmd="GET_TASK_DATA";
            break;
        case CMD_STARTOSC :
            strCmd="START_OSCILLOSCOPE";
            break;
        case CMD_STOPOSC :
            strCmd="STOP_OSCILLOSCOPE";
            break;
        case CMD_GETOSCDATA :
            strCmd="GET_OSCILLOSCOPE_DATA";
            break;
        case CMD_GETOSCSTATUS :
            strCmd="GET_OSCILLOSCOPE_STATUS";
            break;
        case CMD_SENDOSCEVENTS :
            strCmd="SEND_OSCILLOSCOPE_EVENTS";
            break;
        case CMD_SENDOSCDATA :
            strCmd="SEND_OSCILLOSCOPE_DATA";
            break;
        case CMD_DELETESLOT :
            strCmd="DELETE_SLOT";
            break;
        case CMD_SETSLOTSTATUS :
            strCmd="SET_SLOT_STATUS";
            break;
        case CMD_GETGLBDATA :
            strCmd="GET_GLOBAL_DATA";
            break;
        case CMD_SETGLBDATA :
            strCmd="SET_GLOBAL_DATA";
            break;
        case CMD_SENDMSGTOMB :
            strCmd="SEND_MAILBOX_EVENT";
            break;
        case CMD_EVENTOKMSGTOMB :
            strCmd="SEND_MAILBOX_ACK";
            break;
        case CMD_GETDEVICEDATA :
            strCmd="GET_DEVICE_DATA";
            break;
        case CMD_SETDEVICEDATA :
            strCmd="SET_DEVIC_DATA";
            break;
        case CMD_GETAXISDATA :
            strCmd="GET_AXIS_RUNTIME_DATA";
            break;
        case CMD_GETADAMOAXIS :
            strCmd="GET_AXIS_STATIC_DATA";
            break;
        case CMD_GETAXISINFO :
            strCmd="GET_AXIS_INFO";
            break;
        case CMD_GOTOPOS :
            strCmd="GOTO_POSITION";
            break;
        case CMD_GOTOINC :
            strCmd="GOTO_INCREMENTAL_POSITION";
            break;
        case CMD_LINEAR :
            strCmd="GOTO_INTERPOLATING";
            break;
        case CMD_LINEARINC :
            strCmd="GOTO_INCREMENTAL_INTERPOLATING";
            break;
        case CMD_STOP :
            strCmd="STOP_AXES";
            break;
        case CMD_OPENLOOP :
            strCmd="OPEN_LOOP";
            break;
        case CMD_CLOSELOOP :
            strCmd="CLOSE_LOOP";
            break;
        case CMD_GETPARCNT :
            strCmd="GET_CONFIGURATION_PARAMETERS";
            break;
        case CMD_SENDCONFGEN :
            strCmd="SEND_GENERAL_CONFIGURATION";
            break;
        case CMD_SENDSTRINGS :
            strCmd="SEND_USER_STRINGS";
            break;
        case CMD_SENDCC :
            strCmd="SEND_CROSS_CORRECTORS";
            break;
        case CMD_EXISTSYMBOL :
            strCmd="GET_EXIST_SYMBOL";
            break;
        case CMD_SENDTEMPORARY :
            strCmd="SEND_TEMPORARY";
            break;
        case CMD_GETSLOTNUMBER :
            strCmd="GET_SLOT_NUMBER";
            break;
        case CMD_SETRESDATA :
            strCmd="SET_RESOURCE_DATA";
            break;
        case CMD_CLEARCCDATA :
            strCmd="CLEAR_CROSS_CORRECTORS";
            break;
        case CMD_SENDGPROGRAM :
            strCmd="SEND_ISO_PROGRAM";
            break;
        case CMD_STARTGPROGRAM :
            strCmd="START_ISO_PROGRAM";
            break;
        case CMD_STOPGPROGRAM :
            strCmd="STOP_ISO_PROGRAM";
            break;
        case CMD_CLEARGPROGRAM :
            strCmd="CLEAR_ISO_PROGRAM";
            break;
        case CMD_GETGSLOTINFO :
            strCmd="GET_ISO_SLOT_INFO";
            break;
        case CMD_GETGLINEINFO :
            strCmd="GET_ISO_LINE_INFO";
            break;
        case CMD_GETGSOFTWAREINFO :
            strCmd="GET_SOFTWARE_INFO";
            break;
        case CMD_GETGCARDINFO :
            strCmd="GET_CARDS_INFO";
            break;
        case CMD_GETGDEVICEINFO :
            strCmd="GET_DEVICE_INFO";
            break;
        case CMD_GETTABLEKEYS :
            strCmd="GET_TABLE_KEYS";
            break;
        case CMD_GETTABLEVALUES :
            strCmd="GET_TABLE_VALUES";
            break;
        case CMD_SETTABLEVALUES :
            strCmd="SET_TABLE_VALUES";
            break;
        case CMD_STARTCANSCAN :
            strCmd="START_SCAN_CAN_NETWORK";
            break;
        case CMD_SETCANMODE :
            strCmd="SET_CAN_NODE_MODE";
            break;
        case CMD_GETCANSCANSTATUS :
            strCmd="GET_SCAN_STATUS";
            break;
        case CMD_READ_WRITE_SDO :
            strCmd="READ_WRITE_SDO";
            break;
        case CMD_READ_WRITE_SDO_STATUS :
            strCmd="READ_WRITE_SDO_STATUS";
            break;
        case CMD_READ_WRITE_SDO_DATA :
            strCmd="READ_WRITE_SDO_DATA";
            break;
        case CMD_READ_DEBUG_INFO :
            strCmd="GET_DEBUG_INFO";
            break;
        case CMD_WRITE_DEBUG_INFO :
            strCmd="SET_DEBUG_INFO";
            break;
        case CMD_READ_MGA023_PARAMETER :
            strCmd="GET_MGA023_PARAMETERS";
            break;
        case CMD_WRITE_MGA023_PARAMETER :
            strCmd="SET_MGA023_PARAMETERS";
            break;
        case CMD_SENDCONFHW :
            strCmd="SEND_HW_CONFIGURATION";
            break;
        case CMD_READ_MGA023_STATUS :
            strCmd="GET_MGA023_STATUS";
            break;
        case CMD_READ_MGA023_STATIONS :
            strCmd="GET_MGA023_STATIONS";
            break;
        case CMD_READ_MGA023_ALARM :
            strCmd="GET_MGA023_ALARMS";
            break;
        case CMD_MGA023_READY_ON :
            strCmd="SET_MGA023_READY_ON_OFF";
            break;
        case CMD_MGA023_SERVO_ON :
            strCmd="SET_MGA023_SERVO_ON_OFF";
            break;
        case CMD_MGA023_RESET_ALARM :
            strCmd="RESET_MGA023_ALARM";
            break;
        case CMD_MGA023_WRITE_ABS_POS :
            strCmd="SET_MGA023_ABSOLUTE_ENCODER";
            break;
        case CMD_MGA023_READ_ABS_POS :
            strCmd="GET_MGA023_ABSOLUTE_ENCODER";
            break;
        case CMD_MGA023_SET_ABS_POS :
            strCmd="SET_MGA023_ABSOLUTE_POSITION";
            break;
        case CMD_MGA023_INIT_SSCNET :
            strCmd="INIT_SSCNET_NETWORK";
            break;
        case CMD_STARTCANINITIALIZATION :
            strCmd="INIT_CANOPEN_NETWORK";
            break;
        case CMD_GETCANSCANDATA :
            strCmd="GET_SCAN_DATA";
            break;
        case CMDASYNC_CLOSE :
            strCmd="ASYNC_CLOSE";
            break;
        case CMDASYNC_MBMESSAGE :
            strCmd="ASYNC_SEND_MAILBOX_EVENT";
            break;
        case CMDASYNC_ERROR :
            strCmd="ASYNC_SEND_ERROR";
            break;
        case CMDASYNC_BREAK :
            strCmd="ASYNC_PROGRAM_BREAKED";
            break;
        case CMDASYNC_START :
            strCmd="ASYNC_PROGRAM_STARTED";
            break;
        case CMDASYNC_STOP  :
            strCmd="ASYNC_PROGRAM_STOPPED";
            break;
        case CMDASYNC_STRING :
            strCmd="ASYNC_SEND_STRING";
            break;
        case CMDASYNC_GSTART :
            strCmd="ASYNC_GPROGRAM_STARTED";
            break;
        case CMDASYNC_GEND :
            strCmd="ASYNC_GPROGRAM_TERMINATED";
            break;
        case CMDASYNC_GSTOP :
            strCmd="ASYNC_GPROGRAM_STOPPED";
            break;
        case CMDASYNC_GABORT :
            strCmd="ASYNC_GPROGRAM_ABORTED";
            break;
        case CMDASYNC_CHANGEPSW :
            strCmd="ASYNC_CHANGE_PSW";
            break;
        case CMDASYNC_CONNECT :
            strCmd="ASYNC_CLIENT_CONNECTED";
            break;
        case CMDASYNC_DISCONNECT :
            strCmd="ASYNC_CLIENT_DISCONNECTED";
            break;
    }
    return 0;
}

/*
** GetCommandImage :
*/
int CAdamoDGTraceCommFile::GetCommandImage (int nType, int* nImage)
{
    switch (nType)   {
        case TYPE_DOWNLOAD :
            *nImage=0;
            break;
        case TYPE_COMMAND :
            *nImage=1;
            break;
        case TYPE_REQUEST :
            *nImage=2;
            break;
        case TYPE_ANSWER_REQUEST :
            *nImage=3;
            break;
        case TYPE_RECEIVE :
            *nImage=4;
            break;
        case TYPE_DATA :
            *nImage=5;
            break;
    }
    return 0;
}

/*
** FiltersOk :
*/
bool CAdamoDGTraceCommFile::FiltersOk (int nCmd, int nType)
{
    bool b=false;

    if (m_anFilterTypes[nType]==1)
        if (m_nFilterDown!=-1&&m_nFilterUp!=-1)   {
            if (nCmd>=m_nFilterDown&&nCmd<=m_nFilterUp)
                b=true;
        }
        else
            b=true;
    return b;
}

/*
** SetFilters :
*/
void CAdamoDGTraceCommFile::SetFilters (CTreeView* tv, int nFilterDown, int nFilterUp, int* anFilterTypes)
{
    m_nFilterDown=nFilterDown;
    m_nFilterUp=nFilterUp;
    memcpy (m_anFilterTypes, anFilterTypes, sizeof (int)*NMAX_FILTERTYPES);
    tv->GetTreeCtrl ().DeleteAllItems ();
    m_f=0; m_n=0;
    UpdateTree (tv);
}

/*
** GetFilters :
*/
void CAdamoDGTraceCommFile::GetFilters (int* nFilterDown, int* nFilterUp, int* anFilterTypes)
{
    *nFilterDown=m_nFilterDown;
    *nFilterUp=m_nFilterUp;
    memcpy (anFilterTypes, m_anFilterTypes, sizeof (int)*NMAX_FILTERTYPES);
}

/*
** ResetFilters :
*/
void CAdamoDGTraceCommFile::ResetFilters ()
{
    m_nFilterDown=-1;
    m_nFilterUp=-1;
    for (int i=0; i<NMAX_FILTERTYPES; i++)
        m_anFilterTypes[i]=1;
}
