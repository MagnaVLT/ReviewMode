/*****************************************************************************
 *     ADTF Template Project Filter
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

#include "stdafx.h"
#include "ReviewModeFilter.h"
#include <additional/harddiskstorage_intf.h>
#include <stdio.h>
#include <Windows.h>
#include <QtCore/QDate>
#include <QtGui/QTreeView>
#include <QtCore/QTextStream>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include "XML/XMLHandler.h"

#include "DB/Updator.h"
#include <QtGui/QApplication>
#include <QtGui/QProgressDialog>

ADTF_FILTER_PLUGIN("ReviewModeFilter", OID_ADTF_HIL_ReviewModeFilter, ReviewModeFilter);
extern const std::string DB = "fcm_hil_playback";
extern const std::string REVIEW_WORKSPACE = "./review_workspace/";
extern const std::string TEMP_FILE = REVIEW_WORKSPACE + "temp/";
extern const std::string CLIP_BACKUP = REVIEW_WORKSPACE + "clip_backup/";
extern const std::string DB_HOME = REVIEW_WORKSPACE +  "db_home/";
extern const std::string SYSTEM_RELEASE = "./config/system_release.xml";
extern const std::string DB_SCHEME = "./filter/Magna/database_scheme.xml";
extern const std::string STATIC_TABLES = "./filter/Magna/static_tables.sql";
extern const std::string BACKUP_DB = "fcm_hil_playback_backup";
extern const std::string LOGIN = "1";
extern const std::string LOGOUT = "0";

string ReviewModeFilter::USERLOG_ID = "";
bool ReviewModeFilter::isTemporalClose = false;
string ReviewModeFilter::ID = "";

ReviewModeFilter::ReviewModeFilter(const tChar* __info):cBaseQtFilter(__info)
{

}

ReviewModeFilter::~ReviewModeFilter()
{

}

tResult ReviewModeFilter::Init(tInitStage eStage, __exception)
{
    // never miss calling the parent implementation!!
    RETURN_IF_FAILED(cBaseQtFilter::Init(eStage, __exception_ptr))
    
    if (eStage == StageFirst)
    {
		// in StageFirst you can create and register your static pins.
		
		//RETURN_IF_FAILED(this->m_sample_data.Create("Raw_Sample_In",new cMediaType(MEDIA_TYPE_STRUCTURED_DATA, MEDIA_SUBTYPE_STRUCT_STRUCTURED),
		//	static_cast<IPinEventSink*>(this)));
		//RETURN_IF_FAILED(RegisterPin(&this->m_sample_data));

		RETURN_IF_FAILED(m_out_reviewed_data.Create("Reviewed_Data", new adtf::cMediaType(MEDIA_TYPE_STRUCTURED_DATA, MEDIA_SUBTYPE_STRUCT_STRUCTURED), this));
		RETURN_IF_FAILED(RegisterPin(&m_out_reviewed_data));

	}

	else if (eStage == StageNormal)
    {
        
	}
	else if (eStage == StageGraphReady)
	{
		THROW_IF_POINTER_NULL(_kernel);
		// All pin connections have been established in this stage so you can query your pins
		// about their media types and additional meta data.
		// Please take a look at the demo_imageproc example for further reference.
	}


    RETURN_NOERROR;
}

/* Start Function */
tResult ReviewModeFilter::Start(__exception)
{
	return cBaseQtFilter::Start(__exception_ptr);		// TODO	RETURN_IF_FAILED generates a warning [created on 07/02/2014]
}

/** Stop Function */
tResult ReviewModeFilter::Stop(__exception)
{
    return cBaseQtFilter::Stop(__exception_ptr);
}

tResult ReviewModeFilter::Shutdown(tInitStage eStage, __exception)
{
    // In each stage clean up everything that you initialized in the corresponding stage during Init.
    // Pins are an exception:
    // - The base class takes care of static pins that are members of this class.
    // - Dynamic pins have to be cleaned up in the ReleasePins method, please see the demo_dynamicpin
    //   example for further reference.
    
    if (eStage == StageGraphReady)
    {
    }
    else if (eStage == StageNormal)
    {
    }
    else if (eStage == StageFirst)
    {

    }

    // call the base class implementation
    return cBaseQtFilter::Shutdown(eStage, __exception_ptr);
}


tResult ReviewModeFilter::OnPinEvent(IPin *pSource, tInt nEventCode, tInt nParam1, tInt nParam2, IMediaSample *pMediaSample)
{
	if (nEventCode == IPinEventSink::PE_MediaSampleReceived)
	{
		//cObjectPtr<IMediaSampleInfo> pSampleInfo; 
		//pMediaSample->GetInterface(IID_ADTF_MEDIA_SAMPLE_INFO, (tVoid**)&pSampleInfo);
		//LOG_INFO(MagnaUtil::integerToString(pSampleInfo->GetInfoInt(IMediaSampleInfo::MSAI_Counter, 0)).c_str());
		//this->m_oTAPI.timestamp = (me_timestamp_t)pSampleInfo->GetInfoTimestamp(IMediaSampleInfo::MSAI_DeviceOriginalTime, -1);

		//this->m_oTAPI.size = pMediaSample->GetSize();
	}

	RETURN_NOERROR;
}


tResult ReviewModeFilter::Run(tInt nActivationCode, const tVoid* pvUserData, tInt szUserDataSize, ucom::IException** __exception_ptr/* =NULL */)
{
	return cBaseQtFilter::Run(nActivationCode, pvUserData, szUserDataSize, __exception_ptr);
}

tHandle ReviewModeFilter::CreateView()
{
	m_pFilterWidget = new QWidget();
	m_oFilterGUI.setupUi(m_pFilterWidget);
	string tmpFile = TEMP_FILE + "tmp.xml";

	if(QFile().exists(QString(tmpFile.c_str())))
	{
		XMLHandler *xmlHandle = new XMLHandler(tmpFile.c_str());
		string userid = xmlHandle->getNodeValue("login", "value");
		string logid = xmlHandle->getNodeValue("logid", "value");
		if(userid.empty()==false)
		{
			on_btn_login_clicked(userid, logid);
			return (tHandle) m_pFilterWidget;
		}
	}

	this->m_oFilterGUI.btn_logout->setVisible(false);
	this->m_oFilterGUI.grp_pwchange->setVisible(false);
	this->toLoginMode(true);
	
	connect(m_oFilterGUI.txt_loginpw, SIGNAL(returnPressed()),m_oFilterGUI.btn_login,SIGNAL(clicked()));
	connect(m_oFilterGUI.txt_change_pw2, SIGNAL(returnPressed()),m_oFilterGUI.btn_change,SIGNAL(clicked()));
	connect(m_oFilterGUI.btn_login, SIGNAL(clicked()), this, SLOT(on_btn_login_clicked()));
	connect(m_oFilterGUI.btn_change_pw, SIGNAL(clicked()), this, SLOT(on_btn_change_pw_clicked()));
	connect(m_oFilterGUI.btn_change_cancel, SIGNAL(clicked()), this, SLOT(on_btn_change_cancel_clicked()));
	connect(m_oFilterGUI.btn_change, SIGNAL(clicked()), this, SLOT(on_btn_change_clicked()));
	
	return (tHandle) m_pFilterWidget;
}


tResult ReviewModeFilter::ReleaseView()
{
	this->recordLog(USERLOG_ID, false);
	this->setOnline(ID, false);
	string tmpFile = TEMP_FILE + "tmp.xml";
	MagnaUtil::delete_file(tmpFile);
	
	RETURN_NOERROR;
}

void ReviewModeFilter::initAll()
{
	this->initMode();
	this->initModel();
	this->initWorkspaceDirectory();

	string tmpFile = TEMP_FILE + "tmp.xml";
	this->sqlFileHandler = new SQLFileHandler(TEMP_FILE);

	if(QFile().exists(QString(tmpFile.c_str())))
	{
		this->initStatusCombo(this->m_oFilterGUI.cbo_status);
		this->restoreCurrentStatus(tmpFile.c_str());
		this->registerEventHandler();
	}else
	{

		this->initEventList();
		this->initGUI();
	}
}

void ReviewModeFilter::initMode()
{
	this->m_oFilterGUI.txt_clip_directory->setEnabled(false);
	this->m_oFilterGUI.grp_pwchange->setVisible(false);
	this->toLoginMode(false);
	this->toSearchAnnotationMode(false);
	this->toSettingMode(false);
	this->toInsertMode(false);
	this->toAnnotationCategoryMode(false);
}

void ReviewModeFilter::initGUI()
{
	m_oFilterGUI.lbl_reportid->setVisible(false);
	m_oFilterGUI.lbl_table->setVisible(false);
	this->initFeatureList();
	QDate date = QDate::currentDate();
	m_oFilterGUI.dateEdit->setDate(date);
	m_oFilterGUI.dateEdit_2->setDate(date);
	this->toTextAnnotationMode(true);
	this->initStatusCombo(this->m_oFilterGUI.cbo_status);
	this->initProjectCombo(this->m_oFilterGUI.cbo_project);
	this->registerEventHandler();
}

void ReviewModeFilter::initWorkspaceDirectory()
{
	MagnaUtil::make_directory(REVIEW_WORKSPACE);
	MagnaUtil::make_directory(CLIP_BACKUP);
	MagnaUtil::make_directory(DB_HOME);
	MagnaUtil::make_directory(TEMP_FILE);
}

void ReviewModeFilter::initFeatureList()
{
	selectedFeatureModel = new QStandardItemModel();
	this->m_oFilterGUI.listFeatureSelected->setModel(this->selectedFeatureModel);

	vector<string> tokens = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_project->currentText().toStdString(), '-');
	if(tokens.size()>1)
	{
		string projectid = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_project->currentText().toStdString(), '-').at(1);
		string query =	" select b.name, b.id from users_feature_project_map a, feature_list b ";
		query +=		" where a.featureid = b.id and a.userid = '"+ReviewModeFilter::ID+"' and a.projectid = "+projectid+";";

		featureModel = new QStandardItemModel();
		this->listhandle->addItemsFromDB(m_oFilterGUI.listFeature, featureModel, this->getListField4Feature(), query);
	}
}

void ReviewModeFilter::initEventList()
{
	eventListModel = new QStandardItemModel();
	this->m_oFilterGUI.listEventAnnotation->setModel(this->eventListModel);
}


