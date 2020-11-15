/*
 * DeviceVarable.h
 */

#ifndef __ADAMO_DEVICEVARIABLE__
#define __ADAMO_DEVICEVARIABLE__

/* include files */
#include "GlobalVariable.h"

/* class definition */
class CDeviceVariable : public CGlobalVariable
{
    public :
        CDeviceVariable ();
        void SetDevice (CAdamoBase* ad)   { m_ad = ad; }
    protected :
        bool ReadData (COleVariant& v);
        bool WriteData (COleVariant& v);
    private :
        CAdamoBase* m_ad;
};

#endif