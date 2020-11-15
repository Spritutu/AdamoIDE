/*
** NewPreProp.cpp : gestione preprocessing LUA
*/

/* include files */
#include  <stdafx.h>
#ifdef ADAMO_THIRACC
#include "ThiraCC.h"
#else
#include "mainframe.h"
#endif
#include "resource.h"
#include "IdeMisc.h"
#include "AdamoFileDati.h"
#include "preprocessor.h"

/* debug memory directive */
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* data definitions */
stAxisInfo AxisInfo[] =   {
    0x100, "GAI_ST_ALIAS",
    0x101, "GAI_ST_RESOLUTION",
    0x102, "GAI_ST_TYPE",
    0x103, "GAI_ST_CATEGORY",
    0x104, "GAI_ST_UNIT",
    0x105, "GAI_ST_RAMP",
    0x106, "GAI_ST_RAMPW",
    0x107, "GAI_ST_INVPHASE",
    0x108, "GAI_ST_INVRIF",
    0x109, "GAI_ST_ENABLEINDEX",
    0x10a, "GAI_ST_ENALBLEJERK",
    0x10b, "GAI_ST_SPEED",
    0x10c, "GAI_ST_MANUALSPEED",
    0x10d, "GAI_ST_ACC",
    0x10e, "GAI_ST_DEC",
    0x10f, "GAI_ST_K",
    0x110, "GAI_ST_I",
    0x111, "GAI_ST_D",
    0x112, "GAI_ST_FF",
    0x113, "GAI_ST_FFA",
    0x114, "GAI_ST_RIF",
    0x115, "GAI_ST_SPEEDW",
    0x116, "GAI_ST_ACCW",
    0x117, "GAI_ST_DECW",
    0x118, "GAI_ST_PW",
    0x119, "GAI_ST_IW",
    0x11a, "GAI_ST_DW",
    0x11b, "GAI_ST_FFW",
    0x11c, "GAI_ST_FFAW",
    0x11d, "GAI_ST_WAITSTOP",
    0x11e, "GAI_ST_LIMEACONC",
    0x11f, "GAI_ST_LIMEADISC",
    0x120, "GAI_ST_ENABLELIMPOS",
    0x121, "GAI_ST_ENABLELIMNEG",
    0x122, "GAI_ST_LIMPOS",
    0x123, "GAI_ST_LIMNEG",
    0x124, "GAI_ST_WINPOS",
    0x125, "GAI_ST_WINNEG",
    0x126, "GAI_ST_SERVOLIMITTYPE",
    0x127, "GAI_ST_PERCRAMP",
    0x128, "GAI_ST_PERCRAMPW",
    0x129, "GAI_ST_DIMINTEGBUFFER",
    0x12A, "GAI_ST_KG",
    0x12B, "GAI_ST_IG",
    0x12C, "GAI_ST_DG",
    0x200, "GAI_DYN_WINPOS",
    0x201, "GAI_DYN_WINNEG",
    0x202, "GAI_DYN_ACC",
    0x203, "GAI_DYN_DEC",
    0x204, "GAI_DYN_ACCW",
    0x205, "GAI_DYN_DECW",
    0x206, "GAI_DYN_ENABLELIMPOS",
    0x207, "GAI_DYN_ENABLELIMNEG",
    0x208, "GAI_DYN_LIMPOS",
    0x209, "GAI_DYN_LIMNEG",
    0x20a, "GAI_DYN_K",
    0x20b, "GAI_DYN_I",
    0x20c, "GAI_DYN_D",
    0x20d, "GAI_DYN_FF",
    0x20e, "GAI_DYN_FFA",
    0x20f, "GAI_DYN_PW",
    0x210, "GAI_DYN_IW",
    0x211, "GAI_DYN_DW",
    0x212, "GAI_DYN_FFW",
    0x213, "GAI_DYN_FFAW",
    0x214, "GAI_DYN_SPEED",
    0x215, "GAI_DYN_SPEEDW",
    0x216, "GAI_DYN_LIMEACONC",
    0x217, "GAI_DYN_LIMEADISC",
    0x218, "GAI_DYN_ENALBLEJERK",
    0x219, "GAI_DYN_SERVOLIMITTYPE",
    0x21a, "GAI_DYN_RAMP",
    0x21b, "GAI_DYN_RAMPW",
    0x21c, "GAI_DYN_PERCRAMP",
    0x21d, "GAI_DYN_PERCRAMPW",
    0x21e, "GAI_DYN_KG",
    0x21f, "GAI_DYN_IG",
    0x220, "GAI_DYN_DG"
};

/*
** InitPreprocessing :
*/
void CPreProcessor::InitPreprocessing (bool bIsMain)
{
    /* azzeriamo alcune variabili */
    pl.m_bWriteCode=true, pl.m_nError=0; m_bAbortCompilation = false;
	pl.m_strCurrentFunction = ""; pl.m_nBrowseLine = -1;
	pl.m_strCurrentObject = "";	pl.m_strlistVirtualFunction.RemoveAll ();
    /* se siamo nel file di main */
    if (bIsMain)   {
        /* ripuliamo tutti gli elementi */
        m_defglbtok.RemoveAll ();
        /* in caso di macchina simulata, mettiamo la define apposta */
        if (m_objMachine->GetMachineType ()==enVirtual)
            m_defglbtok.SetAt (SIMULATOR_DEFINE, "");
        else
            m_defglbtok.SetAt (PHYSICAL_DEFINE, "");
        /* settiamo la mappa main per la compilazione */
        pl.SetDefineTokenMap (&m_defglbtok);
		pl.m_bGiorgettiClary = false;
    }
    else   {
        /* liberiamo tutte le define eventualmente allocate */
        m_deftok.RemoveAll ();
        /* copiamo la mappa main in quella locale */
        CopyMainMapToLocal ();
        /* settiamo la mappa locale per la compilazione */
        pl.SetDefineTokenMap (&m_deftok);
    }
    BackupPreprocessedFile ();
}

/*
** StartPreProcessing :
*/
int CPreProcessor::StartPreProcessing (CString strPathSource, CString strPathDest, bool bIsMain)
{
    CStdioFile pFS, pFD;
    int nB=0;

    InitPreprocessing (bIsMain);
	if (pFS.Open(strPathSource, CFile::modeRead|CFile::typeText))   {
        if (pFD.Open(strPathDest, CFile::modeCreate|CFile::modeWrite|CFile::typeText))   {
            m_strPathSource=strPathSource;
            m_strPathDest=strPathDest;
            nB=PreProcess (&pFS, &pFD);
            pFD.Close ();
        }
        else
            nB=-1;
        pFS.Close ();
        ViewPreprocessedFile (strPathDest);
    }
    else
        nB=-1;
    return nB;
}

/*
** PreProcess :
*/
int CPreProcessor::PreProcess (CStdioFile* pFS, CStdioFile* pFD)
{
    int nB=0;
    char szBuffer[2048];
    m_nProgLine=0;
    pl.SetPathDest (m_strPathDest);
    while (pFS->ReadString (szBuffer, 2047))   {
			PompaIMessaggi ();
			if (IsCompilationAborted ())   {
				nB=-2; break;
			}
			if (pl.ParsePreProcessorString (szBuffer, m_nProgLine))   {
				nB=-3; break;
			}
			else
				pFD->WriteString (pl.GetResult());
			m_nProgLine++;
		}
    return nB;
}

/*
** GetOutputString :
*/
CString CPreProcessor::GetOutputString ()
{
    char szBuffer[1024];

    if (IsCompilationAborted ())
        strcpy (szBuffer, LOADSTRING (IDS_ADAMO_COMPILATION_ABORTED));
    else
        if (!pl.GetErrorString ().IsEmpty())
            sprintf (szBuffer, "%s: %s:%d:%s\n", (const char*)LOADSTRING (IDS_ADAMO_PREPROCESSOR), (const char*)m_strPathDest, m_nProgLine+1, (const char*)pl.GetErrorString ());
        else
            szBuffer[0]='\0';
    return CString (szBuffer);
}

/*
** CopyMainMapToLocal :
*/
void CPreProcessor::CopyMainMapToLocal ()
{
    CString strKey, strValue;
    for (POSITION pos=m_defglbtok.GetStartPosition(); pos;)   {
        m_defglbtok.GetNextAssoc (pos, strKey, strValue);
        m_deftok.SetAt (strKey, strValue);
    }    
}

/*
** ViewPreprocessedFile :
*/
void CPreProcessor::BackupPreprocessedFile ()
{
	m_bBackupPreprocessedFile = GETOPT()->GetViewPreprocessedFile ();
}

/*
** ViewPreprocessedFile :
*/
void CPreProcessor::ViewPreprocessedFile (CString strPath)
{
    CString strFile;
    char szBuffer[_MAX_PATH], *p;

    if (m_bBackupPreprocessedFile)   {
        strcpy (szBuffer, strPath);
        p=strrchr (szBuffer, '.');
        if (p)   {
            strcpy (p+1, "bck");
            p=strrchr (szBuffer, '\\');
            if (p)
                strFile=GetIntermediateDir()+"\\"+(p+1);
            CopyFile (strPath, strFile, FALSE);
        }
    }
}