void ReviewModeFilter::initModel(){
	featureModel = new QStandardItemModel();
	selectedFeatureModel = new QStandardItemModel();
	eventModel = new QStandardItemModel();
	selectedEventModel = new QStandardItemModel();
	annotationModel = new QStandardItemModel();
	selectedAnnotationModel = new QStandardItemModel();
	eventListModel = new QStandardItemModel();

	this->m_oFilterGUI.listFeature->setModel(this->featureModel);
	this->m_oFilterGUI.listFeatureSelected->setModel(this->selectedFeatureModel);
	this->m_oFilterGUI.listEvent->setModel(this->eventModel);
	this->m_oFilterGUI.listEventSelected->setModel(this->selectedEventModel);
	this->m_oFilterGUI.listAnnotation->setModel(this->annotationModel);
	this->m_oFilterGUI.listAnnotationSelected->setModel(this->selectedAnnotationModel);
}

void ReviewModeFilter::initEventListModel(){
	eventListModel = new QStandardItemModel();	
}

void ReviewModeFilter::initEventCombo(QComboBox* combobox, bool eyeq)
{
	string selectedFeature = m_oFilterGUI.cbo_i_feature->currentText().toStdString();
	combobox->clear();

	if(!selectedFeature.empty())
	{
		selectedFeature = MagnaUtil::stringTokenizer(selectedFeature, '-').at(1);
		vector<string> event_fields = getListField4Event();
		string query = "select a.name, a.id from event_list a, project_event_map b where a.id = b.eventid and a.featureid = "+selectedFeature+";";
		map<string, vector<string>> eventContainer = (new Retriever(event_fields, query))->getData();
		vector<string> event_list = listhandle->getConcatenatedText(eventContainer, event_fields);
		combo_handle->initCombo(combobox, event_list);
	}
}

void ReviewModeFilter::initFeatureCombo(QComboBox* combobox)
{
	string projectid = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_project->currentText().toStdString(), '-').at(1);
	vector<string> feature_fields = getListField4Feature();
	string query = "select b.name as name, b.id as id from users_feature_project_map a, feature_list b ";
	query += " where a.FeatureID = b.ID and a.UserID = '" + ReviewModeFilter::ID + "' and a.projectid = " + projectid +";";

	map<string, vector<string>> featureContainer = (new Retriever(feature_fields, query))->getData();
	vector<string> event_list = listhandle->getConcatenatedText(featureContainer, feature_fields);
	combo_handle->initCombo(combobox, event_list);
}

void ReviewModeFilter::initVINCombo(QComboBox* combobox)
{
	vector<string> vin_fields;
	vin_fields.push_back("vin");
	string projectid = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_project->currentText().toStdString(), '-').at(1);
	string query = "select a.vin as vin from event_report a, event_list b, users_feature_project_map c ";
	query += " where a.eventid = b.id and c.featureid = b.FeatureID and a.projectid = c.projectid and ";
	query += " c.projectID = "+projectid+" and c.UserID = '"+ReviewModeFilter::ID+"' group by a.vin;";

	map<string, vector<string>> vinContainer = (new Retriever( vin_fields, query))->getData();
	combo_handle->initCombo(combobox, vinContainer["vin"]);
}


void ReviewModeFilter::initEventCategoryCombo(QComboBox* combobox)
{
	vector<string> event_category_field;
	event_category_field.push_back("name");
	event_category_field.push_back("id");
	string query = "select name, id from event_category_list ";

	map<string, vector<string>> eCategoryContainer = (new Retriever(event_category_field, query))->getData();
	vector<string> category_list = listhandle->getConcatenatedText(eCategoryContainer, event_category_field);
	combo_handle->initCombo(combobox, category_list);
}

void ReviewModeFilter::initIAnnotationCombo(QComboBox* combobox)
{
	vector<string> annotation_fields = getListField4Annotation();
	string selectedFeature = m_oFilterGUI.cbo_i_feature->currentText().toStdString();
	combobox->clear();
	
	if(!selectedFeature.empty())
	{
		selectedFeature = MagnaUtil::stringTokenizer(selectedFeature, '-').at(1).substr(0, 1);
		vector<string> vstr_fetures;
		vstr_fetures.push_back(selectedFeature);
		string projectid = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_project->currentText().toStdString(), '-').at(1);
		string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, projectid);	
		map<string, vector<string>> annotationContainer = (new Retriever(annotation_fields, query4Annotation))->getData();
		vector<string> status_list = listhandle->getConcatenatedText(annotationContainer, annotation_fields);
		combo_handle->initCombo(combobox, status_list);
	}
}

void ReviewModeFilter::initAnnotationCombo(QComboBox* combobox, vector<string> vstr_fetures)
{
	vector<string> annotation_fields = getListField4Annotation();
	vstr_fetures = MagnaUtil::getTokenedVector(vstr_fetures, '-', 1);
	string projectid = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_project->currentText().toStdString(), '-').at(1);
	string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, projectid);
	
	if(query4Annotation.empty())
	{
		combobox->clear();

	}else
	{
		map<string, vector<string>> annotationContainer = (new Retriever(annotation_fields, query4Annotation))->getData();
		vector<string> status_list = listhandle->getConcatenatedText(annotationContainer, annotation_fields);

		combo_handle->initCombo(combobox, status_list);
	}
}


void ReviewModeFilter::initAnnotationCombo(QComboBox* combobox)
{
	vector<string> annotation_fields = getListField4Annotation();
	vector<string> vstr_fetures = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listFeatureSelected, 1);	
	string projectid = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_project->currentText().toStdString(), '-').at(1);

	string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, projectid);

	if(query4Annotation.empty())
	{
		combobox->clear();

	}else
	{
		map<string, vector<string>> annotationContainer = (new Retriever(annotation_fields, query4Annotation))->getData();
		vector<string> annotation_list = listhandle->getConcatenatedText(annotationContainer, annotation_fields);
		combobox->clear();
		combo_handle->initCombo(combobox, annotation_list);
	}
}

void ReviewModeFilter::initProjectCombo(QComboBox* combobox)
{
	vector<string> project_fields;
	project_fields.push_back("name");
	project_fields.push_back("id");
	string query4Project = "select b.name, b.id from users_project_map a, project b ";
	query4Project += " where a.projectid = b.id and b.isopen = 0 and a.userid = '" + ReviewModeFilter::ID + "';";
	map<string, vector<string>> projectContainer = (new Retriever(project_fields, query4Project))->getData();
	vector<string> project_list = listhandle->getConcatenatedText(projectContainer, project_fields);
	combobox->clear();
	combo_handle->initCombo(combobox, project_list);
}





void ReviewModeFilter::initStatusCombo(QComboBox* combobox)
{

	combobox->clear();
	vector<string> status_fields = getField4Status();
	string query = "select name, id from event_status_list where id<>1";
	map<string, vector<string>> statusContainer = (new Retriever(status_fields, query))->getData();
	vector<string> status_list = listhandle->getConcatenatedText(statusContainer, status_fields);
	combo_handle->initCombo(combobox, status_list);
}

void ReviewModeFilter::registerEventHandler()
{
	connect(m_oFilterGUI.btn_setting, SIGNAL(clicked()), this, SLOT(on_btn_setting_clicked()));
	connect(m_oFilterGUI.btn_setting_cancel, SIGNAL(clicked()), this, SLOT(on_btn_setting_cancel_clicked()));
	connect(m_oFilterGUI.btn_browse, SIGNAL(clicked()), this, SLOT(on_btn_browse_clicked()));
	connect(m_oFilterGUI.btn_execute, SIGNAL(clicked()), this, SLOT(on_btn_execute_clicked()));
	connect(m_oFilterGUI.btn_clip, SIGNAL(clicked()), this, SLOT(on_btn_clip_clicked()));
	connect(m_oFilterGUI.btn_copy, SIGNAL(clicked()), this, SLOT(on_btn_copy_clicked()));
	connect(m_oFilterGUI.btn_logout, SIGNAL(clicked()), this, SLOT(on_btn_logout_clicked()));

	connect(m_oFilterGUI.btn_LR1, SIGNAL(clicked()), this, SLOT(on_btn_LR1_clicked()));
	connect(m_oFilterGUI.btn_RL1, SIGNAL(clicked()), this, SLOT(on_btn_RL1_clicked()));
	connect(m_oFilterGUI.btn_LR2, SIGNAL(clicked()), this, SLOT(on_btn_LR2_clicked()));
	connect(m_oFilterGUI.btn_RL2, SIGNAL(clicked()), this, SLOT(on_btn_RL2_clicked()));
	connect(m_oFilterGUI.btn_LR3, SIGNAL(clicked()), this, SLOT(on_btn_LR3_clicked()));
	connect(m_oFilterGUI.btn_RL3, SIGNAL(clicked()), this, SLOT(on_btn_RL3_clicked()));

	connect(m_oFilterGUI.btn_showclip, SIGNAL(clicked()), this, SLOT(on_btn_showclip_clicked()));
	connect(m_oFilterGUI.btn_update, SIGNAL(clicked()), this, SLOT(on_btn_udpate_clicked()));
	connect(m_oFilterGUI.btn_insert, SIGNAL(clicked()), this, SLOT(on_btn_insert_clicked()));
	connect(m_oFilterGUI.btn_cancel, SIGNAL(clicked()), this, SLOT(on_btn_cancel_clicked()));
	connect(m_oFilterGUI.btn_submit, SIGNAL(clicked()), this, SLOT(on_btn_submit_clicked()));

	connect(m_oFilterGUI.chk_date, SIGNAL(clicked()), this, SLOT(on_chk_date_clicked()));

	connect(m_oFilterGUI.chk_eyeq_event, SIGNAL(clicked()), this, SLOT(on_chk_eyeq_event_clicked()));
	connect(m_oFilterGUI.chk_fcm_event, SIGNAL(clicked()), this, SLOT(on_chk_fcm_event_clicked()));
	connect(m_oFilterGUI.chk_user_event, SIGNAL(clicked()), this, SLOT(on_chk_user_event_clicked()));
	connect(m_oFilterGUI.chk_radar, SIGNAL(clicked()), this, SLOT(on_chk_radar_event_clicked()));
	
	connect(m_oFilterGUI.chk_annotation, SIGNAL(clicked()), this, SLOT(on_chk_annotation_clicked()));
	connect(m_oFilterGUI.chk_search, SIGNAL(clicked()), this, SLOT(on_chk_search_clicked()));
	connect(m_oFilterGUI.chk_text_annotation, SIGNAL(clicked()), this, SLOT(on_chk_text_annotation_clicked()));
	connect(m_oFilterGUI.chk_i_text_annotation, SIGNAL(clicked()), this, SLOT(on_chk_i_text_annotation_clicked()));

	connect(m_oFilterGUI.dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_changed()));
	connect(m_oFilterGUI.dateEdit_2, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_2_changed()));

	connect(m_oFilterGUI.listEventAnnotation, SIGNAL(clicked(const QModelIndex)), this, SLOT(on_list_event_annotation_clicked(QModelIndex)));

	connect(m_oFilterGUI.cbo_i_feature, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbo_i_feature_changed()));
	connect(m_oFilterGUI.cbo_project, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbo_project_changed()));

	connect(m_oFilterGUI.txt_m, SIGNAL(editingFinished()), this, SLOT(on_txt_m_edited()));
	connect(m_oFilterGUI.txt_s, SIGNAL(editingFinished()), this, SLOT(on_txt_s_edited()));
	connect(m_oFilterGUI.txt_ms, SIGNAL(editingFinished()), this, SLOT(on_txt_ms_edited()));
	connect(m_oFilterGUI.txt_search, SIGNAL(textChanged(const QString &)), this, SLOT(on_txt_search_edited(const QString &)));
}


