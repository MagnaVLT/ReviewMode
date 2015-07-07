/*****************************************************************************
 *     ADTF Template Project Filter (header)
 *****************************************************************************
 *  
 *	@file
 *	Copyright &copy; Magna Electronics in Auburn Hills. All rights reserved
 *	
 *	$Author: Sangsig Kim (E37039) $ 
 *	$MAGNA Electronics$
 *	$Date: 2014/08/10 01:59:28EDT $
 *	$Revision: 0 $
 *
 *	@remarks
 *
 ******************************************************************************/
#ifndef _HIL_AnnotationGUIFilter_H_
#define _HIL_AnnotationGUIFilter_H_

#define OID_ADTF_HIL_CanMappingFilter "adtf.hil.CanMappingFilter"

#include "ui_GUIAnnotation.h"


class CanMappingFilter : public QObject, public cBaseQtFilter
{
	Q_OBJECT
	ADTF_DECLARE_FILTER_VERSION(OID_ADTF_HIL_CanMappingFilter, 
		"CanMappingFilter", 
		OBJCAT_Generic, 
		"Magna_Electrocnis_Specified", 
		MAGNA_VERSION_ID, 
		MAGNA_VERSION_MAIN, 
		MAGNA_VERSION_MINOR, 
		"CanMappingFilter")

protected:


private:


public:
    /** 
		*	Class constructor for databaseFilter
		*	@param string for info.
		*
	*/
	QWidget*				m_pFilterWidget;
	Ui_Form		m_oFilterGUI;			// defined in ui_GUIFrontEnd.h

	CanMappingFilter(const tChar* __info);
	/** 
		*	Class destructor for databaseFilter 
	*/
    virtual ~CanMappingFilter();

	tHandle	CreateView();
    tResult	ReleaseView();

protected:
    tResult Init(tInitStage eStage, __exception);
	tResult Start(__exception = NULL);
	tResult Stop(__exception = NULL);
    tResult Shutdown(tInitStage eStage, __exception);
    tResult OnPinEvent(IPin* pSource, tInt nEventCode, tInt nParam1, tInt nParam2, IMediaSample* pMediaSample);
	tResult	Run(tInt nActivationCode, const tVoid* pvUserData, tInt szUserDataSize, ucom::IException** __exception_ptr=NULL);
	
private:

	void Init();
	void registerEventHandler();
	bool change_dbc_files(string txt_vehicle, string txt_fusion, string txt_eyeq);

public slots:
	tResult on_btn_browse_vehicle_clicked();


	tResult on_btn_browse_fusion_clicked();
	tResult on_btn_browse_eyeq_clicked();
	tResult on_btn_apply_clicked();
	
};

#endif