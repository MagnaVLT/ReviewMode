/*****************************************************************************
 *     ADTF Template Project Filter (header)
 *****************************************************************************
 *  
 *	@file
 *	Copyright &copy; Magna Electronics in Auburn Hills. All rights reserved
 *	
 *	$Author: Junha Lee (E39050) $ 
 *	$MAGNA Electronics$
 *	$Date: 2015/04/09 01:59:28EDT $
 *	$Revision: 0 $
 *
 *	@remarks
 *
 ******************************************************************************/
#ifndef _HIL_ReviewModeFilter_H_
#define _HIL_ReviewModeFilter_H_

#define OID_ADTF_HIL_ReviewModeFilter "adtf.hil.ReviewModeFilter"

#include "ui_GUIFrontEnd.h"
#include "UI/ListHandle.h"
#include "QueryFactory.h"
#include "SQLFileHandler.h"
#include "ComboHandle.h"
#include <string>
#include <vector>
#include <sstream>


static string CLIP_HOME;
class ReviewModeFilter : public QObject, public cBaseQtFilter
{
	Q_OBJECT
	ADTF_DECLARE_FILTER_VERSION(OID_ADTF_HIL_ReviewModeFilter, 
		"ReviewModeFilter", 
		OBJCAT_Generic, 
		"Magna_Electrocnis_Specified", 
		MAGNA_VERSION_ID, 
		MAGNA_VERSION_MAIN, 
		MAGNA_VERSION_MINOR, 
		"ReviewModeFilter")

protected:


private:
	tBool				m_bEvtRecTrigger;


public:
	static string ID;
	static string USERLOG_ID;
	static bool isTemporalClose;
    /** 
		*	Class constructor for databaseFilter
		*	@param string for info.
		*
	*/
	QWidget*			m_pFilterWidget;
	QDialog*			m_pDialog;
	Ui_Form				m_oFilterGUI;			// defined in ui_GUIFrontEnd.h

	ReviewModeFilter(const tChar* __info);
	/** 
		*	Class destructor for databaseFilter 
	*/
    virtual ~ReviewModeFilter();
	
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
	


	cOutputPin		m_out_reviewed_data;	// fcm event data to GUI filter
	cInputPin		m_sample_data;	// fcm event data to GUI filter


private:

	//tResult out_reviewed_data(IPin* pSource, IMediaSample* pMediaSample);
	tResult send_data(cOutputPin *outpin, sEvent_Data reviewed_data);

	cMutex	m_oEventProcessMutex;
	ListHandle* listhandle;
	ComboHandle* combo_handle;
	QStandardItemModel* featureModel;
	QStandardItemModel* selectedFeatureModel;
	QStandardItemModel* eventModel;
	QStandardItemModel* selectedEventModel;
	QStandardItemModel* annotationModel;
	QStandardItemModel* selectedAnnotationModel;
	QStandardItemModel* eventListModel;

	QueryFactory* queryFactory;
	SQLFileHandler* sqlFileHandler;
	void initAll();
	vector<string> getListField4Feature();
	vector<string> getListField4Event();
	vector<string> getListField4EventList();
	vector<string> getListField4Annotation();
	vector<string> getField4EventEdit();
	vector<string> getField4Status();
	void initMode();
	void initModel();
	void initFeatureModel();
	void initEventListModel();
	void registerEventHandler();
	void initGUI();
	void initVINCombo(QComboBox* combobox);
	void initEventCategoryCombo(QComboBox* combobox);
	void initEventCombo(QComboBox* combobox, bool eyeq);
	void initFeatureCombo(QComboBox* combobox);
	void initAnnotationCombo(QComboBox* combobox);
	void initAnnotationCombo(QComboBox* combobox, vector<string> vstr_fetures);
	void initStatusCombo(QComboBox* combobox);
	void initCombo(QComboBox* combobox, vector<string> data);
	string getDate(QDateEdit* edit, string comp);
	void initGUI(std::vector<std::string> features, std::vector<std::string> event_type, QDate start, QDate end);
	vector<string> getDateRange();
	void refreshEventGroup();
	void refreshAnnotationGroup();
	vector<string> getEventCategories();
	void saveCurrentStatus(std::string status);
	void restoreCurrentStatus(const char* tmpFile);
	void toInsertMode(bool mode);
	void toAnnotationCategoryMode(bool mode);
	string getCurClipText();
	int checkNumber(QLineEdit* edit, int digits);
	bool changeDatFile();
	void toTextAnnotationMode(bool mode);
	void toITextAnnotationMode(bool mode);
	void toSettingMode(bool mode);
	void initIAnnotationCombo(QComboBox* combobox);
	void refreseInsertPanel();
	string getAnnotationValue(bool isText, QComboBox* combo_box, QTextEdit* text_edit);
	bool existClip(string clip);
	void toSearchAnnotationMode(bool mode);
	void toExecutingMode(bool mode);
	vector<string> getListField4EventAll();
	void transform_data(sEvent_Data *event_data, vector<string> item_list, map<string, vector<string>> containers);
	void get_Clip_data(sEvent_Data *event_data, vector<string> item_list, map<string, vector<string>> containers);
	void toLoginMode(bool mode);
	void initProjectCombo(QComboBox* combobox);
	void initFeatureList();
	void initWorkspaceDirectory();
	tResult on_btn_login_clicked(string userid, string logid);
	string recordLog(string id, bool isLogin);
	void setOnline(string id, bool online);
	bool checkAuth(string id, string pass);
	string getVinOfClip(string clip);
	void initEventList();
	void unregisterEventHandler();

public slots:
	tResult on_btn_clip_clicked();
	tResult on_btn_setting_clicked();
	tResult on_btn_setting_cancel_clicked();
	tResult on_btn_browse_clicked();
	tResult on_btn_execute_clicked();
	tResult on_btn_copy_clicked();
	tResult on_btn_LR1_clicked();
	tResult on_btn_RL1_clicked();
	tResult on_btn_LR2_clicked();
	tResult on_btn_RL2_clicked();
	tResult on_btn_LR3_clicked();
	tResult on_btn_RL3_clicked();
	tResult on_dateEdit_changed();
	tResult on_chk_date_clicked();
	tResult on_dateEdit_2_changed();
	tResult on_chk_eyeq_event_clicked();
	tResult on_chk_fcm_event_clicked();
	tResult on_chk_radar_event_clicked();
	tResult on_chk_user_event_clicked();
	tResult on_chk_annotation_clicked();
	tResult on_chk_search_clicked();
	tResult on_chk_text_annotation_clicked();
	tResult on_chk_i_text_annotation_clicked();
	tResult on_btn_cancel_clicked();
	tResult on_btn_submit_clicked();
	tResult on_txt_m_edited();
	tResult on_txt_s_edited();
	tResult on_txt_ms_edited();
	tResult on_cbo_i_feature_changed();
	tResult on_btn_showclip_clicked();
	tResult on_list_event_annotation_clicked(QModelIndex);
	tResult on_btn_udpate_clicked();
	tResult on_btn_insert_clicked();
	tResult on_txt_search_edited(const QString &search_text);
	tResult on_btn_login_clicked();
	tResult on_cbo_project_changed();
	tResult on_btn_change_pw_clicked();
	tResult on_btn_change_cancel_clicked();
	tResult on_btn_change_clicked();
	tResult on_btn_logout_clicked();
};

#endif