/* constructor / destructor */
CPreProcessorListener::CPreProcessorListener () : m_bBlockComment (false), m_bLineComment (false),
    m_bSomethingInteresting (false), m_bNewLineFlag (false), m_nTokenKey (0), m_bWriteCode (false),
    m_objMachine (NULL), m_tr (NULL), m_stackDefine (32), m_stackSwitch (32), m_nProgLine (0), m_nBrowseLine (-1), m_bGiorgettiClary (false)
{
    std::tstringstream* strm=new std::tstringstream	(
	    _T("[seperators]\n")
        _T("1: \n")
        _T("1:\t\n")
        _T("7:\\n\\r\n")
        _T("8:\\n\n")
        _T("9:\\r\\n\n")
        _T("10:+\n")
        _T("11:-\n")
        _T("12:*\n")
        _T("13:/\n")
        _T("14:^\n")
        _T("15:...\n")
        _T("16:{\n")
        _T("17:}\n")
        _T("18:;\n")
        _T("19:..\n")
        _T("20:>\n")
        _T("21:=\n")
        _T("22:<\n")
        _T("23:>=\n")
        _T("24:~=\n")
        _T("25:<=\n")
        _T("26:==\n")
	    _T("27:,\n")
	    _T("28:[\n")
	    _T("29:]\n")
	    _T("30::\n")
        _T("31:(\n")
        _T("32:)\n")

	    _T("[tokens]\n")
        /* sintassi adamo */    
        /* sintassi lua */
        _T("400:and\n")
        _T("401:break\n")
        _T("402:do\n")
        _T("403:else\n")
        _T("404:elseif\n")
        _T("405:end\n")
        _T("406:for\n")
        _T("408:false\n")
        _T("409:if\n")
        _T("410:in\n")
        _T("411:local\n")
        _T("412:nil\n")
        _T("413:not\n")
        _T("414:or\n")
        _T("415:repeat\n")
        _T("416:then\n")
        _T("417:true\n")
        _T("418:while\n")
        _T("419:--\n")
        _T("420:--[[\n")
        _T("421:]]--\n")
        /* macro adamo */
	    _T("200:global\n")
        _T("201:define\n")
        _T("202:undefine\n")
        _T("203:ifdef\n")
        _T("204:ifnotdef\n")
        _T("205:elsedef\n")
        _T("206:enddef\n")
        _T("207:string.load\n")
        _T("208:conf.GetName\n")
        _T("209:conf.GetPath\n")
        _T("210:conf.GetDescription\n")
        _T("211:group\n")
        _T("212:switch\n")
        _T("213:case\n")
        _T("214:new\n")
        _T("215:delete\n")
        _T("216:iso.DispatchMessage\n")
        _T("217:object\n")
        _T("218:function\n")
	    _T("251:boolean\n")
	    _T("252:number\n")
	    _T("253:string\n")
	    _T("254:as\n")
	    _T("255:matrix\n")
	    _T("256:array\n")
	    _T("257:is\n")
        _T("258:table\n")
        _T("259:public\n")
        _T("260:private\n")
        _T("261:virtual\n")
        /* GetAxisInfo */
        _T("500:GAI_ST_ALIAS\n")
        _T("501:GAI_ST_RESOLUTION\n")
        _T("502:GAI_ST_TYPE\n")
        _T("503:GAI_ST_CATEGORY\n")
        _T("504:GAI_ST_UNIT\n")
        _T("505:GAI_ST_RAMP\n")
        _T("506:GAI_ST_RAMPW\n")
        _T("507:GAI_ST_INVPHASE\n")
        _T("508:GAI_ST_INVRIF\n")
        _T("509:GAI_ST_ENABLEINDEX\n")
        _T("510:GAI_ST_ENALBLEJERK\n")
        _T("511:GAI_ST_SPEED\n")
        _T("512:GAI_ST_MANUALSPEED\n")
        _T("513:GAI_ST_ACC\n")
        _T("514:GAI_ST_DEC\n")
        _T("515:GAI_ST_K\n")
        _T("516:GAI_ST_I\n")
        _T("517:GAI_ST_D\n")
        _T("518:GAI_ST_FF\n")
        _T("519:GAI_ST_FFA\n")
        _T("520:GAI_ST_RIF\n")
        _T("521:GAI_ST_SPEEDW\n")
        _T("522:GAI_ST_ACCW\n")
        _T("523:GAI_ST_DECW\n")
        _T("524:GAI_ST_PW\n")
        _T("525:GAI_ST_IW\n")
        _T("526:GAI_ST_DW\n")
        _T("527:GAI_ST_FFW\n")
        _T("528:GAI_ST_FFAW\n")
        _T("529:GAI_ST_WAITSTOP\n")
        _T("530:GAI_ST_LIMEACONC\n")
        _T("531:GAI_ST_LIMEADISC\n")
        _T("532:GAI_ST_ENABLELIMPOS\n")
        _T("533:GAI_ST_ENABLELIMNEG\n")
        _T("534:GAI_ST_LIMPOS\n")
        _T("535:GAI_ST_LIMNEG\n")
        _T("536:GAI_ST_WINPOS\n")
        _T("537:GAI_ST_WINNEG\n")
        _T("538:GAI_ST_SERVOLIMITTYPE\n")
        _T("539:GAI_ST_PERCRAMP\n")
        _T("540:GAI_ST_PERCRAMPW\n")
        _T("541:GAI_ST_DIMINTEGBUFFER\n")
        _T("542:GAI_ST_KG\n")
        _T("543:GAI_ST_IG\n")
        _T("544:GAI_ST_DG\n")
        _T("545:GAI_DYN_WINPOS\n")
        _T("546:GAI_DYN_WINNEG\n")
        _T("547:GAI_DYN_ACC\n")
        _T("548:GAI_DYN_DEC\n")
        _T("549:GAI_DYN_ACCW\n")
        _T("550:GAI_DYN_DECW\n")
        _T("551:GAI_DYN_ENABLELIMPOS\n")
        _T("552:GAI_DYN_ENABLELIMNEG\n")
        _T("553:GAI_DYN_LIMPOS\n")
        _T("554:GAI_DYN_LIMNEG\n")
        _T("555:GAI_DYN_K\n")
        _T("556:GAI_DYN_I\n")
        _T("557:GAI_DYN_D\n")
        _T("558:GAI_DYN_FF\n")
        _T("559:GAI_DYN_FFA\n")
        _T("560:GAI_DYN_PW\n")
        _T("561:GAI_DYN_IW\n")
        _T("562:GAI_DYN_DW\n")
        _T("563:GAI_DYN_FFW\n")
        _T("564:GAI_DYN_FFAW\n")
        _T("565:GAI_DYN_SPEED\n")
        _T("566:GAI_DYN_SPEEDW\n")
        _T("567:GAI_DYN_LIMEACONC\n")
        _T("568:GAI_DYN_LIMEADISC\n")
        _T("569:GAI_DYN_ENALBLEJERK\n")
        _T("570:GAI_DYN_SERVOLIMITTYPE\n")
        _T("571:GAI_DYN_RAMP\n")
        _T("572:GAI_DYN_RAMPW\n")
        _T("573:GAI_DYN_PERCRAMP\n")
        _T("574:GAI_DYN_PERCRAMPW\n")
        _T("575:GAI_DYN_KG\n")
        _T("576:GAI_DYN_IG\n")
        _T("577:GAI_DYN_DG\n")
        /* sostituzioni */
        _T("600:DigitalInput\n")
        _T("601:DigitalOutput\n")
        _T("602:AnalogInput\n")
        _T("603:AnalogOutput\n")
        _T("604:Axis\n")
        _T("605:Encoder\n")
        _T("606:Timer\n")
        _T("607:Memory\n")
        _T("608:DeviceCan\n")
        _T("609:ON\n")
        _T("610:OFF\n")
        _T("611:UP\n")
        _T("612:DOWN\n")
        _T("613:POSITIVE\n")
        _T("614:NEGATIVE\n")
        _T("615:CLKW\n")
        _T("616:CCLKW\n")
        _T("617:SKIP\n")
        _T("618:WAIT\n")
        _T("619:ACK\n")
        /* regole */
	    _T("[rules]\n")
	    _T("300:strings\n")
	    _T("301:numbers\n") );

    m_map.vLoadFromStream (*strm);
    memset (m_apTokenKey, 0, sizeof (m_apTokenKey));
    InitSyntaxStructures ();
    delete strm;
}

/*
** ParsePreProcessorString :
*/
int CPreProcessorListener::ParsePreProcessorString (char* szBuffer, int i)
{
    __try    {
        /* numero di linea */
		m_nProgLine=i;
        /* memorizziamo il buffer */
        m_strBuffer=szBuffer;
        m_nTokenKey=0;
        m_bLineComment=false;
        m_strResult="";
        m_strError="";
        m_strBlockComment="";
		if (!FilterInputBuffer ())   {
			/* gestiamo la presenza di underscore */
			if (!ManageUnderScore ())   {
				if (Translate ())   {
					Browsing ();					  // memorizziamo i simboli 
					if (Objects ())
						ManageErrors ();
					else
						if (Substitution ())              // se c'e' una riga sensibile, ne verifica la sintassi
							ManageErrors ();
						else
							if (!CheckSyntax ())          // esegue la sostituzione delle stringhe
								ManageErrors ();
							else                             
								if (Action ())            // a seconda della keyword memorizzata esegue o meno una azione
									ManageErrors ();
				}
				else   {
					Browsing ();					  // memorizziamo i simboli 
					if (Objects ())
						ManageErrors ();
					else
					if (Substitution ())   
						ManageErrors ();
				}
			}
		}
		else
			ManageErrors ();
        WriteResult ();
    }
    __finally   {
        CleanUp ();                                          // - ripulire l'array
    }
    return m_nError;
}

/*
** ManageUnderScore : la funzione gestisce la presenza nella linea di un underscore che fa andare
**                    a capo e gestisce anche se la linea corrente arriva da un underscore precedente.
*/
bool CPreProcessorListener::ManageUnderScore ()
{
    char *szBuffer;

	if (m_bNewLineFlag)	  {
        szBuffer=new char [m_strConCat.GetLength()+1];
        strcpy (szBuffer, m_strConCat);
        CString str=CString(szBuffer)+m_strBuffer;
		m_strBuffer=str;
		m_bNewLineFlag=FALSE;
        delete szBuffer;
	}
	if (ThereIsUnderScore (m_strBuffer))	{
		if (m_nBrowseLine == -1)
			m_nBrowseLine = m_nProgLine;
		m_strConCat=m_strBuffer;
		m_bNewLineFlag=TRUE;
	}
    return m_bNewLineFlag;
}

/*
** ThereIsUnderScore : la funzione verifica se nella stringa passata per parametro
**					   c'e' un underscore, nel caso lo sostituisce con un
**					   new line, mette subito dopo un terminatore e ritorna TRUE.
**					   In caso contrario ritorna FALSE.
*/
bool CPreProcessorListener::ThereIsUnderScore (CString& szBuffer)
{
	//char *p, *q, *sz=new char [2048];
	bool bB=false;
	int nPos=szBuffer.ReverseFind ('_');
	if (nPos!=-1)   {
		bB=true;
		for (int i=nPos+1; i<szBuffer.GetLength(); i++)
			if (szBuffer[i]!=0x20 && szBuffer[i]!=0x08 && szBuffer[i]!=0x0a && szBuffer[i]!=0x0d)   {
				bB=false; break;
			}
		if (bB)
			szBuffer.SetAt(nPos, '\0');
	}
	return bB;
}

/*
** Translate : la funzione traduce i tokens in strutture stAdamoToken.
*/
bool CPreProcessorListener::Translate ()
{
	cooLexxerTextInputStream	tis (m_strBuffer);
	cooLexxer					lexxer (&tis, &m_map, this);

    m_bSomethingInteresting=false;
	while(!tis.fIsEofReached())
		lexxer.vParseCharacter();
    return m_bSomethingInteresting;
}

/*
** FilterBlockComment : filtra i caratteri di commento di blocco. Ritorna quindi
**                      l'ID corretto.
*/
int CPreProcessorListener::FilterBlockComment (const char* sz, int nID)
{
    CString str(sz);
    if (str=="-"||str=="["||str=="]")   {
        m_strBlockComment+=str;
        if (m_strBlockComment=="--[[")   {
            m_strBlockComment="";
            nID=420;
        }
        else
            if (m_strBlockComment=="]]--")   {
                m_strBlockComment="";
                nID=421;
            }
            else
                if (m_strBlockComment.GetLength()>4)
                    m_strBlockComment="";    
    }
    else   {
        if (m_strBlockComment=="--")
            nID=419;
        m_strBlockComment="";
    }
    return nID;
}

/*
** vRegisterToken : funzione virtual chiamata ogni volta che c'e' un nuovo token.
*/
void CPreProcessorListener::vRegisterToken (const std::tstring& strTokenText, const cooLexxerTokenRule* pptrRule)
{
    struct stAdamoVettToken* pavt=NULL;
    int nCommentID=0, nID=0;
    bool nB=false;

    if (pptrRule)
        nID=pptrRule->nGetIDValue();
	else
		nID = AssignNumericID (strTokenText);
    nCommentID=FilterBlockComment (strTokenText.data(), nID);
    /* vediamo se siamo in presenza di un commento di riga */
    if (nCommentID==LUALINECOMMENT||nID==LUALINECOMMENT)   {
        if (!m_bLineComment&&nID!=LUALINECOMMENT&&!m_bBlockComment)   {
            delete (stAdamoToken *)m_ptrCodeArray[m_ptrCodeArray.GetUpperBound ()];
            m_ptrCodeArray.RemoveAt (m_ptrCodeArray.GetUpperBound ());
            delete (stAdamoToken *)m_ptrCodeArray[m_ptrCodeArray.GetUpperBound ()];
            m_ptrCodeArray.RemoveAt (m_ptrCodeArray.GetUpperBound ());
        }
        m_bLineComment=true;
    }
    /* vediamo se siamo in presenza di un commento di blocco */
    if (nCommentID==LUAOPENBLOCKCOMMENT||nID==LUAOPENBLOCKCOMMENT)   {
        if (!m_bBlockComment&&nID!=LUAOPENBLOCKCOMMENT)   {
            delete (stAdamoToken *)m_ptrCodeArray[m_ptrCodeArray.GetUpperBound ()];
            m_ptrCodeArray.RemoveAt (m_ptrCodeArray.GetUpperBound ());
            delete (stAdamoToken *)m_ptrCodeArray[m_ptrCodeArray.GetUpperBound ()];
            m_ptrCodeArray.RemoveAt (m_ptrCodeArray.GetUpperBound ());
            delete (stAdamoToken *)m_ptrCodeArray[m_ptrCodeArray.GetUpperBound ()];
            m_ptrCodeArray.RemoveAt (m_ptrCodeArray.GetUpperBound ());
        }
        m_bBlockComment=true;
    }
    if (nCommentID==LUACLOSEBLOCKCOMMENT)
        m_bBlockComment=false;
    else
        if (!m_bLineComment&&!m_bBlockComment)   {
            /* i carriage return e newline non li carichiamo */
            if (nID>=10||!nID)   {
                /* aggiungiamo il token alla lista */
                stAdamoToken* p=new stAdamoToken;
                bool bAdded=false;
                /* vediamo se c'e' un <> */
                if (nID==20 && m_ptrCodeArray.GetSize ())   {
                    int nEl=m_ptrCodeArray.GetSize ()-1;
                    stAdamoToken *pEl=(stAdamoToken *)m_ptrCodeArray[nEl];
                    if (pEl->nID==22)   {
                        pEl->strToken="<>";
                        pEl->nID=TOKSOST_DIFFERENT;
                    }
                    else   {
                        p->strToken=strTokenText.data();
                        p->nIndex=m_ptrCodeArray.Add (p);
                        p->nID=nID;
                        bAdded=true;
                    }
                }
                else   {
                    p->strToken=strTokenText.data();
                    p->nIndex=m_ptrCodeArray.Add (p);
                    p->nID=nID;
                    bAdded=true;
                }
                if (ISTOKENAKEY (nID))   {
                    if (m_nTokenKey<NMAXPTOKENS)   {
						m_apTokenKey[m_nTokenKey++]=p;
						m_bSomethingInteresting=true;
                    }
                }
                if (!bAdded)
                    delete p;
            }
            else   {
                int nSize=m_ptrCodeArray.GetSize ();
                if (nSize)   {
                    if (nID<7)   {
                        stAdamoToken* p=(stAdamoToken*)m_ptrCodeArray[nSize-1];
                        p->strRiporto+=strTokenText.data();
                    }
                }
            }
        }
}

/*
** CleanUp : ripulisce tutta la memoria allocata.
*/
void CPreProcessorListener::CleanUp ()
{
    int i;
    for (i=0; i<m_ptrCodeArray.GetSize(); i++)
        delete (stAdamoToken*)m_ptrCodeArray[i];
    m_ptrCodeArray.RemoveAll ();
}