void ReviewModeFilter::unregisterEventHandler()
{
	disconnect(this, SLOT(on_btn_setting_clicked()));
	disconnect(this, SLOT(on_btn_setting_cancel_clicked()));
	disconnect(this, SLOT(on_btn_browse_clicked()));
	disconnect(this, SLOT(on_btn_execute_clicked()));
	disconnect(this, SLOT(on_btn_clip_clicked()));
	disconnect(this, SLOT(on_btn_copy_clicked()));
	disconnect(this, SLOT(on_btn_logout_clicked()));

	disconnect(this, SLOT(on_btn_LR1_clicked()));
	disconnect(this, SLOT(on_btn_RL1_clicked()));
	disconnect(this, SLOT(on_btn_LR2_clicked()));
	disconnect(this, SLOT(on_btn_RL2_clicked()));
	disconnect(this, SLOT(on_btn_LR3_clicked()));
	disconnect(this, SLOT(on_btn_RL3_clicked()));

	disconnect(this, SLOT(on_btn_showclip_clicked()));
	disconnect(this, SLOT(on_btn_udpate_clicked()));
	disconnect(this, SLOT(on_btn_insert_clicked()));
	disconnect(this, SLOT(on_btn_cancel_clicked()));
	disconnect(this, SLOT(on_btn_submit_clicked()));

	disconnect(this, SLOT(on_chk_date_clicked()));

	disconnect(this, SLOT(on_chk_eyeq_event_clicked()));
	disconnect(this, SLOT(on_chk_fcm_event_clicked()));
	disconnect(this, SLOT(on_chk_user_event_clicked()));
	disconnect(this, SLOT(on_chk_radar_event_clicked()));

	disconnect(this, SLOT(on_chk_annotation_clicked()));
	disconnect(this, SLOT(on_chk_search_clicked()));
	disconnect(this, SLOT(on_chk_text_annotation_clicked()));
	disconnect(this, SLOT(on_chk_i_text_annotation_clicked()));

	disconnect(this, SLOT(on_dateEdit_changed()));
	disconnect(this, SLOT(on_dateEdit_2_changed()));

	disconnect(this, SLOT(on_list_event_annotation_clicked(QModelIndex)));

	disconnect(this, SLOT(on_cbo_i_feature_changed()));
	disconnect(this, SLOT(on_cbo_project_changed()));

	disconnect(this, SLOT(on_txt_m_edited()));
	disconnect(this, SLOT(on_txt_s_edited()));
	disconnect(this, SLOT(on_txt_ms_edited()));
	disconnect(this, SLOT(on_txt_search_edited(const QString &)));
}


string ReviewModeFilter::getCurClipText()
{
	string datFile = "";

	FILE *fp = fopen(SYSTEM_RELEASE.c_str(), "r");
	if (NULL == fp)
	{
		MagnaUtil::show_message("Invalid system_release.xml file");

	}else{
		string att_name = "idref";
		string att_value = "adtf.stg.harddisk_player";

		XMLHandler *t = new XMLHandler(SYSTEM_RELEASE);
		datFile = t->getCurDatFile("adtf:project", "configurations", att_name, att_value);
	}

	return datFile;
}

