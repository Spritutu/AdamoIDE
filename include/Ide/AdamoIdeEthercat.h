/*
** AdamoIdeEthercat.h : contiene definizioni dell' ide.
*/

#ifndef __ADAMO_IDE_ETHERCAT_H__
#define __ADAMO_IDE_ETHERCAT_H__

#define MAX_ETHERCAT_SLAVE			128
#define MAX_ETHERCAT_SLAVE_TYPE		4
#define MAX_ETHERCAT_ANALOG_DEVICE 	16
#define	DEVICE_STATE_INIT			0x0001
#define	DEVICE_STATE_PREOP			0x0002
#define	DEVICE_STATE_BOOTSTRAP		0x0003
#define	DEVICE_STATE_SAFEOP			0x0004
#define	DEVICE_STATE_OP				0x0008
#define	DEVICE_STATE_ERROR			0x0010
#define	ETHERCAT_PORT				0x80000000

/* visibile anche al firmware */
struct stHWEthVarMask
{
	stHWEthVarMask ()		 { Reset (); }
	void BitSet  (int nBit)	 { int nLocIndex=nBit/64; int nLocBit=nBit%64; m_mask[nLocIndex]|=(__int64)1<<nLocBit; }
	void BitRes  (int nBit)  { int nLocIndex=nBit/64; int nLocBit=nBit%64; m_mask[nLocIndex]&=~((__int64)1<<nLocBit); }
	void Reset   ()			 { m_mask[0]=m_mask[1]=m_mask[2]=m_mask[3]=0; }
	bool BitTest (int nBit)	 { int nLocIndex=nBit/64; int nLocBit=nBit%64; return (m_mask[nLocIndex]&(__int64)1<<nLocBit)!=0; }
	__int64 m_mask[4];
};

#if defined (ADAMO_IDE) || defined (ADAMO_KOS)
/* constants definitions */
#define DIRETHERCAT "Ethercat"
#define ECAT_DEVICE_NAMESIZE	32+1
#define ECAT_VARIABLE_NAMESIZE  128+1
#define ECAT_DEVICE_REVISION	128+1
#define IDETHDEVICEGROUP_IO		0
#define IDETHDEVICEGROUP_SERVO	1
#define IDETHDEVICEGROUP_AXIS	2

/* enumeration types */
enum eEthercatDeviceType   {
	eEth,
	eEthAsse,
	eEthIO,
	eEthIOAnalogic
};

/* data type definitions */
struct stAdamoEthSlaveVariable   {
	char szName[ECAT_VARIABLE_NAMESIZE];
	int  nDataType;
	int  nBitSize;
	int  nBitOffs;
	int  nInputOutput;
};

struct stAdamoEthSlave   {
	stAdamoEthSlave () : nBitStartSend (-1), nBitLenghtSend (-1), nBitStartReceive (-1), nBitLenghtReceive (-1), nRequestedState (1)   {
		szName[0]='\0';
		for (int i=0; i<MAX_ETHERCAT_SLAVE_TYPE; i++) nDeviceType[i]=eEth;
	}
	~stAdamoEthSlave () { ClearVarList (); }
	char szName[ECAT_DEVICE_NAMESIZE];
	int  nPhysAddress;
	int  nVendorID;
	int  nProductCode;
	int  nRevisionNumber;
	int  nSerialNumber;
	char szProductRevision[ECAT_DEVICE_REVISION];
	int  nBitStartSend;
	int  nBitLenghtSend;
	int  nBitStartReceive;
	int  nBitLenghtReceive;
	int  nRequestedState;
	eEthercatDeviceType nDeviceType[MAX_ETHERCAT_SLAVE_TYPE];
	stHWEthVarMask liEnableVariables[MAX_ETHERCAT_SLAVE_TYPE];
	CPtrList VariablesList;
	void ClearVarList ()   {
		for (POSITION pos=VariablesList.GetHeadPosition (); pos; )   {
			stAdamoEthSlaveVariable *p=(stAdamoEthSlaveVariable *)	VariablesList.GetNext (pos);
			delete p;
		}
	}
	void AddTail (stAdamoEthSlaveVariable *p)   {
		VariablesList.AddTail (p);
	}
	void SetDeviceType (eEthercatDeviceType e, int nIndex)   {
		nDeviceType[nIndex]=e;		
	}
	int GetInputVariables ()   {
		int nInp=0;
		for (POSITION pos=VariablesList.GetHeadPosition (); pos; )   {
			stAdamoEthSlaveVariable *p=(stAdamoEthSlaveVariable *)	VariablesList.GetNext (pos);
			if (p->nInputOutput==0)
				nInp++;
		}
		return nInp;
	}
	int GetOutputVariables ()   {
		int nInp=0;
		for (POSITION pos=VariablesList.GetHeadPosition (); pos; )   {
			stAdamoEthSlaveVariable *p=(stAdamoEthSlaveVariable *)	VariablesList.GetNext (pos);
			if (p->nInputOutput==1)
				nInp++;
		}
		return nInp;
	}
	stAdamoEthSlaveVariable *FindInputVariable (CString &str)   {
		for (POSITION pos=VariablesList.GetHeadPosition (); pos; )   {
			stAdamoEthSlaveVariable *p=(stAdamoEthSlaveVariable *)	VariablesList.GetNext (pos);
			if (p->nInputOutput==0)
				if (CString (p->szName)==str)
					return p;
		}
		return NULL;
	}
	stAdamoEthSlaveVariable *FindOutputVariable (CString &str)   {
		for (POSITION pos=VariablesList.GetHeadPosition (); pos; )   {
			stAdamoEthSlaveVariable *p=(stAdamoEthSlaveVariable *)	VariablesList.GetNext (pos);
			if (p->nInputOutput==1)
				if (CString (p->szName)==str)
					return p;
		}
		return NULL;
	}
	stAdamoEthSlaveVariable *FindInputVariable (int nOffset, int nLength)   {
		for (POSITION pos=VariablesList.GetHeadPosition (); pos; )   {
			stAdamoEthSlaveVariable *p=(stAdamoEthSlaveVariable *)	VariablesList.GetNext (pos);
			if (p->nInputOutput==0)
				if (p->nBitOffs==nOffset && p->nBitSize==nLength)
					return p;
		}
		return NULL;
	}
	stAdamoEthSlaveVariable *FindOutputVariable (int nOffset, int nLength)   {
		for (POSITION pos=VariablesList.GetHeadPosition (); pos; )   {
			stAdamoEthSlaveVariable *p=(stAdamoEthSlaveVariable *)	VariablesList.GetNext (pos);
			if (p->nInputOutput==1)
				if (p->nBitOffs==nOffset && p->nBitSize==nLength)
					return p;
		}
		return NULL;
	}
};

#endif
#endif