/*
** WriteResult : scrive il risultato delle elaborazioni.
*/
void CPreProcessorListener::WriteResult ()
{
    int i;
    if (m_bWriteCode&&!m_bNewLineFlag)   {
        for (i=0; i<m_ptrCodeArray.GetSize(); i++)   {
            stAdamoToken* p=(stAdamoToken*)m_ptrCodeArray[i];
            if (p->nID==RULSTRINGS)   {
                m_strResult+="\"";
                m_strResult+=p->strToken;
                m_strResult+="\"";
                m_strResult+=p->strRiporto;
            }
            else    
                m_strResult+=(p->strToken+p->strRiporto);
        }
    }
	if (m_bNewLineFlag == false)
		m_nBrowseLine = -1;
    m_strResult+="\n";
}

/*
** CheckSyntax :
*/
bool CPreProcessorListener::CheckSyntax ()
{
    stAdamoToken* p;
    int n, l, k, nToken, nType;
    bool bOk = true, bFound, bCheckSyntax;
    /* c'e' qualche cosa di interessante ? */
    if (m_bSomethingInteresting)   {
        for (n=0; n<m_nTokenKey; n++)   {
            /* token da esplorare */
            p=(stAdamoToken*)m_ptrCodeArray[m_apTokenKey[n]->nIndex];
            nToken=p->nID-TOKGLOBAL; nType=0;
            bOk=false;
            l=0; k=0;
            bCheckSyntax = false;
            while (TokenSyntax[nToken][nType].nID[l][k]!=0xffff&&nType<NMAXCHKSYNTYPES&&!bOk)   {
                bCheckSyntax = true;
                l=0; k=0;
                while (true)   {
                    /* non siamo alla fine, vediamo se il token scritto e' quello che ci aspettiamo */
                    if (p->nID!=TokenSyntax[nToken][nType].nID[l][k])   {
                        /* non lo e', vediamo se questo syntax template poteva essere gia' finito */
                        if (TokenSyntax[nToken][nType].nModificatore[l-1]==0xff)   {
                            /* non era finito, vediamo se ci sono altri ID possibili */
                            bFound=false;
                            k=0;
                            while (TokenSyntax[nToken][nType].nID[l][k]!=0xffff)   {
                                if (TokenSyntax[nToken][nType].nID[l][k]==p->nID)   {
                                    /* abbiamo trovato un'altra sintassi possibile, andiamo avanti a esaminare */
                                    bFound=true; break;
                                }
                                else   {
                                    /* non lo e', vediamo se potrebbe essere qualcosa di particolare */
                                    if (CheckParticularRules (p, TokenSyntax[nToken][nType].nID[l][k]))   {
                                        bFound=true; break;
                                    }
                                }
                                k++;
                            }
                            if (!bFound)   {
                                /* non ce n'erano, la sintassi non e' corretta */
                                bOk=false; break;
                            }
                        }
                        else   {
                            /* poteva essere gia' finito, quindi la sintassi e' corretta */
                            bOk=TokenSyntax[nToken][nType].nModificatore[l-1]==0x01;
                            break;
                        }
                    }
                    /* prendiamo i successivi elementi del syntax template e degli argomenti */
                    if (p->nIndex+1<m_ptrCodeArray.GetSize ())    {
                        l++; k=0; p=(stAdamoToken*)m_ptrCodeArray[p->nIndex+1];
                    }
                    else   {
                        /* se non ci sono piu' elementi, verifichiamo se la sintassi poteva dirsi conclusa */
                        bOk=TokenSyntax[nToken][nType].nModificatore[l]==0x01||
                            TokenSyntax[nToken][nType].nModificatore[l]==0x02;
                        break;
                    }
                }
                /* andiamo a vedere un altro syntax template */
                nType++;
                /* primo elemento del token */
                p=(stAdamoToken*)m_ptrCodeArray[m_apTokenKey[n]->nIndex];
                /* reinizializziamo gli indici */
                l=k=0;
            }
        }
        if (!bCheckSyntax)
            bOk = true;
        else
            if (!bOk)
                /* errore di sintassi */
                m_nError=-2;
    }
    return bOk;
}

/*
 * InitSyntaxStructures :
 */