tResult ReviewModeFilter::on_cbo_project_changed()
{
	this->initFeatureList();
	this->refreshEventGroup();
	this->refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_change_pw_clicked(){
	this->m_oFilterGUI.grp_pwchange->setVisible(true);
	this->m_oFilterGUI.txt_cur_pw->setEchoMode(QLineEdit::Password);
	this->m_oFilterGUI.txt_change_pw->setEchoMode(QLineEdit::Password);
	this->m_oFilterGUI.txt_change_pw2->setEchoMode(QLineEdit::Password);
	this->m_oFilterGUI.txt_change_id->setFocus(Qt::MouseFocusReason);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_change_cancel_clicked(){
	this->m_oFilterGUI.txt_cur_pw->setText(QString(""));
	this->m_oFilterGUI.txt_change_pw->setText(QString(""));
	this->m_oFilterGUI.txt_change_pw2->setText(QString(""));
	this->m_oFilterGUI.txt_change_id->setText(QString(""));
	this->m_oFilterGUI.grp_pwchange->setVisible(false);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_change_clicked(){

	string userid = this->m_oFilterGUI.txt_change_id->text().toStdString();
	string password = this->m_oFilterGUI.txt_cur_pw->text().toStdString();
	string p1 = this->m_oFilterGUI.txt_change_pw->text().toStdString();
	string p2 = this->m_oFilterGUI.txt_change_pw2->text().toStdString();

	if(this->checkAuth(userid, password)==false)
	{
		MagnaUtil::show_message("Invalid ID and Password.");
		RETURN_NOERROR;
	}else if(p1!=p2)
	{
		MagnaUtil::show_message("The passwords are not matched");
		RETURN_NOERROR;
	}
	string query = "update users set password = PASSWORD('"+p1+"') where id = '" + userid + "';";
	(new Updator(query))->execute();

	this->m_oFilterGUI.grp_pwchange->setVisible(false);
	MagnaUtil::show_message("Your password has been changed.");

	this->m_oFilterGUI.txt_loginpw->setFocus(Qt::MouseFocusReason);
	this->m_oFilterGUI.txt_loginid->setText(this->m_oFilterGUI.txt_change_id->text());
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_login_clicked(){
	string userid = this->m_oFilterGUI.txt_loginid->text().toStdString();
	string password = this->m_oFilterGUI.txt_loginpw->text().toStdString();

	if(this->checkAuth(userid, password)==true)
	{
		ReviewModeFilter::ID = userid;
		string text = ReviewModeFilter::ID + " is logged in";
		setOnline(ReviewModeFilter::ID, true);
		string logid = recordLog(ReviewModeFilter::ID, true);
		if(logid!="-1") ReviewModeFilter::USERLOG_ID = logid;
		this->m_oFilterGUI.lbl_user->setText(QString(text.c_str()));
		this->m_oFilterGUI.txt_loginid->setText(QString(""));
		this->m_oFilterGUI.txt_loginpw->setText(QString(""));
		this->initAll();
	}else
	{
		MagnaUtil::show_message("Invalid ID and Password.");
	}

	RETURN_NOERROR;
}
tResult ReviewModeFilter::on_btn_logout_clicked(){
	ReviewModeFilter::ID = "";
	this->m_oFilterGUI.lbl_user->setText(QString(""));
	MagnaUtil::delete_file(TEMP_FILE + "tmp.xml");
	this->m_oFilterGUI.txt_loginid->setFocus(Qt::MouseFocusReason);
	unregisterEventHandler();
	this->toLoginMode(true);

	RETURN_NOERROR;
}

bool ReviewModeFilter::checkAuth(string id, string pass)
{
	vector<string> field;
	field.push_back("cnt");
	string query = "select count(*) as cnt from users where id = '" + id + "' and password = PASSWORD('"+pass+"');";
	map<string, vector<string>> data = (new Retriever(field, query))->getData();

	int record = MagnaUtil::stringTointeger(data["cnt"].at(0));
	if(record == 0) return false;
	else return true;
}

void ReviewModeFilter::setOnline(string userid, bool online)
{
	if(online==true)
	{
		(new Updator("update users set online=1 where id = '" + userid + "';"))->execute();
	}else
	{
		(new Updator("update users set online=0 where id = '" + userid + "';"))->execute();
	}
}

string ReviewModeFilter::recordLog(string id, bool isLogin)
{
	QDate cd = QDate::currentDate();
	QTime ct = QTime::currentTime();

	string currDate = cd.toString(Qt::ISODate).toStdString();
	string currTime = ct.toString(Qt::TextDate).toStdString();
	string currDateTime = currDate + " " + currTime;

	if(isLogin==true)
	{
		(new Updator("insert into user_log (userid, reviewstarttime) value ('" + id + "', '" + currDateTime + "');"))->execute();
		vector<string> field;
		field.push_back("id");
		string query = "select id from user_log where userid = '" + id + "' and reviewstarttime = '" + currDateTime + "';";
		map<string, vector<string>> dataContainer = (new Retriever(field, query))->getData();
		return dataContainer["id"].at(0);
	}else
	{
		string query = "update user_log set reviewendtime = '" + currDateTime + "' where id = "+ReviewModeFilter::USERLOG_ID+";";
		(new Updator(query))->execute();
	}

	return "-1";
}

tResult ReviewModeFilter::on_btn_login_clicked(string userid, string logid)
{
	ReviewModeFilter::isTemporalClose=false;
	ReviewModeFilter::ID = userid;
	ReviewModeFilter::USERLOG_ID = logid;
	toLoginMode(false);
	string text = ReviewModeFilter::ID + " is logged in";
	this->m_oFilterGUI.lbl_user->setText(QString(text.c_str()));
	this->initAll();

	RETURN_NOERROR;
}



tResult ReviewModeFilter::on_cbo_i_feature_changed()
{
	this->refreseInsertPanel();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_i_text_annotation_clicked()
{
	this->toITextAnnotationMode(m_oFilterGUI.chk_i_text_annotation->isChecked());
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_text_annotation_clicked()
{
	this->toTextAnnotationMode(m_oFilterGUI.chk_text_annotation->isChecked());
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_txt_m_edited()
{
	checkNumber(this->m_oFilterGUI.txt_m, 2);
	RETURN_NOERROR;
}
tResult ReviewModeFilter::on_txt_s_edited()
{
	checkNumber(this->m_oFilterGUI.txt_s, 2);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_txt_ms_edited()
{
	checkNumber(this->m_oFilterGUI.txt_ms, 3);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_txt_search_edited(const QString &search_text)
{
	this->refreshAnnotationGroup();
	RETURN_NOERROR;
}


tResult ReviewModeFilter::on_chk_annotation_clicked()
{
	toAnnotationCategoryMode(this->m_oFilterGUI.chk_annotation->isChecked());
	annotationModel = new QStandardItemModel();
	selectedAnnotationModel = new QStandardItemModel();
	this->m_oFilterGUI.listAnnotation->setModel(this->annotationModel);
	this->m_oFilterGUI.listAnnotationSelected->setModel(this->selectedAnnotationModel);

	
	vector<string> vstr_fetures = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listFeatureSelected, 1);	
	string projectid = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_project->currentText().toStdString(), '-').at(1);

	string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, projectid);
	if(!query4Annotation.empty())
	{
		this->listhandle->addItemsFromDB(m_oFilterGUI.listAnnotation, annotationModel, this->getListField4Annotation(), query4Annotation);
	}

	refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_search_clicked()
{
	this->toSearchAnnotationMode(this->m_oFilterGUI.chk_search->isChecked());
	this->refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_eyeq_event_clicked()
{
	refreshEventGroup();
	refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_radar_event_clicked()
{
	refreshEventGroup();
	refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_fcm_event_clicked()
{
	refreshEventGroup();
	refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_user_event_clicked()
{
	refreshEventGroup();
	refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_list_event_annotation_clicked(QModelIndex index)
{
	string id = this->listhandle->getSelectedItemText(index, 1);
	string prefix = this->listhandle->getSelectedItemText(index, 0);
	this->m_oFilterGUI.txt_annotation->clear();
	string table = "";

	this->m_oFilterGUI.lbl_table->setText(QString(prefix.c_str()));
	
	string query = "select a.reportid, b.clipname as clipname, b.adtfstarttime, a.adtftime, a.userannotation, a.predefinedannotationid, ";
	query += " a.eventstatusid, c.name as name, d.name as annotation ";
	query += " from event_report a, clip_info b, event_status_list c, predefined_annotation_list d ";
	query += " where a.predefinedannotationid = d.id and b.clipid=a.clipid and a.vin = b.vin and a.eventstatusid = c.id and a.reportid = "+id+" and eventcategoryid = " +prefix+ ";";
	
	vector<string> fields = this->getField4EventEdit();
	map<string, vector<string>> dataContainer = (new Retriever(fields, query))->getData();

	if(dataContainer[fields.at(0).c_str()].size()>0)
	{
		this->m_oFilterGUI.lbl_clip->setText(dataContainer["clipname"].at(0).c_str());
		string adtfTime = dataContainer["adtftime"].at(0);
		string startTime = dataContainer["adtfstarttime"].at(0);
		string r_time = MagnaUtil::integerToString(MagnaUtil::stringTointeger(adtfTime) - MagnaUtil::stringTointeger(startTime));
		r_time = MagnaUtil::convertMicroSecondToTime(r_time);
		
		this->m_oFilterGUI.lbl_adtftime->setText(r_time.c_str());
		this->m_oFilterGUI.lbl_reportid->setText(dataContainer["reportid"].at(0).c_str());
		string annotation_text = dataContainer["annotation"].at(0) + "-" + dataContainer["predefinedannotationid"].at(0);
		int index_annotation = this->m_oFilterGUI.cbo_annotation->findText(annotation_text.c_str());
		
		if(index_annotation==-1)
		{
			this->m_oFilterGUI.chk_text_annotation->setChecked(false);
			this->on_chk_text_annotation_clicked();
			this->initAnnotationCombo(this->m_oFilterGUI.cbo_annotation);
			this->m_oFilterGUI.txt_annotation->setPlainText(QString(dataContainer["userannotation"].at(0).c_str()));
		}else
		{
			this->m_oFilterGUI.chk_text_annotation->setChecked(true);
			this->on_chk_text_annotation_clicked();
			this->m_oFilterGUI.cbo_annotation->setCurrentIndex(index_annotation);
		}

		int index_status = m_oFilterGUI.cbo_status->findText((dataContainer["name"].at(0) +"-"+ dataContainer["eventstatusid"].at(0)).c_str());
		this->m_oFilterGUI.cbo_status->setCurrentIndex(index_status);
	}

	this->saveCurrentStatus(LOGOUT);

	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_udpate_clicked()
{
	string reportid = this->m_oFilterGUI.lbl_reportid->text().toStdString();
	if(reportid.empty()){
		RETURN_NOERROR;
	}

	string prefix = this->m_oFilterGUI.lbl_table->text().toStdString();	
	string status = m_oFilterGUI.cbo_status->currentText().toStdString();
	string annotation = m_oFilterGUI.txt_annotation->toPlainText().toStdString();//getAnnotationValue(m_oFilterGUI.chk_text_annotation->isChecked(), m_oFilterGUI.cbo_annotation, m_oFilterGUI.txt_annotation);
	string predefined_annotation = this->m_oFilterGUI.cbo_annotation->currentText().toStdString();

	if(status.empty() || (this->m_oFilterGUI.chk_text_annotation->isChecked() && predefined_annotation.empty()))
	{
		QMessageBox box;
		box.setText(QString("Please select an event-status and annotation"));
		box.exec();
	}else
	{
		status = MagnaUtil::stringTokenizer(m_oFilterGUI.cbo_status->currentText().toStdString(), '-').at(1);
		string query = "";
		string curTime = MagnaUtil::bigIntegerToString(MagnaUtil::getCurrentSystemTime());
		if(this->m_oFilterGUI.chk_text_annotation->isChecked())
		{
			predefined_annotation = MagnaUtil::stringTokenizer(predefined_annotation, '-').at(1);
			query = "update event_report set eventstatusid = " + status + ", predefinedannotationid = " + predefined_annotation + ", updator = '" + ReviewModeFilter::ID + "', updatedtime = " + curTime+ " ";
			query += " where reportid = " + reportid + ";";
		}else
		{
			query = "update event_report set eventstatusid = " + status + ", userannotation = '" + annotation + "', predefinedannotationid = 0, updator = '" + ReviewModeFilter::ID + "', updatedtime = " + curTime+ " "; 
			query += " where reportid = " + reportid + ";";
		}

		(new Updator(query))->execute();
		this->refreshAnnotationGroup();
	}

	RETURN_NOERROR;
}


void ReviewModeFilter::transform_data(sEvent_Data *event_data, vector<string> item_list, map<string, vector<string>> containers)
{
	event_data->report_id = containers[item_list.at(0).c_str()].at(0);
	event_data->vin = containers[item_list.at(1).c_str()].at(0);
	event_data->company_id = containers[item_list.at(2).c_str()].at(0);
	event_data->clip_id = containers[item_list.at(3).c_str()].at(0);
	event_data->event_id = containers[item_list.at(4).c_str()].at(0);
	event_data->event_status_id = containers[item_list.at(5).c_str()].at(0);
	event_data->event_category_id = containers[item_list.at(6).c_str()].at(0);
	event_data->local_pc_time = containers[item_list.at(7).c_str()].at(0);
	event_data->adtf_time = containers[item_list.at(8).c_str()].at(0);
	event_data->user_annotation = containers[item_list.at(9).c_str()].at(0);
	event_data->gps_longitude = containers[item_list.at(10).c_str()].at(0);
	event_data->gps_latitude = containers[item_list.at(11).c_str()].at(0);
	event_data->grap_index = containers[item_list.at(12).c_str()].at(0);
	event_data->predefined_annotation_id = containers[item_list.at(13).c_str()].at(0);
	event_data->country_code_id = containers[item_list.at(14).c_str()].at(0);
	event_data->day_type_id = containers[item_list.at(15).c_str()].at(0);
	event_data->weather_type_id = containers[item_list.at(16).c_str()].at(0);
	event_data->road_type_id = containers[item_list.at(17).c_str()].at(0);

	event_data->clip_name = containers[item_list.at(18).c_str()].at(0);
	event_data->start_time = containers[item_list.at(19).c_str()].at(0);
	event_data->stop_time = containers[item_list.at(20).c_str()].at(0);
	event_data->local_pc_time_clip = containers[item_list.at(21).c_str()].at(0);
	event_data->fcm_sw_ver = containers[item_list.at(22).c_str()].at(0);
	event_data->mest_ver = containers[item_list.at(23).c_str()].at(0);
	event_data->adtf_ver = containers[item_list.at(24).c_str()].at(0);
	event_data->driver_name = containers[item_list.at(25).c_str()].at(0);
	event_data->gps_timestamp = containers[item_list.at(26).c_str()].at(0);
	event_data->gps_longitude_clip = containers[item_list.at(27).c_str()].at(0);
	event_data->gps_latitude_clip = containers[item_list.at(28).c_str()].at(0);
	event_data->gps_speed = containers[item_list.at(29).c_str()].at(0);
	event_data->gps_heading = containers[item_list.at(30).c_str()].at(0);
	event_data->gps_invalid_counter = containers[item_list.at(31).c_str()].at(0);
	event_data->gps_elevation = containers[item_list.at(32).c_str()].at(0);
	event_data->recording_purpose = containers[item_list.at(33).c_str()].at(0);
}

tResult ReviewModeFilter::on_btn_insert_clicked()
{

	if(this->getCurClipText().empty())
	{
		MagnaUtil::show_message("No Clip Error");
		RETURN_NOERROR;
	}

	if(this->m_oFilterGUI.cbo_project->currentText().toStdString().empty())
	{
		MagnaUtil::show_message("Select Project");
		RETURN_NOERROR;
	}

	string clip = MagnaUtil::removeURLOfFile(this->getCurClipText());
	string vin = getVinOfClip(clip);
	if(!this->existClip(clip) || vin =="")
	{
		MagnaUtil::show_message("The information regarding the clip that you are reviewing does not exist in the database");
	}
	else
	{

		this->toInsertMode(true);
		this->m_oFilterGUI.cbo_i_feature->clear();
		this->m_oFilterGUI.cbo_i_event->clear();
		this->m_oFilterGUI.cbo_i_annotation->clear();
		this->m_oFilterGUI.cbo_i_status->clear();
		this->m_oFilterGUI.txt_i_annotation->clear();
		this->m_oFilterGUI.chk_i_text_annotation->setChecked(true);
		this->on_chk_i_text_annotation_clicked();
		

		this->m_oFilterGUI.txt_m->clear();
		this->m_oFilterGUI.txt_s->clear();
		this->m_oFilterGUI.txt_ms->clear();
		this->m_oFilterGUI.txt_clip->setEnabled(false);
		this->m_oFilterGUI.txt_clip->setText(QString(clip.c_str()));
		this->m_oFilterGUI.txt_vin->setText(QString(vin.c_str()));

		this->initIAnnotationCombo(this->m_oFilterGUI.cbo_i_annotation);
		this->initStatusCombo(this->m_oFilterGUI.cbo_i_status);
		this->initFeatureCombo(this->m_oFilterGUI.cbo_i_feature);
	}

	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_cancel_clicked()
{
	this->toInsertMode(false);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_submit_clicked()
{
	string reportid = MagnaUtil::bigIntegerToString(MagnaUtil::getCurrentSystemTime());
	
	string status=m_oFilterGUI.cbo_i_status->currentText().toStdString();
	string feature =m_oFilterGUI.cbo_i_feature->currentText().toStdString();
	string eventid = m_oFilterGUI.cbo_i_event->currentText().toStdString();

	if(!status.empty()){
		status = MagnaUtil::stringTokenizer(m_oFilterGUI.cbo_i_status->currentText().toStdString(), '-').at(1);
	}
	if(!feature.empty()){
		feature = MagnaUtil::stringTokenizer(m_oFilterGUI.cbo_i_feature->currentText().toStdString(), '-').at(1);
	}

	if(!eventid.empty()){
		eventid = MagnaUtil::stringTokenizer(m_oFilterGUI.cbo_i_event->currentText().toStdString(), '-').at(1);
	}
	//string annotation = getAnnotationValue(m_oFilterGUI.chk_i_text_annotation->isChecked(), m_oFilterGUI.cbo_i_annotation, m_oFilterGUI.txt_i_annotation);
	string annotation = m_oFilterGUI.txt_i_annotation->toPlainText().toStdString();
	string predefined_annotation = this->m_oFilterGUI.cbo_i_annotation->currentText().toStdString();


	string vin = this->m_oFilterGUI.txt_vin->text().toStdString();
	string clip = MagnaUtil::removeURLOfFile(this->m_oFilterGUI.txt_clip->text().toStdString());
	string project_raw_text = this->m_oFilterGUI.cbo_project->currentText().toStdString();

	if(status.empty() || feature.empty() || vin.empty() || clip.empty() || (!this->m_oFilterGUI.chk_i_text_annotation->isChecked() && annotation.empty()) ||
		(this->m_oFilterGUI.chk_i_text_annotation->isChecked() && predefined_annotation.empty()))
	{
		MagnaUtil::show_message("Select all the mandatory(*) items");
		
	}else if(project_raw_text.empty()){
		MagnaUtil::show_message("Select Project");
	}else{
		
		string query = "select adtfstarttime, clipid from clip_info where clipname='" + clip + "'";

		vector<string> field;
		field.push_back("adtfstarttime");
		field.push_back("clipid");
		map<string, vector<string>> containers = (new Retriever(field, query))->getData();
		
		int starttime = MagnaUtil::stringTointeger(containers["adtfstarttime"].at(0));

		string clipid = containers["clipid"].at(0);

		string m = this->m_oFilterGUI.txt_m->text().toStdString();
		string s = this->m_oFilterGUI.txt_s->text().toStdString();
		string ms = this->m_oFilterGUI.txt_ms->text().toStdString();
		
		if(m.empty()) m = "00";
		if(s.empty()) s = "00";
		if(ms.empty()) ms = "000";

		int adtfTimeStamp = MagnaUtil::stringTointeger(MagnaUtil::getMicroSecond(m,s,ms));
		int adtftime = starttime + adtfTimeStamp;
		string etime = MagnaUtil::convertMicroSecondToTime(MagnaUtil::integerToString(adtfTimeStamp));
		string insert_query = "";
		string projectid = MagnaUtil::stringTokenizer(project_raw_text, '-').at(1);

		string curTime = MagnaUtil::bigIntegerToString(MagnaUtil::getCurrentSystemTime());
		if(this->m_oFilterGUI.chk_i_text_annotation->isChecked()){
			predefined_annotation = MagnaUtil::stringTokenizer(predefined_annotation, '-').at(1);

			insert_query = " insert into event_report (reportid, vin, clipid, eventcategoryid, eventid, localpctime, adtftime, eventstatusid, predefinedannotationid, projectid, updator, updatedtime, insertor, insertedtime) ";
			insert_query += " values (" + reportid + ", '" + vin + "', " + clipid + ", 3, " + eventid + ", " + "0" + ", " + MagnaUtil::integerToString(adtftime) + ", " + status + ", " +
				predefined_annotation + ", " + projectid + ", '"+ReviewModeFilter::ID+"', "+curTime+", '"+ReviewModeFilter::ID+"', "+curTime+");";
		}else
		{
			insert_query = " insert into event_report (reportid, vin, clipid, eventcategoryid, eventid, localpctime, adtftime, userannotation, eventstatusid, predefinedannotationid, projectid, updator, updatedtime, insertor, insertedtime) ";
			insert_query += " values (" + reportid + ", '" + vin + "', " + clipid + ", 3, " + eventid + ", " + "0" + ", " + MagnaUtil::integerToString(adtftime) + ", '" + annotation + "', " 
				+ status + ", 0, "+ projectid + ", '"+ReviewModeFilter::ID+"', "+curTime+", '"+ReviewModeFilter::ID+"', "+curTime+");";
		}

		(new Updator(insert_query))->execute();

		if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "More Item?", "The event is added. More item?", QMessageBox::Yes|QMessageBox::No).exec()) 
		{
			this->on_btn_insert_clicked();
		}else{
			this->on_btn_cancel_clicked();
		}

	}

	RETURN_NOERROR;
}


tResult ReviewModeFilter::on_dateEdit_changed()
{
	this->m_oFilterGUI.dateEdit->blockSignals(true);
	refreshEventGroup();
	refreshAnnotationGroup();
	this->m_oFilterGUI.dateEdit->blockSignals(false);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_dateEdit_2_changed()
{
	this->m_oFilterGUI.dateEdit_2->blockSignals(true);
	refreshEventGroup();
	refreshAnnotationGroup();
	this->m_oFilterGUI.dateEdit_2->blockSignals(false);
	RETURN_NOERROR;
}


tResult ReviewModeFilter::on_btn_showclip_clicked()
{
	string clip = this->m_oFilterGUI.lbl_clip->text().toStdString();

	if(clip.empty())
	{
		MagnaUtil::show_message("No .dat clip.");
	}
	else
	{
		bool result = this->changeDatFile();
		if(result==true)
		{
			this->saveCurrentStatus(LOGIN);
			ReviewModeFilter::isTemporalClose=true;
			MagnaUtil::restartADTF();
		}
	}

	//nResult = _runtime->SetRunLevel(IRuntime::RL_Running, &oInternEx);

	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_setting_clicked()
{
	this->toSettingMode(true);
	RETURN_NOERROR;
}


tResult ReviewModeFilter::on_btn_setting_cancel_clicked()
{
	this->sqlFileHandler->delete_temporal_database("temporal_database", "root", "hil");	
	this->initAll();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_copy_clicked()
{
	if(this->m_oFilterGUI.txt_clip_directory->text().toStdString().empty())
	{
		MagnaUtil::show_message("Please select the clip-directory");
		RETURN_NOERROR;
	}

	string from_clip = this->m_oFilterGUI.txt_clip_directory->text().toStdString() + this->m_oFilterGUI.lbl_clip->text().toStdString();
	string to_clip = CLIP_BACKUP + this->m_oFilterGUI.lbl_clip->text().toStdString();
	
	MagnaUtil::copyfile(from_clip, to_clip, this->m_pFilterWidget);

	RETURN_NOERROR;
}
tResult ReviewModeFilter::on_btn_clip_clicked()
{
	QFileDialog *fd = new QFileDialog;
	QTreeView *tree = fd->findChild <QTreeView*>();
	tree->setRootIsDecorated(true);
	tree->setItemsExpandable(true);
	tree->setExpandsOnDoubleClick(true);
	fd->setFileMode(QFileDialog::Directory);
	fd->setOption(QFileDialog::ShowDirsOnly);
	fd->setViewMode(QFileDialog::Detail);
	int result = fd->exec();
	QString directory;
	if (result)
	{
		directory = fd->selectedFiles()[0] + "/";
		this->m_oFilterGUI.txt_clip_directory->setText(directory);
	}

	fd->close();
	tree->close();
	RETURN_NOERROR;
}
tResult ReviewModeFilter::on_btn_browse_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this->m_pFilterWidget,
		tr("Open Text file"), "", tr(".sql files (*.sql)"));
	this->m_oFilterGUI.txt_sqlfile->setText(fileName);
	this->m_oFilterGUI.btn_execute->setEnabled(true);

	RETURN_NOERROR;
}


tResult ReviewModeFilter::on_btn_execute_clicked()
{

	string sql = this->m_oFilterGUI.txt_sqlfile->text().toStdString();

	if(sql.empty()){
		MagnaUtil::show_message("Please fill in all the text boxes.");
		RETURN_NOERROR;
	}

	if(!MagnaUtil::isExist(sql))
	{
		MagnaUtil::show_message("The selected .sql file does not exist.");
		RETURN_NOERROR;
	}

	if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "CAUTION!!", "If yes, all the data in your database will be removed", QMessageBox::Yes|QMessageBox::No).exec()) 
	{
		QProgressDialog *progress = new QProgressDialog(this->m_pFilterWidget);
		progress->setWindowTitle(QString("Copying SQL File to Database..."));
		progress->autoClose();
		string fileName = MagnaUtil::removeURLOfFile(sql);
		fileName += " is copying...";
		progress->setLabelText(QString(fileName.c_str()));
		progress->setFixedWidth(500);
		progress->setWindowModality(Qt::WindowModal);
		progress->setRange(0,100);
		progress->show();
		
		QApplication::processEvents();
		
		progress->setLabelText(QString("Init Temporal Database ..."));


		this->sqlFileHandler->init_temporal_database("temporal_database", "root", "hil", sql, false);
		progress->setValue(10);
		progress->setLabelText(QString("Integrity Checking..."));
		bool integrity = this->sqlFileHandler->check_integrity(DB_SCHEME, "temporal_database");

		progress->setValue(20);

		if(integrity==true)
		{	
			string cur_dir = MagnaUtil::getCurPath();
			string event_report_temp_url = cur_dir + "/review_workspace/event_report.zuna";
			string clip_info_temp_url = cur_dir + "/review_workspace/clip_info.zuna";

			this->sqlFileHandler->insertNewRecords(this->m_pFilterWidget, "event_report", event_report_temp_url);
			progress->setValue(40);
			this->sqlFileHandler->insertNewRecords(this->m_pFilterWidget, "clip_info", clip_info_temp_url);
			progress->setValue(80);
			MagnaUtil::delete_file(TEMP_FILE + "tmp.xml");
			this->sqlFileHandler->delete_temporal_database("temporal_database", "root", "hil");	
			MagnaUtil::restartADTF();
		}

		progress->hide();
	}

	RETURN_NOERROR;
}

tResult ReviewModeFilter::send_data(cOutputPin *outpin, sEvent_Data reviewed_data)
{
	cObjectPtr<IMediaSample> pNewSample;
	RETURN_IF_FAILED(AllocMediaSample((tVoid**)&pNewSample));
	RETURN_IF_FAILED(pNewSample->Update(_clock->GetStreamTime(), &reviewed_data, sizeof(sEvent_Data), 0));
	RETURN_IF_FAILED(outpin->Transmit(pNewSample));

	RETURN_NOERROR;
}


tResult ReviewModeFilter::on_btn_LR1_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listFeature, this->featureModel, m_oFilterGUI.listFeatureSelected, this->selectedFeatureModel);
	this->refreshEventGroup();
	this->refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_RL1_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listFeatureSelected, this->selectedFeatureModel, m_oFilterGUI.listFeature, this->featureModel);
	refreshEventGroup();
	refreshAnnotationGroup();
	RETURN_NOERROR;
}


tResult ReviewModeFilter::on_btn_LR2_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listEvent, this->eventModel, m_oFilterGUI.listEventSelected, this->selectedEventModel);
	refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_RL2_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listEventSelected, this->selectedEventModel, m_oFilterGUI.listEvent, this->eventModel);
	refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_LR3_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listAnnotation, this->annotationModel, m_oFilterGUI.listAnnotationSelected, this->selectedAnnotationModel);
	refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_RL3_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listAnnotationSelected, this->selectedAnnotationModel, m_oFilterGUI.listAnnotation, this->annotationModel);
	refreshAnnotationGroup();
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_date_clicked()
{
	if(!m_oFilterGUI.chk_date->isChecked())
	{
		m_oFilterGUI.dateEdit->setEnabled(true);
		m_oFilterGUI.dateEdit_2->setEnabled(true);
	}else{
		m_oFilterGUI.dateEdit->setEnabled(false);
		m_oFilterGUI.dateEdit_2->setEnabled(false);
	}

	refreshEventGroup();
	refreshAnnotationGroup();
	RETURN_NOERROR;
}


int ReviewModeFilter::checkNumber(QLineEdit* edit, int digits)
{

	QString str = edit->text();
	if(str.size()==0) return 1;

	bool ok;
	str.toInt(&ok, 10);
	if(!ok)
	{
		MagnaUtil::show_message("Only numbers are allowed!");
		edit->setFocus(Qt::OtherFocusReason);
		edit->selectAll();
		return 0;
	}

	if(str.size() > digits)
	{
		MagnaUtil::show_message("Exceeding the number of digits!");
		edit->setFocus(Qt::OtherFocusReason);
		edit->selectAll();
		return 0;
	}

	return 1;
}
string ReviewModeFilter::getAnnotationValue(bool is_text, QComboBox* combo_box, QTextEdit* text_edit)
{
	string annotation = "";
	if(is_text){
		annotation = combo_box->currentText().toStdString();
	}else{
		annotation = text_edit->toPlainText().toStdString();
	}

	return annotation;
}

bool ReviewModeFilter::changeDatFile()
{

	if(this->m_oFilterGUI.txt_clip_directory->text().toStdString().empty())
	{
		MagnaUtil::show_message("Please select the clip-directory");
	}

	string dat = this->m_oFilterGUI.txt_clip_directory->text().toStdString() + this->m_oFilterGUI.lbl_clip->text().toStdString();
	if (!QFile().exists(QString(dat.c_str())))
	{
		MagnaUtil::show_message("No .dat clip.");
		return false;
	}else{
		string att_name = "idref";
		string att_value = "adtf.stg.harddisk_player";

		XMLHandler *t = new XMLHandler(SYSTEM_RELEASE);
		t->update("adtf:project", "configurations", att_name, att_value, dat);

		return true;
	}
}

void ReviewModeFilter::toTextAnnotationMode(bool mode)
{
	this->m_oFilterGUI.txt_annotation->setVisible(!mode);
	this->m_oFilterGUI.cbo_annotation->setVisible(mode);
}

void ReviewModeFilter::toITextAnnotationMode(bool mode)
{
	this->m_oFilterGUI.txt_i_annotation->setVisible(!mode);
	this->m_oFilterGUI.cbo_i_annotation->setVisible(mode);
}

void ReviewModeFilter::toAnnotationCategoryMode(bool mode)
{
	this->m_oFilterGUI.chk_annotation->setChecked(mode);
	this->m_oFilterGUI.listAnnotation->setEnabled(mode);
	this->m_oFilterGUI.listAnnotationSelected->setEnabled(mode);
	this->m_oFilterGUI.btn_LR3->setEnabled(mode);
	this->m_oFilterGUI.btn_RL3->setEnabled(mode);
}

void ReviewModeFilter::toLoginMode(bool mode)
{
	
	this->m_oFilterGUI.GrpEdit->setVisible(!mode);
	this->m_oFilterGUI.GrpFeature->setVisible(!mode);
	this->m_oFilterGUI.GrpEvent->setVisible(!mode);
	this->m_oFilterGUI.grp_login->setVisible(mode);
	this->m_oFilterGUI.GrpInsert->setVisible(!mode);
	this->m_oFilterGUI.GrpSetting->setVisible(!mode);
	this->m_oFilterGUI.AnnotationFilter->setVisible(!mode);
	this->m_oFilterGUI.btn_clip->setVisible(!mode);
	this->m_oFilterGUI.txt_clip_directory->setVisible(!mode);
	this->m_oFilterGUI.btn_setting->setVisible(!mode);
	this->m_oFilterGUI.txt_loginpw->setEchoMode(QLineEdit::Password);
}

void ReviewModeFilter::toSearchAnnotationMode(bool mode)
{
	this->m_oFilterGUI.chk_search->setChecked(mode);
	this->m_oFilterGUI.txt_search->clear();
	this->m_oFilterGUI.txt_search->setVisible(mode);
	if(mode && this->m_oFilterGUI.chk_annotation->isChecked())
	{
		this->m_oFilterGUI.chk_annotation->setChecked(false);
		this->on_chk_annotation_clicked();
	}
}

void ReviewModeFilter::toSettingMode(bool mode)
{
	this->m_oFilterGUI.GrpSetting->setVisible(mode);
	if(this->m_oFilterGUI.GrpInsert->isVisible()) this->on_btn_cancel_clicked();
	this->m_oFilterGUI.GrpFeature->setEnabled(!mode);
	this->m_oFilterGUI.GrpEvent->setEnabled(!mode);
	this->m_oFilterGUI.GrpEdit->setEnabled(!mode);
	this->m_oFilterGUI.btn_setting->setEnabled(!mode);
	this->m_oFilterGUI.btn_browse->setFocus(Qt::OtherFocusReason);
}

void ReviewModeFilter::toInsertMode(bool mode)
{
	//this->toITextAnnotationMode(mode);
	this->m_oFilterGUI.GrpInsert->setVisible(mode);
	this->m_oFilterGUI.GrpFeature->setEnabled(!mode);
	this->m_oFilterGUI.GrpEvent->setEnabled(!mode);
	this->m_oFilterGUI.GrpEdit->setEnabled(!mode);
	this->m_oFilterGUI.btn_clip->setEnabled(!mode);
	this->m_oFilterGUI.btn_setting->setVisible(!mode);
	if(mode==true) this->m_oFilterGUI.txt_m->setFocus(Qt::OtherFocusReason);
}

void ReviewModeFilter::restoreCurrentStatus(const char* tmpFile)
{
	this->m_oFilterGUI.lbl_reportid->setVisible(false);
	this->m_oFilterGUI.lbl_table->setVisible(false);
	this->m_oFilterGUI.txt_annotation->setVisible(false);
	XMLHandler *xmlHandle = new XMLHandler(tmpFile);

	vector<string> feature_list = xmlHandle->getNodeListAtSecondLevel("feature_list");
	vector<string> selected_feature_list = xmlHandle->getNodeListAtSecondLevel("selected_feature_list");
	vector<string> event_list = xmlHandle->getNodeListAtSecondLevel("event_list");
	vector<string> selected_event_list = xmlHandle->getNodeListAtSecondLevel("selected_event_list");
	vector<string> annotation_list = xmlHandle->getNodeListAtSecondLevel("annotation_list");
	vector<string> selected_annotation_list = xmlHandle->getNodeListAtSecondLevel("selected_annotation_list");
	vector<string> event_annotation_list = xmlHandle->getNodeListAtSecondLevel("event_annotation_list");
	vector<string> table_list = xmlHandle->getNodeListAtSecondLevel("table_list");
	vector<string> project_list = xmlHandle->getNodeListAtSecondLevel("project_list");

	string clip_directory = xmlHandle->getNodeValue("clip_directory", "value");
	string selected_event = xmlHandle->getNodeValue("selected_event", "value");
	string selected_project = xmlHandle->getNodeValue("selected_project", "value");
	string chk_date = xmlHandle->getNodeValue("date_check", "value");
	string chk_annotation = xmlHandle->getNodeValue("annotation_check", "value");
	string chk_text_annotation = xmlHandle->getNodeValue("text_annotation_check", "value");
	string text_annotation = xmlHandle->getNodeValue("text_annotation", "value");

	string start_date = xmlHandle->getNodeValue("start_date", "value");
	string end_date = xmlHandle->getNodeValue("end_date", "value");

	combo_handle->initCombo(this->m_oFilterGUI.cbo_project, project_list);
	int index_project = m_oFilterGUI.cbo_project->findText(selected_project.c_str());
	this->m_oFilterGUI.cbo_project->setCurrentIndex(index_project);


	this->initAnnotationCombo(this->m_oFilterGUI.cbo_annotation, selected_feature_list);
	this->listhandle->addItems(this->m_oFilterGUI.listFeature, this->featureModel, feature_list);
	this->listhandle->addItems(this->m_oFilterGUI.listFeatureSelected, this->selectedFeatureModel, selected_feature_list);
	this->listhandle->addItems(this->m_oFilterGUI.listEvent, this->eventModel, event_list);
	this->listhandle->addItems(this->m_oFilterGUI.listEventSelected, this->selectedEventModel, selected_event_list);
	this->listhandle->addItems(this->m_oFilterGUI.listAnnotation, this->annotationModel, annotation_list);
	this->listhandle->addItems(this->m_oFilterGUI.listAnnotationSelected, this->selectedAnnotationModel, selected_annotation_list);
	this->listhandle->addItems(this->m_oFilterGUI.listEventAnnotation, this->eventListModel, event_annotation_list);

	string str_true = "true";
	if(chk_date.compare(str_true)==0){
		this->m_oFilterGUI.chk_date->setChecked(true);
		this->m_oFilterGUI.dateEdit->setEnabled(false);
		this->m_oFilterGUI.dateEdit_2->setEnabled(false);

		QDate s_date = QDate::fromString(QString("2015/02/01"), "yyyy/MM/dd");
		QDate e_date = QDate::fromString(QString("2015/03/06"), "yyyy/MM/dd");

		m_oFilterGUI.dateEdit->setDate(s_date);
		m_oFilterGUI.dateEdit_2->setDate(e_date);

	}else {
		this->m_oFilterGUI.chk_date->setChecked(false);
		this->m_oFilterGUI.dateEdit->setEnabled(true);
		this->m_oFilterGUI.dateEdit_2->setEnabled(true);

		QDate s_date = QDate::fromString(QString(start_date.c_str()), "yyyy/MM/dd");
		QDate e_date = QDate::fromString(QString(end_date.c_str()), "yyyy/MM/dd");

		m_oFilterGUI.dateEdit->setDate(s_date);
		m_oFilterGUI.dateEdit_2->setDate(e_date);
	}

	if(chk_text_annotation.compare(str_true)==0)
	{
		this->m_oFilterGUI.txt_annotation->setText(QString(text_annotation.c_str()));
		this->toITextAnnotationMode(true);
	}else
	{
		this->toITextAnnotationMode(false);
	}
	
	if(chk_annotation.compare(str_true)==0)
	{	
		this->toAnnotationCategoryMode(true);
	}else
	{
		this->toAnnotationCategoryMode(false);
	}

	for(unsigned int idx = 0 ; idx < table_list.size() ; idx++)
	{
		string tmp = table_list.at(idx);
		string eyeq = "eyeq";
		string fcm = "fcm";
		string user = "user";

		if(tmp.compare(eyeq)==0) this->m_oFilterGUI.chk_eyeq_event->setChecked(true);
		if(tmp.compare(fcm)==0) this->m_oFilterGUI.chk_fcm_event->setChecked(true);
		if(tmp.compare(user)==0) this->m_oFilterGUI.chk_user_event->setChecked(true);
	}

	if(!selected_event.empty()){
		QModelIndex idx = this->listhandle->getSelectedItemIndex(this->m_oFilterGUI.listEventAnnotation, selected_event);
		this->m_oFilterGUI.listEventAnnotation->setCurrentIndex(idx);
		this->on_list_event_annotation_clicked(idx);
	}

	this->m_oFilterGUI.txt_clip_directory->setText(QString(clip_directory.c_str()));
	saveCurrentStatus(LOGOUT);
}


void ReviewModeFilter::saveCurrentStatus(string status)
{

	bool datechecker=this->m_oFilterGUI.chk_date->isChecked();
	bool text_annotation_checker= this->m_oFilterGUI.chk_text_annotation->isChecked();
	bool annotation_checker= this->m_oFilterGUI.chk_annotation->isChecked();

	string sdate;
	string edate;
	string text_annotation;
	string clip_directory;
	string selected_project;
	vector<string> project_list;
	vector<string> table;
	vector<string> featureList;
	vector<string> selectedFeatureList;
	vector<string> eventList;
	vector<string> selectedEventList;
	vector<string> annotationList;
	vector<string> selectedAnnotationList;
	vector<string> eventAnnotationList;
	string selectedEvent;
	
	if(this->m_oFilterGUI.chk_eyeq_event->isChecked()) table.push_back("eyeq");
	if(this->m_oFilterGUI.chk_fcm_event->isChecked()) table.push_back("fcm");
	if(this->m_oFilterGUI.chk_user_event->isChecked()) table.push_back("user");
	
	text_annotation = this->m_oFilterGUI.txt_annotation->toPlainText().toStdString();
	clip_directory = this->m_oFilterGUI.txt_clip_directory->text().toStdString();

	sdate = this->getDate(this->m_oFilterGUI.dateEdit, "/");
	edate = this->getDate(this->m_oFilterGUI.dateEdit_2, "/");
	selected_project = this->m_oFilterGUI.cbo_project->currentText().toStdString();

	for(int idx = 0 ; idx < this->m_oFilterGUI.cbo_project->count() ; idx++)
		project_list.push_back(this->m_oFilterGUI.cbo_project->itemText(idx).toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listFeature->model()->rowCount() ; idx++)
		featureList.push_back(this->m_oFilterGUI.listFeature->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listFeatureSelected->model()->rowCount() ; idx++)
		selectedFeatureList.push_back(this->m_oFilterGUI.listFeatureSelected->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listEvent->model()->rowCount() ; idx++)
		eventList.push_back(this->m_oFilterGUI.listEvent->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listEventSelected->model()->rowCount() ; idx++)
		selectedEventList.push_back(this->m_oFilterGUI.listEventSelected->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listAnnotation->model()->rowCount() ; idx++)
		annotationList.push_back(this->m_oFilterGUI.listAnnotation->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());
	
	for(int idx = 0 ; idx < this->m_oFilterGUI.listAnnotationSelected->model()->rowCount() ; idx++)
		selectedAnnotationList.push_back(this->m_oFilterGUI.listAnnotationSelected->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listEventAnnotation->model()->rowCount() ; idx++)
		eventAnnotationList.push_back(this->m_oFilterGUI.listEventAnnotation->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());
	
	vector<string> v_selectedEventList = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listEventAnnotation, this->eventListModel);
	if(!v_selectedEventList.empty())
		selectedEvent = v_selectedEventList.at(0);


	XMLHandler xmlHandle(TEMP_FILE + "tmp.xml");
	xmlHandle.open();
	xmlHandle.addHeader("backup");

	xmlHandle.addItem("clip_directory", "value", clip_directory);
	xmlHandle.addItem("date_check", "value", !datechecker);
	xmlHandle.addItem("text_annotation_check", "value", !text_annotation_checker);
	xmlHandle.addItem("annotation_check", "value", !annotation_checker);
	xmlHandle.addItem("text_annotation", "value", text_annotation);
	xmlHandle.addItem("start_date", "value", sdate);
	xmlHandle.addItem("end_date", "value", edate);
	xmlHandle.addItem("selected_event", "value", selectedEvent);
	xmlHandle.addItem("selected_project", "value", selected_project);
	if(status==LOGIN) xmlHandle.addItem("logid", "value", ReviewModeFilter::USERLOG_ID);
	else
	{
		string id = "";
		xmlHandle.addItem("logid", "value", id);
	}
	if(status==LOGIN) xmlHandle.addItem("login", "value", ReviewModeFilter::ID);
	else
	{
		string id = "";
		xmlHandle.addItem("login", "value", id);
	}
	

	xmlHandle.addItems("project_list", "item", project_list);
	xmlHandle.addItems("feature_list", "item", featureList);
	xmlHandle.addItems("selected_feature_list", "item", selectedFeatureList);
	xmlHandle.addItems("event_list", "item", eventList);
	xmlHandle.addItems("selected_event_list", "item", selectedEventList);
	xmlHandle.addItems("annotation_list", "item", annotationList);
	xmlHandle.addItems("selected_annotation_list", "item", selectedAnnotationList);
	xmlHandle.addItems("event_annotation_list", "item", eventAnnotationList);
	xmlHandle.addItems("table_list", "item", table);

	xmlHandle.addFooter("backup");
	xmlHandle.close();
}

void ReviewModeFilter::refreshEventGroup()
{

	string cbo_project_text = this->m_oFilterGUI.cbo_project->currentText().toStdString();
	
	if(cbo_project_text.empty()==true)
	{
		this->initModel();
	}else{

		QProgressDialog *progress = new QProgressDialog(this->m_pFilterWidget);
		progress->setWindowTitle(QString("Initializing..."));
		progress->autoClose();
		progress->setFixedWidth(500);
		progress->setWindowModality(Qt::WindowModal);
		progress->setRange(0,100);
		progress->show();
		QApplication::processEvents();
		progress->setLabelText(QString("Init Event Filter Panel ..."));

		this->m_oFilterGUI.cbo_annotation->clear();
		eventModel = new QStandardItemModel();
		selectedEventModel = new QStandardItemModel();
		annotationModel = new QStandardItemModel();
		selectedAnnotationModel = new QStandardItemModel();

		this->m_oFilterGUI.listEvent->setModel(this->eventModel);
		this->m_oFilterGUI.listEventSelected->setModel(this->selectedEventModel);
		this->m_oFilterGUI.listAnnotation->setModel(this->annotationModel);
		this->m_oFilterGUI.listAnnotationSelected->setModel(this->selectedAnnotationModel);
		this->m_oFilterGUI.chk_search->setChecked(false);
		this->on_chk_search_clicked();
		string stime, etime;
		vector<string> vstr_fetures = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listFeatureSelected, 1);	

		string txt_project = this->m_oFilterGUI.cbo_project->currentText().toStdString();
		string projectid = MagnaUtil::stringTokenizer(txt_project, '-').at(1);
		progress->setValue(20);

		if(!this->m_oFilterGUI.chk_date->isChecked())
		{
			vector<string> dateRange= getDateRange();
			stime = dateRange.at(0);
			etime = dateRange.at(1);
		}

		vector<string> categories = this->getEventCategories();

		if(vstr_fetures.size()>0)
		{
			string query = this->queryFactory->getEventTypeQuery(this->getListField4Event(), " event_list a, event_report b", categories, stime, etime, projectid, vstr_fetures);
			this->listhandle->addItemsFromDB(m_oFilterGUI.listEvent, eventModel, this->getListField4Event(), query);
			progress->setValue(90);
		}
		
		string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, projectid);
		if(!query4Annotation.empty())
		{
			this->listhandle->addItemsFromDB(m_oFilterGUI.listAnnotation, annotationModel, this->getListField4Annotation(), query4Annotation);
			progress->setValue(100);
		}


		progress->hide();
	}


}

void ReviewModeFilter::refreshAnnotationGroup()
{
	if(this->m_oFilterGUI.cbo_project->currentText().toStdString().empty()==false)
	{

		QProgressDialog *progress = new QProgressDialog(this->m_pFilterWidget);
		progress->setWindowTitle(QString("Initializing..."));
		progress->autoClose();
		progress->setFixedWidth(500);
		progress->setWindowModality(Qt::WindowModal);
		progress->setRange(0,100);
		progress->show();
		QApplication::processEvents();
		progress->setLabelText(QString("Init Review Panel ..."));


		this->initEventListModel();
		this->m_oFilterGUI.lbl_table->setText(QString(""));
		this->m_oFilterGUI.lbl_reportid->setText(QString(""));
		this->m_oFilterGUI.listEventAnnotation->setModel(this->eventListModel);
		vector<string> events = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listEventSelected, 1);

		vector<string> vstr_fetures = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listFeatureSelected, 1);	
		vector<string> annotations = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listAnnotationSelected, 1);
		string projectid = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_project->currentText().toStdString(), '-').at(1);
		string search_condition = this->m_oFilterGUI.txt_search->text().toStdString();
		bool chk_search = this->m_oFilterGUI.chk_search->isChecked();

		bool chk_annotation = this->m_oFilterGUI.chk_annotation->isChecked();
		this->m_oFilterGUI.chk_text_annotation->setChecked(true);
		this->on_chk_text_annotation_clicked();
		this->m_oFilterGUI.txt_annotation->clear();

		string stime, etime;

		progress->setValue(30);
		if(!this->m_oFilterGUI.chk_date->isChecked())
		{
			vector<string> dateRange= getDateRange();
			stime = dateRange.at(0);
			etime = dateRange.at(1);
		}

		this->m_oFilterGUI.lbl_status->setText("");
		this->m_oFilterGUI.lbl_clip->setText("");
		this->m_oFilterGUI.lbl_adtftime->setText("");
		this->m_oFilterGUI.cbo_status->setCurrentIndex(0);
		this->m_oFilterGUI.cbo_annotation->setCurrentIndex(0);

		progress->setValue(40);
		int recordsize =0;
		if(events.size()==0 || (annotations.size()==0 && chk_annotation)){

			this->m_oFilterGUI.lbl_status->setText("");

		}else{

			vector<string> event_categories = getEventCategories();
			string query = this->queryFactory->getEventListQuery(
				this->getListField4EventList(), ReviewModeFilter::ID , projectid, events, stime, etime, event_categories, annotations, search_condition, chk_search);
			
			progress->setValue(50);
			recordsize += this->listhandle->addItemsFromDB(m_oFilterGUI.listEventAnnotation, eventListModel, this->getListField4EventList(), query);
		}

		progress->setValue(90);
		this->initAnnotationCombo(this->m_oFilterGUI.cbo_annotation);
		QString str = QVariant(recordsize).toString() + " events have been selected.";
		this->m_oFilterGUI.lbl_status->setText(str);
		this->saveCurrentStatus(LOGOUT);

		progress->hide();
	}
}


