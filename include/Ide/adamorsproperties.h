#if !defined(AFX_ADAMORSPROPERTIES_H__BC8F6323_195A_4B5E_9ED4_EBAD388627A4__INCLUDED_)
#define AFX_ADAMORSPROPERTIES_H__BC8F6323_195A_4B5E_9ED4_EBAD388627A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// adamorsproperties.h : header file
//

#include "AdamoRSDiagramObjects.h"
#include "charcheck.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSProperties view

/* constants definitions */
#define ID_RS_NAME          100
#define ID_RS_ID            101
#define ID_RS_TABINDEX      102
#define ID_RS_ZORDER        103
#define ID_RS_TOP           104
#define ID_RS_LEFT          105
#define ID_RS_BOTTOM        106
#define ID_RS_RIGHT         107
#define ID_RS_BUTTON_VALUE  108
#define ID_RS_ALIGNMENT     109
#define ID_RS_HASABORDER    110
#define ID_RS_BORDER_STYLE  111
#define ID_RS_FONT          112
#define ID_RS_STYLE_FORE_COLOR 113
#define ID_RS_STYLE_BACK_COLOR 114
#define ID_RS_BORDER_COLOR  115
#define ID_RS_BORDER_SIZE   116
#define ID_RS_PADDING_TOP   117
#define ID_RS_PADDING_LEFT  118
#define ID_RS_PADDING_RIGHT 119
#define ID_RS_PADDING_BOTTOM 120
#define ID_RS_EDIT_VALUE    121
#define ID_RS_EDIT_SIZE     122
#define ID_RS_EDIT_PSW      123
#define ID_RS_VAR_NAME      124
#define ID_RS_VAR_TYPE      125
#define ID_RS_VAR_ROW       126
#define ID_RS_VAR_COLUMN    127
#define ID_RS_RADIO_GROUP   128
#define ID_RS_RADIO_VALUE   129
#define ID_RS_RADIO_CHECKED 130
#define ID_RS_RADIO_UPDATE	131
#define ID_RS_CHECKBOX_VALUE    131
#define ID_RS_CHECKBOX_CHECKED  132
#define ID_RS_GROUPBOX_VALUE    133
#define ID_RS_LABEL_VALUE       134
#define ID_RS_LIST_OPTIONS      135
#define ID_RS_LIST_MULTISEL     136
#define ID_RS_LIST_SIZE         137
#define ID_RS_COMBO_OPTIONS     138
#define ID_RS_IMAGE             139
#define ID_RS_IMAGE_STRETCH     140
#define ID_RS_DB_TABLE          141
#define ID_RS_DB_FIELDS         142
#define ID_RS_DB_INDEX          143
#define ID_RS_DB_INDEXVALUE     144
#define ID_RS_FORM_STORE_LOCATION 145
#define ID_RS_FORM_STORE_FILE   146
#define ID_RS_FORM_STORE_MODE   147
#define ID_RS_FORM_OPEN_FILE    148
#define ID_RS_GRID_DATA         149
#define ID_RS_MATRIX_NAME       150
#define ID_RS_MATRIX_DB         151
#define ID_RS_FORM_ENABLE_BCKGR 152
#define ID_RS_FORM_IMAGE        153
#define ID_RS_FORM_MAIN_SYN     154
#define ID_RS_BUTTON_IMAGE      155
#define ID_RS_BUTTON_TYPE       156
#define ID_RS_BUTTON_ACTION_DOWNLOAD	157
#define ID_RS_BUTTON_ACTION_LOAD		158
#define ID_RS_BUTTON_ACTION_SAVE		159
#define ID_RS_BUTTON_ACTION_CHANGEPAGE	160
#define ID_RS_BUTTON_NEXT_PAGE			161
#define ID_RS_CHECKBOX_UPDATE	162
#define ID_RS_EDIT_UPDATE		163
#define ID_RS_EDIT_TYPE			164
#define ID_RS_AXIS_VALUE		165
#define ID_RS_AXIS_NAME			166
#define ID_RS_DEVICE_NAME		167
#define ID_RS_DEVICE_DATA		168
#define ID_RS_AG_TRANSPARENT    171
#define ID_RS_AG_POSMIN         172
#define ID_RS_AG_POSMAX         173
#define ID_RS_AG_SHOW_INNER_ARC 174
#define ID_RS_AG_SHOW_OUTER_ARC 175
#define ID_RS_AG_REVERSE_SCALE  176
#define ID_RS_AG_BORDER_STYLE   177
#define ID_RS_AG_BACK_COLOR     178
#define ID_RS_AG_POINTER_STYLE  179
#define ID_RS_AG_POINTER_SIZE   180
#define ID_RS_AG_POINTER_MARGIN 181
#define ID_RS_AG_POINTER_COLOR  182
#define ID_RS_AG_HUB_SHOW	    183
#define ID_RS_AG_HUB_SHOW_3D	184
#define ID_RS_AG_HUB_SIZE   	185
#define ID_RS_AG_HUB_COLOR      186
#define ID_RS_AG_ARC_RADIUS	    187
#define ID_RS_AG_ARC_START	    188
#define ID_RS_AG_ARC_RANGE	    189
#define ID_RS_AG_FACE_SHOW	    190
#define ID_RS_AG_FACE_STYLE	    191
#define ID_RS_AG_BEVEL_STYLE	192
#define ID_RS_AG_BEVEL_SIZE 	193
#define ID_RS_AG_FACE_COLOR 	194
#define ID_RS_AG_TICK_MARGIN 	195
#define ID_RS_AG_TICK_LABEL_MARGIN 196
#define ID_RS_AG_TMJ_SHOW	    197
#define ID_RS_AG_TMJ_NUMBER	    198
#define ID_RS_AG_TMJ_LENGHT	    199
#define ID_RS_AG_TMJ_COLOR	    200
#define ID_RS_AG_TMM_SHOW	    201
#define ID_RS_AG_TMM_NUMBER	    202
#define ID_RS_AG_TMM_LENGHT	    203
#define ID_RS_AG_TMM_COLOR	    204
#define ID_RS_AG_SEC_NUMBER	    205
#define ID_RS_AG_SEC1_COLOR	    206
#define ID_RS_AG_SEC2_COLOR	    207
#define ID_RS_AG_SEC3_COLOR	    208
#define ID_RS_AG_SEC4_COLOR	    209
#define ID_RS_AG_SEC5_COLOR	    210
#define ID_RS_AG_SEC5_LIMIT1    211
#define ID_RS_AG_SEC5_LIMIT2    212
#define ID_RS_AG_SEC5_LIMIT3    213
#define ID_RS_AG_SEC5_LIMIT4    214
#define ID_RS_LD_STYLE			215
#define ID_RS_LD_TRANSPARENT	216
#define ID_RS_LD_SHOWREFLECTION 217
#define ID_RS_LD_ACTIVE_COLOR   218
#define ID_RS_LD_BEVEL_STYLE    219
#define ID_RS_LD_HORZ_SPACING   220
#define ID_RS_LD_VERT_SPACING   221
#define ID_RS_LD_ROW_COUNT      222
#define ID_RS_LD_COL_COUNT      223
#define ID_RS_LD_BORDER_STYLE   224
#define ID_RS_LD_BACK_COLOR     225
#define ID_RS_LG_TRANSPARENT	226
#define ID_RS_LG_POSMIN			227
#define ID_RS_LG_POSMAX			228
#define ID_RS_LG_REVERSE_SCALE	229
#define ID_RS_LG_ORIENTATION	230
#define ID_RS_LG_ORIENTATION_TICKS	231
#define ID_RS_LG_STYLE			232
#define ID_RS_LG_BACK_COLOR		233
#define ID_RS_LG_POINTER_STYLE	234
#define ID_RS_LG_POINTER_SIZE	235
#define ID_RS_LG_POINTER_MARGIN 236
#define ID_RS_LG_POINTER_COLOR  237
#define ID_RS_LG_POINTER_3D     238
#define ID_RS_LG_POINTER_SCALESIDE 239
#define ID_RS_LG_TA_SHOW		240
#define ID_RS_LG_TA_MARGIN		241
#define ID_RS_LG_LBL_SHOW		242
#define ID_RS_LG_LBL_MARGIN		243
#define ID_RS_LG_TMJ_SHOW	    244
#define ID_RS_LG_TMJ_NUMBER	    245
#define ID_RS_LG_TMJ_LENGHT	    246
#define ID_RS_LG_TMJ_COLOR	    247
#define ID_RS_LG_TMM_SHOW	    248
#define ID_RS_LG_TMM_NUMBER	    249
#define ID_RS_LG_TMM_LENGHT	    250
#define ID_RS_LG_TMM_COLOR	    251
#define ID_RS_LG_SEC_NUMBER	    252
#define ID_RS_LG_SEC1_COLOR	    253
#define ID_RS_LG_SEC2_COLOR	    254
#define ID_RS_LG_SEC3_COLOR	    255
#define ID_RS_LG_SEC4_COLOR	    256
#define ID_RS_LG_SEC5_COLOR	    257
#define ID_RS_LG_SEC5_LIMIT1    258
#define ID_RS_LG_SEC5_LIMIT2    259
#define ID_RS_LG_SEC5_LIMIT3    260
#define ID_RS_LG_SEC5_LIMIT4    261
#define ID_RS_DG_TRANSPARENT	262
#define ID_RS_DG_OFFSEGMENTS	263
#define ID_RS_DG_SIGN			264
#define ID_RS_DG_AUTOOFFSEGCOL  265
#define ID_RS_DG_PRECISION		266
#define ID_RS_DG_BORDERSTYLE	267
#define ID_RS_DG_LEADINGSTYLE	268
#define ID_RS_DG_BACK_COLOR		269
#define ID_RS_DD_COUNT			270
#define ID_RS_DD_SPACING		271
#define ID_RS_DD_SEGSIZE		272
#define ID_RS_DD_SEGSEPARATION	273
#define	ID_RS_DD_SEGMARGIN		274
#define	ID_RS_DD_SEGON_COLOR	275
#define	ID_RS_DD_SEGOFF_COLOR	276
#define	ID_RS_LBLG_VALUE		277
#define	ID_RS_LBLG_TRANSPARENT	278
#define	ID_RS_LBLG_AUTOSIZE		279
#define	ID_RS_LBLG_BORDERSTYLE	280
#define	ID_RS_LBLG_ALIGNMENT	281
#define	ID_RS_LBLG_SHADOW		282
#define	ID_RS_LBLG_OFFX			283
#define	ID_RS_LBLG_OFFY 		284
#define	ID_RS_LBLG_FORECOLOR 	285
#define	ID_RS_LBLG_BACKCOLOR 	286
#define	ID_RS_LBLG_SHADOWCOLOR	287
#define	ID_RS_BUTTON_TRANSPARENCY	        288
#define	ID_RS_IMAGE_TRANSPARENCY            289
#define ID_RS_IMAGE_TRANSPARENT_COLOR       290
#define ID_RS_PICTURE_STRETCH               291
#define ID_RS_PICTURE_TRANSPARENCY          292
#define ID_RS_PICTURE_TRANSPARENT_COLOR     293
#define ID_RS_PICTURE_1         294
#define ID_RS_PICTURE_2         295
#define ID_RS_PICTURE_3         296
#define ID_RS_PICTURE_4         297
#define ID_RS_PICTURE_5         298
#define ID_RS_PICTURE_6         299
#define ID_RS_PICTURE_7         300
#define ID_RS_PICTURE_8         301

