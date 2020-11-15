// ctkFlagsMixin.h: interface for the ctkFlagsMixin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTKFLAGSMIXIN_H__29B1A8C1_F9B4_4725_BE18_31BFFBD0F658__INCLUDED_)
#define AFX_CTKFLAGSMIXIN_H__29B1A8C1_F9B4_4725_BE18_31BFFBD0F658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class T>
class ctkFlagsMixin
{
// Construction/Destruction
public:
	ctkFlagsMixin()
		{
		m_flags=(T)0;
		}
	ctkFlagsMixin(const ctkFlagsMixin<T>& other)
		{
		m_flags	=other.m_flags;
		}
	ctkFlagsMixin(T mask)
		{
		m_flags	=mask;
		}
	//virtual ~ctkFlagsMixin();

// Attributes
protected:
	unsigned long	m_flags;

// Operations
public:
	bool	fIsFlagSet(T mask) const
		{
		if( ((T)(m_flags&mask))==mask )
			return true;
		else
			return false;
		}

	void	vSetFlag(T mask, bool fSet = true)
		{
		if(!fSet)
			m_flags	&=~mask;
		else
			m_flags	|=mask;
		}

	void	vResetFlag(T mask)
		{
		return vSetFlag(mask,false);
		}

	void	vClearAllFlags()
		{
		m_flags	=(T)0;
		}

	bool	operator==(T mask) const
		{
		return m_flags==mask;
		}

	ctkFlagsMixin&	operator|=(T mask)
		{
		vSetFlag(mask);
		return *this;
		}
};

#endif // !defined(AFX_CTKFLAGSMIXIN_H__29B1A8C1_F9B4_4725_BE18_31BFFBD0F658__INCLUDED_)