void ReviewModeFilter::refreseInsertPanel()
{
	this->initEventCombo(this->m_oFilterGUI.cbo_i_event, true);
	this->initIAnnotationCombo(this->m_oFilterGUI.cbo_i_annotation);
}

vector<string> ReviewModeFilter::getEventCategories()
{
	vector<string> categories;
	if(this->m_oFilterGUI.chk_fcm_event->isChecked()) categories.push_back("1");
	if(this->m_oFilterGUI.chk_eyeq_event->isChecked()) categories.push_back("2");
	if(this->m_oFilterGUI.chk_user_event->isChecked()) categories.push_back("3");
	if(this->m_oFilterGUI.chk_radar->isChecked()) categories.push_back("4");

	return categories;

}

vector<string> ReviewModeFilter::getDateRange(){
	vector<string> dateRange;
	dateRange.push_back(this->getDate(this->m_oFilterGUI.dateEdit, "-"));
	dateRange.push_back(this->getDate(this->m_oFilterGUI.dateEdit_2, "-"));

	return dateRange;
}

string ReviewModeFilter::getDate(QDateEdit* edit, string comp)
{
	string year = MagnaUtil::integerToString(edit->date().year());
	string month = MagnaUtil::integerToString(edit->date().month());
	if(edit->date().month()<10) month = "0" + month;
	string day = MagnaUtil::integerToString(edit->date().day());
	if(edit->date().day()<10) day = "0" + day;
	return year+comp+month+comp+day;
}