void CPreProcessorListener::InitSyntaxStructures ()
{
    int i, n;
    /* inizializziamo tutto */
    memset (TokenSyntax, 0XFF, sizeof (TokenSyntax));
    /* inizializziamo prima la matrice global, cominciamo dalle variabili singole */
    TokenSyntax[0][0].nID[0][0]=TOKGLOBAL;
    TokenSyntax[0][0].nID[1][0]=0;
    TokenSyntax[0][0].nID[2][0]=TOKAS;
    TokenSyntax[0][0].nID[3][0]=TOKBOOLEAN;
    TokenSyntax[0][0].nID[3][1]=TOKNUMBER;
    TokenSyntax[0][0].nID[3][2]=TOKSTRING;
	TokenSyntax[0][0].nID[3][3]=TOKTABLE;
    TokenSyntax[0][0].nModificatore[3]=2;
    /* ora gli array */
    TokenSyntax[0][1].nID[0][0]=TOKGLOBAL;
    TokenSyntax[0][1].nID[1][0]=0;
    TokenSyntax[0][1].nID[2][0]=SEPPARENTESIQUADRAAPERTA;
    TokenSyntax[0][1].nID[3][0]=RULNUMBERS;
    TokenSyntax[0][1].nID[4][0]=SEPPARENTESIQUADRACHIUSA;
    TokenSyntax[0][1].nID[5][0]=TOKAS;
    TokenSyntax[0][1].nID[6][0]=TOKBOOLEAN;
    TokenSyntax[0][1].nID[6][1]=TOKNUMBER;
    TokenSyntax[0][1].nID[6][2]=TOKSTRING;
    TokenSyntax[0][1].nModificatore[6]=2;
    /* ora le matrici */
    TokenSyntax[0][2].nID[0][0]=TOKGLOBAL;
    TokenSyntax[0][2].nID[1][0]=0;
    TokenSyntax[0][2].nID[2][0]=SEPPARENTESIQUADRAAPERTA;
    TokenSyntax[0][2].nID[3][0]=RULNUMBERS;
    TokenSyntax[0][2].nID[4][0]=SEPPARENTESIQUADRACHIUSA;
    TokenSyntax[0][2].nID[5][0]=TOKAS;
    TokenSyntax[0][2].nID[6][0]=TOKBOOLEAN;
    TokenSyntax[0][2].nID[6][1]=TOKNUMBER;
    TokenSyntax[0][2].nID[6][2]=TOKSTRING;
    TokenSyntax[0][2].nID[7][0]=SEPDUEPUNTI;
    TokenSyntax[0][2].nID[8][0]=0;
    TokenSyntax[0][2].nModificatore[8]=2;
    for (i=0, n=9; i<MAXCOLUMNS; i++, n+=4)   {
        TokenSyntax[0][2].nID[n][0]=SEPVIRGOLA;
        TokenSyntax[0][2].nID[n+1][0]=TOKBOOLEAN;
        TokenSyntax[0][2].nID[n+1][1]=TOKNUMBER;
        TokenSyntax[0][2].nID[n+1][2]=TOKSTRING;
        TokenSyntax[0][2].nID[n+2][0]=SEPDUEPUNTI;
        TokenSyntax[0][2].nID[n+3][0]=0;
        TokenSyntax[0][2].nModificatore[n+3]=2;
    }
    /* ora table e oggetti */
    TokenSyntax[0][3].nID[0][0]=TOKGLOBAL;
    TokenSyntax[0][3].nID[1][0]=0;
    TokenSyntax[0][3].nID[3][0]=TOKNEW;
    TokenSyntax[0][3].nModificatore[8]=2;
    /* define */
    TokenSyntax[1][0].nID[0][0]=TOKDEFINE;
    TokenSyntax[1][0].nID[1][0]=0;
    TokenSyntax[1][0].nID[2][0]=RULFPNUMBERS;
    TokenSyntax[1][0].nID[2][1]=RULSTRINGS;
    TokenSyntax[1][0].nID[2][2]=RULNUMBERS;
    TokenSyntax[1][0].nModificatore[2]=1;
    TokenSyntax[1][1].nID[0][0]=TOKDEFINE;
    TokenSyntax[1][1].nID[1][0]=0;
    TokenSyntax[1][1].nModificatore[1]=1;
    /* undefine */
    TokenSyntax[2][0].nID[0][0]=TOKUNDEFINE;
    TokenSyntax[2][0].nID[1][0]=0;
    TokenSyntax[2][0].nModificatore[1]=1;
    /* ifdefine */
    TokenSyntax[3][0].nID[0][0]=TOKIFDEF;
    TokenSyntax[3][0].nID[1][0]=0;
    TokenSyntax[3][0].nID[1][1]=RULFPNUMBERS;
    TokenSyntax[3][0].nID[1][2]=RULSTRINGS;
    TokenSyntax[3][0].nID[1][3]=RULNUMBERS;
    TokenSyntax[3][0].nID[2][0]=20;
    TokenSyntax[3][0].nID[2][1]=21;
    TokenSyntax[3][0].nID[2][2]=22;
    TokenSyntax[3][0].nID[2][3]=23;
    TokenSyntax[3][0].nID[2][4]=24;
    TokenSyntax[3][0].nID[2][5]=25;
    TokenSyntax[3][0].nID[3][0]=0;
    TokenSyntax[3][0].nID[3][1]=RULFPNUMBERS;
    TokenSyntax[3][0].nID[3][2]=RULSTRINGS;
    TokenSyntax[3][0].nID[3][3]=RULNUMBERS;
    TokenSyntax[3][0].nModificatore[3]=1;
    TokenSyntax[3][1].nID[0][0]=TOKIFDEF;
    TokenSyntax[3][1].nID[1][0]=0;
    TokenSyntax[3][1].nModificatore[1]=1;
    /* ifnotdefine */
    TokenSyntax[4][0].nID[0][0]=TOKIFNOTDEF;
    TokenSyntax[4][0].nID[1][0]=0;
    TokenSyntax[4][0].nID[1][1]=RULFPNUMBERS;
    TokenSyntax[4][0].nID[1][2]=RULSTRINGS;
    TokenSyntax[4][0].nID[1][3]=RULNUMBERS;
    TokenSyntax[4][0].nID[2][0]=20;
    TokenSyntax[4][0].nID[2][1]=21;
    TokenSyntax[4][0].nID[2][2]=22;
    TokenSyntax[4][0].nID[2][3]=23;
    TokenSyntax[4][0].nID[2][4]=24;
    TokenSyntax[4][0].nID[2][5]=25;
    TokenSyntax[4][0].nID[3][0]=0;
    TokenSyntax[4][0].nID[3][1]=RULFPNUMBERS;
    TokenSyntax[4][0].nID[3][2]=RULSTRINGS;
    TokenSyntax[4][0].nID[3][3]=RULNUMBERS;
    TokenSyntax[4][0].nModificatore[3]=1;
    TokenSyntax[4][1].nID[0][0]=TOKIFNOTDEF;
    TokenSyntax[4][1].nID[1][0]=0;
    TokenSyntax[4][1].nModificatore[1]=1;
    /* elsedef */
    TokenSyntax[5][0].nID[0][0]=TOKELSEDEF;
    TokenSyntax[5][0].nModificatore[0]=1;
    /* enddef */
    TokenSyntax[6][0].nID[0][0]=TOKENDDEF;
    TokenSyntax[6][0].nModificatore[0]=1;
    /* string.load */
    TokenSyntax[7][0].nID[0][0]=TOK_STRING_LOAD;
    TokenSyntax[7][0].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[7][0].nID[2][0]=0;
    TokenSyntax[7][0].nID[2][1]=RULNUMBERS;
    TokenSyntax[7][0].nID[3][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[7][0].nModificatore[3]=1;
    /* conf.GetName */
    TokenSyntax[8][0].nID[0][0]=TOK_CONF_GETNAME;
    TokenSyntax[8][0].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[8][0].nID[2][0]=0;
    TokenSyntax[8][0].nID[3][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[8][0].nModificatore[3]=1;
    TokenSyntax[8][1].nID[0][0]=TOK_CONF_GETNAME;
    TokenSyntax[8][1].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[8][1].nID[2][0]=0;
    TokenSyntax[8][1].nID[3][0]=SEPVIRGOLA;
    TokenSyntax[8][1].nID[4][0]=RULNUMBERS;
    TokenSyntax[8][1].nID[5][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[8][1].nModificatore[5]=1;
    /* conf.GetPath */
    TokenSyntax[9][0].nID[0][0]=TOK_CONF_GETPATH;
    TokenSyntax[9][0].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[9][0].nID[2][0]=0;
    TokenSyntax[9][0].nID[3][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[9][0].nModificatore[3]=1;
    TokenSyntax[9][1].nID[0][0]=TOK_CONF_GETPATH;
    TokenSyntax[9][1].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[9][1].nID[2][0]=0;
    TokenSyntax[9][1].nID[3][0]=SEPVIRGOLA;
    TokenSyntax[9][1].nID[4][0]=RULNUMBERS;
    TokenSyntax[9][1].nID[5][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[9][1].nModificatore[5]=1;
    /* conf.GetDescription */
    TokenSyntax[10][0].nID[0][0]=TOK_CONF_GETDESCRIPTION;
    TokenSyntax[10][0].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[10][0].nID[2][0]=0;
    TokenSyntax[10][0].nID[3][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[10][0].nModificatore[3]=1;
    TokenSyntax[10][1].nID[0][0]=TOK_CONF_GETDESCRIPTION;
    TokenSyntax[10][1].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[10][1].nID[2][0]=0;
    TokenSyntax[10][1].nID[3][0]=SEPVIRGOLA;
    TokenSyntax[10][1].nID[4][0]=RULNUMBERS;
    TokenSyntax[10][1].nID[5][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[10][1].nModificatore[5]=1;
    /* group */
    TokenSyntax[11][1].nID[0][0]=TOKGROUP;
    TokenSyntax[11][1].nID[1][0]=0;
    TokenSyntax[11][1].nModificatore[1]=2;
    /* switch */
    /*
    TokenSyntax[12][0].nID[0][0]=TOKSWITCH;
    TokenSyntax[12][0].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[12][0].nID[2][0]=0;
    TokenSyntax[12][0].nID[3][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[12][0].nModificatore[3]=1;
    TokenSyntax[12][1].nID[0][0]=TOKSWITCH;
    TokenSyntax[12][1].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[12][1].nID[2][0]=11;
    TokenSyntax[12][1].nID[3][0]=0;
    TokenSyntax[12][1].nID[4][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[12][1].nModificatore[4]=1;
    TokenSyntax[12][2].nID[0][0]=TOKSWITCH;
    TokenSyntax[12][2].nID[1][0]=405;
    TokenSyntax[12][2].nModificatore[1]=1;  
    */
    /* case */
    TokenSyntax[13][0].nID[0][0]=TOKCASE;
    TokenSyntax[13][0].nID[1][0]=RULNUMBERS;
    TokenSyntax[13][0].nID[2][0]=SEPDUEPUNTI;
    TokenSyntax[13][0].nModificatore[2]=1;  
    TokenSyntax[13][1].nID[0][0]=TOKCASE;
    TokenSyntax[13][1].nID[1][0]=TOKIS;
    TokenSyntax[13][1].nID[2][0]=20;
    TokenSyntax[13][1].nID[2][1]=21;
    TokenSyntax[13][1].nID[2][2]=22;
    TokenSyntax[13][1].nID[2][3]=23;
    TokenSyntax[13][1].nID[2][4]=24;
    TokenSyntax[13][1].nID[2][5]=25;
    TokenSyntax[13][1].nID[2][6]=26;
    TokenSyntax[13][1].nID[3][0]=RULNUMBERS;
    TokenSyntax[13][1].nID[3][1]=RULFPNUMBERS;
    TokenSyntax[13][1].nID[4][0]=SEPDUEPUNTI;
    TokenSyntax[13][1].nModificatore[4]=1;
    TokenSyntax[13][2].nID[0][0]=TOKCASE;
    TokenSyntax[13][2].nID[1][0]=0;
    TokenSyntax[13][2].nID[2][0]=SEPDUEPUNTI;
    TokenSyntax[13][2].nModificatore[2]=1;  
    /* new */
    //TokenSyntax[14][0].nID[0][0]=TOKNEW;
    //TokenSyntax[14][0].nID[1][0]=TOKTABLE;
    //TokenSyntax[14][0].nModificatore[1]=1;  
    /* delete */
    TokenSyntax[15][0].nID[0][0]=TOKDELETE;
    TokenSyntax[15][0].nID[1][0]=0;
    TokenSyntax[15][0].nModificatore[1]=1;  
    /* iso.DispatchMessage */
    TokenSyntax[16][0].nID[0][0]=TOK_ISO_DISPATCHMESSAGE;
    TokenSyntax[16][0].nID[1][0]=SEPPARENTESITONDAAPERTA;
    TokenSyntax[16][0].nID[2][0]=0;
    TokenSyntax[16][0].nID[3][0]=SEPPARENTESITONDACHIUSA;
    TokenSyntax[16][0].nModificatore[3]=1;
	/* object */
    TokenSyntax[17][0].nID[0][0]=TOKOBJECT;
    TokenSyntax[17][0].nID[1][0]=0;
    TokenSyntax[17][0].nModificatore[1]=1;
    /* object con derivazione */
    TokenSyntax[17][1].nID[0][0]=TOKOBJECT;
    TokenSyntax[17][1].nID[1][0]=0;
    TokenSyntax[17][1].nID[2][0]=SEPDUEPUNTI;
    TokenSyntax[17][1].nID[3][0]=0;
    TokenSyntax[17][1].nID[4][0]=SEPPARENTESIQUADRACHIUSA;
    TokenSyntax[17][1].nModificatore[4]=1;
	/* object, chiusura */
    TokenSyntax[17][2].nID[0][0]=TOKOBJECT;
    TokenSyntax[17][2].nID[1][0]=TOKEND;
    TokenSyntax[17][2].nModificatore[1]=1;
}

/*
** Substitution : 
*/
bool CPreProcessorListener::Substitution ()
{
    bool bIsDefineWorld=false;
    int i;

    for (i=0; i<m_ptrCodeArray.GetSize(); i++)   {
        if (!i)
            bIsDefineWorld=IsDefineWorld (((stAdamoToken*)m_ptrCodeArray[0])->nID);
        stAdamoToken* p=(stAdamoToken*) m_ptrCodeArray[i];
        if (p->nID>=AXISINFOBASE&&p->nID<=AXISINFOLAST)
            GestioneGetAxisInfo (p);
        else
            switch (p->nID)   {
                case 0                       :
					if (!ManageEnumSyntax (p))
                    if (!ManageConfigurationSyntax (p))
                        if (!bIsDefineWorld)
                            ManageDefineValue (p);
                    break;
                case TOKSOST_DIGITALINPUT    :
                    SetTokenNumber (p, "1");
                     break;
                case TOKSOST_DIGITALOUTPUT   :
                    SetTokenNumber (p, "2");
                    break;
                case TOKSOST_ANALOGINPUT     :
                    SetTokenNumber (p, "3");
                    break;
                case TOKSOST_ANALOGOUTPUT    :
                    SetTokenNumber (p, "4");
                    break;
                case TOKSOST_AXIS            :
                    SetTokenNumber (p, "5");
                    break;
                case TOKSOST_ENCODER           :
                    SetTokenNumber (p, "6");
                    break;
                case TOKSOST_TIMER           :
                    SetTokenNumber (p, "7");
                    break;
                case TOKSOST_STORAGE         :
                    SetTokenNumber (p, "8");
                    break;
                case TOKSOST_DEVICECAN       :
                    SetTokenNumber (p, "9");
                    break;
                case TOKSOST_ON              :
                    SetTokenNumber (p, "1");
                    break;
                case TOKSOST_OFF             :
                    SetTokenNumber (p, "0");
                    break;
                case TOKSOST_UP              :
                    SetTokenNumber (p, "1");
                    break;
                case TOKSOST_DOWN            :
                    SetTokenNumber (p, "-1");
                    break;
                case TOKSOST_POSITIVE        :
                    SetTokenNumber (p, "1");
                    break;
                case TOKSOST_NEGATIVE        :
                    SetTokenNumber (p, "-1");
                    break;
                case TOKSOST_CLKW            :
                    SetTokenNumber (p, "1");
                    break;
                case TOKSOST_CCLKW     :
                    SetTokenNumber (p, "0");
                    break;
                case TOKSOST_SKIP            :
                    SetTokenNumber (p, "0");
                    break;
                case TOKSOST_WAIT            :
                    SetTokenNumber (p, "1");
                    break;
                case TOKSOST_ACK             :
                    SetTokenNumber (p, "2");
                    break;
                case TOKSOST_DIFFERENT       :
                    p->strToken="~=";
                    break;
            }
    }
    return false;
}

/*
** ManageConfigurationSyntax :
*/
bool CPreProcessorListener::ManageConfigurationSyntax (stAdamoToken* p)
{
    bool b=false;
    if (IsConfigurationSyntax (p->strToken))   {
        char szConfigToken[512];
		char szReadToken[512];
		strcpy (szReadToken, p->strToken);
        if (GetConfigurationToken (szReadToken, szConfigToken)>=0)   {
            p->strToken=szConfigToken;
            b=true;
        }
    }
    return b;
}

/*
** ManageEnumSyntax :
*/
bool CPreProcessorListener::ManageEnumSyntax (stAdamoToken* p)
{
    bool b=false;
    if (IsConfigurationSyntax (p->strToken))   {
        char szConfigToken[512];
		char szReadToken[512];
		strcpy (szReadToken, p->strToken);
        if (GetEnumToken (szReadToken, szConfigToken)==0)   {
            p->strToken=szConfigToken;
            b=true;
        }
    }
    return b;
}

/*
** IsConfigurationSyntax : 
*/
int CPreProcessorListener::IsConfigurationSyntax (const char* szConf)
{
    return strchr (szConf, '.')!=NULL;
}

/*
** GetConfigurationToken :
*/
int CPreProcessorListener::GetConfigurationToken (char* szConf, char* szNewToken)
{
    char szBuffer[512];
    CAdamoBase* obj;
    int nB=-1;

    /* se il primo carattere e' un punto, non consideriamolo */
    if (szConf[0]=='.')
        szConf++;
    /* aggiungiamo il nome della macchina */
    if (m_strWithName!="")
        strcpy (szBuffer, m_strMachineName + "." + m_strWithName + "." + szConf);
    else
        strcpy (szBuffer, m_strMachineName + "." + szConf);
    /* verifichiamo che esista nel map dei dispositivi */
    if (obj=(CAdamoBase*)m_objMachine->GetItem(szBuffer))  {
        /* se esiste, scriviamolo nel file dei dispositivi */
        nB=obj->GetDeviceID ();
        if (nB>=0)
            sprintf (szNewToken, "%d", nB);
    }
    return nB;
}

/*
** GetEnumToken :
*/
int CPreProcessorListener::GetEnumToken (char* szEnum, char* szNewToken)
{
	CString strEnumValue;
    int nB=-1;

    /* verifichiamo che esista nel map dei dispositivi */
	if (m_synArray->GetEnumValue (szEnum, strEnumValue))    {
        strcpy (szNewToken, strEnumValue);
		nB = 0;
	}
    return nB;
}

/*
** SetTokenNumber :
*/
void CPreProcessorListener::SetTokenNumber (stAdamoToken* p, const char* sz)
{
    p->strToken=sz;
    p->nID=RULNUMBERS;
}

/*
** Action
*/
bool CPreProcessorListener::Action ()
{
    int n;

    if (m_bSomethingInteresting)   {
        for (n=0; n<m_nTokenKey; n++)   {
            /* cominciamo con uno bello swiccione sul codice da interpretare */
            switch (m_apTokenKey[n]->nID)   {
                case TOKGLOBAL :
                    ManageGlobal (m_apTokenKey[n]);
                    break;
                case TOKGROUP :
                    ManageGroup (m_apTokenKey[n]);
                    break;
                case TOK_STRING_LOAD :
                    ManageStringLoad (m_apTokenKey[n]);
                    break;
                case TOKDEFINE :
                    ManageDefine (m_apTokenKey[n]);
                    break;
                case TOKUNDEFINE :
                    ManageUnDefine (m_apTokenKey[n]);
                    break;
                case TOKIFDEF :
                    ManageIfDefine (m_apTokenKey[n]);
                    break;
                case TOKIFNOTDEF :
                    ManageIfNotDefine (m_apTokenKey[n]);
                    break;
                case TOKELSEDEF :
                    ManageElseDefine (m_apTokenKey[n]);
                    break;
                case TOKENDDEF :
                    ManageEndDefine (m_apTokenKey[n]);
                    break;
                case TOK_CONF_GETNAME :
                    ManageConfSyntax (m_apTokenKey[n]);
                    break;
                case TOK_CONF_GETPATH :
                    ManageConfSyntax (m_apTokenKey[n]);
                    break;
                case TOK_CONF_GETDESCRIPTION :
                    ManageConfSyntax (m_apTokenKey[n]);
                    break;
                case TOKSWITCH :
                    ManageSwitch (m_apTokenKey[n]);
                    break;
                case TOKCASE :
                    ManageCase (m_apTokenKey[n]);
                    break;
                case TOKNEW :
                    ManageNew (m_apTokenKey[n]);
                    break;
                case TOKDELETE :
                    ManageDelete (m_apTokenKey[n]);
                    break;
                case TOK_ISO_DISPATCHMESSAGE :
                    ManageIsoDispatchMessage (m_apTokenKey[n]);
                    break;
				case TOKOBJECT :
					ManageObjects ();
					break;
            }
        }
    }
    return m_nError!=0;
}

/*
** ManageGlobal
*/
bool CPreProcessorListener::ManageGlobal (stAdamoToken* p)
{
    stGlobalDescriptor gd;

    InitGlobalToken (&gd);
    switch (ProbablyIs (p))   {
        case GLOBALMATRIX :
            ManageMatrix (p, &gd);
            break;
        case GLOBALARRAY :
            ManageArray (p, &gd);
            break;
        case GLOBALVARIABLE :
            ManageVariable (p, &gd);
            break;
        case GLOBALOBJECT :
            ManageObjectInstance (p, &gd);
			break;
    }
    return SaveResult (&gd);
}

/*
** ManageGroup
*/
bool CPreProcessorListener::ManageGroup (stAdamoToken* p)
{
    bool b=true;
    m_strWithName=p->strToken;
    /* verifichiamo che esista nel map dei dispositivi */
    if (!(CAdamoBase*)m_objMachine->GetItem(m_strMachineName+"."+m_strWithName))  {
        /* se non esiste, diamo un errore */
        m_nError=-8; b=false;
    }
    CleanUp ();
    return b;
}

/*
** ManageStringLoad
*/
bool CPreProcessorListener::ManageStringLoad (stAdamoToken* p)
{
    CString strMsg, strDesc;
    long dw;
    bool b=true;
    char szBuffer[32];

    int nIndex=p->nIndex;
    p=(stAdamoToken*)m_ptrCodeArray[nIndex+2];
    if (p->nID==0)   {
        /* cerchiamo se c'e' nella base dati una stringa */
        if (m_tr->MessaggioStringa (0, p->strToken, &dw, &strMsg, &strDesc))   {
            sprintf (szBuffer, "%d", dw);
            p->nID=RULNUMBERS;
            p->strToken=szBuffer;
        }
        else   {
            /* non c'e', diamo un errore */
            m_nError=-9; b=false;
        }
    }
    return b;
}

/*
** ManageStringLoad
*/
bool CPreProcessorListener::ManageIsoDispatchMessage (stAdamoToken* p)
{
    stAdamoToken* q;
    CString str;
    bool b=false;

    int nIndex=p->nIndex;
    q=(stAdamoToken*)m_ptrCodeArray[nIndex+2];
    if (q->nID==0)   {
        /* cerchiamo se c'e' nella base dati una stringa */
        str.Format ("%s[0](%s[1], %s[2])", q->strToken, q->strToken, q->strToken);
        b=true;
    }
    if (b)   {
        p->strToken=str;
        RemoveEl (nIndex+1, 3);
    }
    return b;
}

/*
** InitGlobalToken
*/
void CPreProcessorListener::InitGlobalToken (stGlobalDescriptor* gd)
{
    gd->mat.nCols=0;
    memset (gd->mat.strColumnName, NULL, MAXCOLUMNS*sizeof (char *));
}

/*
** ProbablyIs :
*/
int CPreProcessorListener::ProbablyIs (stAdamoToken* p)
{
    int n, nB;
    WORD nBitMask=0;

    for (n=p->nIndex; n<m_ptrCodeArray.GetSize(); n++)   {
        p=(stAdamoToken*)m_ptrCodeArray[n];
        if (p->nID==SEPPARENTESIQUADRAAPERTA)
            nBitMask|=1;
        else
            if (p->nID==SEPDUEPUNTI)   {
                nBitMask|=2;
                break;
            }
			else
				if (p->nID==TOKNEW)   {
					nBitMask|=0x04;
					break;
				}
    }
    switch (nBitMask)   {
        case 0 :
            nB=GLOBALVARIABLE; break;
        case 1 :
            nB=GLOBALARRAY;    break;
        case 2 :
        case 3 :
            nB=GLOBALMATRIX;   break;
		case 4 :
			nB=GLOBALOBJECT;   break;
    }
    return nB;
}

/*
** SaveResult :
*/
bool CPreProcessorListener::SaveResult (stGlobalDescriptor* gd)
{
	int nB;
    bool b=true;

	stGlobalDescriptor* pMat=new stGlobalDescriptor(*gd);
	pMat->strModule=m_strPathDest;
	nB = m_glbArray->Add(pMat);
	if (nB == -1)   {
		delete pMat;
		m_nError=-7;
        b=false;
	}
	else   {
		if (nB == -2)   {
			for (int i=0; i<pMat->mat.nCols; i++)
				delete pMat->mat.strColumnName[i];
			delete pMat;
			CleanUp ();
		}
		else   {
			if (pMat->nType != GLOBALOBJECT)   {
				if (pMat->nType == GLOBALMATRIX && !m_bGiorgettiClary)   {
					stGlobalDescriptor* pVar = new stGlobalDescriptor;
					pVar->strModule = m_strPathDest;
					pVar->nType = GLOBALVARIABLE;
					pVar->strName = "_GIORGETTI_CLARY";
					pVar->nVarType = LUA_TNUMBER;
					pVar->arr.nArrElements = 0;
					pVar->mat.nCols = 0;
					memset (pVar->mat.strColumnName, NULL, MAXCOLUMNS*sizeof (char *));
					m_glbArray->Add(pVar);
					m_bGiorgettiClary = true;
				}
				ChangeLine (WriteResultLine (gd));
			}
		}
	}
    return b;
}

/*
** WriteResultLine : scrive la stringa di risultato
*/
CString CPreProcessorListener::WriteResultLine (struct stGlobalDescriptor* gd)
{
    char szBuffer[256];
    switch (gd->nType)   {
        case GLOBALVARIABLE :
            switch (gd->nVarType)   {
                case LUA_TBOOLEAN :
                    sprintf (szBuffer, "%s=false", (const char*) gd->strName);
                    break;
                case LUA_TNUMBER :
                    sprintf (szBuffer, "%s=0.0", (const char*) gd->strName);
                    break;
                case LUA_TSTRING :
                    sprintf (szBuffer, "%s=\"\"", (const char*) gd->strName);
                    break;
                case LUA_TTABLE :
					sprintf (szBuffer, "%s={}", (const char*) gd->strName);
					break;
            }
            break;
        case GLOBALARRAY :
            sprintf (szBuffer, "%s={}", (const char*) gd->strName);
            break;
        case GLOBALMATRIX :
            sprintf (szBuffer, "%s={}; for _GIORGETTI_CLARY=0, %d do %s[_GIORGETTI_CLARY]={} end;", (const char*) gd->strName, gd->mat.nRows, (const char*) gd->strName);
            break;
    }
    return szBuffer;
}

/*
** ManageVariable :
*/
bool CPreProcessorListener::ManageVariable (stAdamoToken* p, stGlobalDescriptor* gd)
{
    int nIndex=p->nIndex;
    gd->nType=GLOBALVARIABLE;
    gd->strName=((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken;
    switch (((stAdamoToken*)m_ptrCodeArray[nIndex+3])->nID)   {
        case TOKBOOLEAN : gd->nVarType=LUA_TBOOLEAN; break;
        case TOKNUMBER  : gd->nVarType=LUA_TNUMBER;  break;
        case TOKSTRING  : gd->nVarType=LUA_TSTRING;  break;
        case TOKTABLE   : gd->nVarType=LUA_TTABLE; break;
    }
    return true;
}

/*
** ManageArray :
*/
bool CPreProcessorListener::ManageArray (stAdamoToken* p, stGlobalDescriptor* gd)
{
    int nIndex=p->nIndex;
    bool b=false;

    gd->nType=GLOBALARRAY;
    gd->strName=((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken;
    gd->arr.nArrElements=atoi(((stAdamoToken*)m_ptrCodeArray[nIndex+3])->strToken);
    if (gd->arr.nArrElements>0&&gd->arr.nArrElements<MAXROWS)   {
        switch (((stAdamoToken*)m_ptrCodeArray[nIndex+6])->nID)   {
            case TOKBOOLEAN :  gd->arr.nArrType=LUA_TBOOLEAN;	break;
            case TOKNUMBER  :  gd->arr.nArrType=LUA_TNUMBER;	break;
            case TOKSTRING  :  gd->arr.nArrType=LUA_TSTRING;	break;
        }
        b=true;
    }
    else
        m_nError=-3;
    return b;
}

/*
** ManageMatrix :
*/
bool CPreProcessorListener::ManageMatrix (stAdamoToken* p, stGlobalDescriptor* gd)
{
    int nIndex=p->nIndex, i, n;
    bool b=false;

    gd->nType=GLOBALMATRIX;
    gd->strName=((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken;
    gd->mat.nRows=atoi(((stAdamoToken*)m_ptrCodeArray[nIndex+3])->strToken);
    if (gd->mat.nRows>0&&gd->mat.nRows<MAXROWS)   {
        i=0; n=nIndex+i+6;
        b=true;
        while (n<m_ptrCodeArray.GetSize())   {
            if (gd->mat.nCols==MAXCOLUMNS)   {
                m_nError=-4; b=false; break;
            }
            switch (((stAdamoToken*)m_ptrCodeArray[n])->nID)   {
                case TOKBOOLEAN : gd->mat.anTypes[gd->mat.nCols]=LUA_TBOOLEAN; break;
                case TOKNUMBER  : gd->mat.anTypes[gd->mat.nCols]=LUA_TNUMBER; break;
                case TOKSTRING  : gd->mat.anTypes[gd->mat.nCols]=LUA_TSTRING; break;
            }
            CString strFieldName=((stAdamoToken*)m_ptrCodeArray[n+2])->strToken;
            if (strFieldName.GetLength()>NMAXIDENTIFIER)   {
                m_nError=-6; b=false; break;
            }
            else   {
                gd->mat.strColumnName[gd->mat.nCols]=new char [strFieldName.GetLength()+1];
                strcpy (gd->mat.strColumnName[gd->mat.nCols], strFieldName);
				gd->mat.nCols++;
            }
            i+=4; n=nIndex+i+6;
        }
    }
    else
        m_nError=-3;
    return b;
}

/*
** ManageObjectInstance :
*/
bool CPreProcessorListener::ManageObjectInstance (stAdamoToken* p, stGlobalDescriptor* gd)
{
	int nIndex=p->nIndex;

    gd->nType=GLOBALOBJECT;
    gd->strName=((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken;
    gd->nVarType=LUA_TTABLE;
	RemoveEl (nIndex, 1);
	return true;
}

/*
** GestioneGetAxisInfo :
*/
void CPreProcessorListener::GestioneGetAxisInfo (stAdamoToken* p)
{
    char szBuffer[256];
    sprintf (szBuffer, "%d", AxisInfo[p->nID-AXISINFOBASE].nCode);
    p->strToken=szBuffer;
    p->nID=RULNUMBERS;
}

/*
** ManageDefine :
*/
bool CPreProcessorListener::ManageDefine (stAdamoToken* p)
{
    CString strToken, strValue, strKey;
    int nIndex=p->nIndex, n=0;
    bool b=false;

    if (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken.Find ('.')==-1)
        if (m_deftok->Lookup (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken, strKey))
            m_nError=-12;
        else   {
            strToken=((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken;
            if (m_ptrCodeArray.GetSize ()>nIndex+2)   {
                switch (((stAdamoToken*)m_ptrCodeArray[nIndex+2])->nID)   {
					case RULHEXNUMBERS :
						strValue=((stAdamoToken*)m_ptrCodeArray[nIndex+2])->strToken;
						n = 1;
						break;
                    case RULFPNUMBERS :
                    case RULNUMBERS :
                        strValue=((stAdamoToken*)m_ptrCodeArray[nIndex+2])->strToken;
                        n=1;
                        break;
                    case RULSTRINGS :
                        strValue='"'+((stAdamoToken*)m_ptrCodeArray[nIndex+2])->strToken+'"';
                        n=1;
                        break;
					default :
						m_nError = -2;
                }
            }
            m_deftok->SetAt (strToken, strValue);
            RemoveEl (nIndex, 2+n);
            b=true;
        }
    else
        m_nError=-11;
    return b;
}

/*
** ManageUnDefine :
*/
bool CPreProcessorListener::ManageUnDefine (stAdamoToken* p)
{
    CString strToken, strKey;
    int nIndex=p->nIndex;
    bool b=false;

    if (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken.Find ('.')==-1)
        if (!m_deftok->Lookup (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken, strKey))
            m_nError=-12;
        else   {
            strToken=((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken;
            m_deftok->RemoveKey (strToken);
            RemoveEl (nIndex, 2);
            b=true;
        }
    else
        m_nError=-11;
    return b;
}

/*
** ManageIfDefine :
*/
bool CPreProcessorListener::ManageIfDefine (stAdamoToken* p)
{
    stStackDefine sd;
    CAdamoData adOp1;
    CAdamoData adOp2;
    CString key;
    int nOp;
    int nIndex=p->nIndex;
    int n=0;
    bool bDefined, nB=false, m_bComplexSyntax=false;

    if (m_bWriteCode==false)   {
        sd.bWriteCode=true;
        sd.bSaveCode=false;
        sd.bElse=false;
        m_stackDefine.push (sd);
    }
    else   {
        if (m_ptrCodeArray.GetSize()>2)
            m_bComplexSyntax=true;
        if (m_bComplexSyntax)   {
            if (!GetIfDefOperator (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->nID, ((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken, &adOp1, &bDefined))
                m_nError=-2;
            else
                if (GetOperation (((stAdamoToken*)m_ptrCodeArray[nIndex+2])->nID, &nOp))   {
                    if (!GetIfDefOperator (((stAdamoToken*)m_ptrCodeArray[nIndex+3])->nID, ((stAdamoToken*)m_ptrCodeArray[nIndex+3])->strToken, &adOp2, &bDefined))   {
                        m_nError=-2;
                    }
                    else   {
                        Cast (&adOp1, &adOp2);
                        if (Operation (&adOp1, nOp, &adOp2))
                            m_bWriteCode=true;
                        else
                            m_bWriteCode=false;
                        sd.bWriteCode=m_bWriteCode;
                        sd.bSaveCode=true;
                        sd.bElse=false;
                        m_stackDefine.push (sd);
                        n=2; nB=true;
                    }
                }
                else
                    m_nError=-2;
        }
        else   {
            if (IsTokenDefined (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken))
                m_bWriteCode=true;
            else
                m_bWriteCode=false;
            sd.bWriteCode=m_bWriteCode;
            sd.bSaveCode=true;
            sd.bElse=false;
            m_stackDefine.push (sd);
            nB=true;
        }
    }
    RemoveEl (nIndex, 2+n);
    return nB;
}

/*
** ManageIfNotDefine :
*/
bool CPreProcessorListener::ManageIfNotDefine (stAdamoToken* p)
{
    stStackDefine sd;
    CAdamoData adOp1;
    CAdamoData adOp2;
    CString key;
    int nOp;
    int nIndex=p->nIndex;
    int n=0;
    bool bDefined, nB=false, m_bComplexSyntax=false;

    if (m_bWriteCode==false)   {
        sd.bWriteCode=true;
        sd.bSaveCode=false;
        sd.bElse=false;
        m_stackDefine.push (sd);
    }
    else   {
        if (m_ptrCodeArray.GetSize()>2)
            m_bComplexSyntax=true;
        if (m_bComplexSyntax)   {
            if (!GetIfDefOperator (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->nID, ((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken, &adOp1, &bDefined))
                m_nError=-2;
            else
                if (GetOperation (((stAdamoToken*)m_ptrCodeArray[nIndex+2])->nID, &nOp))   {
                    if (!GetIfDefOperator (((stAdamoToken*)m_ptrCodeArray[nIndex+3])->nID, ((stAdamoToken*)m_ptrCodeArray[nIndex+3])->strToken, &adOp2, &bDefined))   {
                        m_nError=-2;
                    }
                    else   {
                        Cast (&adOp1, &adOp2);
                        if (Operation (&adOp1, nOp, &adOp2))
                            m_bWriteCode=false;
                        else
                            m_bWriteCode=true;
                        sd.bWriteCode=m_bWriteCode;
                        sd.bSaveCode=true;
                        sd.bElse=false;
                        m_stackDefine.push (sd);
                        n=2; nB=true;
                    }
                }
                else
                    m_nError=-2;
        }
        else   {
            if (IsTokenDefined (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken))
                m_bWriteCode=false;
            else
                m_bWriteCode=true;
            stStackDefine sd;
            sd.bWriteCode=m_bWriteCode;
            sd.bSaveCode=true;
            sd.bElse=false;
            m_stackDefine.push (sd);
            nB=true;
        }
    }
    RemoveEl (nIndex, 2+n);
    return nB;
}

/*
** ManageElseDefine :
*/
bool CPreProcessorListener::ManageElseDefine (stAdamoToken* p)
{
    stStackDefine sd;
    if (m_stackDefine.getcount ()>0)   {
        sd=m_stackDefine.pop ();
        if (sd.bElse)
            m_nError=-13;
        else   {
            sd.bElse=true;
            m_bWriteCode=sd.bWriteCode=!sd.bWriteCode;
            m_stackDefine.push (sd);
        }
    }
    else
        m_nError=-14;
    CleanUp ();
    return true;
}

/*
** ManageEndDefine :
*/
bool CPreProcessorListener::ManageEndDefine (stAdamoToken* p)
{
    stStackDefine sd;
    if (m_stackDefine.getcount ()>0)   {
        sd=m_stackDefine.pop ();
        m_bWriteCode=sd.bSaveCode;
    }
    else
        m_nError=-15;
    CleanUp ();
    return true;
}

/*
** ChangeLine :
*/
void CPreProcessorListener::ChangeLine (CString str)
{
    CleanUp ();
    stAdamoToken* p=new stAdamoToken;
    p->nID=0;
    p->strToken=str;
    p->nIndex=m_ptrCodeArray.Add (p);
}

/*
** GetIfDefOperator :
*/
bool CPreProcessorListener::GetIfDefOperator (int nId, CString strToken, CAdamoData* pOp, bool* bDefined)
{
    CString key;
    double fp;
    char* p;
    bool nB=true;

    *bDefined=false;
    if (nId==0)
        if (m_deftok->Lookup (strToken, key))   {
            *bDefined=true;
            if (key[0]=='"')   {
                pOp->nType=LUA_TSTRING;
                strlimcpy (pOp->AdamoData.str, key, NMAXSTRING+1);
            }
            else   {
                fp=strtod (key, &p);
                if (*p=='\0')   {
                    pOp->nType=LUA_TNUMBER;
                    pOp->AdamoData.fp=fp;
                }
                else   {
                    m_nError=-2;
                    nB=false;
                }
            }
        }
        else   {
            fp=strtod (strToken, &p);
            if (*p=='\0')   {
                pOp->nType=LUA_TNUMBER;
                pOp->AdamoData.fp=fp;
            }
            else
                nB=false;
        }
    else
        switch  (nId)   {
            case RULSTRINGS :
                pOp->nType=LUA_TSTRING;
                strlimcpy (pOp->AdamoData.str, strToken, NMAXSTRING+1);
                break;
            case RULNUMBERS :
                pOp->nType=LUA_TNUMBER;
                pOp->AdamoData.fp=(double) atof (strToken);
                break;
            default :
                m_nError=-2;
       }
    return nB;
}

/*
** Cast :
*/
void CPreProcessorListener::Cast (CAdamoData* adOp1, CAdamoData* adOp2)
{
    double fp;
    char szBuffer[NMAXSTRING+1], *p;

    if (adOp1->nType==LUA_TSTRING)   {
        if (adOp2->nType==LUA_TNUMBER)   {
            sprintf (szBuffer, "%8.3f", adOp2->AdamoData.fp);
            strcpy (adOp2->AdamoData.str, szBuffer);
        }
    }
    else
        if (adOp1->nType==LUA_TNUMBER)   {
            if (adOp2->nType==LUA_TSTRING)   {
                fp=strtod (adOp2->AdamoData.str, &p);
                adOp2->AdamoData.fp=0.0;
                if (*p=='\0')
                    adOp2->AdamoData.fp=fp;
            }
        }        
}

/*
** Operation :
*/
bool CPreProcessorListener::Operation (CAdamoData* adOp1, int nOp, CAdamoData* adOp2)
{
    int n=0;
    if (adOp1->nType==LUA_TNUMBER)   {
        if (nOp&0x01)   {
            n|=(adOp1->AdamoData.fp>adOp2->AdamoData.fp ? 1 : 0);
        }
        if (nOp&0x02)   {
            n|=(ISEQUAL (adOp1->AdamoData.fp, adOp2->AdamoData.fp) ? 1 : 0);
        }
        if (nOp&0x04)   {
            n|=(adOp1->AdamoData.fp<adOp2->AdamoData.fp ? 1 : 0);
        }
    }
    else   {
        if (nOp&0x01)   {
            n|=(strcmp (adOp1->AdamoData.str, adOp2->AdamoData.str)>0 ? 1 : 0);
        }
        if (nOp&0x02)   {
            n|=(strcmp (adOp1->AdamoData.str, adOp2->AdamoData.str)==0 ? 1 : 0);
        }
        if (nOp&0x04)   {
            n|=(strcmp (adOp1->AdamoData.str, adOp2->AdamoData.str)<0 ? 1 : 0);
        }
    }
    return n!=0;
}

/*
** GetOperation :
*/
bool CPreProcessorListener::GetOperation (int nID, int* nOp)
{
    bool b=true;

    switch (nID)   {
        case 20 : *nOp=0x01; break;
        case 21 : *nOp=0x02; break;
        case 22 : *nOp=0x04; break;
        case 23 : *nOp=0x03; break;
        case 24 : *nOp=0x05; break;
        case 25 : *nOp=0x06; break;
        default : b=false;
    }
    return b;
}

/*
** ManageErrors :
*/
void CPreProcessorListener::ManageErrors ()
{
    switch (m_nError)   {
        case -1     :
            /* errore durante il preprocessing */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_1);
            break;
        case -2 :
            /* errore di sintassi */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_2);
            break;
        case -3 :
            /* troppe righe di matrice o troppi elementi di array */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_3);
            break;
        case -4 :
            /* troppe colonne di matrice */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_4);
            break;
        case -5 :
            /* riga non completa */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_5);
            break;
        case -6 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_6);
            break;
        case -7 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_7);
            break;
        case -8 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_8);
            break;
        case -9 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_9);
            break;
        case -10 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_10);
            break;
        case -11 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_11);
            break;
        case -12 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_12);
            break;
        case -13 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_13);
            break;
        case -14 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_14);
            break;
        case -15 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_15);
            break;
        case -16 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_16);
            break;
        case -17 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_17);
            break;
        case -18 :
            /* identificatore troppo lungo */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_18);
            break;
        case -19 :
            /* numero di " sbagliato */
            m_strError = LOADSTRING (IDS_ADAMOPREPROCESSOR_19);
            break;
        default :
            break;
    }
}

bool CPreProcessorListener::CheckParticularRules (stAdamoToken* p, int nCode)
{
    bool nB=false;
    char* pChar, *pStr;

    switch (nCode)   {
        case RULFPNUMBERS :
            if (p->strToken=="-"||p->strToken=="+")   {
                int nIndex=p->nIndex+1;
                pStr=((stAdamoToken*)m_ptrCodeArray[nIndex])->strToken.GetBuffer (0);
                strtod (pStr, &pChar);
                if (*pChar=='\0')   {
                    p->strToken=p->strToken+((stAdamoToken*)m_ptrCodeArray[nIndex])->strToken;
                    delete (stAdamoToken *)m_ptrCodeArray[nIndex];
                    m_ptrCodeArray.RemoveAt (nIndex);
                }
            }
            pStr=p->strToken.GetBuffer (p->strToken.GetLength()+1);
            strtod (pStr, &pChar);
            if (*pChar=='\0')   {
                p->nID=RULFPNUMBERS;
                nB=true;
            }
            p->strToken.ReleaseBuffer ();
            break;
        case RULFPDEVICE :
            if (ExistConfigurationToken (p))   {
                p->nID=RULFPDEVICE;
                nB=true;
            }
            break;
        case RULFPDEVICETYPE :
            if (p->nID>=TOKSOST_DIGITALINPUT&&p->nID<=TOKSOST_DEVICECAN)
                nB=true;
            break;
    }
    return nB;    
}

bool CPreProcessorListener::ExistConfigurationToken (stAdamoToken* p)
{
    CString str;
    CAdamoBase* obj;
    int nB=-1;

    /* se e' gia stato assegnato un dispositivo, vuol dire che esiste in configurazione */
    if (p->pDev)
        return true;
    char* szConf=p->strToken.GetBuffer (p->strToken.GetLength()+1);
    /* se il primo carattere e' un punto, non consideriamolo */
    if (szConf[0]=='.')
        szConf++;
    /* aggiungiamo il nome della macchina */
    if (m_strWithName!="")
        str=m_strMachineName + "." + m_strWithName + "." + szConf;
    else
        str=m_strMachineName + "." + szConf;
    /* verifichiamo che esista nel map dei dispositivi */
    if (obj=(CAdamoBase*)m_objMachine->GetItem(str))  {
        /* se esiste, scriviamolo nel file dei dispositivi */
        nB=obj->GetDeviceID ();
        if (nB>=0)   {
            p->strToken.Format ("%d", nB);
            p->pDev=obj;
        }
    }
    p->strToken.ReleaseBuffer ();
    return nB>=0;
}

/*
** ManageConfSyntax : gestisce i token conf.GetName, conf.GetPath e 
**                    conf.GetDescription.
*/
bool CPreProcessorListener::ManageConfSyntax (stAdamoToken* p)
{
    CAdamoBase* pDev;
    int nIndex=p->nIndex;

    if (((stAdamoToken*)m_ptrCodeArray[nIndex+3])->nID==SEPPARENTESITONDACHIUSA)   {
        pDev=((stAdamoToken*)m_ptrCodeArray[nIndex+2])->pDev;
        if (pDev)   {
            /* siamo in presenza di sintassi cui manca il tipo di dispositivo, correggiamo inserendolo */
            /* la virgola */
            stAdamoToken* pNew=new stAdamoToken;
            pNew->strToken=",";
            pNew->nID=SEPVIRGOLA;
            pNew->nIndex=nIndex+3;
            m_ptrCodeArray.InsertAt (pNew->nIndex, pNew);
            /* il tipo di dispositivo gia' sostituito */
            pNew=new stAdamoToken;
            switch (pDev->GetType ())   {
                case enInputDigitale :
                    pNew->strToken="1";
                    break;
                case enOutputDigitale :
                    pNew->strToken="2";
                    break;
                case enInputAnalogico :
                    pNew->strToken="3";
                    break;
                case enOutputAnalogico :
                    pNew->strToken="4";
                    break;
                case enAsse :
                    pNew->strToken="5";
                    break;
                case enTimer :
                    pNew->strToken="6";
                    break;
                case enCounter :
                    pNew->strToken="7";
                    break;
                case enGenericCan :
                    pNew->strToken="8";
                    break;
            }
            pNew->nID=RULNUMBERS;
            pNew->nIndex=nIndex+4;
            m_ptrCodeArray.InsertAt (pNew->nIndex, pNew);
        }
    }
    return true;
}

/*
** IsTokenDefined : ritorna true se il token passato per parametro
**                  e' definito.
*/
bool CPreProcessorListener::IsTokenDefined (CString strToken)
{
    CString key;
    bool b=false;

    if (m_deftok->Lookup (strToken, key))
        b=true;
    return b;
}

/*
** ManageSwitch : gestisce la sintassi di switch e switch end
*/
bool CPreProcessorListener::ManageSwitch (stAdamoToken* p)
{
    int nIndex=p->nIndex, nMode=0;
    if (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->nID==405)
        nMode=1;
    if (nMode)   {
        /* dobbiamo chiudere lo switch, poppiamo dallo stack */
        if (m_stackSwitch.getcount ()>0)   {
            m_stackSwitch.pop ();    
        }
        else
            m_nError=-16;
        /* rimuoviamo l'indice dello switch */
        delete p;
        m_ptrCodeArray.RemoveAt (nIndex);
    }
    else   {
        /* nuovo switch */
        stStackSwitch sw;
        sw.strVariable=BuildSwitchVariable (p);
        m_stackSwitch.push (sw);
    }
    return true;
}

/*
** BuildSwitchVariable :
*/
CString CPreProcessorListener::BuildSwitchVariable (stAdamoToken* p)
{
    CString str, strVar, strLoc;
    int i = 0;
	int nParentesiTonda = 0;
    bool bVarOk = false;

	i = p->nIndex + 1;
    while (i < m_ptrCodeArray.GetCount ())   {
        if (((stAdamoToken*)m_ptrCodeArray[i])->nID == 31)   {
            /* parentesi aperta */
			if (nParentesiTonda == 0)   {
				str = ((stAdamoToken*)m_ptrCodeArray[i])->strToken;
				bVarOk = true;
			}
			else
				/* quello che ci sta in mezzo */
                str += ((stAdamoToken*)m_ptrCodeArray[i])->strToken;
			nParentesiTonda++;
        }
        else   {
            if (bVarOk)
				/* quello che ci sta in mezzo */
                str += ((stAdamoToken*)m_ptrCodeArray[i])->strToken;
			if (((stAdamoToken*)m_ptrCodeArray[i])->nID == 32)   {
				/* parentesi chiusa */
				nParentesiTonda--;
				if (nParentesiTonda == 0)
					break;
			}
        }
        i++;
    }
	/* formiamo ora la variabile per i case */
	strVar.Format ("__internal_switch__%d", m_stackSwitch.getcount ());
    /* cancelliamo ogni traccia dello switch */
    RemoveEl (p->nIndex + 1, m_ptrCodeArray.GetCount () - p->nIndex - 1);
	/* ora sostituiamolo con l'assegnamento */
	strLoc = "local";
	str = strLoc + " " + strVar + " = " + str;
    p->nID=0;
    p->strToken=str;
    return strVar;
}

/*
** ManageCase : gestisce la sintassi di case
*/
bool CPreProcessorListener::ManageCase (stAdamoToken* p)
{
    stStackSwitch sw;
    CString str;
    int nIndex=p->nIndex, nMode=0;

    if (m_stackSwitch.getcount()>0)   {
        if (((stAdamoToken*)m_ptrCodeArray[nIndex+1])->nID==TOKIS)
            nMode=1;
        sw=m_stackSwitch.peek ();
        if (nMode==0)   {
            /* prendiamo l'ultima variabile dello switch */
            if (!sw.nStep)   {
                str.Format ("if (%s==%s) then", sw.strVariable, ((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken);
                sw.nStep=1;
                m_stackSwitch.pop ();
                m_stackSwitch.push (sw);
            }
            else
                str.Format ("elseif (%s==%s) then", sw.strVariable, ((stAdamoToken*)m_ptrCodeArray[nIndex+1])->strToken);
            /* cancelliamo ogni traccia dello switch */
            RemoveEl (nIndex+1, 2);
            p->nID=0;
            p->strToken=str;
        }
        else   {
            if (!sw.nStep)   {
                str.Format ("if (%s", sw.strVariable);
                sw.nStep=1;
                m_stackSwitch.pop ();
                m_stackSwitch.push (sw);
            }
            else
                str.Format ("elseif (%s", sw.strVariable);
            switch (((stAdamoToken*)m_ptrCodeArray[nIndex+2])->nID)   {
                case 20 :
                    str+=">";
                    break;
                case 21 :
                    str+="==";
                    break;
                case 22 :
                    str+="<";
                    break;
                case 23 :
                    str+=">=";
                    break;
                case 24 :
                    str+="~=";
                    break;
                case 25 :
                    str+="<=";
                    break;
                case 26 :
                    str+="==";
                    break;
            }
            str+=((stAdamoToken*)m_ptrCodeArray[nIndex+3])->strToken;
            str+=") then";
            RemoveEl (nIndex+1, 4);
            p->nID=0;
            p->strToken=str;
        }
    }
    else
        m_nError=-18;
    return true;
}

/*
** RemoveEl : rimuove dall'array count token a partire da index
*/
void CPreProcessorListener::RemoveEl (int nIndex, int nCount)
{
    int i;

    for (i=0; i<nCount; i++)
        delete (stAdamoToken *)m_ptrCodeArray[nIndex+i];
    m_ptrCodeArray.RemoveAt (nIndex, nCount);
    for (i=nIndex; i<m_ptrCodeArray.GetSize(); i++)
        ((stAdamoToken*)m_ptrCodeArray[i])->nIndex=i;
}

/*
** ManageNew : gestisce la sintassi della key new
*/
bool CPreProcessorListener::ManageNew (stAdamoToken* p)
{
	int nIndex;

	/* allocazione di una table */
	if (((stAdamoToken*)m_ptrCodeArray[p->nIndex+1])->nID == TOKTABLE)   {
		p->nID=0;
		p->strToken="{}";
		p->strRiporto+=((stAdamoToken*)m_ptrCodeArray[p->nIndex+1])->strRiporto;
		RemoveEl (p->nIndex+1, 1);
	}
	else
	/* allocazione di un oggetto */
	if (((stAdamoToken*)m_ptrCodeArray[p->nIndex+1])->nID == 0)   {
		nIndex = p->nIndex;
		RemoveEl (nIndex, 1);
		/* aggiungiamo il tipo all'oggetto */
		if (m_ptrCodeArray.GetSize () >= 2)   {
            stAdamoToken* p=new stAdamoToken;
			CString str, strInstanceName = GetInstanceName (nIndex), strClassName = ((stAdamoToken*)m_ptrCodeArray[nIndex])->strToken;
			str.Format (" %s.%s = \"%s\"", strInstanceName, "class__name", strClassName);
			p->nID = 0;
			p->nIndex = m_ptrCodeArray.GetSize ();
			p->pDev = NULL;
			p->strRiporto = "";
			p->strToken = str;
			m_ptrCodeArray.Add (p);
		}
	}
    return true;
}

/*
** GetInstanceName :
*/
CString CPreProcessorListener::GetInstanceName (int nIndex)
{
	CString strRet;
	int nBaseIndex = nIndex, nParatentesiQuadra = 0;

	nIndex -= 2;
	nBaseIndex = nIndex;
	while (nIndex >= 0)   {
		stAdamoToken* p = ((stAdamoToken*)m_ptrCodeArray[nIndex]);
		if (p->nID == 0)
			if (nParatentesiQuadra == 0)  {
				break;
		}
		else
		if (p->nID == SEPPARENTESIQUADRACHIUSA)
			nParatentesiQuadra++;
		else
		if (p->nID == SEPPARENTESIQUADRAAPERTA)
			nParatentesiQuadra--;
		nIndex--;
	}
	while (nIndex <= nBaseIndex)   {
		stAdamoToken* p = ((stAdamoToken*)m_ptrCodeArray[nIndex]);
		strRet += p->strToken; 
		nIndex++;
	}
	return strRet;
}

/*
** ManageDelete : gestisce la sintassi della key delete
*/
bool CPreProcessorListener::ManageDelete (stAdamoToken* p)
{
    CString str, strArg;
    int i=1;

    while ((p->nIndex+i+1)<m_ptrCodeArray.GetSize()&&(((stAdamoToken*)m_ptrCodeArray[p->nIndex+i+1])->nID==SEPPARENTESIQUADRACHIUSA||
           ((stAdamoToken*)m_ptrCodeArray[p->nIndex+i+1])->nID==SEPPARENTESIQUADRAAPERTA||
           ((stAdamoToken*)m_ptrCodeArray[p->nIndex+i+1])->nID==RULNUMBERS))   {
           strArg+=((stAdamoToken*)m_ptrCodeArray[p->nIndex+i+1])->strToken;
           i++;
    }
    str.Format ("misc.DeleteTable (%s%s)", ((stAdamoToken*)m_ptrCodeArray[p->nIndex+1])->strToken, strArg);
    p->nID=0;
    p->strToken=str;
    p->strRiporto+=((stAdamoToken*)m_ptrCodeArray[p->nIndex+i])->strRiporto;
    RemoveEl (p->nIndex+1, i);
    return true;
}

/*
** FilterInputBuffer :
*/
int CPreProcessorListener::FilterInputBuffer ()
{
    CString str (m_strBuffer);
    int nPos, nStartPos=0, nRet = 0;

    while ((nPos=str.Find ('"', nStartPos))!=-1&&nPos>0)   {
        DeleteBlanks (str, nPos);
        if ((nStartPos=str.Find ('"', nPos+1))==-1)
            break;
        nStartPos++;    
    }
    m_strBuffer=str;
	if (CharacterCount (m_strBuffer, '"') % 2)   {
		nRet = -1;
		m_nError = -19;
	}
	return nRet;
}

/*
** CharacterCount :
*/
int CPreProcessorListener::CharacterCount (CString str, char ch )
{
    if (str.IsEmpty())    {
        return 0;
    }
    int nFind = -1;
    int nCount = 0;
    while( -1 != ( nFind = str.Find( ch, nFind + 1 )))
    {
        nCount++;
    }
    return nCount;
}

/*
** DeleteBlanks :
*/
void CPreProcessorListener::DeleteBlanks (CString& str, int& nPos)
{
    int nCounter=nPos-1;
    while (nCounter&&(str[nCounter]==' '||str[nCounter]=='\t'))   {
        str.Delete (nCounter);
        nCounter--; nPos--;
    }
    while (nCounter&&IsSeparator(str[nCounter]))   {
        nCounter--;
    }
    while (nCounter&&(str[nCounter]==' '||str[nCounter]=='\t'))   {
        str.Delete (nCounter);
        nCounter--; nPos--;
    }
}

/*
** IsSeparator : ritorna true se il carattere passato e' un carattere di separazione
*/
bool CPreProcessorListener::IsSeparator (char ch)
{
    CString str ("+-*/^.{};>=<~,[]:()");
    return str.Find (ch)!=-1;
}

/*
** ManageDefineValue : verifica se il valore passato per parametro e' una define e nel caso esegue la
**                     sostituzione.
*/
bool CPreProcessorListener::ManageDefineValue (stAdamoToken* p)
{
    CString strValue;
    char *c;

    bool b=false;
    if (m_deftok->Lookup (p->strToken, strValue))   {
        p->strToken=strValue;
        strtol (strValue, &c, 10);
        if (*c=='\0'||*c=='\t')
            p->nID=RULNUMBERS;
        else
            p->nID=0;
        b=true;
    }
    return b;
}

/*
** IsDefineWorld :
*/
bool CPreProcessorListener::IsDefineWorld (int nID)
{
    bool b=false;

    switch (nID)   {
        case TOKDEFINE   :
            b=true; break;
        case TOKUNDEFINE :
            b=true; break;
        case TOKIFDEF    :
            b=true; break;
        case TOKIFNOTDEF :
            b=true; break;
        case TOKELSEDEF  :
            b=true; break;
        case TOKENDDEF   :
            b=true; break;
    }
    return b;
}

int CPreProcessorListener::AssignNumericID (const std::tstring& strTokenText)
{
	int nID = 0;
	const char *n = strTokenText.data ();
	char* p;
	/* verifichiamo se e' un numero in virgola mobile */
	strtod (n, &p);
	if (*p == '\0')
		nID = RULFPNUMBERS;
	else   {
		if (n[0] == '0' && (n[1] == 'x' || n[1] == 'X'))   {
			strtol (n, &p, 16);
			if (*p == '\0')
				nID = RULHEXNUMBERS;
		}
	}
	/* verifichiamo se e' un numero esadecimale */
	return nID;
}

/*
** Browsing : 
*/
void CPreProcessorListener::Browsing ()
{
	stAdamoToken* p, *p1;
    CAdamoBase* obj;
	CString str;
	CString strModule;
	CString strCurrentFunction;
	int nLine, nSymbolType;
	char* szConf;

	/* salviamo la function corrente */
	if (m_ptrCodeArray.GetSize () > 1)   {
		p=(stAdamoToken*) m_ptrCodeArray[0];
		p1=(stAdamoToken*) m_ptrCodeArray[1];
		if (p->nID == TOKFUNCTION && p1->nID == 0)
			m_strCurrentFunction = p1->strToken;
	}
	for (int i=0; i<m_ptrCodeArray.GetSize(); i++)   {
        p=(stAdamoToken*) m_ptrCodeArray[i];
		if (p->nID == 0 && p->strToken[0] != '.')   {
			int nBrowseLine = m_nBrowseLine == -1 ? m_nProgLine : m_nBrowseLine;
			/* verifichiamo che il simbolo sia stato definito */
            nSymbolType = -1;
            if (m_symArray->GetDefinitionSymbol (p->strToken, m_strCurrentFunction, "", nSymbolType, strModule, nLine))   {
				if (strModule != m_strPathDest || nLine != nBrowseLine + 1)   {
					strCurrentFunction = nSymbolType == BROWSER_TYPE_LOCAL ? m_strCurrentFunction : "";
					int nItem = CalcolaIstanza (p->strToken, i - 1);
					m_symArray->Add (BROWSER_TYPE_SYMBOL, BROWSER_REFERENCE, p->strToken, strCurrentFunction, "", m_strPathDest, nBrowseLine + 1, nItem);
				}
			}
			else   {
				szConf=p->strToken.GetBuffer (p->strToken.GetLength()+1);
				/* aggiungiamo il nome della macchina */
				if (m_strWithName!="")
					str=m_strMachineName + "." + m_strWithName + "." + szConf;
				else
					str=m_strMachineName + "." + szConf;
				/* verifichiamo che esista nel map dei dispositivi */
				if (obj=(CAdamoBase*)m_objMachine->GetItem (str))   {
					int nItem = CalcolaIstanza (p->strToken, i - 1);
					m_symArray->Add (BROWSER_TYPE_SYMBOL, BROWSER_REFERENCE, p->strToken, "", "", m_strPathDest, nBrowseLine + 1, nItem);
				}
				else   {
					CStringArray sa;
					CString strLibrary, strFunction;
					int nNumElements;

					str = szConf;
					nNumElements = ((CMainFrame* )AfxGetMainWnd ())->ParseDottedString (str, sa);
					if (nNumElements == 2)   {
						strLibrary = sa[0];
						strFunction = sa[1];
						/* verifichiamo ora che non sia una function C */
						if (m_synArray->ExistFunction (strLibrary, strFunction))   {
							int nItem = CalcolaIstanza (p->strToken, i - 1);
							m_symArray->Add (BROWSER_TYPE_SYMBOL , BROWSER_REFERENCE, p->strToken, "", "", m_strPathDest, nBrowseLine + 1, nItem);
						}
					}
				}
			}
		}
		else   {
			int nBrowseLine = m_nBrowseLine == -1 ? m_nProgLine : m_nBrowseLine;
			if (p->nID == 0 && p->strToken[0] == '.')   {
				if (i > 3)   { 
					p1=(stAdamoToken*) m_ptrCodeArray[i-1];
					if (p1->nID == 29)   {
						p1=(stAdamoToken*) m_ptrCodeArray[i-2];
						if (p1->nID == 0 || p1->nID == 301)   {
							p1=(stAdamoToken*) m_ptrCodeArray[i-3];
							if (p1->nID == 28)   {
								CString strField = p->strToken.Right (p->strToken.GetLength () - 1);
								p1=(stAdamoToken*) m_ptrCodeArray[i-4];
                                nSymbolType = -1;
                                if (m_symArray->GetDefinitionSymbol (strField, "", p1->strToken, nSymbolType, strModule, nLine))   {
									int nItem = CalcolaIstanzaTable (strField, i - 1);
									m_symArray->Add (BROWSER_TYPE_SYMBOL , BROWSER_REFERENCE, strField, p1->strToken, "", m_strPathDest, nBrowseLine + 1, nItem);
								}
							}
						}
					}
				}
			}
		}
	}
}

/*
** CalcolaIstanza :
*/
int CPreProcessorListener::CalcolaIstanza (CString strSymbol, int nIndex)
{
	stAdamoToken* p;
	int nItem = 0;

	for (int i = 0; i<nIndex; i++)   {
        p=(stAdamoToken*) m_ptrCodeArray[i];
		if (p->strToken == strSymbol)   {
			nItem++;
		}
	}
	return nItem;
}

/*
** CalcolaIstanzaTable :
*/
int CPreProcessorListener::CalcolaIstanzaTable (CString strSymbol, int nIndex)
{
	stAdamoToken* p;
	int nItem = 0;

	for (int i = 0; i<nIndex; i++)   {
        p=(stAdamoToken*) m_ptrCodeArray[i];
		CString str = p->strToken.Right (p->strToken.GetLength () - 1);
		if (str == strSymbol)   {
			nItem++;
		}
	}
	return nItem;
}

/*
** ManageObjects :
*/
void CPreProcessorListener::ManageObjects ()
{
	if (m_ptrCodeArray.GetCount () > 1)   {
		switch (((stAdamoToken*)m_ptrCodeArray[1])->nID)   {
			case 0 :
				InitCreateObject ();
				break;
			case TOKEND :
				EndCreateObject ();
				break;
		}
	}
	else
		m_nError = -2;
}

/*
** InitCreateObject :
*/
void CPreProcessorListener::InitCreateObject ()
{
	CString str, strParentObject;
	int nItems = 1;
	bool bDerived = false;

	m_strCurrentObject = ((stAdamoToken*)m_ptrCodeArray[1])->strToken;
	if (m_ptrCodeArray.GetSize () == 4)   {
		strParentObject = ((stAdamoToken*)m_ptrCodeArray[3])->strToken;
		nItems = 3;
		bDerived = true;			
	}
	if (!bDerived)
		str.Format ("%s = class (\"%s\")", m_strCurrentObject, m_strCurrentObject);
	else
		str.Format ("%s = class (\"%s\", %s)", m_strCurrentObject, m_strCurrentObject, strParentObject);
	((stAdamoToken*)m_ptrCodeArray[0])->strToken = str;
	((stAdamoToken*)m_ptrCodeArray[0])->nID = 0;
	RemoveEl (1, nItems);
}

/*
** EndCreateObject :
*/
void CPreProcessorListener::EndCreateObject ()
{
	CString str, strVirtual, strAllVirtual;
	/* scriviamo le funzioni virtuali */
	for (POSITION pos = m_strlistVirtualFunction.GetHeadPosition (); pos; )   {
		str = m_strlistVirtualFunction.GetNext (pos);
		strVirtual.Format ("%s:virtual (\"%s\")", m_strCurrentObject, str);
		if (pos)
			strVirtual += " ";
		strAllVirtual += strVirtual;
	}
	((stAdamoToken*)m_ptrCodeArray[0])->strToken = strAllVirtual;
	((stAdamoToken*)m_ptrCodeArray[0])->nID = 0;
	RemoveEl (1, 1);
	/* elaborazione dell' oggetto terminata */
	m_strlistVirtualFunction.RemoveAll ();
	m_strCurrentObject = "";
}

/*
** Objects :
*/
int CPreProcessorListener::Objects ()
{
	CString str;
	bool bIsData = false;
	int nB = 0;
	/* per prima cosa rimuoviamo public e private che nel preprocessore non ci servono */
	if (m_strCurrentObject != "" && m_ptrCodeArray.GetCount () > 2)   {
		if (((stAdamoToken*)m_ptrCodeArray[0])->nID == TOKPUBLIC || ((stAdamoToken*)m_ptrCodeArray[0])->nID == TOKPRIVATE)
			RemoveEl (0, 1);
		if (((stAdamoToken*)m_ptrCodeArray[1])->nID == TOKAS)
				bIsData = true;
		if (bIsData)   {
			if (((stAdamoToken*)m_ptrCodeArray[0])->nID == 0)   {
				if (m_ptrCodeArray.GetCount () == 3 || m_ptrCodeArray.GetCount () == 5)   {
					/* se ci sono solo 3 elementi, non c'e' inizializzazione */
					if (m_ptrCodeArray.GetSize () == 3)   {
						switch (((stAdamoToken*)m_ptrCodeArray[2])->nID)   {
							case TOKBOOLEAN :
								str.Format ("%s.%s = false", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKNUMBER :
								str.Format ("%s.%s = 0.0", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSTRING :
								str.Format ("%s.%s = \"\"", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSOST_DIGITALINPUT :
								str.Format ("%s.%s = -1", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSOST_DIGITALOUTPUT :
								str.Format ("%s.%s = -1", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSOST_ANALOGINPUT :
								str.Format ("%s.%s = -1", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSOST_ANALOGOUTPUT :
								str.Format ("%s.%s = -1", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSOST_AXIS :
								str.Format ("%s.%s = -1", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSOST_ENCODER :
								str.Format ("%s.%s = -1", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSOST_TIMER :
								str.Format ("%s.%s = -1", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSOST_STORAGE :
								str.Format ("%s.%s = -1", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								break;
							case TOKSOST_DEVICECAN :
								break;
							case TOKOBJECT :
								str.Format ("%s.%s = {}", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								m_bSomethingInteresting = false;
								break;
							case 0 :
								if (ManageEnumSyntax (((stAdamoToken*)m_ptrCodeArray[2])))   {
									str.Format ("%s.%s = -1", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
								}
								break;
						}
						((stAdamoToken*)m_ptrCodeArray[0])->strToken = str;
						((stAdamoToken*)m_ptrCodeArray[0])->nID = 0;
						RemoveEl (1, 2);
					}
					else   {
						if (m_ptrCodeArray.GetSize () == 5)   {
							switch (((stAdamoToken*)m_ptrCodeArray[2])->nID)   {
								case TOKBOOLEAN :
								case TOKNUMBER :
									str.Format ("%s.%s = %s", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken, ((stAdamoToken*)m_ptrCodeArray[4])->strToken);
									break;
								case TOKSTRING :
									str.Format ("%s.%s = \"%s\"", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken, ((stAdamoToken*)m_ptrCodeArray[4])->strToken);
									break;
							}
							((stAdamoToken*)m_ptrCodeArray[0])->strToken = str;
							((stAdamoToken*)m_ptrCodeArray[0])->nID = 0;
							RemoveEl (1, 4);
					}
				}
				}
				else
					if (((stAdamoToken*)m_ptrCodeArray[4])->nID == TOKNEW)   {
						str.Format ("%s.%s = ", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[0])->strToken);
						((stAdamoToken*)m_ptrCodeArray[0])->strToken = str;
						((stAdamoToken*)m_ptrCodeArray[0])->nID = 0;
						RemoveEl (1, 4);
						m_bSomethingInteresting = false;
					}
				else
					nB = -1;
			}
			else
				nB = -1;
		}
		else   {
			/* verifichiamo se e' una funzione virtual */
			bool bIsVirtual = false;
			if (((stAdamoToken*)m_ptrCodeArray[0])->nID == TOKVIRTUAL)   {
				RemoveEl (0, 1);
				bIsVirtual = true;
			}
			/* facciamo diventare la funzione un metodo */
			if (m_ptrCodeArray.GetCount () > 2 && ((stAdamoToken*)m_ptrCodeArray[0])->nID == TOKFUNCTION && ((stAdamoToken*)m_ptrCodeArray[1])->nID == 0)   {
				if (bIsVirtual && ((stAdamoToken*)m_ptrCodeArray[1])->strToken != m_strCurrentObject)
					m_strlistVirtualFunction.AddTail (((stAdamoToken*)m_ptrCodeArray[1])->strToken);
				if (((stAdamoToken*)m_ptrCodeArray[1])->strToken != m_strCurrentObject)
					str.Format ("%s:%s", m_strCurrentObject, ((stAdamoToken*)m_ptrCodeArray[1])->strToken);
				else
					str.Format ("%s:%s", m_strCurrentObject, "init");
				((stAdamoToken*)m_ptrCodeArray[1])->strToken = str;
			}
		}
	}
	if (nB)
		m_nError = -2;
	return nB;
}
