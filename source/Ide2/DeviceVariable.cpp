/*
 * DeviceVarable.h
 */

/* include files */
#include "stdafx.h"
#include "DeviceVariable.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceVariable::CDeviceVariable () : m_ad (NULL)
{
}

bool CDeviceVariable::ReadData (COleVariant& v)
{
    HRESULT hr;

    CDeviceData dd;
    dd.m_eDev=m_ad->GetType ();
    dd.m_nAddress=m_ad->GetDeviceID ();
    hr=GetMachine()->GetDevice (1, &dd);
    if (hr==S_OK)   {
        switch (m_nType)   {
            case LUA_TBOOLEAN :
                v=(long)dd.m_nDati;
                break;
            case LUA_TNUMBER :
                v=dd.m_fpDati;
                break;
            case LUA_TSTRING :
                v=dd.m_strDati;
                break;
        }
    }
    return hr==S_OK;
}

bool CDeviceVariable::WriteData (COleVariant& v)
{
    CDeviceData dd;

    dd.m_eDev=m_ad->GetType ();
    dd.m_nAddress=m_ad->GetDeviceID ();
    switch (m_ad->GetType ())   {
        case enInputAnalogico :
            dd.m_fpDati=v.dblVal;
            break;
        case enOutputAnalogico :
            dd.m_fpDati=v.dblVal;
            break;
        case enTimer :
            dd.m_fpDati=v.dblVal;
            break;
        case enCounter :
            dd.m_nType=((CAdamoCounter*)m_ad)->GetCounterType();
            switch (dd.m_nType)   {
                case LUA_TBOOLEAN :
                    dd.m_nDati=v.boolVal;
                    break;
                case LUA_TNUMBER :
                    dd.m_fpDati=v.dblVal;
                    break;
                case LUA_TSTRING :
                    strlimcpy (dd.m_strDati, CString (v.bstrVal), NMAXSTRINGCOUNTER+1);
                    break;
            }
            break;
    }
    return GetMachine ()->SetDevice (1, &dd) == S_OK;
}
