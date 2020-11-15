/*
** CAdamoHTMLFilter.cpp
*/

#include "stdafx.h"

#include <afxwin.h>
#include <afxcoll.h>
#include "HTMLFilter.h"

CAdamoHTMLFilter::CAdamoHTMLFilter () : m_rf (NULL)
{
}

CAdamoHTMLFilter::~CAdamoHTMLFilter ()
{
    RemoveTemporaryImageFile ();
}

void CAdamoHTMLFilter::Filter (CString &strHTML)
{
    CFile f;
    CString str, strIDImage;
    int n=0, m, nIni, nFin;

    /* se ci sono dei file temporanei, cancelliamoli prima di filtrare un altro codice HTML */
    if (m_pathTmp.GetCount ())
        Clean ();
    /* cerchiamo l'HTML dell'immagine */
    while ((n=strHTML.Find ("<img", n))!=-1)   {
        m=strHTML.Find ("</img>", n);
        nIni=n+strlen ("<img");
        nFin=m-1;
        str=strHTML.Mid (nIni, nFin-nIni);
        strIDImage=GetIDImage (str);
        if (strIDImage!="")   {
            /* Prendiamo l'immagine */
            DWORD dw = m_rf->LunghezzaImmagine (strIDImage);
            if (dw)   {
                char* p=new char [dw];
                if (p)   {
                    m_rf->CaricaImmagine (strIDImage, p);
                    /* cominciamo a caricare il file di immagine */
                    CPath path = GetTemporaryImageFile ();
                    if (f.Open (path, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite))   {
                        f.Write (p, dw);
                        f.Close ();
                        /* aggiugiamola alla lista di file temporanei */
                        m_pathTmp.AddTail (path);
                        /* sostituiamo i dati nell' HTML */
                        ReplaceIDImage (str, path);
                        /* ora cerchiamo se l'immagine e' strecciata */
                        if (str.Find ("checked")==-1)
                            DeleteWidthAndHeight (str);
                        strHTML.Delete (nIni, nFin-nIni);
                        strHTML.Insert (nIni, str);
                    }
                    delete p;
                }
            }
        }
        n=strHTML.Find ("</img>", nIni)+strlen ("</img>");
    }
}

/*
** Clean :
*/
void CAdamoHTMLFilter::Clean ()
{
    RemoveTemporaryImageFile ();
}

/*
** GetIDImage :
*/
CString CAdamoHTMLFilter::GetIDImage (CString str)
{
    int nIni, nFin;
    int n=str.Find ("src=");
    if (n!=-1)   {
        n+=strlen ("src=");
        while (str[n]==' '||str[n]=='\"')
            n++;
        nIni=n;
        while (str[n]!=' ')
            n++;
        nFin=n-1;
        return str.Mid (nIni, nFin-nIni);
    }
    return "";
}

/*
** GetTemporaryImageFile :
*/
CPath CAdamoHTMLFilter::GetTemporaryImageFile ()
{
    CPath path;
    char pPath[_MAX_PATH];
    char pFile[_MAX_PATH];

    if (pPath&&pFile)   {
        if (GetTempPath (_MAX_PATH, pPath))   {
            if (GetTempFileName (pPath, "img", 0, pFile))
                path=pFile;
        }
    }
    return path;
}

/*
** ReplaceIDImage :
*/
void CAdamoHTMLFilter::ReplaceIDImage (CString& str, CPath path)
{
    int nIni, nFin;
    int n=str.Find ("src=");
    n+=strlen ("src=");
    while (str[n]==' ')
        n++;
    nIni=n;
    while (str[n]!=' ')
        n++;
    nFin=n;
    str.Delete (nIni, nFin-nIni);
    str.Insert (nIni, path);
}

/*
** DeleteWidthAndHeight :
*/
void CAdamoHTMLFilter::DeleteWidthAndHeight (CString& str)
{
    int s, nIni, nFin;

    if ((s=str.Find ("width:"))!=-1)   {
        nIni=s;
        while (str[s]!=' ')
            s++;
        nFin=s;
        str.Delete (nIni-1, nFin-nIni+1);
    }        
    if ((s=str.Find ("height:"))!=-1)   {
        nIni=s;
        while (str[s]!=' ')
            s++;
        nFin=s;
        str.Delete (nIni-1, nFin-nIni+1);
    }        
}

/*
** RemoveTemporaryImageFile :
*/
void CAdamoHTMLFilter::RemoveTemporaryImageFile ()
{
    CString str;
    if (m_pathTmp.GetCount ())   {
        for (POSITION pos=m_pathTmp.GetHeadPosition (); pos; )   {
            str=m_pathTmp.GetNext (pos);
            CFile::Remove (str);
        }
        m_pathTmp.RemoveAll ();
    }
}
