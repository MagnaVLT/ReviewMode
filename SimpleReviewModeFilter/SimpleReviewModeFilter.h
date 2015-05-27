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
#ifndef _HIL_SimpleReviewModeFilter_H_
#define _HIL_SimpleReviewModeFilter_H_

#define OID_ADTF_HIL_SimpleReviewModeFilter "adtf.hil.SimpleReviewModeFilter"

#include "ui_GUISimpleReview.h"
#include "Utils/Utilities.h"


class SimpleReviewModeFilter : public QObject, public cBaseQtFilter
{
	Q_OBJECT
	ADTF_DECLARE_FILTER_VERSION(OID_ADTF_HIL_SimpleReviewModeFilter, 
		"SimpleReviewModeFilter", 
		OBJCAT_Generic, 
		"Magna_Electrocnis_Specified", 
		MAGNA_VERSION_ID, 
		MAGNA_VERSION_MAIN, 
		MAGNA_VERSION_MINOR, 
		"SimpleReviewModeFilter")

protected:


private:


public:
    /** 
		*	Class constructor for databaseFilter
		*	@param string for info.
		*
	*/
	QWidget*	m_pFilterWidget;
	Ui_Form		m_oFilterGUI;			// defined in ui_GUIFrontEnd.h

	SimpleReviewModeFilter(const tChar* __info);
	/** 
		*	Class destructor for databaseFilter 
	*/
    virtual ~SimpleReviewModeFilter();

	tHandle	CreateView();
    tResult	ReleaseView();

protected:
	/** 
		* The method is inherited from base class, which initializes all the input pin and output pin.
		* @param [in] eStage The current state of the state machine during initialization.
		* @param [in,out] __exception   An Exception pointer where exceptions will be put when failed.
		* @return   Returns a standard result code.
	*/
    tResult Init(tInitStage eStage, __exception);
	/**
		* The function is called automatically by ADTF. It contains the start routine.
		* The method is inherited from base class.
		* @param [in,out] __exception   An Exception pointer where exceptions will be put when failed.
		* @return   Returns a standard result code.
	*/
	tResult Start(__exception = NULL);
	/**
		* The function is called automatically by ADTF. It contains the stop routine.
		* The method is inherited from base class.
		* @param [in,out] __exception   An Exception pointer where exceptions will be put when failed.
		* @return   Returns a standard result code.
	*/
	tResult Stop(__exception = NULL);
	/**
		* The function is called automatically by ADTF. It contains the shutdown routine.
		* The method is inherited from base class.
		* @param  [in] eStage The current state of the state machine during shutdown
		* @param [in,out] __exception   An Exception pointer where exceptions will be put when failed.
		* @return   Returns a standard result code.
	*/
    tResult Shutdown(tInitStage eStage, __exception);

    /**
		* The function is called automatically by ADTF. It is called if media sample is received.
		* The method is inherited from base class, overwrites cBaseQtFilter and implements IPinEventSink.
		* @param [in] pSource Pointer to the sending pin's IPin interface.
		* @param [in] nEventCode Event code. For allowed values see @ref IPinEventSink::tPinEventCode
		* @param [in] nParam1 Optional integer parameter.
		* @param [in] nParam2 Optional integer parameter.
		* @param [in] pMediaSample Address of an IMediaSample interface pointers.
		* @return   Returns a standard result code.
	*/
    tResult OnPinEvent(IPin* pSource, tInt nEventCode, tInt nParam1, tInt nParam2, IMediaSample* pMediaSample);

	/**
		* The function is called to set the component in running state. 
		* @param [in] nActivationCode The activation type for running.
		* @param [in] pvUserData pointer to a activation structure depending on the activation type. 
		* @param [in] szUserDatasize Size of the activation structure. (in byte)
		* @param [inout] __exception_ptr Address of variable that points to an IException interface. If not using the cException smart pointer, the interface has to be released by calling Unref()...
		* @return   Returns a standard result code.

	*/
	tResult	Run(tInt nActivationCode, const tVoid* pvUserData, tInt szUserDataSize, ucom::IException** __exception_ptr=NULL);
	
private:
	void registerEventHandler();
	void initGUI();
	bool changeDatFile(string dat);
	void saveCurrentStatus();
	void restoreCurrentStatus();
	void setFileList(QString fileName);
	int getCurrentIdx(string current_clip);

	vector<string> fileList;

public slots:
	tResult on_btn_clip_clicked();
	tResult on_btn_next_clicked();
	tResult on_btn_prev_clicked();
	tResult on_btn_showclip_clicked();

};

#endif