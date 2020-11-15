/*
**
*/

/* include files */
#include "stdafx.h"
#include <io.h>
#include "adamo.h"
#include "AdamoFileDati.h"

int CCavStrzFile::Open (const char* szNomeFile, bool bRead)
{
    int nB=0;

    if (_access (szNomeFile, 00))   {
        if (!bRead)
            nB=CStdioFile::Open (szNomeFile, CFile::modeCreate | CFile::modeWrite);
    }
    else    {
        if (bRead==true)
            nB=CStdioFile::Open (szNomeFile, CFile::modeRead);
        else 
            nB=CStdioFile::Open (szNomeFile, CFile::modeCreate | CFile::modeWrite);
    }
    return nB;
}

void CCavStrzFile::Close ()
{
    CStdioFile::Close ();
}

int CCavStrzFile::WriteData ()
{
    int n;

    CString str, strToWrite;
    str.Format ("AXIS NAME : %s, %s, %s, %s, %s\n", m_objAxList[0], m_objAxList[1], m_objAxList[2], m_objAxList[3], m_objAxList[4]);
    WriteString (str);
    str.Format ("ENABLED : %d %d %d %d %d\n", m_bEnabled[0], m_bEnabled[1], m_bEnabled[2], m_bEnabled[3], m_bEnabled[4]);
    WriteString (str);
    str.Format ("LIMNEG : %8.3f\n", m_fpLimNeg);
    WriteString (str);
    str.Format ("LIMPOS : %8.3f\n", m_fpLimPos);
    WriteString (str);
    str.Format ("INTERVAL : %8.3f\n", m_fpIntervallo);
    WriteString (str);
    str.Format ("STEP : %8.3f\n", m_fpStep);
    WriteString (str);
    str.Format ("NUMBER OF RECORDS : %d\n", m_nMaxRow);
    WriteString (str);
    for (int i=0; i<m_nMaxRow; i++)   {
        strToWrite="";
        for (n=0; n<NMAXAXESCORRETTED-1; n++)   {
            if (m_data[i][n]!=NOVALUE)
                str.Format ("%8.3f, ", m_data[i][n]);
            else
                str=", ";
            strToWrite+=str;
        }
        if (m_data[i][n]!=NOVALUE)
            str.Format ("%8.3f\n", m_data[i][n]);
        else
            str="\n";
        strToWrite+=str;
        WriteString (strToWrite);
    }
    return 0;
}

int CCavStrzFile::ReadData ()
{
    CString str, strEnableMask;
    int i, n;
    char* p, *q, *c, *s, szBuffer[256];

    /* nomi degli assi incrociati */
    ReadString (str);
    strcpy (szBuffer, str);
    p=strrchr (szBuffer, ':');
    if (p)   {
        q=strdblk(p+1);
        for (n=0; n<NMAXAXESCORRETTED-1; n++)   {
            p=strchr (q, ',');
            if (p)   {
                *p='\0';
                s=strdblk(q);
                AddAx (s, n);
                q=(p+1);
            }
        }
        s=strdblk(q);
        AddAx (s, n);
    }
    /* maschera di abilitazione */
    ReadString (str);
    strcpy (szBuffer, str);
    p=strrchr (szBuffer, ':');
    if (p)
        strEnableMask=strdblk(p+1);
    i=0, n=0;
    while (n<strEnableMask.GetLength())  {
        if (strEnableMask[n]!='\0')   {
            if (strEnableMask[n]=='1')
                m_bEnabled[i++]=1;
            else
                if (strEnableMask[n]=='0')
                    m_bEnabled[i++]=0;
        }
        else
            break;
        n++;
    }
    /* limite negativo */
    ReadString (str);
    strcpy (szBuffer, str);
    p=strrchr (szBuffer, ':');
    if (p)
        m_fpLimNeg=strtod (strdblk(p+1), &c);
    /* limite positivo */
    ReadString (str);
    strcpy (szBuffer, str);
    p=strrchr (szBuffer, ':');
    if (p)
        m_fpLimPos=strtod (strdblk(p+1), &c);
    /* intervallo */
    ReadString (str);
    strcpy (szBuffer, str);
    p=strrchr (szBuffer, ':');
    if (p)
        m_fpIntervallo=strtod (strdblk(p+1), &c);
    /* step */
    ReadString (str);
    strcpy (szBuffer, str);
    p=strrchr (szBuffer, ':');
    if (p)
        m_fpStep=strtod (strdblk(p+1), &c);
    /* numero di records */
    ReadString (str);
    strcpy (szBuffer, str);
    p=strrchr (szBuffer, ':');
    if (p)
        m_nMaxRow=atoi (strdblk(p+1));
    /* ora finalmente i dati */
    for (i=0; i<m_nMaxRow; i++)   {
        ReadString (str);
        strcpy (szBuffer, str);
        q=szBuffer;
        for (n=0; n<NMAXAXESCORRETTED-1; n++)   {
            p=strchr (q, ',');
            if (p)   {
                *p='\0';
                s=strdblk(q);
                if (strlen(s)==0)
                    m_data[i][n]=NOVALUE;
                else
                    m_data[i][n]=strtod (strdblk(q), &c);
                q=(p+1);
            }
        }
        s=strdblk(q);
        if (strlen(s)==0)
            m_data[i][n]=NOVALUE;
        else
            m_data[i][n]=strtod (strdblk(q), &c);
    }
    return 0;
}