bool ReviewModeFilter::existClip(string clip)
{
	string query = "select adtfstarttime, clipid from clip_info where clipname='" + clip + "'";
	vector<string> field;
	field.push_back("adtfstarttime");
	field.push_back("clipid");
	map<string, vector<string>> containers = (new Retriever(field, query))->getData();

	return !(containers["adtfstarttime"].size()<1);
}

string ReviewModeFilter::getVinOfClip(string clip)
{
	string query = "select vin from clip_info where clipname='" + clip + "'";
	vector<string> field;
	field.push_back("vin");
	map<string, vector<string>> containers = (new Retriever(field, query))->getData();

	if(containers["vin"].size()>0)
		return containers["vin"].at(0);
	else return "";
}


vector<string> ReviewModeFilter::getListField4Feature()
{
	vector<string> itemsFromDB;
	itemsFromDB.push_back("name");
	itemsFromDB.push_back("id");

	return itemsFromDB;
}

vector<string> ReviewModeFilter::getListField4Annotation()
{
	vector<string> itemsFromDB;
	itemsFromDB.push_back("name");
	itemsFromDB.push_back("id");

	return itemsFromDB;
}


vector<string> ReviewModeFilter::getListField4EventList()
{
	vector<string> itemsFromDB;


	itemsFromDB.push_back("eventcategoryid");
	itemsFromDB.push_back("reportid");
	itemsFromDB.push_back("eventid");
	itemsFromDB.push_back("b.name");

	return itemsFromDB;
}