class CAdamoRSProperties : public CPaneView
{
protected:
	CAdamoRSProperties();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoRSProperties)

// Attributes
public:

// Operations
public:
    void Select (CAdamoRSBase* pObj);
    void Clear ();
    void ClearRSObject (CAdamoRSBase* pObj);
    void UpdateSelected ();
    void OnEditListOptions ();
    void OnEditComboOptions ();
	void OnEditGrid ();
	void OnEditMatrix (CXTPPropertyGridInplaceButton* pButton);
	void OnEditDevice (CXTPPropertyGridInplaceButton* pButton);
	void RenameRSForm (CString strFormName);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoRSProperties)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoRSProperties();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoRSProperties)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
private :
  	CXTPPropertyGrid m_wndRSPropGrid;
    CAdamoRSBase*    m_pRSObject;
    CCharCheck       m_objCheckKeys;
    CCharCheck       m_objCheckIdentifier;
    CCharCheck       m_objCheckCaption;
    CCharCheck       m_objCheckNumbers;
    CCharCheck       m_objCheckHex;
private :
    void CreateGrid ();
    void SelectRSObject ();
    CXTPPropertyGridItem *AddGridItem (int iId,CString strCaption, CString strValue, CXTPPropertyGridItem *pItemNode);
    CXTPPropertyGridItem *AddGridItemCheckBox  (int iId, CString strCaption, BOOL bValue, CXTPPropertyGridItem *pItemNode);
    CXTPPropertyGridItem *AddGridComboVarType  (int nType, CXTPPropertyGridItem *pItemNode);
    CXTPPropertyGridItem *AddSpinBorders (int nID, CString strText, int nValue, CXTPPropertyGridItem *pItemNode);
    CXTPPropertyGridItem *AddGridComboImageID  (CString strID, int nID, CXTPPropertyGridItem* pGridItem, CString strCaption);
	CXTPPropertyGridItem* AddGridComboButtonType (enum eRSButtonType e, int nID, CXTPPropertyGridItem* pGridItem);
    CXTPPropertyGridItem *AddGridComboDBTable  (CString strTable, CXTPPropertyGridItem* pGridItem);
    CXTPPropertyGridItem *AddGridComboDBFields (CString strTable, CString strField, CXTPPropertyGridItem* pGridItem);
    CXTPPropertyGridItem *AddGridComboDBIndex  (CString strTable, CString strIndex, CXTPPropertyGridItem* pGridItem);
	CXTPPropertyGridItem *AddGridComboNextPage (CString strPage, int nID, CXTPPropertyGridItem* pGridItem);
    CXTPPropertyGridItem *AddGridItemButtonText (int iId, CString strCaption, bool bOpen, CString strValue, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboEditType (eEditType e, int nID, CXTPPropertyGridItem* pGridItem);
	CXTPPropertyGridItem *AddGridComboAsse (CString strAsse, CXTPPropertyGridItem* pGridItem);
    void SelectRSForm ();
    void SelectRSButton ();
    void SelectRSInputText ();
    void SelectRSStyle ();
    void SelectRSStylePosition ();
    void SelectRSStyleFont ();
    void SelectRSVars ();
	void SelectRSMatrix ();
    void SelectRSRadioButton ();
    void SelectRSCheckBox ();
    void SelectRSLabel ();
    void SelectRSGroup ();
    void SelectRSList ();
    void SelectRSCombo ();
    void SelectRSImage ();
	void SelectRSGrid ();
    void SelectRSDB ();
	void SelectRSDBMatrix ();
	void SelectRSAx ();
	void SelectRSLed ();
	void SelectRSIAngularGauge ();
	void SelectRSILinearGauge ();
	void SelectRSIDisplay ();
	void SelectRSILabel ();
    void SelectRSIPicture ();
    void InitCharCheck ();
    void StringControl (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength);
    void AssignRSObject (CXTPPropertyGridItem *pItem);
    void AssignRSButton (CXTPPropertyGridItem *pItem);
    void AssignRSStyle (CXTPPropertyGridItem *pItem);
    void AssignRSInputText (CXTPPropertyGridItem *pItem);
    void AssignRSRadioButton (CXTPPropertyGridItem *pItem);
    void AssignRSCheckBox (CXTPPropertyGridItem *pItem);
    void AssignRSForm (CXTPPropertyGridItem *pItem);
    bool AssignRSVar (CXTPPropertyGridItem *pItem);
	bool AssignRSMatrix (CXTPPropertyGridItem *pItem);
	bool AssignRSDBMatrix (CXTPPropertyGridItem *pItem);
    bool AssignRSDB (CXTPPropertyGridItem *pItem);
    void AssignRSGroup (CXTPPropertyGridItem *pItem);
    void AssignRSLabel (CXTPPropertyGridItem *pItem);
    void AssignRSList (CXTPPropertyGridItem *pItem);
    void AssignRSCombo (CXTPPropertyGridItem *pItem);
    void AssignRSImage (CXTPPropertyGridItem *pItem);
    void AssignRSGrid (CXTPPropertyGridItem *pItem);
	void AssignRSAx (CXTPPropertyGridItem *pItem);
	void AssignRSLed (CXTPPropertyGridItem *pItem);
	void AssignRSIAngularGauge (CXTPPropertyGridItem *pItem);
	void AssignRSILinearGauge (CXTPPropertyGridItem *pItem);
	void AssignRSIDisplay (CXTPPropertyGridItem *pItem);
	void AssignRSILabel (CXTPPropertyGridItem *pItem);
    void AssignRSIPicture (CXTPPropertyGridItem *pItem);
    void UpdateDiagramEditor ();
    void UpdateDiagramSize ();
    void UpdateDiagramValue ();
    void UpdateDiagramSelected ();
	void UpdateActiveXPreviewSize (CRect &rc);
    void CheckInput (int nID, CXTPPropertyGridInplaceEdit* pEdit);
    void CheckChars (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength);
    bool CheckRange (int nValue, int nRangeMin, int nRangeMax);
    void ReloadField (CXTPPropertyGridItem *pItem);
    void AssignRange (CXTPPropertyGridItem* pItem, int* nVar, int nRangeMin, int nRangeMax);
    void PopulateFieldCombo (CString strTable);
    void PopulateIndexCombo (CString strTable);
    void PopulateIndexValues ();
    void RemoveVariableValue (stRSVariabili& v);
	bool IsValidGrid (int nRows, int nCols);
	void ManageGridData (int nRows, int nCols, stGridColumnData *pgcd, CAdamoRSGrid *pGrid);
	bool CheckExistMainSyn ();
	bool FindDevice (CString strPath, int *nID);
	void SelectActiveXPreview ();
	void UpdateActiveXPreview ();
	virtual void RefreshMetrics(AppTheme nTheme);
	CXTPPropertyGridItem *AddGridComboLedType (eLedType eLT, CXTPPropertyGridItem *pItemNode);
	void SelectDevice (CXTPPropertyGridItem* pGridItem);
	CXTPPropertyGridItem *AddComboDeviceData (int nDeviceData, CXTPPropertyGridItem *pItemNode); 
	CXTPPropertyGridItem *AddGridComboBorderStyle (int nID, CString strCaption, eBorderStyle e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboPointerStyle (int nID, CString strCaption, ePointerStyle e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboStyle (int nID, CString strCaption, eStyle e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridColor (int nID, CString strCaption, COLORREF rgb, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboLedStyle (int nID, CString strCaption, eLedType e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboLedBevelStyle (int nID, CString strCaption, eLedStyle e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboOrientation (int nID, CString strCaption, eOrientation e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboOrientationTicks (int nID, CString strCaption, eOrientationTicks e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboLGStyle (int nID, CString strCaption, eLinearGaugeStyle e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboLGPointerStyle (int nID, CString strCaption, eLinearGaugePointerStyle e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboDisplayStyle (int nID, CString strCaption, eDisplayStyle e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboDisplayLeadingStyle (int nID, CString strCaption, eDisplayLeadingStyle e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboLabelStyle (int nID, CString strCaption, eLabelStyle e, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboLabelAlignment (int nID, CString strCaption, eLabelAlignment e, CXTPPropertyGridItem *pItemNode);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMORSPROPERTIES_H__BC8F6323_195A_4B5E_9ED4_EBAD388627A4__INCLUDED_)
