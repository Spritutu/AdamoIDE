/*
** AdamoRSObjects.cpp
*/

/* include files */
#include "stdafx.h"
#include "adamo.h"
#include "AdamoLib.h"
#include "AdamoRSObjects.h"

/* constants definitions */
#define CRLF "\r\n"

/* static functions */
CString GetXMLProperty (const char* sz, const char* szProp);
COLORREF TranslateRGB (char* p);

/* costruction / destruction */
CAdamoRSCtrlList::CAdamoRSCtrlList () : m_nValidID (DEFAULT_RSCTRL_ID), m_bIsModified (false), m_tr (NULL)
{
}

CAdamoRSCtrlList::~CAdamoRSCtrlList ()
{
    Clear ();
}

/*
** Clear :
*/
void CAdamoRSCtrlList::Clear ()
{
    for (POSITION pos=GetHeadPosition (); pos;)   {
        CAdamoRSControl* p=(CAdamoRSControl*) GetNext (pos);
        delete p;
    }
    RemoveAll ();
}

/*
** AddCtrl :
*/
void CAdamoRSCtrlList::AddCtrl (CAdamoRSBase* pCtrl)
{
    /* se il controllo che dobbiamo inserire e' un gruppo, inseriamolo per primo */
    if (pCtrl->KindOf()==RSGroup)   {
        /* vediamo se il primo elemento e' un form */
        POSITION pos=GetHeadPosition ();
        if (pos)   {
            CAdamoRSControl* pFirst=(CAdamoRSControl*) GetNext (pos);
            if (pFirst->KindOf ()==RSForm)
                /* e' un form, mettiamolo subito dopo */
                InsertAfter (pos, pCtrl);
            else
                /* mettiamolo in cima alla lista */
                AddHead (pCtrl);
        }
        else
            AddHead (pCtrl);
    }
    else
        AddTail (pCtrl);
	pCtrl->SetCtrlList (this);
    pCtrl->LoadCaption ();
    Modify ();
}

/*
** RemoveCtrl :
*/
void CAdamoRSCtrlList::RemoveCtrl (CAdamoRSBase* pCtrl)
{
    for (POSITION pos2, pos=GetHeadPosition (); pos2=pos; )   {
        CAdamoRSControl* p=(CAdamoRSControl*) GetNext (pos);
        if (p==pCtrl)   {
            delete p; RemoveAt (pos2); break;
        }
    }
    Modify ();
}

/*
** RemoveAllCtrl : 
*/
void CAdamoRSCtrlList::RemoveAllCtrl ()
{
    CAdamoRSControl* pForm=(CAdamoRSControl*)GetFormCtrl ();
    for (POSITION pos2, pos=GetHeadPosition (); pos2=pos; )   {
        CAdamoRSControl* p=(CAdamoRSControl*) GetNext (pos);
        if (p==pForm)
            continue;
        delete p; RemoveAt (pos2);
    }
    Modify ();
}

/*
** Load :
*/
int  CAdamoRSCtrlList::Load (CString strXML)
{
    Clear ();
    LoadXML (strXML);
    Ready ();
    return 0;
}

/*
** Save :
*/
int  CAdamoRSCtrlList::Save (CString& strXML)
{
    /* salviamo il prologo */
    SavePrologue (strXML);
    /* salviamo i controlli */
    SaveControls (strXML);
    /* salviamo l'epilogo */
    SaveEpilogue (strXML);
    return 0;
}

/*
** GetValidID :
*/
int  CAdamoRSCtrlList::GetValidID ()
{
    int n=-1;
    for (int i=DEFAULT_RSCTRL_ID; i<DEFAULT_RSCTRL_ID+MAX_RSCONTROLS; i++)   {
        if (!GetRSCtrl (i))   {
            n=i; break;
        }
    }
    return n;
}

/*
** GetRSCtrl :
*/
CAdamoRSControl* CAdamoRSCtrlList::GetRSCtrl (int nID)
{
    CAdamoRSControl* pB=NULL;
    for (POSITION pos=GetHeadPosition (); pos; )   {
        CAdamoRSControl* p=(CAdamoRSControl*) GetNext (pos);
        if (p->GetID ()==nID)   {
            pB=p; break;
        }
    }
    return pB;
}

/*
** SavePrologue :
*/
void CAdamoRSCtrlList::SavePrologue (CString& strXML)
{
	strXML.Format ("<?xml version=\"1.0\" encoding=\"utf-8\"?><body>\r\n");
}

/*
** SaveControls :
*/
void CAdamoRSCtrlList::SaveControls (CString& strXML)
{
    for (POSITION pos=GetHeadPosition (); pos; )   {
        /* salviamo tutti i controlli uno ad uno */
        CAdamoRSControl* p=(CAdamoRSControl*) GetNext (pos);
        p->SaveXML (strXML);
        /* salviamo eventualmente la caption tradotta */
        p->SaveCaption ();
    }
}

/*
** SaveEpilogue :
*/
void CAdamoRSCtrlList::SaveEpilogue (CString& strXML)
{
    strXML+="</form>\r\n</body>\r\n";
}

/*
** GetFormCtrl :
*/
CAdamoRSForm* CAdamoRSCtrlList::GetFormCtrl ()
{
    CAdamoRSForm* pForm=NULL;

    for (POSITION pos=GetHeadPosition (); pos;)   {
        CAdamoRSControl* p=(CAdamoRSControl*) GetNext (pos);
        if (p->KindOf ()==RSForm)   {
            pForm=(CAdamoRSForm*)p; break;
        }
    }
    return pForm;
}

/*
** SetMainSyn :
*/
void CAdamoRSCtrlList::SetMainSyn (bool bMainSyn)
{
    CAdamoRSForm *pForm;

    if (pForm=GetFormCtrl ())
        pForm->SetMainSynoptic (bMainSyn);
}

/*
** SetName :
*/
void CAdamoRSForm::SetName (CString strName)
{
    CRect rc;
    
    CString str=GetXMLProperty (strName, "left");
    str.TrimRight ("px");
    rc.left=atoi (str);
    str=GetXMLProperty (strName, "top");
    str.TrimRight ("px");
    rc.top=atoi (str);
    str=GetXMLProperty (strName, "width");
    str.TrimRight ("px");
    rc.right=rc.left+atoi (str);
    str=GetXMLProperty (strName, "height");
    str.TrimRight ("px");
    rc.bottom=rc.top+atoi (str);
    SetPositionAbs (rc.top, rc.left, rc.right, rc.bottom);
}

/*
** SaveXML :
*/
int CAdamoRSForm::SaveXML (CString& strSave)
{
    CString str;
    CRect rc;

    CSize size=m_pCtrlList->GetFormSize ();
    rc.SetRect (CPoint (0, 0), CPoint(size));
    str.Format ("<form name=\"position: absolute; left: %dpx; top: %dpx; width: %dpx; height: %dpx\" formname=\"%s\" src=\"%s\">\r\n",
        rc.left, rc.top, rc.Width(), rc.Height(), GetName (), m_strBackgroundImage);
    strSave+=str;
    return 0;
}

/*
** GetXMLProperty :
*/
CString GetXMLProperty (const char* sz, const char* szProp)
{
    CString str;
    int n;
    char szBuffer[256];
    const char *p, *q;

    if (p=strstr (sz, szProp))   {
        if (p=strchr (p, ':')+1)   {
            q=strchr (p, ';');
            if (q)   {
                n=q-p;
                memcpy (szBuffer, p, n);
                szBuffer[n]='\0';
                str=strdblk (szBuffer);
            }
            else   {
                strcpy (szBuffer, p);
                str=strdblk (szBuffer);
            }
        }
    }
    return str;
}