vector<string> ReviewModeFilter::getField4EventEdit()
{
	vector<string> itemsFromDB;
	
	itemsFromDB.push_back("reportid");
	itemsFromDB.push_back("clipname");
	itemsFromDB.push_back("adtfstarttime");
	itemsFromDB.push_back("adtftime");
	itemsFromDB.push_back("userannotation");
	itemsFromDB.push_back("predefinedannotationid");
	itemsFromDB.push_back("eventstatusid");
	itemsFromDB.push_back("name");
	itemsFromDB.push_back("annotation");

	return itemsFromDB;
}

vector<string> ReviewModeFilter::getListField4Event()
{
	vector<string> itemsFromDB;
	itemsFromDB.push_back("a.name");
	itemsFromDB.push_back("a.id");

	return itemsFromDB;
}

vector<string> ReviewModeFilter::getField4Status()
{
	vector<string> itemsFromDB;
	itemsFromDB.push_back("name");
	itemsFromDB.push_back("id");

	return itemsFromDB;
}

vector<string> ReviewModeFilter::getListField4EventAll()
{
	vector<string> itemsFromDB;
	itemsFromDB.push_back("reportid");
	itemsFromDB.push_back("vin");
	itemsFromDB.push_back("projectid");
	itemsFromDB.push_back("clipid");
	itemsFromDB.push_back("eventid");
	itemsFromDB.push_back("eventstatusid");
	itemsFromDB.push_back("eventcategoryid");
	itemsFromDB.push_back("localpctime");
	itemsFromDB.push_back("adtftime");
	itemsFromDB.push_back("userannotation");
	itemsFromDB.push_back("gpslongitude");
	itemsFromDB.push_back("gpslatitude");
	itemsFromDB.push_back("grapindex");
	itemsFromDB.push_back("predefinedannotationid");
	itemsFromDB.push_back("countrycodeid");
	itemsFromDB.push_back("daytypeid");
	itemsFromDB.push_back("weathertypeid");
	itemsFromDB.push_back("roadtypeid");
	itemsFromDB.push_back("clipname");
	itemsFromDB.push_back("adtfstarttime");
	itemsFromDB.push_back("adtfstoptime");
	itemsFromDB.push_back("localpctime");
	itemsFromDB.push_back("fcmswver");
	itemsFromDB.push_back("mestver");
	itemsFromDB.push_back("adtfver");
	itemsFromDB.push_back("drivername");
	itemsFromDB.push_back("gpstimestamp");
	itemsFromDB.push_back("gpslongitude");
	itemsFromDB.push_back("gpslatitude");
	itemsFromDB.push_back("gpsspeed");
	itemsFromDB.push_back("gpsheading");
	itemsFromDB.push_back("gpsinvalidcounter");
	itemsFromDB.push_back("gpselevation");
	itemsFromDB.push_back("recordingpurpose");
	return itemsFromDB;
}


//SLOTS