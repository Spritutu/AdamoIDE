// ctkMisc.h: interface for the ctkMisc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTKMISC_H__02DB3E9E_00AB_4FC7_9F58_E01EECC31A91__INCLUDED_)
#define AFX_CTKMISC_H__02DB3E9E_00AB_4FC7_9F58_E01EECC31A91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tkCommon.h"

class ctkMisc  
{
// Purely static helper class
public:
	static bool			fStripOffBraces(std::tstring& strTarget)
		{
		int		nLen = strTarget.length();
		
		if(nLen<2)
			return false;

		if(nLen==2)
			return false;

		if(strTarget[0]!='{' || strTarget[nLen-1]!='}')
			return false;

		strTarget	=strTarget.substr(1,strTarget.length()-2);
		return true;
		}

	static std::tstring	strEscapeBraces(const std::tstring& strSource)
		{
		std::tstring	strDest;
		int				nLen = strSource.length();
		int				i;
		TCHAR			tcBefore,tcTemp;
		bool			fSkip;

		if(nLen==0)
			return strSource;

		tcBefore	=_T('\0');
		for(i=0;i<nLen;i++)
			{
			fSkip		=false;
			tcTemp		=strSource[i];

			if(tcBefore!=_T('\\') && tcTemp==_T('{'))
				strDest+=_T("{{"), fSkip=true;

			if(tcBefore!=_T('\\') && tcTemp==_T('}'))
				strDest+=_T("}}"), fSkip=true;

			if(!fSkip)
				strDest+=tcTemp;

			tcBefore	=tcTemp;
			}

		return strDest;
		}

	static std::tstring	strUnescapeBraces(const std::tstring& strSource)
		{
		std::tstring	strDest;
		int			nLen = strSource.length();
		int			i;
		TCHAR		tcTemp;
		bool		fSkip;

		if(nLen==0)
			return strSource;

		for(i=0;i<nLen-1;i++)
			{
			fSkip		=false;
			tcTemp		=strSource[i];
			if(tcTemp=='\\' && strSource[i+1]=='{')
				{
				strDest	+=_T("{");
				fSkip=true;
				i++;
				}
			if(tcTemp=='\\' && strSource[i+1]=='}')
				{
				strDest	+=_T("}");
				fSkip=true;
				i++;
				}
			if(!fSkip)
				strDest	+=tcTemp;
			}

		if(i<nLen)
			strDest+=strSource[i];

		return strDest;
		}

    static std::tstring strParseEscapes(const std::tstring& strSource, bool* pfEscapeError)
        {
        (*pfEscapeError)=false;

        enum    {
            esc_not=-1,
            esc_start=0,
            esc_hex_first=1,
            esc_hex_second=2
            };

        std::tstring    strDest;
        std::tstring    strTemp;

        int         i, nLen = strSource.length();
        int         nEscape = esc_not;
        TCHAR       tcTemp;

        if(nLen==0)
            return strSource;

        for(i=0;i<nLen;i++)
            {
            tcTemp  =strSource[i];

            if(nEscape == esc_not)
                {
                if(tcTemp=='\\')
                    nEscape = esc_start;
                else
                    strDest+=tcTemp;

                continue;
                }
            if(nEscape == esc_start)
                {
                if(tcTemp=='x')
                    {
                    strTemp=_T("");
                    nEscape=esc_hex_first;
                    continue;
                    }
                if(tcTemp=='n')
                    {
                    nEscape=esc_not;
                    strDest+='\n';
                    continue;
                    }
                if(tcTemp=='r')
                    {
                    nEscape=esc_not;
                    strDest+='\r';
                    continue;
                    }

                (*pfEscapeError)=true;
                nEscape=esc_not;
                continue;
                }
            if(nEscape == esc_hex_first)
                {
                nEscape=esc_hex_second;
                strTemp+=tcTemp;
                continue;
                }
            if(nEscape == esc_hex_second)
                {
                nEscape=esc_not;
                strTemp+=tcTemp;
                strDest+=TCHAR(_tcstoul(strTemp.data(),NULL,16));
                continue;
                }

            // May not happen
            DebugBreak();
            }

        if(nEscape != esc_not)
            (*pfEscapeError) = true;

        return strDest;
        }
};

#endif // !defined(AFX_CTKMISC_H__02DB3E9E_00AB_4FC7_9F58_E01EECC31A91__INCLUDED_)