/*
**
*/
int CCavStrzFile::FormatBuffer (int& s, unsigned char* pBuf, int nDim, struct stAdamoParametri* pAP)
{
    static int nLastRow;
    unsigned char* p;
    int i, n, nAddress;

    p=pBuf;
    if (s==0)   {
        /* primo dato, inidirizzo logico dell'asse da master */
        GetAxAddress (m_strNomeAsse, &nAddress, pAP);
        *((int*)p)=nAddress;
        p+=sizeof (int);
        /* ora gli assi incrociati */
        for (i=0; i<NMAXAXESCORRETTED; i++)   {
            nAddress=-1;
            if (m_objAxList[i]!="")
                GetAxAddress (m_objAxList[i], &nAddress, pAP);
            *((int*)p)=nAddress;
            p+=sizeof (int);
        }
        /* ora la maschera di abilitazione */
        n=0;
        for (i=0; i<NMAXAXESCORRETTED; i++)   {
            if (m_bEnabled[i])
                n|=(1<<i);
        }
        *((int*)p)=n;
        p+=sizeof (int);
        /* limite negativo assi */
        *((double*)p)=m_fpLimNeg;
        p+=sizeof (double);
        /* limite positivo assi */
        *((double*)p)=m_fpLimPos;
        p+=sizeof (double);
        /* passo di incremento */
        *((double*)p)=m_fpStep;
        p+=sizeof (double);
        /* numero massimo di righe */
        *((int*)p)=m_nMaxRow;
        p+=sizeof (int);
        nLastRow=0;
    }
    else   {
        int nRows=nDim/(NMAXAXESCORRETTED*sizeof(double));
        int s=nLastRow;
        /* copiamo ora i dati nel buffer */
        while (s<(nLastRow+nRows)&&s<m_nMaxRow)   {
            for (i=0; i<NMAXAXESCORRETTED; i++)   {
                *((double*)p)=m_data[s][i];
                p+=sizeof (double);
            }
            s++;
        }
        nLastRow=s;
    }
    return p-pBuf;
}

void CCavStrzFile::GetAxAddress (CString m_strNomeAsse, int* nAddress, struct stAdamoParametri* pAP)
{
    char* p;
    for (int i=0; i<NMAXAXIS; i++)   {
        if (pAP->AdamoAsse[i].szNome!='\0')   {
            p=strrchr (pAP->AdamoAsse[i].szNome, '.');
            if (p)
                p++;
            else
                p=pAP->AdamoAsse[i].szNome;
            if (!strcmp (m_strNomeAsse, p))   {
                *nAddress=i;
                break;
            }
        }
    }
}