/*
** SaveXML :
*/
int CAdamoRSInputText::SaveXML (CString& strSave)
{
    CString str, strTab, strVar, strType, strAutoUpdate;

	switch (m_eType)   {
		case eNumbers :
			strType = "numbers"; break;
		case eFloat :
			strType = "float"; break;
		case eHex :
			strType = "hex"; break;
		case eLetters :
			strType = "letters"; break;
	}
    str.Format ("<input type=%s class=\"controls\" mode=\"%s\" name=\"%s\" value=\"%s\" size=\"%d\" id=\"%d\"",
        m_bIsPsw ? "\"password\"" : "\"text\"", strType, GetName (), GetValue(), GetSize (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format ("tabindex=\"%d\"", GetTabIndex());
	strAutoUpdate.Format ("autoupdate=%s", IsAutoUpdate () ? "\"true\"" : "\"false\"");
    str=str+" "+strTab+" "+strAutoUpdate+" ";
    m_objStyle.Save (str);
    str=str+">"+CRLF;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</input>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSInputRadio::SaveXML (CString& strSave)
{
    CString str, strTab, strVar, strGroup, strChecked, strAutoUpdate;

    str.Format ("<input type=\"radio\" class=\"controls\" name=\"%s\" value=\"%s\" id=\"%d\"", GetName (), GetValue(), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    strChecked.Format (" checked=%s", IsChecked () ? "\"true\"" : "\"false\"");
	strAutoUpdate.Format (" autoupdate=%s", IsAutoUpdate () ? "\"true\"" : "\"false\"");
	strGroup.Format (" group=\"%s\"", GetGroup ());
    str=str+strTab+strChecked+" "+strAutoUpdate+" "+strGroup+" ";
	m_objStyle.Save (str);
    str=str+">"+CRLF;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</input>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSInputCheck::SaveXML (CString& strSave)
{
    CString str, strTab, strVar, strChecked, strAutoUpdate;

    str.Format ("<input type=\"checkbox\" class=\"controls\" name=\"%s\" value=\"%s\" id=\"%d\"", GetName (), GetValue(), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    strChecked.Format (" checked=%s", IsChecked () ? "\"true\"" : "\"false\"");
	strAutoUpdate.Format (" autoupdate=%s", IsAutoUpdate () ? "\"true\"" : "\"false\"");
    str=str+strTab+strChecked+" "+strAutoUpdate+" ";
    m_objStyle.Save (str);
    str=str+">"+CRLF;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</input>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSButton::SaveXML (CString& strSave)
{
    CString str, strTab, strType;

	switch (m_eType)   {
		case eStandard :
			strType = "standard"; break;
		case eBitmap :
			strType = "bitmap"; break;
	}
    str.Format ("<button class=\"controls\" name=\"%s\" value=\"%s\" id=\"%d\" src=\"%s\" type=\"%s\" transparency=\"%d\" action=\"%d\" nextpage=\"%s\"", GetName (), m_strValue, GetID (), m_strImage, strType, m_bTransparency, m_dwAction, m_strPage);
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
    m_objStyle.Save (str);
    str+=">";
    str+=GetValue ();
    str=str+"</button>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** Save :
*/
void CAdamoRSStyle::Save (CString& strSave)
{
    CString str, strDim, strFont, strBorder, strColor, strAlignment;
    strDim.Format ("position:absolute; left:%dpx; top:%dpx; width:%dpx; height:%dpx; ", m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height());
    strFont.Format ("font-family:%s; font-size:%dpx; ", m_rsFont.strFaceName, m_rsFont.nSize);
    if (m_rsFont.bStrikeOut||m_rsFont.bUnderLine)   {
        strFont+="text-decoration:";
        if (m_rsFont.bUnderLine)
            strFont+="underline";
        if (m_rsFont.bStrikeOut)   {
            if (m_rsFont.bUnderLine)
                strFont+=" ";    
            strFont+="line-through";
        }
        strFont+="; ";
    }
    if (m_rsFont.bItalic)
        strFont+="font-style:italic; ";
    if (m_rsFont.bBold)
        strFont+="font-weight:bold; ";
    if (m_rsBorder.HasABorder)   {
        switch (m_rsBorder.nBorderStyle)   {
            case dottedStyle :
                str="dotted";
                break;
            case solidStyle  :
                str="solid";
                break;
            case dashedStyle :
                str="dashed";
                break;
            case doubleStyle :
                str="double";
                break;
            case grooveStyle :
                str="groove";
                break;
            case ridgeStyle  :
                str="ridge";
                break;
            case insetStyle  :
                str="inset";
                break;
            case outsetStyle :
                str="outset";
                break;
        };
        strBorder.Format ("border: %dpx %s", m_rsBorder.nSize, str);
        if (!m_rsBorder.bAutomaticColor)   {
            str.Format (" #%02x%02x%02x; ", GetRValue(m_rsBorder.color), GetGValue(m_rsBorder.color), GetBValue(m_rsBorder.color));
            strBorder+=str;
        }
        else
            strBorder+="; ";
        str.Format ("padding-left:%dpx; padding-right:%dpx; padding-top:%dpx; padding-bottom:%dpx; ", m_rsBorder.padding_left,
            m_rsBorder.padding_rigth, m_rsBorder.padding_top, m_rsBorder.padding_bottom);
        strBorder+=str;
    }
    str="";
    if (!m_bAutomaticColor)
        str.Format ("color:#%02x%02x%02x; ", GetRValue(m_rsColor), GetGValue(m_rsColor), GetBValue(m_rsColor));
    strColor+=str;
    str="";
    if (!m_bAutomaticBackColor)
        str.Format ("background-color:#%02x%02x%02x; ", GetRValue(m_rsBackColor), GetGValue(m_rsBackColor), GetBValue(m_rsBackColor));
    strColor+=str;
    switch (m_rsAlignment)   {
        case leftAlignment   :
            str="left";
            break;
        case centerAlignment :
            str="center";
            break;
        case rigthAlignment  :
            str="right";
            break;
        case justifyAlignment:
            str="justify";
            break;
    }
    strAlignment.Format ("text-align:%s;", str);
    strSave=strSave+"style="+"\""+strDim+strFont+strBorder+strColor+strAlignment+"\"";
}

/*
** Load :
*/
void CAdamoRSStyle::Load (CString  strLoad)
{
    try   {
        /* posizione */
        CString str=GetXMLProperty (strLoad, "left");
        str.TrimRight ("px");
        m_rc.left=atoi (str);
        str=GetXMLProperty (strLoad, "top");
        str.TrimRight ("px");
        m_rc.top=atoi (str);
        str=GetXMLProperty (strLoad, "width");
        str.TrimRight ("px");
        m_rc.right=m_rc.left+atoi (str);
        str=GetXMLProperty (strLoad, "height");
        str.TrimRight ("px");
        m_rc.bottom=m_rc.top+atoi (str);
        /* font */
        str=GetXMLProperty (strLoad, "font-family");
        m_rsFont.strFaceName=str;
        str=GetXMLProperty (strLoad, "font-size");
        str.TrimRight ("px");
        m_rsFont.nSize=atoi (str);
        str=GetXMLProperty (strLoad, "text-decoration");
        if (str.Find ("underline")!=-1)
            m_rsFont.bUnderLine=true;
        if (str.Find ("line-through")!=-1)
            m_rsFont.bStrikeOut=true;
        str=GetXMLProperty (strLoad, "font-style");
        if (str=="italic")
            m_rsFont.bItalic=true;
        str=GetXMLProperty (strLoad, "font-weight");
        if (str=="bold")
            m_rsFont.bBold=true;
        /* border-style */
        str=GetXMLProperty (strLoad, "border");
        m_rsBorder.HasABorder=false;
        char *h, *q, *s, *p;
        if (str!="")   {
            p=str.GetBuffer (str.GetLength());
            q=strchr (p, ' ');
            if (q)   {
                *q='\0';
                s=q+1;
                q=strchr (s, ' ');
                if (q)   {
                    *q='\0';
                    q=q+1;
                }
            }
            strdblk (p);
            if (h=strstr (p, "px"))
                *h='\0';
            m_rsBorder.nSize=atoi (p);
            strdblk (s);
            if (!strcmp (s, "dotted"))
                m_rsBorder.nBorderStyle=dottedStyle;
            else
            if (!strcmp (s, "solid"))
                m_rsBorder.nBorderStyle=solidStyle;
            else
            if (!strcmp (s, "dashed"))
                m_rsBorder.nBorderStyle=dashedStyle;
            else
            if (!strcmp (s, "double"))
                m_rsBorder.nBorderStyle=doubleStyle;
            else
            if (!strcmp (s, "groove"))
                m_rsBorder.nBorderStyle=grooveStyle;
            else
            if (!strcmp (s, "ridge"))
                m_rsBorder.nBorderStyle=ridgeStyle;
            else
            if (!strcmp (s, "inset"))
                m_rsBorder.nBorderStyle=insetStyle;
            else
            if (!strcmp (s, "outset"))
                m_rsBorder.nBorderStyle=outsetStyle;
            else
                m_rsBorder.nBorderStyle=solidStyle;
            m_rsBorder.bAutomaticColor=true;
            if (q)   {
                strdblk (q);
                if (*q=='#')
                    m_rsBorder.color=TranslateRGB (q+1);
                m_rsBorder.bAutomaticColor=false;
            }
            str.ReleaseBuffer (-1);
            str=GetXMLProperty (strLoad, "padding-left");
            str.TrimRight ("px");
            m_rsBorder.padding_left=atoi (str);
            str=GetXMLProperty (strLoad, "padding-top");
            str.TrimRight ("px");
            m_rsBorder.padding_top=atoi (str);
            str=GetXMLProperty (strLoad, "padding-right");
            str.TrimRight ("px");
            m_rsBorder.padding_rigth=atoi (str);
            str=GetXMLProperty (strLoad, "padding-bottom");
            str.TrimRight ("px");
            m_rsBorder.padding_bottom=atoi (str);
            m_rsBorder.HasABorder=true;
        }
        str=GetXMLProperty (strLoad, "color");
        m_bAutomaticColor=true;
        if (str!="")   {
            p=str.GetBuffer (str.GetLength());
            if (p[0]=='#')  
                m_rsColor=TranslateRGB (p+1);
            m_bAutomaticColor=false;
            str.ReleaseBuffer (-1);
        }
        str=GetXMLProperty (strLoad, "background-color");
        m_bAutomaticBackColor=true;
        if (str!="")   {
            p=str.GetBuffer (str.GetLength());
            if (p[0]=='#')  
                m_rsBackColor=TranslateRGB (p+1);
            m_bAutomaticBackColor=false;
            str.ReleaseBuffer (-1);
        }
        str=GetXMLProperty (strLoad, "text-align");
        if (str=="left")
            m_rsAlignment=leftAlignment;
        else
        if (str=="center")
            m_rsAlignment=centerAlignment;
        else
        if (str=="right")
            m_rsAlignment=rigthAlignment;
        else
        if (str=="justify")
            m_rsAlignment=justifyAlignment;
        else
            m_rsAlignment=leftAlignment;
    }
    catch (...)   {
    }
}

/*
** SaveXML :
*/
void stRSVariabili::SaveXML (CString& str, int nID)
{
    CString strHead, strVar, strIndex, strType;

	strHead.Format ("<Variable id=\"%d\" ", nID);
	strVar += strHead;
    if (m_strName!="")   {
        strHead.Format ("VariableName=\"%s\" ", m_strName);
		strVar += strHead;
        switch (m_nType)   {
            case LUA_TNUMBER :
                strType="number";
                break;
            case LUA_TSTRING :
                strType="string";
                break;
            case LUA_TBOOLEAN :
                strType="boolean";
                break;
        }
        if (m_nRow!=-1&&m_strCol!="")
            strIndex.Format ("VariableIndex=\"row: %d; column: %s; type: %s;\" ", m_nRow, m_strCol, strType);
        else
        if (m_nRow!=-1)
            strIndex.Format ("VariableIndex=\"row: %d; type: %s;\" ", m_nRow, strType);
        else
            strIndex.Format ("VariableIndex=\"type: %s;\" ", strType);
        strVar+=strIndex;
    }
	else
		AzzeraVariabili ();
    if (m_strTable!="")   {
        strHead.Format ("Table=\"%s\"", m_strTable);
		strVar += strHead;
		strIndex = "";
		if (m_strField!=""&&m_strKeyName!=""&&m_strKeyValue!="") 
			strIndex.Format (" Field=\"%s\" KeyName=\"%s\" KeyValue=\"%s\"", m_strField, m_strKeyName, m_strKeyValue);
        strVar+=strIndex;
    }
	else
		AzzeraDB ();
    strVar+="></Variable>";
    strVar+=CRLF;
    str+=strVar;
}

/*
** Load :
*/
void stRSVariabili::Load (CString strLoad)
{
    CString str;

    str=GetXMLProperty (strLoad, "row");
    if (str!="")
        m_nRow=atoi (str);
    str=GetXMLProperty (strLoad, "column");
    if (str!="")
        m_strCol=str;
    str=GetXMLProperty (strLoad, "type");
    if (str!="")   {
        if (str=="number")
            m_nType=LUA_TNUMBER;
        if (str=="string")
            m_nType=LUA_TSTRING;
        if (str=="boolean")
            m_nType=LUA_TBOOLEAN;
    }
}

/*
** SaveXML :
*/
int CAdamoRSLabel::SaveXML (CString& strSave)
{
    CString str, strTab, strValue;

    str.Format ("<label name=\"%s\" value=\"%s\" class=\"controls\" id=\"%d\"", GetName (), GetValue(), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
    m_objStyle.Save (str);
    str+=">";
    strValue.Format ("%s", GetValue ());
    str+=strValue;
    str+="</label>";
    str+=CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSGroup::SaveXML (CString& strSave)
{
    CString str, strValue;

    str.Format ("<fieldset class=\"controls\" id=\"%d\" ", GetID ());
    m_objStyle.Save (str);
    str=str+">"+CRLF;
    strValue.Format ("<legend>%s</legend>\r\n", GetValue ());
    str=str+strValue+"</fieldset>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSCombo::SaveXML (CString& strSave)
{
    CString str, strTab, strMultiple, strValue, strOptions;

    str.Format ("<select class=\"controls\" name=\"%s\" size=\"1\" id=\"%d\"",  GetName (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
    m_objStyle.Save (str);
    str=str+">"+CRLF;
    for (int i=0; i<m_nOptions; i++)   {
        strValue=strValue+"<option";
        strOptions.Format (" selected=%s", m_stOpzioni[i].m_bSelected ? "\"true\"" : "\"false\"");
        strValue=strValue+strOptions;
        strOptions="";
        if (m_stOpzioni[i].strValue!="")   {
            strOptions.Format (" value=\"%s\"", m_stOpzioni[i].strValue);
        }
        strValue=strValue+strOptions+">";
        strValue+=m_stOpzioni[i].strOpzione;
        strValue+="</option>";
        strValue+=CRLF;
    }
    str+=strValue;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</select>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSList::SaveXML (CString& strSave)
{
    CString str, strTab, strMultiple, strValue, strOptions;

    str.Format ("<select class=\"controls\" name=\"%s\" size=\"%d\" id=\"%d\"",  GetName (), GetSize (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
    strMultiple.Format ("multiple=%s", IsMultipleSelection () ? "\"true\"" : "\"false\"");
    str=str+strMultiple+" ";
    m_objStyle.Save (str);
    str=str+">"+CRLF;
    for (int i=0; i<m_nOptions; i++)   {
        strValue=strValue+"<option";
        strOptions.Format (" selected=%s", m_stOpzioni[i].m_bSelected ? "\"true\"" : "\"false\"");
        strValue=strValue+strOptions;
        strOptions="";
        if (m_stOpzioni[i].strValue!="")   {
            strOptions.Format (" value=\"%s\"", m_stOpzioni[i].strValue);
        }
        strValue=strValue+strOptions+">";
        strValue+=m_stOpzioni[i].strOpzione;
        strValue+="</option>";
        strValue+=CRLF;
    }
    str+=strValue;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</select>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** LoadCaption :
*/
int CAdamoRSList::LoadCaption ()
{
    CString str;

    for (int i=0; i<m_nOptions; i++)   {
        if (m_stOpzioni[i].strValue!="")   {
            m_pCtrlList->LoadCtrlCaption (GetID(), m_stOpzioni[i].strValue, str);
            if (str!="")
                m_stOpzioni[i].strOpzione=str;
        }
    }
    return 0;
}

/*
** SaveCaption :
*/
int CAdamoRSList::SaveCaption ()
{
    for (int i=0; i<m_nOptions; i++)   {
        if (m_stOpzioni[i].strValue!="")
            m_pCtrlList->SaveCtrlCaption (GetID(), m_stOpzioni[i].strValue, m_stOpzioni[i].strOpzione);
    }
    return 0;
}

/*
** AddRSOption :
*/
void CAdamoRSList::AddRSOption (stRSOpzioni* pOpt, int nPos)
{
    if (m_nOptions<MAX_RSOPTIONS)   {
        if (nPos!=-1)   {
            memmove (&m_stOpzioni[nPos], &m_stOpzioni[nPos+1], MAX_RSOPTIONS-nPos-1);
            m_stOpzioni[nPos]=*pOpt;
        }
        else
            m_stOpzioni[m_nOptions]=*pOpt;
        m_nOptions++;
    }    
}

/*
** GetRSOptions :
*/
void CAdamoRSList::GetRSOptions (stRSOpzioni* pOptions, int* nOptions)
{
    for (int i=0; i<m_nOptions; i++)
        pOptions[i]=m_stOpzioni[i];
    *nOptions=m_nOptions;
}

/*
** AddRSOption :
*/
void CAdamoRSCombo::AddRSOption (stRSOpzioni* pOpt, int nPos)
{
    if (m_nOptions<MAX_RSOPTIONS)   {
        if (nPos!=-1)   {
            memmove (&m_stOpzioni[nPos], &m_stOpzioni[nPos+1], MAX_RSOPTIONS-nPos-1);
            m_stOpzioni[nPos]=*pOpt;
        }
        else
            m_stOpzioni[m_nOptions]=*pOpt;
        m_nOptions++;
    }    
}

/*
** GetRSOptions :
*/
void CAdamoRSCombo::GetRSOptions (stRSOpzioni* pOptions, int* nOptions)
{
    for (int i=0; i<m_nOptions; i++)
        pOptions[i]=m_stOpzioni[i];
    *nOptions=m_nOptions;
}

/*
** LoadCaption :
*/
int CAdamoRSCombo::LoadCaption ()
{
    CString str;

    for (int i=0; i<m_nOptions; i++)   {
        if (m_stOpzioni[i].strValue!="")   {
            m_pCtrlList->LoadCtrlCaption (GetID(), m_stOpzioni[i].strValue, str);
            if (str!="")
                m_stOpzioni[i].strOpzione=str;
        }
    }
    return 0;
}

/*
** SaveCaption :
*/
int CAdamoRSCombo::SaveCaption ()
{
    for (int i=0; i<m_nOptions; i++)   {
        if (m_stOpzioni[i].strValue!="")
            m_pCtrlList->SaveCtrlCaption (GetID(), m_stOpzioni[i].strValue, m_stOpzioni[i].strOpzione);
    }
    return 0;
}

/*
** LoadTranslatedCaption :
*/
int CAdamoRSBase::LoadTranslatedCaption (CString& strValue)
{
    CString strCaption;

    if (!m_pCtrlList->LoadCtrlCaption (GetID(), "", strCaption))   {
        strValue=strCaption;
        m_bTranslated=true;
    }
    return 0;
}

/*
** SaveTranslatedCaption :
*/
int CAdamoRSBase::SaveTranslatedCaption (CString strValue)
{
    m_pCtrlList->SaveCtrlCaption (GetID(), "", strValue);
    return 0;    
}

/*
** SaveXML :
*/
int CAdamoRSImage::SaveXML (CString& strSave)
{
    CString str, strTab, strVar, strChecked, strTransparentColor;

    str.Format ("<img src=\"%s\" class=\"controls\" name=\"%s\" id=\"%d\"", 
        GetIDImage(), GetName (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
    strChecked.Format ("checked=%s ", IsStretched () ? "\"true\"" : "\"false\"");    
    str=str+strChecked+" ";
    strTransparentColor.Format ("transparent=%s transparentcolor=\"#%02x%02x%02x\"", IsTransparent () ? "\"true\"" : "\"false\"", GetRValue(GetTransparentColor ()), GetGValue(GetTransparentColor ()), GetBValue(GetTransparentColor ()));
    str=str+strTransparentColor + " ";
    m_objStyle.Save (str);
    str=str+">"+"</img>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SetGridData :
*/
void CAdamoRSGrid::SetGridData (int nRows, int nColumns)
{
	if (nRows > 0 && nColumns > 0)   {
		m_nRows = nRows;
		m_nCols = nColumns;
	}
}

/*
** SetGridData :
*/
void CAdamoRSGrid::SetColumnData (int nColumn, stGridColumnData *pgcd)
{
	if (nColumn >= 0 && nColumn <= MAXCOLUMNS)   {
		m_gcd[nColumn].m_nType = pgcd->m_nType;
		m_gcd[nColumn].m_strName = pgcd->m_strName;
		m_gcd[nColumn].m_strImage = pgcd->m_strImage;
	}
}

/*
** SetGridData :
*/
void CAdamoRSGrid::GetGridData (int *pnRow, int *pnColumns)
{
	*pnRow = m_nRows;
	*pnColumns = m_nCols;
}

/*
** GetColumnData :
*/
void CAdamoRSGrid::GetColumnData (int nColumn, stGridColumnData *pgcd)
{
	if (nColumn >= 0 && nColumn < MAXCOLUMNS)   {
		pgcd->m_nType = m_gcd[nColumn].m_nType;
		pgcd->m_strName = m_gcd[nColumn].m_strName;
		pgcd->m_strImage = m_gcd[nColumn].m_strImage;
	}
}

/*
** SaveXML :
*/
int CAdamoRSGrid::SaveXML (CString& strSave)
{
    CString str, strGrid, strTab, strType, strValue, strOptions;

    str.Format ("<grid class=\"controls\" name=\"%s\" size=\"1\" id=\"%d\"",  GetName (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
	strGrid.Format (" rows=\"%d\" cols=\"%d\"", m_nRows, m_nCols);
    str=str+strGrid+" ";
    m_objStyle.Save (str);
    str=str+">"+CRLF;
    for (int i=0; i<m_nCols; i++)   {
        strValue=strValue+"<column";
		switch (m_gcd[i].m_nType)   {
			case LUA_TNUMBER:
				strType = "number"; break;
			case LUA_TBOOLEAN:
				strType = "boolean"; break;
			case LUA_TSTRING:
				strType = "string"; break;
		};
        strOptions.Format (" type=\"%s\" image=\"%s\">", strType, m_gcd[i].m_strImage);
		strValue+=strOptions;
		strValue=strValue+m_gcd[i].m_strName;
        strValue+="</column>";
        strValue+=CRLF;
    }
    str+=strValue;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</grid>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSAx::SaveXML (CString& strSave)
{
    CString str, strTab, strValue;

    str.Format ("<ax class=\"controls\" name=\"%s\" size=\"1\" id=\"%d\"",  GetName (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
	strValue.Format ("path=\"%s\" address=\"%d\"", m_strPath, m_nID);
	str=str+strValue+" ";
    m_objStyle.Save (str);
    str=str+">"+CRLF;
    str=str+"</ax>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSILed::SaveXML (CString& strSave)
{
    CString str, strTab, strValue, strCircular, strTransparent, strColor;

    str.Format ("<led class=\"controls\" name=\"%s\" size=\"1\" id=\"%d\"",  GetName (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
	/* caratteristiche dispositivo */
	strValue.Format ("path=\"%s\" device=\"%d\" address=\"%d\" data=\"%d\"", GetPath (), GetDeviceType (), GetDeviceID (), GetDeviceData ());
	str=str+strValue+" ";
	/* caratteristiche led */
	strValue.Format ("L_G_Type=\"%d\" L_G_Transparent=\"%d\" L_G_ShowReflection=\"%d\" L_G_ActiveColor=\"#%02x%02x%02x\" "
		"L_G_BevelStyle=\"%d\" L_G_SpacingHorizontal=\"%d\" L_G_SpacingVertical=\"%d\" L_G_RowCount=\"%d\" L_G_ColCount=\"%d\" "
		"L_G_BorderStyle=\"%d\" L_G_BackgroundColor=\"#%02x%02x%02x\"",
		GetLedType (), GetTransparent (), GetShowReflection (), GetRValue(GetActiveColor ()), GetGValue(GetActiveColor ()), GetBValue(GetActiveColor ()), 
		GetBevelStyle (), GetHorizontalSpacing (), GetVerticalSpacing (), GetRowCount (), GetColCount (),
		GetBorderStyle (), GetRValue(GetBackgroundColor ()), GetGValue(GetBackgroundColor ()), GetBValue(GetBackgroundColor ()));
	str=str+strValue+" ";
	m_objStyle.Save (str);
    str=str+">"+CRLF;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</led>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSIAngularGauge::SaveXML (CString& strSave)
{
    CString str, strTab, strValue;

    str.Format ("<angulargauge class=\"controls\" name=\"%s\" size=\"1\" id=\"%d\"",  GetName (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
	/* caratteristiche dispositivo */
	strValue.Format ("path=\"%s\" device=\"%d\" address=\"%d\" data=\"%d\"", GetPath (), GetDeviceType (), GetDeviceID (), GetDeviceData ());
	str=str+strValue+" ";
	/* caratteristiche generali */
	strValue.Format ("G_G_Transparent=\"%d\" G_G_PosMin=\"%d\" G_G_PosMax=\"%d\" G_G_ShowInArcR=\"%d\" G_G_ShowOutArcR=\"%d\" "
		"G_G_RevScale=\"%d\" G_G_BorderStyle=\"%d\" G_G_BackColor=\"#%02x%02x%02x\"",
		IsTransparent (), GetPosMin (), GetPosMax (), IsInnerRadius (), IsOuterRadius (), IsReverseScale (), GetBorderStyle (),
		GetRValue(GetBackGroundColor ()), GetGValue(GetBackGroundColor ()), GetBValue(GetBackGroundColor ()));
	str=str+strValue+" ";
	/* pointers */
	strValue.Format ("G_P_Type=\"%d\" G_P_Size=\"%d\" G_P_Margin=\"%d\" G_P_Color=\"#%02x%02x%02x\"",
		GetPointerStyle (), GetPointerSize (), GetPointerMargin (),
		GetRValue(GetPointerColor ()), GetGValue(GetPointerColor ()), GetBValue(GetPointerColor ()));
	str=str+strValue+" ";
	/* arc */
	strValue.Format ("G_A_Radius=\"%d\" G_A_Angle=\"%d\" G_A_StartAngle=\"%d\"",
		GetArcRadius (), GetArcAngle (), GetArcStartAngle ());
	str=str+strValue+" ";
	/* hub */
	strValue.Format ("G_H_Show=\"%d\" G_H_Show3D=\"%d\" G_H_Size=\"%d\" G_H_Color=\"#%02x%02x%02x\"",
		IsHub (), IsHub3D (), GetHubSize (), 
		GetRValue(GetHubColor ()), GetGValue(GetHubColor ()), GetBValue(GetHubColor ()));
	str=str+strValue+" ";
	/* face */
	strValue.Format ("G_F_Show=\"%d\" G_F_Type=\"%d\" G_B_Type=\"%d\" G_B_Size=\"%d\" G_F_Color=\"#%02x%02x%02x\"",
		IsFace (), GetFaceStyle (), GetBevelStyle (), GetBevelSize (), 
		GetRValue(GetFaceColor ()), GetGValue(GetFaceColor ()), GetBValue(GetFaceColor ()));
	str=str+strValue+" ";
	/* tick */
	strValue.Format ("G_T_Margin=\"%d\"", GetTickMargin ());
	str=str+strValue+" ";
	/* tick label */
	strValue.Format ("G_TL_Margin=\"%d\"", GetTickLabelMargin ());
	str=str+strValue+" ";
	/* tick major */
	strValue.Format ("G_TMA_Show=\"%d\" G_TMA_Number=\"%d\" G_TMA_Lenght=\"%d\" G_TMA_Color=\"#%02x%02x%02x\"",
		IsTickMajor (), GetTickMajorNumber (), GetTickMajorLenght (),
		GetRValue(GetTickMajorColor ()), GetGValue(GetTickMajorColor ()), GetBValue(GetTickMajorColor ()));
	str=str+strValue+" ";
	/* tick minor */
	strValue.Format ("G_TMI_Show=\"%d\" G_TMI_Number=\"%d\" G_TMI_Lenght=\"%d\" G_TMI_Color=\"#%02x%02x%02x\"",
		IsTickMinor (), GetTickMinorNumber (), GetTickMinorLenght (),
		GetRValue(GetTickMinorColor ()), GetGValue(GetTickMinorColor ()), GetBValue(GetTickMinorColor ()));
	str=str+strValue+" ";
	/* sections */
	strValue.Format ("G_S_Count=\"%d\" G_S_Color1=\"#%02x%02x%02x\" G_S_Color2=\"#%02x%02x%02x\" G_S_Color3=\"#%02x%02x%02x\" "
		"G_S_Color4=\"#%02x%02x%02x\" G_S_Color5=\"#%02x%02x%02x\" G_S_End1=\"%d\" G_S_End2=\"%d\" G_S_End3=\"%d\" G_S_End4=\"%d\" ",
		GetSectionsNumber (),
		GetRValue(GetSection1Color ()), GetGValue(GetSection1Color ()), GetBValue(GetSection1Color ()),
		GetRValue(GetSection2Color ()), GetGValue(GetSection2Color ()), GetBValue(GetSection2Color ()),
		GetRValue(GetSection3Color ()), GetGValue(GetSection3Color ()), GetBValue(GetSection3Color ()),
		GetRValue(GetSection4Color ()), GetGValue(GetSection4Color ()), GetBValue(GetSection4Color ()),
		GetRValue(GetSection5Color ()), GetGValue(GetSection5Color ()), GetBValue(GetSection5Color ()),
		GetEndSection1 (), GetEndSection2 (), GetEndSection3 (), GetEndSection4 ());
	str=str+strValue+" ";
	m_objStyle.Save (str);
    str=str+">"+CRLF;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</angulargauge>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSILinearGauge::SaveXML (CString& strSave)
{
    CString str, strTab, strValue;

	str.Format ("<lineargauge class=\"controls\" name=\"%s\" size=\"1\" id=\"%d\"",  GetName (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
	/* caratteristiche dispositivo */
	strValue.Format ("path=\"%s\" device=\"%d\" address=\"%d\" data=\"%d\"", GetPath (), GetDeviceType (), GetDeviceID (), GetDeviceData ());
	str=str+strValue+" ";
	/* caratteristiche generali */
	strValue.Format ("LI_G_Transparent =\"%d\" LI_G_PosMin=\"%d\" LI_G_PosMax=\"%d\" "
		"LI_G_RevScale=\"%d\" LI_G_Orientation=\"%d\" LI_G_OrientationTicks=\"%d\" LI_G_BorderStyle=\"%d\" "
		"LI_G_BackColor=\"#%02x%02x%02x\"",
		IsTransparent (), GetPosMin (), GetPosMax (), IsReverseScale (), GetOrientation (), GetOrientationTicks (), GetBorderStyle (),
		GetRValue(GetBackGroundColor ()), GetGValue(GetBackGroundColor ()), GetBValue(GetBackGroundColor ()));
	str=str+strValue+" ";
	strValue.Format ("LI_P_Style =\"%d\" LI_P_Size=\"%d\" LI_P_Margin=\"%d\" LI_P_Color=\"#%02x%02x%02x\" "
		"LI_P_3D=\"%d\" LI_P_DrawScaleSide=\"%d\"",
		GetPointerStyle (), GetPointerSize (), GetPointerOffset (),
		GetRValue(GetPointerColor ()), GetGValue(GetPointerColor ()), GetBValue(GetPointerColor ()),
		GetPointer3D (), GetDrawScaleSide ());
	str=str+strValue+" ";
	strValue.Format ("LI_T_Show =\"%d\" LI_T_Margin=\"%d\" LI_T_ShowLabels=\"%d\" LI_T_LabelMargin=\"%d\"",
		IsTickAxes (), GetTickMargin (), IsShowLabels (), GetLabelMargin ());
	str=str+strValue+" ";
	strValue.Format ("LI_TMA_Show=\"%d\" LI_TMA_Number=\"%d\" LI_TMA_Lenght=\"%d\" LI_TMA_Color=\"#%02x%02x%02x\"",
		IsTickMajor (), GetTickMajorNumber (), GetTickMajorLenght (),
		GetRValue(GetTickMajorColor ()), GetGValue(GetTickMajorColor ()), GetBValue(GetTickMajorColor ()));
	str=str+strValue+" ";
	/* tick minor */
	strValue.Format ("LI_TMI_Show=\"%d\" LI_TMI_Number=\"%d\" LI_TMI_Lenght=\"%d\" LI_TMI_Color=\"#%02x%02x%02x\"",
		IsTickMinor (), GetTickMinorNumber (), GetTickMinorLenght (),
		GetRValue(GetTickMinorColor ()), GetGValue(GetTickMinorColor ()), GetBValue(GetTickMinorColor ()));
	str=str+strValue+" ";
	/* sections */
	strValue.Format ("LI_S_Count=\"%d\" LI_S_Color1=\"#%02x%02x%02x\" LI_S_Color2=\"#%02x%02x%02x\" LI_S_Color3=\"#%02x%02x%02x\" "
		"LI_S_Color4=\"#%02x%02x%02x\" LI_S_Color5=\"#%02x%02x%02x\" LI_S_End1=\"%d\" LI_S_End2=\"%d\" LI_S_End3=\"%d\" LI_S_End4=\"%d\"",
		GetSectionsNumber (),
		GetRValue(GetSection1Color ()), GetGValue(GetSection1Color ()), GetBValue(GetSection1Color ()),
		GetRValue(GetSection2Color ()), GetGValue(GetSection2Color ()), GetBValue(GetSection2Color ()),
		GetRValue(GetSection3Color ()), GetGValue(GetSection3Color ()), GetBValue(GetSection3Color ()),
		GetRValue(GetSection4Color ()), GetGValue(GetSection4Color ()), GetBValue(GetSection4Color ()),
		GetRValue(GetSection5Color ()), GetGValue(GetSection5Color ()), GetBValue(GetSection5Color ()),
		GetEndSection1 (), GetEndSection2 (), GetEndSection3 (), GetEndSection4 ());
	str=str+strValue+" ";
	m_objStyle.Save (str);
    str=str+">"+CRLF;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</lineargauge>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSIDisplay::SaveXML (CString& strSave)
{
    CString str, strTab, strValue;

    str.Format ("<display class=\"controls\" name=\"%s\" size=\"1\" id=\"%d\"",  GetName (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
	/* caratteristiche dispositivo */
	strValue.Format ("path=\"%s\" device=\"%d\" address=\"%d\" data=\"%d\"", GetPath (), GetDeviceType (), GetDeviceID (), GetDeviceData ());
	str=str+strValue+" ";
	/* caratteristiche generali */
	strValue.Format ("D_G_ShowOffsement=\"%d\" D_G_Transparent=\"%d\" D_G_ShowSign=\"%d\" D_G_AutosegmentColor=\"%d\" D_G_Precision=\"%d\" "
		"D_G_BorderStyle=\"%d\" D_G_LeadingStyle=\"%d\" D_G_BackColor=\"#%02x%02x%02x\"",
		IsOffSegment (), IsTransparent (), IsSign (), IsAutoSegmentOffColor (), GetPrecision (), GetDisplayStyle (), GetLeadingStyle (),
		GetRValue(GetBackGroundColor ()), GetGValue(GetBackGroundColor ()), GetBValue(GetBackGroundColor ()));
	str=str+strValue+" ";
	strValue.Format ("D_D_Count=\"%d\" D_D_Spacing=\"%d\" D_D_Size=\"%d\" D_D_Separation=\"%d\" D_D_Margin=\"%d\" "
		"D_D_Color=\"#%02x%02x%02x\" D_D_OffColor=\"#%02x%02x%02x\"",
		GetDisplayCount (), GetDisplaySpacing (), GetDisplaySize (), GetDisplaySeparation (), GetDisplayMargin (),
		GetRValue(GetDisplayColor ()), GetGValue(GetDisplayColor ()), GetBValue(GetDisplayColor ()),
		GetRValue(GetDisplayOffColor ()), GetGValue(GetDisplayOffColor ()), GetBValue(GetDisplayOffColor ()));
	str=str+strValue+" ";
	m_objStyle.Save (str);
    str=str+">"+CRLF;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</display>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSILabel::SaveXML (CString& strSave)
{
    CString str, strTab, strValue;

	str.Format ("<static class=\"controls\" name=\"%s\" value=\"%s\" size=\"1\" id=\"%d\"",  GetName (), GetValue (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
	/* caratteristiche dispositivo */
	strValue.Format ("path=\"%s\" device=\"%d\" address=\"%d\" data=\"%d\"", GetPath (), GetDeviceType (), GetDeviceID (), GetDeviceData ());
	str=str+strValue+" ";
	/* caratteristiche generali */
	strValue.Format ("L_G_Transparent=\"%d\" L_G_AutoSize=\"%d\" L_G_BorderStyle=\"%d\" L_G_Alignment=\"%d\" L_G_Shadow=\"%d\" "
		"L_G_OffsetX=\"%d\" L_G_OffsetY=\"%d\" L_G_Color=\"#%02x%02x%02x\" L_G_BackColor=\"#%02x%02x%02x\" L_G_ShadowColor=\"#%02x%02x%02x\"",
		IsTransparent (), IsAutoSize (), GetStyle (), GetAlignment (), IsShadow (), GetOffsetX (), GetOffsetY (),
		GetRValue(GetColor ()), GetGValue(GetColor ()), GetBValue(GetColor ()),
		GetRValue(GetBackColor ()), GetGValue(GetBackColor ()), GetBValue(GetBackColor ()),
		GetRValue(GetShadowColor ()), GetGValue(GetShadowColor ()), GetBValue(GetShadowColor ()));
	str=str+strValue+" ";
	m_objStyle.Save (str);
    str=str+">"+CRLF;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</static>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** SaveXML :
*/
int CAdamoRSIPicture::SaveXML (CString& strSave)
{
    CString str, strTab, strVar, strValue, strChecked, strTransparentColor, strPictures;

    str.Format ("<picture class=\"controls\" name=\"%s\" id=\"%d\"", GetName (), GetID ());
    if (GetTabIndex()!=0)
        strTab.Format (" tabindex=\"%d\"", GetTabIndex());
    str=str+strTab+" ";
	/* caratteristiche dispositivo */
	strValue.Format ("path=\"%s\" device=\"%d\" address=\"%d\" data=\"%d\"", GetPath (), GetDeviceType (), GetDeviceID (), GetDeviceData ());
	str=str+strValue+" ";
    strChecked.Format ("checked=%s ", IsStretched () ? "\"true\"" : "\"false\"");    
    str=str+strChecked+" ";
    strTransparentColor.Format ("transparent=%s transparentcolor=\"#%02x%02x%02x\"", IsTransparent () ? "\"true\"" : "\"false\"", GetRValue(GetTransparentColor ()), GetGValue(GetTransparentColor ()), GetBValue(GetTransparentColor ()));
    str=str+strTransparentColor + " ";
    strPictures.Format ("picture1=\"%s\" picture2=\"%s\" picture3=\"%s\" picture4=\"%s\" picture5=\"%s\" picture6=\"%s\" picture7=\"%s\" picture8=\"%s\"", 
        GetImageID (0), GetImageID (1), GetImageID (2), GetImageID (3), GetImageID (4), GetImageID (5), GetImageID (6), GetImageID (7));
    str=str+strPictures+" ";
    m_objStyle.Save (str);
    str=str+">"+CRLF;
    m_objVar.SaveXML (str, GetID ());
    str=str+"</picture>"+CRLF;
    strSave+=str;
    return 0;
}

/*
** TranslateRGB :
*/
COLORREF TranslateRGB (char* p)
{
    char pBuf[3], pRGB[3];
    pBuf[2]='\0';
    for (int l=0; l<3; l++)   {
        pBuf[0]=*p++;
        pBuf[1]=*p++;
        pRGB[l]=_tcstoul (pBuf, 0, 16);
    }
    return RGB (pRGB[0], pRGB[1], pRGB[2]);
}

/*
** LoadXML :
*/
int CAdamoRSCtrlList::LoadXML (CString strXML)
{
    HRESULT hr = m_pDocXml.CreateInstance( __uuidof(MSXML2::DOMDocument60));
    if (SUCCEEDED(hr))   {
        m_pDocXml->async = false;
        if (m_pDocXml->loadXML(strXML.AllocSysString ()))   {
            ParseXMLString ();
            hr=S_OK;
        }
        m_pDocXml.Release ();
    }
    return hr == S_OK ? 0 : -1;
}

/*
** ParseXMLString :
*/
int CAdamoRSCtrlList::ParseXMLString ()
{
    ElementXmlPtr  p=m_pDocXml->documentElement;
	NodeListXmlPtr pRootChild;

    if (p->hasChildNodes())	  {
        pRootChild = p->childNodes;
        COleVariant v=pRootChild->Getitem (0)->nodeName;
        if (CString (v.bstrVal) == "form")   {
            CreateFormXML (pRootChild->Getitem (0));
            CreateXMLControls (pRootChild->Getitem (0));
        }
    }
    return 0;
}

/*
** CreateFormXML :
*/
int CAdamoRSCtrlList::CreateFormXML (ElementXmlPtr p)
{
	COleVariant v;
	CString strFormName, strFormPosition, strImageName;

    CAdamoRSForm *pForm=new CAdamoRSForm;
    v=p->getAttribute("name");
    if (v.vt==VT_BSTR)   {
        strFormPosition=CString (v.bstrVal);
		pForm->SetName (strFormPosition);
	}
    v=p->getAttribute("formname");
    if (v.vt==VT_BSTR)   {
        strFormName=CString (v.bstrVal);
		pForm->CAdamoRSBase::SetName (strFormName);
	}
    v=p->getAttribute("src");
    if (v.vt==VT_BSTR)   {
        strImageName = CString (v.bstrVal);
		pForm->SetBackgroundImage (strImageName);
		if (strImageName != "")
			pForm->EnableBackground (true);
	}
    AddCtrl (pForm);
    return 0;
}
 
/*
** CreateXMLControls :
*/
int CAdamoRSCtrlList::CreateXMLControls (ElementXmlPtr p)
{
	NodeListXmlPtr pRootChild;
    int nB=-1;

    if (p->hasChildNodes())	  {
        pRootChild = p->childNodes;
        int n=pRootChild->length, i=0;
        while (n>0)   {
            COleVariant v=pRootChild->Getitem (i)->nodeName;
            CString str=CString (v.bstrVal);
            if (str=="input")
                nB=CreateXMLInput (pRootChild->Getitem (i));
            else
            if (str=="select")
                nB=CreateXMLComboList (pRootChild->Getitem (i));
            else
            if (str=="fieldset")
                nB=CreateXMLGroupBox (pRootChild->Getitem (i));
            else
            if (str=="button")
                nB=CreateXMLButton (pRootChild->Getitem (i));
            else
            if (str=="label")
                nB=CreateXMLLabel (pRootChild->Getitem (i));
            else
            if (str=="img")
                nB=CreateXMLInputImage (pRootChild->Getitem (i));
			else
            if (str=="grid")
                nB=CreateXMLGrid (pRootChild->Getitem (i));
			else
            if (str=="ax")
                nB=CreateXMLAx (pRootChild->Getitem (i));
			else
            if (str=="led")
                nB=CreateXMLLed (pRootChild->Getitem (i));
			else
            if (str=="angulargauge")
                nB=CreateXMLAngularGauge (pRootChild->Getitem (i));
			else
            if (str=="lineargauge")
                nB=CreateXMLLinearGauge (pRootChild->Getitem (i));
			else
            if (str=="display")
                nB=CreateXMLDisplay (pRootChild->Getitem (i));
			else
            if (str=="static")
                nB=CreateXMLStatic (pRootChild->Getitem (i));
			else
            if (str=="picture")
                nB=CreateXMLPicture (pRootChild->Getitem (i));
            n--; i++;
        }
    }
    return nB;
}

/*
** CreateXMLInput :
*/
int CAdamoRSCtrlList::CreateXMLInput (ElementXmlPtr p)
{
    COleVariant v=p->getAttribute("type");
    if (v.vt==VT_BSTR)   {
        CString strType=CString (v.bstrVal);
        if (strType=="password")
            return CreateXMLInputText (p);
        else
        if (strType=="text")
            return CreateXMLInputText (p);
        else
        if (strType=="radio")
            return CreateXMLInputRadio (p);
        else
        if (strType=="checkbox")
            return CreateXMLInputCheckBox (p);
    }
    return -1;
}

/*
** CreateXMLComboList :
*/
int CAdamoRSCtrlList::CreateXMLComboList (ElementXmlPtr p)
{
    COleVariant v=p->getAttribute("size");
    int nSize=atoi (CString (v.bstrVal));
    if (nSize==1)
        CreateXMLCombo (p);
    else
        CreateXMLList (p);
    return 0;
}

/*
** CreateXMLCombo :
*/
int CAdamoRSCtrlList::CreateXMLCombo (ElementXmlPtr p)
{
    CAdamoRSCombo *pIT=new CAdamoRSCombo;
    COleVariant v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLOptions (p, pIT, true);
    LoadXMLVariable (p, pIT);
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLList :
*/
int CAdamoRSCtrlList::CreateXMLList (ElementXmlPtr p)
{
    CAdamoRSList *pIT=new CAdamoRSList;
    COleVariant v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("size");
    pIT->SetSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLOptions (p, pIT, false);
    LoadXMLVariable (p, pIT);
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLGroupBox :
*/
int CAdamoRSCtrlList::CreateXMLGroupBox (ElementXmlPtr p)
{
    CAdamoRSGroup *pIT=new CAdamoRSGroup;
    COleVariant v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    if (p->hasChildNodes())   {
        ElementXmlPtr pLegend = p->childNodes->Getitem (0);
        COleVariant v=pLegend->nodeName;
        if (CString (v.bstrVal)=="legend")   {
            v=pLegend->text;
            pIT->SetValue (CString (v.bstrVal));
        }
    }
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLButton :
*/
int CAdamoRSCtrlList::CreateXMLButton (ElementXmlPtr p)
{
    CAdamoRSButton *pIT=new CAdamoRSButton;
    COleVariant v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pIT->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    v=p->getAttribute("src");
    pIT->SetButtonImage (CString (v.bstrVal));
	v=p->getAttribute("type");
	CString str = CString (v.bstrVal);
	if (str == "standard")
		pIT->SetButtonType (eStandard);
	else
	if (str == "bitmap")
		pIT->SetButtonType (eBitmap);
	v=p->getAttribute("transparency");
	str = CString (v.bstrVal);
	pIT->SetTransparency (atoi ((const char *)str) != 0);
	v=p->getAttribute("action");
	str = CString (v.bstrVal);
	DWORD dwAction = atoi ((const char *)str);
	if (bittest (dwAction, 0))
		pIT->EnableDownloadAction ();
	if (bittest (dwAction, 1))
		pIT->EnableLoadAction ();
	if (bittest (dwAction, 2))
		pIT->EnableSaveAction ();
	if (bittest (dwAction, 3))
		pIT->EnableChangePageAction ();
	v=p->getAttribute("nextpage");
	pIT->SetNextPage (CString (v.bstrVal));
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLLabel :
*/
int CAdamoRSCtrlList::CreateXMLLabel (ElementXmlPtr p)
{
    CAdamoRSLabel *pIT=new CAdamoRSLabel;
    COleVariant v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pIT->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLInputText :
*/
int CAdamoRSCtrlList::CreateXMLInputText (ElementXmlPtr p)
{
	eEditType e = eNone;
    CAdamoRSInputText *pIT=new CAdamoRSInputText;
    COleVariant v=p->getAttribute("type");
    CString strType=CString (v.bstrVal);
    if (strType=="password")
        pIT->SetPsw (true);
    else
    if (strType=="text")
        pIT->SetPsw (false);
    v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pIT->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("size");
    pIT->SetSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("autoupdate");
    pIT->SetAutoUpdate(CString (v.bstrVal)=="true");
	v=p->getAttribute("mode");
	CString str = CString (v.bstrVal);
	if (str == "numbers")
		e = eNumbers;
	else
	if (str == "float")
		e = eFloat;
	else
	if (str == "hex")
		e = eHex;
	else
	if (str == "letters")
		e = eLetters;
	pIT->SetType (e);
    pIT->SetSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, pIT);
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLInputRadio :
*/
int CAdamoRSCtrlList::CreateXMLInputRadio (ElementXmlPtr p)
{
    CAdamoRSInputRadio *pIT=new CAdamoRSInputRadio;
    COleVariant v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pIT->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("group");
    pIT->SetGroup (CString (v.bstrVal));
    v=p->getAttribute("checked");
    pIT->SetChecked (CString (v.bstrVal)=="true");
    v=p->getAttribute("autoupdate");
    pIT->SetAutoUpdate (CString (v.bstrVal)=="true");
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, pIT);
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLInputCheckBox :
*/
int CAdamoRSCtrlList::CreateXMLInputCheckBox (ElementXmlPtr p)
{
    CAdamoRSInputCheck *pIT=new CAdamoRSInputCheck;
    COleVariant v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pIT->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("checked");
    pIT->SetChecked (CString (v.bstrVal)=="true");
    v=p->getAttribute("autoupdate");
    pIT->SetAutoUpdate (CString (v.bstrVal)=="true");
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, pIT);
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLInputImage :
*/
int CAdamoRSCtrlList::CreateXMLInputImage (ElementXmlPtr p)
{
    CAdamoRSImage *pIT=new CAdamoRSImage;
    CString str;
    char *pColor;

    COleVariant v=p->getAttribute("src");
    pIT->SetIDImage (CString (v.bstrVal));
    v=p->getAttribute("checked");
    pIT->SetStretch (CString (v.bstrVal)=="true");
    v=p->getAttribute("transparent");
    pIT->SetTransparent (CString (v.bstrVal)=="true");
    v=p->getAttribute("transparentcolor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pIT->SetTransparentColor (TranslateRGB (pColor + 1));
    pIT->SetStretch (CString (v.bstrVal)=="true");
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLGrid :
*/
int CAdamoRSCtrlList::CreateXMLGrid (ElementXmlPtr p)
{
	int nRows, nCols;
	char *pC;

    CAdamoRSGrid *pGrid=new CAdamoRSGrid;
    COleVariant v=p->getAttribute("name");
    pGrid->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pGrid->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pGrid->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("rows");
	nRows = strtol (CString (v.bstrVal), &pC, 10);
    v=p->getAttribute("cols");
	nCols = strtol (CString (v.bstrVal), &pC, 10);
	pGrid->SetGridData (nRows, nCols);
    v=p->getAttribute("style");
    pGrid->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLColumns (p, pGrid);
    LoadXMLVariable (p, pGrid);
    AddCtrl (pGrid);
	return 0;
}

/*
** CreateXMLAx :
*/
int CAdamoRSCtrlList::CreateXMLAx (ElementXmlPtr p)
{
    CAdamoRSAx *pAx=new CAdamoRSAx;
	CString strPath;
	int nID;

    COleVariant v=p->getAttribute("name");
    pAx->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pAx->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pAx->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    v=p->getAttribute("address");
    nID = atoi (CString (v.bstrVal));
	v=p->getAttribute("style");
	pAx->SetData (strPath, nID);
    pAx->GetObjStyle().Load (CString (v.bstrVal));
    AddCtrl (pAx);
	return 0;
}

/*
** CreateXMLLed :
*/
int CAdamoRSCtrlList::CreateXMLLed (ElementXmlPtr p)
{
    CAdamoRSILed *pLed=new CAdamoRSILed;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nID, nData;

    COleVariant v=p->getAttribute("name");
    pLed->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pLed->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pLed->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
    v=p->getAttribute("address");
    nID = atoi (CString (v.bstrVal));
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pLed->SetDevice (strPath, eDev, nID, nData);
    v=p->getAttribute("L_G_Type");
	pLed->SetLedType ((eLedType)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_Transparent");
    pLed->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("L_G_ShowReflection");
    pLed->SetShowReflection (atoi (CString (v.bstrVal)) != 0);
    v=p->getAttribute("L_G_ActiveColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLed->SetActiveColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("L_G_BevelStyle");
	pLed->SetBevelStyle ((eLedStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_SpacingHorizontal");
    pLed->SetHorizontalSpacing (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_SpacingVertical");
    pLed->SetVerticalSpacing (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_RowCount");
    pLed->SetRowCount (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_ColCount");
    pLed->SetColCount (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_BorderStyle");
	pLed->SetBorderStyle ((eLedStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_BackgroundColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLed->SetBackgroundColor (TranslateRGB (pColor + 1));
	v=p->getAttribute("style");
    pLed->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, pLed);
    AddCtrl (pLed);
	return 0;
}

/*
** CreateXMLAngularGauge :
*/
int CAdamoRSCtrlList::CreateXMLAngularGauge (ElementXmlPtr p)
{
    CAdamoRSIAngularGauge *pAngGauge=new CAdamoRSIAngularGauge;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nID, nData;

    COleVariant v=p->getAttribute("name");
    pAngGauge->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pAngGauge->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pAngGauge->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
    v=p->getAttribute("address");
    nID = atoi (CString (v.bstrVal));
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pAngGauge->SetDevice (strPath, eDev, nID, nData);
	/* caratteristiche generali */
    v=p->getAttribute("G_G_Transparent");
	pAngGauge->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_G_PosMin");
	pAngGauge->SetPosMin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_G_PosMax");
	pAngGauge->SetPosMax (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_G_ShowInArcR");
	pAngGauge->ShowInnerRadius (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_G_ShowOutArcR");
	pAngGauge->ShowOuterRadius (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_G_RevScale");
	pAngGauge->SetReverseScale (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_G_BorderStyle");
	pAngGauge->SetBorderStyle ((eBorderStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_G_BackColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetBackGroundColor (TranslateRGB (pColor + 1));
	/* pointers */
    v=p->getAttribute("G_P_Type");
	pAngGauge->SetPointerStyle ((ePointerStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_P_Size");
	pAngGauge->SetPointerSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_P_Margin");
	pAngGauge->SetPointerMargin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_P_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetPointerColor (TranslateRGB (pColor + 1));
	/* arc */
    v=p->getAttribute("G_A_Radius");
	pAngGauge->SetArcRadius (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_A_Angle");
	pAngGauge->SetArcAngle (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_A_StartAngle");
	pAngGauge->SetArcStartAngle (atoi (CString (v.bstrVal)));
	/* hub */
    v=p->getAttribute("G_H_Show");
	pAngGauge->ShowHub (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_H_Show3D");
	pAngGauge->ShowHub3D (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_H_Size");
	pAngGauge->SetHubSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_H_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetHubColor (TranslateRGB (pColor + 1));
	/* face */
    v=p->getAttribute("G_F_Show");
	pAngGauge->ShowFace (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_F_Type");
	pAngGauge->SetFaceStyle ((eStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_B_Type");
	pAngGauge->SetBevelStyle ((eStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_B_Size");
	pAngGauge->SetBevelSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_F_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetFaceColor (TranslateRGB (pColor + 1));
	/* tick */
    v=p->getAttribute("G_T_Margin");
	pAngGauge->SetTickMargin (atoi (CString (v.bstrVal)));
	/* tick label */
    v=p->getAttribute("G_TL_Margin");
	pAngGauge->SetTickLabelMargin (atoi (CString (v.bstrVal)));
	/* tick major */
    v=p->getAttribute("G_TMA_Show");
	pAngGauge->ShowTickMajor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_TMA_Number");
	pAngGauge->SetTickMajorNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_TMA_Lenght");
	pAngGauge->SetTickMajorLenght (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_TMA_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetTickMajorColor (TranslateRGB (pColor + 1));
	/* tick minor */
    v=p->getAttribute("G_TMI_Show");
	pAngGauge->ShowTickMinor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_TMI_Number");
	pAngGauge->SetTickMinorNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_TMI_Lenght");
	pAngGauge->SetTickMinorLenght (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_TMI_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetTickMinorColor (TranslateRGB (pColor + 1));
	/* sections */
    v=p->getAttribute("G_S_Count");
	pAngGauge->SetSectionsNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_S_Color1");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection1Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_Color2");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection2Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_Color3");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection3Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_Color4");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection4Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_Color5");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection5Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_End1");
	pAngGauge->SetEndSection1 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_S_End2");
	pAngGauge->SetEndSection2 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_S_End3");
	pAngGauge->SetEndSection3 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_S_End4");
	pAngGauge->SetEndSection4 (atoi (CString (v.bstrVal)));
	v=p->getAttribute("style");
    pAngGauge->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, pAngGauge);
    AddCtrl (pAngGauge);
	return 0;
}

/*
** CreateXMLLinearGauge :
*/
int CAdamoRSCtrlList::CreateXMLLinearGauge (ElementXmlPtr p)
{
    CAdamoRSILinearGauge *pLinGauge=new CAdamoRSILinearGauge;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nID, nData;

    COleVariant v=p->getAttribute("name");
    pLinGauge->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pLinGauge->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pLinGauge->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
    v=p->getAttribute("address");
    nID = atoi (CString (v.bstrVal));
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pLinGauge->SetDevice (strPath, eDev, nID, nData);
	/* general */
    v=p->getAttribute("LI_G_Transparent");
	pLinGauge->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_G_PosMin");
	pLinGauge->SetPosMin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_PosMax");
	pLinGauge->SetPosMax (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_RevScale");
	pLinGauge->SetReverseScale (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_G_Orientation");
	pLinGauge->SetOrientation ((eOrientation)atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_OrientationTicks");
	pLinGauge->SetOrientationTicks ((eOrientationTicks)atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_BorderStyle");
	pLinGauge->SetBorderStyle ((eLinearGaugeStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_BackColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetBackGroundColor (TranslateRGB (pColor + 1));
	/* pointers */
    v=p->getAttribute("LI_P_Style");
	pLinGauge->SetPointerStyle ((eLinearGaugePointerStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_P_Size");
	pLinGauge->SetPointerSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_P_Margin");
	pLinGauge->SetPointerOffset (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_P_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetPointerColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_P_3D");
	pLinGauge->SetPointer3D (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_P_DrawScaleSide");
	pLinGauge->SetDrawScaleSide (atoi (CString (v.bstrVal))!=0);
	/* ticks */
    v=p->getAttribute("LI_T_Show");
	pLinGauge->ShowTickAxes (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_T_Margin");
	pLinGauge->SetTickMargin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_T_ShowLabels");
	pLinGauge->ShowLabels (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_T_LabelMargin");
	pLinGauge->SetLabelMargin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMA_Show");
	pLinGauge->ShowTickMajor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_TMA_Number");
	pLinGauge->SetTickMajorNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMA_Lenght");
	pLinGauge->SetTickMajorLenght (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMA_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetTickMajorColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_TMI_Show");
	pLinGauge->ShowTickMinor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_TMI_Number");
	pLinGauge->SetTickMinorNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMI_Lenght");
	pLinGauge->SetTickMinorLenght (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMI_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetTickMinorColor (TranslateRGB (pColor + 1));
	/* sections */
    v=p->getAttribute("LI_S_Count");
	pLinGauge->SetSectionsNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_S_Color1");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection1Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_Color2");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection2Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_Color3");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection3Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_Color4");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection4Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_Color5");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection5Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_End1");
	pLinGauge->SetEndSection1 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_S_End2");
	pLinGauge->SetEndSection2 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_S_End3");
	pLinGauge->SetEndSection3 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_S_End4");
	pLinGauge->SetEndSection4 (atoi (CString (v.bstrVal)));
	v=p->getAttribute("style");
    pLinGauge->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, pLinGauge);
    AddCtrl (pLinGauge);
	return 0;
}

/*
** CreateXMLDisplay :
*/
int CAdamoRSCtrlList::CreateXMLDisplay (ElementXmlPtr p)
{
    CAdamoRSIDisplay *pDisplay=new CAdamoRSIDisplay;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nID, nData;

    COleVariant v=p->getAttribute("name");
    pDisplay->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pDisplay->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pDisplay->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
    v=p->getAttribute("address");
    nID = atoi (CString (v.bstrVal));
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pDisplay->SetDevice (strPath, eDev, nID, nData);
	/* general */
    v=p->getAttribute("D_G_ShowOffsement");
	pDisplay->ShowOffSegment (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("D_G_Transparent");
	pDisplay->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("D_G_ShowSign");
	pDisplay->ShowSign (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("D_G_AutosegmentColor");
	pDisplay->SetAutoSegmentOffColor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("D_G_Precision");
	pDisplay->SetPrecision (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_G_BorderStyle");
	pDisplay->SetDisplayStyle ((eDisplayStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_G_LeadingStyle");
	pDisplay->SetLeadingStyle ((eDisplayLeadingStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_G_BackColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pDisplay->SetBackGroundColor (TranslateRGB (pColor + 1));
	/* display */
    v=p->getAttribute("D_D_Count");
	pDisplay->SetDisplayCount (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Spacing");
	pDisplay->SetDisplaySpacing (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Size");
	pDisplay->SetDisplaySize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Separation");
	pDisplay->SetDisplaySeparation (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Margin");
	pDisplay->SetDisplayMargin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pDisplay->SetDisplayColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("D_D_OffColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pDisplay->SetDisplayOffColor (TranslateRGB (pColor + 1));
	v=p->getAttribute("style");
    pDisplay->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, pDisplay);
    AddCtrl (pDisplay);
	return 0;
}

/*
** CreateXMLStatic :
*/
int CAdamoRSCtrlList::CreateXMLStatic (ElementXmlPtr p)
{
    CAdamoRSILabel *pLabel=new CAdamoRSILabel;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nID, nData;

    COleVariant v=p->getAttribute("name");
    pLabel->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pLabel->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pLabel->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
    v=p->getAttribute("address");
    nID = atoi (CString (v.bstrVal));
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pLabel->SetDevice (strPath, eDev, nID, nData);
    v=p->getAttribute("value");
    pLabel->SetValue (CString (v.bstrVal));
	/* general */
    v=p->getAttribute("L_G_Transparent");
	pLabel->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("L_G_AutoSize");
	pLabel->SetAutoSize (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("L_G_BorderStyle");
	pLabel->SetStyle ((eLabelStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_Alignment");
	pLabel->SetAlignment ((eLabelAlignment)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_Shadow");
	pLabel->ShowShadow (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("L_G_OffsetX");
	pLabel->SetOffsetX (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_OffsetY");
	pLabel->SetOffsetY (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLabel->SetColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("L_G_BackColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLabel->SetBackColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("L_G_ShadowColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLabel->SetShadowColor (TranslateRGB (pColor + 1));
	v=p->getAttribute("style");
    pLabel->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, pLabel);
    AddCtrl (pLabel);
	return 0;
}

/*
** CreateXMLPicture :
*/
int CAdamoRSCtrlList::CreateXMLPicture (ElementXmlPtr p)
{
    CAdamoRSIPicture *pPicture=new CAdamoRSIPicture;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nID, nData;

    COleVariant v=p->getAttribute("name");
    pPicture->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pPicture->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pPicture->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
    v=p->getAttribute("address");
    nID = atoi (CString (v.bstrVal));
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pPicture->SetDevice (strPath, eDev, nID, nData);
    v=p->getAttribute("checked");
    pPicture->SetStretch (CString (v.bstrVal)=="true");
    v=p->getAttribute("transparent");
    pPicture->SetTransparent (CString (v.bstrVal)=="true");
    v=p->getAttribute("transparentcolor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pPicture->SetTransparentColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("picture1");
    pPicture->SetImageID (0, CString (v.bstrVal));
    v=p->getAttribute("picture2");
    pPicture->SetImageID (1, CString (v.bstrVal));
    v=p->getAttribute("picture3");
    pPicture->SetImageID (2, CString (v.bstrVal));
    v=p->getAttribute("picture4");
    pPicture->SetImageID (3, CString (v.bstrVal));
    v=p->getAttribute("picture5");
    pPicture->SetImageID (4, CString (v.bstrVal));
    v=p->getAttribute("picture6");
    pPicture->SetImageID (5, CString (v.bstrVal));
    v=p->getAttribute("picture7");
    pPicture->SetImageID (6, CString (v.bstrVal));
    v=p->getAttribute("picture8");
    pPicture->SetImageID (7, CString (v.bstrVal));
    v=p->getAttribute("style");
    pPicture->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, pPicture);
    AddCtrl (pPicture);
    return 0;
}

/*
** LoadXMLVariable :
*/
int CAdamoRSCtrlList::LoadXMLVariable (ElementXmlPtr p, CAdamoRSVarControl *pCtrl)
{
	NodeListXmlPtr pRootChild;

    if (p->hasChildNodes())   {
        pRootChild = p->childNodes;
        int n=pRootChild->length, i=0;
        while (n>0)   {
            ElementXmlPtr pVar = p->childNodes->Getitem (i);
            COleVariant v=pVar->nodeName;
            if (CString (v.bstrVal)=="Variable")   {
                v=pVar->getAttribute("VariableName");
                if (v.vt==VT_BSTR)   {
                    /* e' una variabile semplice */
                    pCtrl->GetObjVar ().m_strName=CString (v.bstrVal);
                    v=pVar->getAttribute("VariableIndex");
                    pCtrl->GetObjVar ().Load (CString (v.bstrVal));
                }
                v=pVar->getAttribute("Table");
                if (v.vt==VT_BSTR)   {
                    /* e' una tabella */
                    pCtrl->GetObjVar ().m_strTable=CString (v.bstrVal);
                    v=pVar->getAttribute("Field");
					if (v.vt==VT_BSTR)
						pCtrl->GetObjVar ().m_strField=CString (v.bstrVal);
                    v=pVar->getAttribute("KeyName");
					if (v.vt==VT_BSTR)
						pCtrl->GetObjVar ().m_strKeyName=CString (v.bstrVal);
                    v=pVar->getAttribute("KeyValue");
					if (v.vt==VT_BSTR)
						pCtrl->GetObjVar ().m_strKeyValue=CString (v.bstrVal);
                }
            }
            n--; i++;
        }
    }
    return 0;
}

/*
** LoadXMLOptions :
*/
int CAdamoRSCtrlList::LoadXMLOptions (ElementXmlPtr p, CAdamoRSControl *pCtrl, bool bIsCombo)
{
	NodeListXmlPtr pRootChild;
    ElementXmlPtr  pChild;
    struct stRSOpzioni stOpzioni;
    COleVariant v;

    if (p->hasChildNodes())   {
        pRootChild = p->childNodes;
        int n=pRootChild->length, i=0;
        while (n>0)   {
            pChild=pRootChild->Getitem (i);
            v=pChild->nodeName;
            if (CString (v.bstrVal)=="option")   {
                v=pChild->getAttribute("selected");
                stOpzioni.m_bSelected=CString (v.bstrVal)=="true";
                v=pChild->getAttribute("value");
                if (v.vt!=VT_NULL)
                    stOpzioni.strValue=CString (v.bstrVal);
                v=pChild->text;
                stOpzioni.strOpzione=CString (v.bstrVal);
                bIsCombo ? ((CAdamoRSCombo *)pCtrl)->AddRSOption (&stOpzioni, -1) : ((CAdamoRSList *)pCtrl)->AddRSOption (&stOpzioni, -1);
            }
            n--; i++;
        }
    }
    return 0;
}

/*
** LoadXMLColumns :
*/
int CAdamoRSCtrlList::LoadXMLColumns (ElementXmlPtr p, CAdamoRSControl *pCtrl)
{
	NodeListXmlPtr pRootChild;
    ElementXmlPtr  pChild;
    COleVariant v;
	CString strType, strImage;
	stGridColumnData gcd;

    if (p->hasChildNodes())   {
        pRootChild = p->childNodes;
        int n=pRootChild->length, i=0;
        while (n>0)   {
            pChild=pRootChild->Getitem (i);
            v=pChild->nodeName;
            if (CString (v.bstrVal)=="column")   {
                v=pChild->getAttribute("type");
				strType = CString (v.bstrVal);
				if (strType == "number")
					gcd.m_nType = LUA_TNUMBER;
				else
				if (strType == "boolean")
					gcd.m_nType = LUA_TBOOLEAN;
				else
				if (strType == "string")
					gcd.m_nType = LUA_TSTRING;
                v=pChild->getAttribute("image");
				strImage = CString (v.bstrVal);
                if (v.vt!=VT_NULL)
                    gcd.m_strImage = CString (v.bstrVal);
                v=pChild->text;
                gcd.m_strName=CString (v.bstrVal);
				((CAdamoRSGrid *) pCtrl)->SetColumnData (i, &gcd);
            }
            n--; i++;
        }
    }
    return 0;
}

/*
** LoadCaption :
*/
int CAdamoRSCtrlList::LoadCtrlCaption (int nId, CString strTitle, CString& strCaption)
{
    CString strKey, strMessaggio, strDescrizione;
    LONG dwNumero;
    CString str, strId;

    if (m_tr)   {
        strId.Format ("%d", nId);
        str=GetResourceName (strId);
        if (strTitle!="")   {
            str+="//";
            str+=strTitle;
        }
        m_tr->MessaggioStringa (2, str, &dwNumero, &strMessaggio, &strDescrizione);
        strCaption=strMessaggio;
    }
    return 0;
}

/*
** SaveCaption :
*/
int CAdamoRSCtrlList::SaveCtrlCaption (int nId, CString strTitle, CString strCaption)
{
    CString str, strId;
    if (m_tr)   {
        strId.Format ("%d", nId);
        str=GetResourceName (strId);
        if (strTitle!="")   {
            str+="//";
            str+=strTitle;
        }
        m_tr->CancellaMessaggioConStringa (2, str);
        m_tr->ScriviMessaggio (2, -1, str, strCaption, "");
    }
    return 0;
}

/*
** GetResourceName :
*/
CString CAdamoRSCtrlList::GetResourceName (CString strId)
{
    CString strResourceName, strResourceType;
    eResources eType;

    eType=GetResourceType ();
    strResourceName=GetResourceName ();
    switch (eType)   {
        case eParametriche :
            strResourceType="Parametriche";
            break;
        case eUtensili :
            strResourceType="Utensili";
            break;
        case eTestine :
            strResourceType="Testine";
            break;
        case eDialogs :
            strResourceType="Dialogs";
            break;
    }
    return strResourceType+"//"+strResourceName+"//"+strId;
}
