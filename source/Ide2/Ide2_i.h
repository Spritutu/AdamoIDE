

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for Ide2.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Ide2_i_h__
#define __Ide2_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICAdamoSink_FWD_DEFINED__
#define __ICAdamoSink_FWD_DEFINED__
typedef interface ICAdamoSink ICAdamoSink;

#endif 	/* __ICAdamoSink_FWD_DEFINED__ */


#ifndef __CAdamoSink_FWD_DEFINED__
#define __CAdamoSink_FWD_DEFINED__

#ifdef __cplusplus
typedef class CAdamoSink CAdamoSink;
#else
typedef struct CAdamoSink CAdamoSink;
#endif /* __cplusplus */

#endif 	/* __CAdamoSink_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICAdamoSink_INTERFACE_DEFINED__
#define __ICAdamoSink_INTERFACE_DEFINED__

/* interface ICAdamoSink */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICAdamoSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2131DAEB-CA2D-4B50-AF37-DA933B09F854")
    ICAdamoSink : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ICAdamoSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICAdamoSink * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICAdamoSink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICAdamoSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICAdamoSink * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICAdamoSink * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICAdamoSink * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICAdamoSink * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } ICAdamoSinkVtbl;

    interface ICAdamoSink
    {
        CONST_VTBL struct ICAdamoSinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICAdamoSink_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICAdamoSink_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICAdamoSink_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICAdamoSink_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICAdamoSink_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICAdamoSink_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICAdamoSink_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICAdamoSink_INTERFACE_DEFINED__ */



#ifndef __Ide2Lib_LIBRARY_DEFINED__
#define __Ide2Lib_LIBRARY_DEFINED__

/* library Ide2Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Ide2Lib;

EXTERN_C const CLSID CLSID_CAdamoSink;

#ifdef __cplusplus

class DECLSPEC_UUID("B0B29B8E-9B89-4FA5-85BF-CF4A703572DC")
CAdamoSink;
#endif
#endif /* __Ide2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


