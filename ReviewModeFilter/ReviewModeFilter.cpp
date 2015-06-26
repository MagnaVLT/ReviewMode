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
#include <QtGui/QShortcut>
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
extern const std::string PLAY_LIST_HOME = "./hil_playback_playlist/";
extern const __int64 th = 180;

string ReviewModeFilter::USERLOG_ID = "";
bool ReviewModeFilter::isTemporalClose = false;
string ReviewModeFilter::ID = "";
map<string, string> ReviewModeFilter::localtime_clipid_map;

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

void ReviewModeFilter::initAllAtStart()
{
	
	this->initProjectModel();
	this->initFeatureModel();
	this->initAI();
	this->initModel();
	this->initMode();
	this->initStatusCombo(this->m_oFilterGUI.cbo_status);
	this->initWorkspaceDirectory();

	string tmpFile = TEMP_FILE + "tmp.xml";
	this->sqlFileHandler = new SQLFileHandler(TEMP_FILE, ID);
	this->m_oFilterGUI.txt_directory->setText(QString(""));

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


void ReviewModeFilter::initBasicGroup()
{
	QProgressDialog *progress = new QProgressDialog(this->m_pFilterWidget);
	progress->setWindowTitle(QString("Initializing VIN and Clustering Collections..."));
	progress->autoClose();
	progress->setLabelText(QString("Initializing VIN and Clustering Collections..."));
	progress->setFixedWidth(500);
	progress->setWindowModality(Qt::WindowModal);
	progress->setRange(0,100);
	progress->show();

	this->initVINList();
	progress->setValue(30);
	this->initCollectionCombo(this->m_oFilterGUI.cbo_collection);
	progress->setValue(80);

	progress->hide();
}

void ReviewModeFilter::initCollectionCombo(QComboBox *combobox)
{
	map<string, vector<string>> list_container = getClipIDList();
	vector<string> clipidList = this->getClustersOfClips(list_container["clipid"], list_container["localpctime"]);
	combo_handle->initCombo(combobox, clipidList);
	if(this->m_oFilterGUI.chk_tour->isChecked()==true) combobox->setEnabled(false);
	else combobox->setEnabled(true);

}

vector<string> ReviewModeFilter::getClustersOfClips(vector<string> clipidList, vector<string> localpctime_list)
{

	vector<string> collection;
	if(clipidList.size()==0){
		return collection;
	}

	bool isNew = true;
	string cur_start_clip_id;
	string cur_end_clip_id;
	string cur_start_localtime_ymd;
	string cur_start_localtime;
	string cur_end_localtime_ymd;
	string cur_end_localtime;

	int cnt = 1;

	for(unsigned int i = 0 ; i < clipidList.size()-1 ; i++)
	{
		if(isNew == true) {
			cur_start_clip_id = clipidList.at(i);
			cur_start_localtime = MagnaUtil::convertSystemTimeToTime(localpctime_list.at(i));
			
			isNew = false;
		}

		__int64 diff = MagnaUtil::convertSystemTimeToSecond(clipidList.at(i+1)) - MagnaUtil::convertSystemTimeToSecond(clipidList.at(i));
		
		if(diff>=th){
			cur_end_clip_id = clipidList.at(i);
			cur_end_localtime = MagnaUtil::convertSystemTimeToTime(localpctime_list.at(i));

			string cur_text = "tour " + MagnaUtil::integerToString(cnt) + "-" + ":" + cur_start_localtime + "-" + cur_end_localtime;
			string cur_value = cur_start_clip_id + "-" + cur_end_clip_id;
			++cnt;
			collection.push_back(cur_text);
			ReviewModeFilter::localtime_clipid_map[cur_text] = string(cur_value);

			isNew = true;
		}else{
			cur_end_clip_id = clipidList.at(i);
		}
	}

	cur_end_clip_id = clipidList.at(clipidList.size()-1);
	cur_end_localtime = MagnaUtil::convertSystemTimeToTime(localpctime_list.at(localpctime_list.size()-1));
	string cur_value = cur_start_clip_id + "-" + cur_end_clip_id;

	string cur_text = "tour " + MagnaUtil::integerToString(cnt) + "-" + ":" + cur_start_localtime + "-" + cur_end_localtime;

	collection.push_back(cur_text);
	ReviewModeFilter::localtime_clipid_map[cur_text] = string(cur_value);



	return collection;
}

map<string, vector<string>> ReviewModeFilter::getClipIDList()
{

	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	
	vector<string> vin_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listVinSelected, 0);
	if(!project_id.empty()){

		vector<string> collection_fields;
		collection_fields.push_back("clipid");
		collection_fields.push_back("localpctime");

		string query = "select a.clipid as clipid, d.localpctime as localpctime from event_report a, event_list b, users_feature_project_map c, clip_info d ";
		query += " where a.eventid = b.id and c.featureid = b.FeatureID and a.projectid = c.projectid and a.clipid = d.clipid ";
		query += " and c.UserID = '"+ReviewModeFilter::ID+"' ";
		query = this->queryFactory->addFieldsViaInStatement("c.projectID", project_id, query, 2, false);
		if(!vin_id.empty()) query = this->queryFactory->addFieldsViaInStatement("a.vin", vin_id, query, 2, true);
		query += " group by a.clipid order by a.clipid;";

		map<string, vector<string>> collectionContainer = (new Retriever( collection_fields, query))->getData();

		return collectionContainer;
	}else{
		map<string, vector<string>> collectionContainer;
		vector<string> emptyVector;
		collectionContainer["clipid"] = emptyVector;
		collectionContainer["localpctime"] = emptyVector;

		return collectionContainer;
	}
}

void ReviewModeFilter::initEventGroup()
{
	this->initModel();
	this->m_oFilterGUI.chk_date->setChecked(true);
	this->m_oFilterGUI.chk_eyeq_event->setChecked(false);
	this->m_oFilterGUI.chk_fcm_event->setChecked(false);
	this->m_oFilterGUI.chk_user_event->setChecked(false);
	this->m_oFilterGUI.chk_radar->setChecked(false);
}

void ReviewModeFilter::initAnnotationGroup()
{
	this->initEventListModel();
	this->m_oFilterGUI.lbl_table->setText(QString(""));
	this->m_oFilterGUI.lbl_reportid->setText(QString(""));
	this->m_oFilterGUI.listEventAnnotation->setModel(this->eventListModel);
	this->m_oFilterGUI.lbl_adtftime->setText(QString(""));
	this->m_oFilterGUI.lbl_clip->setText(QString(""));
	this->m_oFilterGUI.chk_tour->setChecked(true);
}

void ReviewModeFilter::initMode()
{
	this->m_oFilterGUI.txt_clip_directory->setEnabled(false);
	this->m_oFilterGUI.grp_pwchange->setVisible(false);
	this->toTextAnnotationMode(true);
	this->toLoginMode(false);
	this->toAIMode(true);
	this->toSearchAnnotationMode(true);
	this->toSettingMode(false);
	this->toInsertMode(false);
	this->toAnnotationCategoryMode(false);
}

void ReviewModeFilter::initGUI()
{
	this->m_oFilterGUI.chk_day_1->setChecked(true);
	this->m_oFilterGUI.chk_day_2->setChecked(true);
	this->m_oFilterGUI.chk_day_3->setChecked(true);

	this->m_oFilterGUI.chk_weather_1->setChecked(true);
	this->m_oFilterGUI.chk_weather_2->setChecked(true);
	this->m_oFilterGUI.chk_weather_3->setChecked(true);
	this->m_oFilterGUI.chk_weather_4->setChecked(true);

	this->m_oFilterGUI.chk_road_1->setChecked(true);
	this->m_oFilterGUI.chk_road_2->setChecked(true);
	this->m_oFilterGUI.chk_road_3->setChecked(true);

	this->m_oFilterGUI.chk_event_status_1->setChecked(true);
	this->m_oFilterGUI.chk_event_status_2->setChecked(true);
	this->m_oFilterGUI.chk_event_status_3->setChecked(true);
	this->m_oFilterGUI.chk_event_status_4->setChecked(true);
	this->m_oFilterGUI.chk_event_status_5->setChecked(true);

	m_oFilterGUI.lbl_reportid->setVisible(false);
	m_oFilterGUI.lbl_table->setVisible(false);
	QDate date = QDate::currentDate();
	m_oFilterGUI.dateEdit->setDate(date);
	m_oFilterGUI.dateEdit_2->setDate(date);
	this->m_oFilterGUI.cbo_collection->clear();
	this->m_oFilterGUI.cbo_collection->setEnabled(false);
	this->m_oFilterGUI.cbo_AI->clear();
	this->m_oFilterGUI.cbo_AI->setEnabled(false);
	this->m_oFilterGUI.chk_tour->setChecked(true);
	this->m_oFilterGUI.chk_AI->setChecked(true);

	this->initProjectList();
	this->registerEventHandler();
}

void ReviewModeFilter::initWorkspaceDirectory()
{
	MagnaUtil::make_directory(REVIEW_WORKSPACE);
	MagnaUtil::make_directory(CLIP_BACKUP);
	MagnaUtil::make_directory(DB_HOME);
	MagnaUtil::make_directory(TEMP_FILE);
	MagnaUtil::make_directory(PLAY_LIST_HOME);
}

void ReviewModeFilter::initProjectList()
{
	selectedProjectModel = new QStandardItemModel();
	this->m_oFilterGUI.listProjectSelected->setModel(this->selectedProjectModel);
	featureModel = new QStandardItemModel();
	string query4Project = "select b.name, b.id from users_project_map a, project b ";
	query4Project += " where a.projectid = b.id and b.isopen = 0 and a.userid = '" + ReviewModeFilter::ID + "';";
	this->listhandle->addItemsFromDB(m_oFilterGUI.listProject, projectModel, this->getListField4Project(), query4Project);
}

void ReviewModeFilter::initVINList()
{
	selectedVinModel = new QStandardItemModel();
	this->m_oFilterGUI.listVinSelected->setModel(this->selectedVinModel);
	vinModel = new QStandardItemModel();
	this->m_oFilterGUI.listVin->setModel(this->vinModel);

	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	

	if(!project_id.empty())
	{
		string query = "select a.vin as vin from event_report a, event_list b, users_feature_project_map c ";
		query += " where a.eventid = b.id and c.featureid = b.FeatureID and a.projectid = c.projectid ";
		query = this->queryFactory->addFieldsViaInStatement("c.projectID", project_id, query, 2, false);
		query += " and c.UserID = '"+ReviewModeFilter::ID+"' group by a.vin;";

		vector<string> vin_field;
		vin_field.push_back("vin");

		this->listhandle->addItemsFromDB(m_oFilterGUI.listVin, vinModel, vin_field, query);
	}
}


void ReviewModeFilter::initFeatureList()
{
	selectedFeatureModel = new QStandardItemModel();
	this->m_oFilterGUI.listFeatureSelected->setModel(this->selectedFeatureModel);

	featureModel = new QStandardItemModel();
	this->m_oFilterGUI.listFeature->setModel(this->featureModel);
	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);

	if(!project_id.empty()){
		string query =	" select b.name, b.id from users_feature_project_map a, feature_list b ";
		query +=		" where a.featureid = b.id and a.userid = '"+ReviewModeFilter::ID+"'";
		query = queryFactory->addFieldsViaInStatement("a.projectid", project_id, query, 2, false);
		this->listhandle->addItemsFromDB(m_oFilterGUI.listFeature, featureModel, this->getListField4Feature(), query);
	}
}

void ReviewModeFilter::initEventList()
{
	eventListModel = new QStandardItemModel();
	this->m_oFilterGUI.listEventAnnotation->setModel(this->eventListModel);
}

void ReviewModeFilter::initProjectModel(){
	projectModel = new QStandardItemModel();
	selectedProjectModel = new QStandardItemModel();
	vinModel = new QStandardItemModel();
	selectedVinModel = new QStandardItemModel();
	this->m_oFilterGUI.listProject->setModel(this->projectModel);
	this->m_oFilterGUI.listProjectSelected->setModel(this->selectedProjectModel);
	this->m_oFilterGUI.listVin->setModel(this->vinModel);
	this->m_oFilterGUI.listVinSelected->setModel(this->selectedVinModel);
}

void ReviewModeFilter::initAI()
{
	this->AIModel = new QStandardItemModel();
	this->m_oFilterGUI.listAI->setModel(this->AIModel);

	this->selectedAIModel = new QStandardItemModel();
	this->m_oFilterGUI.listAISelected->setModel(this->selectedAIModel);
}

void ReviewModeFilter::initModel()
{
	eventModel = new QStandardItemModel();
	selectedEventModel = new QStandardItemModel();
	annotationModel = new QStandardItemModel();
	selectedAnnotationModel = new QStandardItemModel();
	eventListModel = new QStandardItemModel();
	playlistModel = new QStandardItemModel(); 

	this->m_oFilterGUI.listEvent->setModel(this->eventModel);
	this->m_oFilterGUI.listEventSelected->setModel(this->selectedEventModel);
	this->m_oFilterGUI.listAnnotation->setModel(this->annotationModel);
	this->m_oFilterGUI.listAnnotationSelected->setModel(this->selectedAnnotationModel);
	this->m_oFilterGUI.listEventAnnotation->setModel(this->eventListModel);
	this->m_oFilterGUI.list_playlist->setModel(this->playlistModel);
}

void ReviewModeFilter::initFeatureModel()
{
	featureModel = new QStandardItemModel();
	selectedFeatureModel = new QStandardItemModel();

	this->m_oFilterGUI.listFeature->setModel(this->featureModel);
	this->m_oFilterGUI.listFeatureSelected->setModel(this->selectedFeatureModel);
}

void ReviewModeFilter::initEventListModel(){
	eventListModel = new QStandardItemModel();	
	this->m_oFilterGUI.listEventAnnotation->setModel(this->eventListModel);

	if(this->offset==0)
	{
		playlistModel = new QStandardItemModel();	
		this->m_oFilterGUI.list_playlist->setModel(this->playlistModel);
	}
}

void ReviewModeFilter::initIProjectCombo(QComboBox* combobox)
{

	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	
	vector<string> project_name = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 0);	
	
	if(!project_id.empty() && project_id.size() == project_name.size())
	{
		vector<string> text_list;
		for(unsigned int i = 0 ; i < project_id.size() ; i++)
		{
			text_list.push_back(project_name.at(i) + "-" + project_id.at(i));
		}
		combo_handle->initCombo(this->m_oFilterGUI.cbo_i_project, text_list);
	}else
	{
		MagnaUtil::show_message("Select Project");
	}
}

void ReviewModeFilter::initIVinCombo(QComboBox* combobox)
{

	vector<string> vin = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listVinSelected, 0);	
	if(!vin.empty())
	{		
		combo_handle->initCombo(this->m_oFilterGUI.cbo_i_vin, vin);
	}else
	{
		MagnaUtil::show_message("Select VIN");
	}
}

void ReviewModeFilter::initEventCombo(QComboBox* combobox, bool eyeq)
{
	string selectedFeature = m_oFilterGUI.cbo_i_feature->currentText().toStdString();
	combobox->clear();

	if(!selectedFeature.empty())
	{
		string projectid = MagnaUtil::stringTokenizer(this->m_oFilterGUI.cbo_i_project->currentText().toStdString(), '-').at(1);
		selectedFeature = MagnaUtil::stringTokenizer(selectedFeature, '-').at(1);
		vector<string> event_fields = getListField4Event();
		string query = "select a.name, a.id from event_list a, project_event_map b where a.id = b.eventid and b.projectid = "+projectid+" and a.featureid = "+selectedFeature+";";
		map<string, vector<string>> eventContainer = (new Retriever(event_fields, query))->getData();
		vector<string> event_list = listhandle->getConcatenatedText(eventContainer, event_fields);
		combo_handle->initCombo(combobox, event_list);
	}
}

void ReviewModeFilter::initFeatureCombo(QComboBox* combobox)
{

	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	

	vector<string> feature_fields = getListField4Feature();
	string query = "select b.name as name, b.id as id from users_feature_project_map a, feature_list b ";
	query += " where a.FeatureID = b.ID and a.UserID = '" + ReviewModeFilter::ID + "' ";
	query = this->queryFactory->addFieldsViaInStatement("a.projectid", project_id, query, 2, false);

	map<string, vector<string>> featureContainer = (new Retriever(feature_fields, query))->getData();
	vector<string> event_list = listhandle->getConcatenatedText(featureContainer, feature_fields);
	combo_handle->initCombo(combobox, event_list);
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
		vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	


		string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, project_id);	
		map<string, vector<string>> annotationContainer = (new Retriever(annotation_fields, query4Annotation))->getData();
		vector<string> status_list = listhandle->getConcatenatedText(annotationContainer, annotation_fields);
		combo_handle->initCombo(combobox, status_list);
	}
}

void ReviewModeFilter::initAnnotationCombo(QComboBox* combobox, vector<string> vstr_fetures)
{
	vector<string> annotation_fields = getListField4Annotation();
	vstr_fetures = MagnaUtil::getTokenedVector(vstr_fetures, '-', 1);

	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	

	string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, project_id);
	
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
	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	

	string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, project_id);

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

	connect(m_oFilterGUI.btn_LR, SIGNAL(clicked()), this, SLOT(on_btn_LR_clicked()));
	connect(m_oFilterGUI.btn_RL, SIGNAL(clicked()), this, SLOT(on_btn_RL_clicked()));
	connect(m_oFilterGUI.btn_LR0, SIGNAL(clicked()), this, SLOT(on_btn_LR0_clicked()));
	connect(m_oFilterGUI.btn_RL0, SIGNAL(clicked()), this, SLOT(on_btn_RL0_clicked()));

	connect(m_oFilterGUI.btn_LR1, SIGNAL(clicked()), this, SLOT(on_btn_LR1_clicked()));
	connect(m_oFilterGUI.btn_RL1, SIGNAL(clicked()), this, SLOT(on_btn_RL1_clicked()));
	connect(m_oFilterGUI.btn_LR2, SIGNAL(clicked()), this, SLOT(on_btn_LR2_clicked()));
	connect(m_oFilterGUI.btn_RL2, SIGNAL(clicked()), this, SLOT(on_btn_RL2_clicked()));
	connect(m_oFilterGUI.btn_LR3, SIGNAL(clicked()), this, SLOT(on_btn_LR3_clicked()));
	connect(m_oFilterGUI.btn_RL3, SIGNAL(clicked()), this, SLOT(on_btn_RL3_clicked()));
	connect(m_oFilterGUI.btn_LR4, SIGNAL(clicked()), this, SLOT(on_btn_LR4_clicked()));
	connect(m_oFilterGUI.btn_RL4, SIGNAL(clicked()), this, SLOT(on_btn_RL4_clicked()));

	connect(m_oFilterGUI.btn_showclip, SIGNAL(clicked()), this, SLOT(on_btn_showclip_clicked()));
	connect(m_oFilterGUI.btn_update, SIGNAL(clicked()), this, SLOT(on_btn_udpate_clicked()));
	connect(m_oFilterGUI.btn_insert, SIGNAL(clicked()), this, SLOT(on_btn_insert_clicked()));
	connect(m_oFilterGUI.btn_cancel, SIGNAL(clicked()), this, SLOT(on_btn_cancel_clicked()));
	connect(m_oFilterGUI.btn_submit, SIGNAL(clicked()), this, SLOT(on_btn_submit_clicked()));
	connect(m_oFilterGUI.btn_prev, SIGNAL(clicked()), this, SLOT(on_btn_prev_clicked()));
	connect(m_oFilterGUI.btn_next, SIGNAL(clicked()), this, SLOT(on_btn_next_clicked()));

	connect(m_oFilterGUI.btn_show_playlist, SIGNAL(clicked()), this, SLOT(on_btn_show_play_list_clicked()));
	connect(m_oFilterGUI.btn_generate, SIGNAL(clicked()), this, SLOT(on_btn_generate_play_list()));
	connect(m_oFilterGUI.btn_dir, SIGNAL(clicked()), this, SLOT(on_btn_directory_clicked()));
	
	connect(m_oFilterGUI.chk_date, SIGNAL(clicked()), this, SLOT(on_chk_date_clicked()));
	connect(m_oFilterGUI.chk_tour, SIGNAL(clicked()), this, SLOT(on_chk_tour_clicked()));

	connect(m_oFilterGUI.chk_eyeq_event, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_fcm_event, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_user_event, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_radar, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));

	connect(m_oFilterGUI.chk_day_1, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_day_2, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_day_3, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));

	connect(m_oFilterGUI.chk_weather_1, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_weather_2, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_weather_3, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_weather_4, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));

	connect(m_oFilterGUI.chk_road_1, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_road_2, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_road_3, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));

	connect(m_oFilterGUI.chk_event_status_1, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_event_status_2, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_event_status_3, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_event_status_4, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));
	connect(m_oFilterGUI.chk_event_status_5, SIGNAL(clicked()), this, SLOT(on_chk_clicked()));

	connect(m_oFilterGUI.chk_annotation, SIGNAL(clicked()), this, SLOT(on_chk_annotation_clicked()));
	connect(m_oFilterGUI.chk_search, SIGNAL(clicked()), this, SLOT(on_chk_search_clicked()));
	connect(m_oFilterGUI.chk_text_annotation, SIGNAL(clicked()), this, SLOT(on_chk_text_annotation_clicked()));
	connect(m_oFilterGUI.chk_i_text_annotation, SIGNAL(clicked()), this, SLOT(on_chk_i_text_annotation_clicked()));

	connect(m_oFilterGUI.chk_AI, SIGNAL(clicked()), this, SLOT(on_chk_AI_clicked()));
	connect(m_oFilterGUI.dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_changed()));
	connect(m_oFilterGUI.dateEdit_2, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_2_changed()));

	connect(m_oFilterGUI.listEventAnnotation, SIGNAL(clicked(const QModelIndex)), this, SLOT(on_list_event_annotation_clicked(QModelIndex)));

	connect(m_oFilterGUI.cbo_i_feature, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbo_i_feature_changed()));
	connect(m_oFilterGUI.cbo_AI, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbo_AI_changed()));
	//connect(m_oFilterGUI.cbo_vin, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbo_vin_changed()));
	connect(m_oFilterGUI.cbo_collection, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbo_collection_changed()));

	connect(m_oFilterGUI.txt_m, SIGNAL(editingFinished()), this, SLOT(on_txt_m_edited()));
	connect(m_oFilterGUI.txt_s, SIGNAL(editingFinished()), this, SLOT(on_txt_s_edited()));
	connect(m_oFilterGUI.txt_ms, SIGNAL(editingFinished()), this, SLOT(on_txt_ms_edited()));
	connect(m_oFilterGUI.txt_search, SIGNAL(textChanged(const QString &)), this, SLOT(on_txt_search_edited(const QString &)));

	QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_P), this->m_pFilterWidget);
	QObject::connect(shortcut, SIGNAL(activated()), this->m_oFilterGUI.btn_show_playlist, SLOT(clicked()));
}

void ReviewModeFilter::unregisterEventHandler()
{
	disconnect(this, SLOT(on_btn_setting_clicked()));
	disconnect(this, SLOT(on_btn_setting_cancel_clicked()));
	disconnect(this, SLOT(on_btn_browse_clicked()));
	disconnect(this, SLOT(on_btn_execute_clicked()));
	disconnect(this, SLOT(on_btn_clip_clicked()));
	disconnect(this, SLOT(on_btn_copy_clicked()));

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
	disconnect(this, SLOT(on_btn_prev_clicked()));
	disconnect(this, SLOT(on_btn_next_clicked()));

	disconnect(this, SLOT(on_chk_date_clicked()));

	disconnect(this, SLOT(on_chk_clicked()));
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
		datFile = t->getCurDatFile("adtf:project", "configurations", "Review", att_name, att_value);
	}

	return datFile;
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
		this->initAllAtStart();
	}else
	{
		MagnaUtil::show_message("Invalid ID and Password.");
	}

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
	this->initAllAtStart();

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
	this->refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_AI_clicked()
{
	this->toAIMode(this->m_oFilterGUI.chk_AI->isChecked());
	this->refreshAnnotationGroup(0);
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
	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	

	string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, project_id);
	if(!query4Annotation.empty())
	{
		this->listhandle->addItemsFromDB(m_oFilterGUI.listAnnotation, annotationModel, this->getListField4Annotation(), query4Annotation);
	}

	refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_search_clicked()
{
	this->toSearchAnnotationMode(this->m_oFilterGUI.chk_search->isChecked());
	this->refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_clicked()
{
	refreshEventGroup();
	refreshAnnotationGroup(0);
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
		string r_time = MagnaUtil::longLongIntToString(MagnaUtil::stringToLongLongInt(adtfTime) - MagnaUtil::stringToLongLongInt(startTime));
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

tResult ReviewModeFilter::on_btn_prev_clicked()
{

	if(this->offset >= 100)
	{
		this->refreshAnnotationGroup(this->offset - 100);
	}else
	{
		MagnaUtil::show_message("No Previous Event!");
	}
	
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_directory_clicked()
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
		this->m_oFilterGUI.txt_directory->setText(directory);
	}

	fd->close();
	tree->close();

	saveCurrentStatus(LOGOUT);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_next_clicked()
{
	this->refreshAnnotationGroup(this->offset + 100);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_show_play_list_clicked()
{

	this->toPlayListMode(this->m_oFilterGUI.list_playlist->isVisible());
	RETURN_NOERROR;
}
tResult ReviewModeFilter::on_btn_generate_play_list()
{

	string prefix = this->m_oFilterGUI.txt_directory->text().toStdString();
	vector<string> clip_list = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.list_playlist, 0);
	
	if(clip_list.empty()) 
	{
		MagnaUtil::show_message("The clip list is empty.");
		RETURN_NOERROR;
	}

	string dir = this->m_oFilterGUI.txt_directory->text().toStdString();
	
	if(dir.empty()){
		MagnaUtil::show_message("Please set the directory.");
		RETURN_NOERROR;
	}

	MagnaUtil::outToFile(PLAY_LIST_HOME, prefix, clip_list);
	MagnaUtil::show_message("The play list of the clips has been created.");

	MagnaUtil::delete_file(TEMP_FILE + "tmp.xml");
	this->initAllAtStart();
	this->m_oFilterGUI.txt_clip_directory->setText(QString(""));
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_chk_tour_clicked()
{
	if(this->m_oFilterGUI.chk_tour->isChecked()==true) this->m_oFilterGUI.cbo_collection->setEnabled(false);
	else this->m_oFilterGUI.cbo_collection->setEnabled(true);

	this->refreshEventGroup();
	this->refreshAnnotationGroup(0);

	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_cbo_AI_changed()
{
	
	string ai_type = this->m_oFilterGUI.cbo_AI->currentText().toStdString();
	this->AIModel = new QStandardItemModel();
	this->m_oFilterGUI.listAI->setModel(this->AIModel);
	if(ai_type.empty())
		RETURN_NOERROR;
	
	string ai_type_id = MagnaUtil::stringTokenizer(ai_type, '-').at(1);
	
	string query = "SELECT b.name as typename, a.name as name, b.id as typeid, a.value as id ";
	query += " FROM additional_event_value a, additional_event_type b where a.typeid = b.id and a.typeid = "+ai_type_id+";";

	map<string, vector<string>> containers = (new Retriever(getListField4AI(), query))->getData();
	this->listhandle->addItemsFromDB(m_oFilterGUI.listAI, AIModel, this->getListField4AI(), query);

	saveCurrentStatus(LOGOUT);
	
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_cbo_collection_changed()
{
	
	this->refreshEventGroup();
	this->refreshAnnotationGroup(0);

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
		this->refreshAnnotationGroup(0);
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


	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	
	if(project_id.empty())
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

		this->initIProjectCombo(this->m_oFilterGUI.cbo_i_project);
		this->initIVinCombo(this->m_oFilterGUI.cbo_i_vin);
		this->initIAnnotationCombo(this->m_oFilterGUI.cbo_i_annotation);
		this->initStatusCombo(this->m_oFilterGUI.cbo_i_status);
		this->initFeatureCombo(this->m_oFilterGUI.cbo_i_feature);

	}

	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_cancel_clicked()
{
	this->toInsertMode(false);
	this->refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_submit_clicked()
{
	string reportid = MagnaUtil::bigIntegerToString(MagnaUtil::getCurrentSystemTime());
	string localpctime = MagnaUtil::bigIntegerToString(MagnaUtil::getCurrentLocalTime());
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


	string vin = this->m_oFilterGUI.cbo_i_vin->currentText().toStdString();
	string clip = MagnaUtil::removeURLOfFile(this->m_oFilterGUI.txt_clip->text().toStdString());
	string project_raw_text = this->m_oFilterGUI.cbo_i_project->currentText().toStdString();

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
		string clipid = containers["clipid"].at(0);
		string m = this->m_oFilterGUI.txt_m->text().toStdString();
		string s = this->m_oFilterGUI.txt_s->text().toStdString();
		string ms = this->m_oFilterGUI.txt_ms->text().toStdString();
		
		if(m.empty()) m = "00";
		if(s.empty()) s = "00";
		if(ms.empty()) ms = "000";
		
		__int64 starttime = MagnaUtil::stringToLongLongInt(containers["adtfstarttime"].at(0));
		__int64 current_time = MagnaUtil::stringToLongLongInt(MagnaUtil::getMicroSecond(m,s,ms));
		__int64 adtftime = starttime + current_time;

		string insert_query = "";
		string projectid = MagnaUtil::stringTokenizer(project_raw_text, '-').at(1);

		string curTime = MagnaUtil::bigIntegerToString(MagnaUtil::getCurrentSystemTime());
		if(this->m_oFilterGUI.chk_i_text_annotation->isChecked()){
			predefined_annotation = MagnaUtil::stringTokenizer(predefined_annotation, '-').at(1);

			insert_query = " insert into event_report (reportid, vin, clipid, eventcategoryid, eventid, localpctime, adtftime, eventstatusid, predefinedannotationid, projectid, updator, updatedtime, insertor, insertedtime) ";
			insert_query += " values (" + reportid + ", '" + vin + "', " + clipid + ", 3, " + eventid + ", " + localpctime + ", " + MagnaUtil::longLongIntToString(adtftime) + ", " + status + ", " +
				predefined_annotation + ", " + projectid + ", '"+ReviewModeFilter::ID+"', "+curTime+", '"+ReviewModeFilter::ID+"', "+curTime+");";
		}else
		{
			insert_query = " insert into event_report (reportid, vin, clipid, eventcategoryid, eventid, localpctime, adtftime, userannotation, eventstatusid, predefinedannotationid, projectid, updator, updatedtime, insertor, insertedtime) ";
			insert_query += " values (" + reportid + ", '" + vin + "', " + clipid + ", 3, " + eventid + ", " + localpctime + ", " + MagnaUtil::longLongIntToString(adtftime) + ", '" + annotation + "', " 
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
	refreshAnnotationGroup(0);
	this->m_oFilterGUI.dateEdit->blockSignals(false);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_dateEdit_2_changed()
{
	this->m_oFilterGUI.dateEdit_2->blockSignals(true);
	refreshEventGroup();
	refreshAnnotationGroup(0);
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
	this->initAllAtStart();
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

tResult ReviewModeFilter:: on_btn_LR_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listProject, this->projectModel, m_oFilterGUI.listProjectSelected, this->selectedProjectModel);	
	this->initBasicGroup();
	this->initFeatureList();
	this->refreshEventGroup();
	this->refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter:: on_btn_RL_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listProjectSelected, this->selectedProjectModel, m_oFilterGUI.listProject, this->projectModel);
	this->initBasicGroup();
	this->initFeatureList();
	this->refreshEventGroup();
	this->refreshAnnotationGroup(0);

	RETURN_NOERROR;
}
tResult ReviewModeFilter:: on_btn_LR0_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listVin, this->vinModel, m_oFilterGUI.listVinSelected, this->selectedVinModel);
	this->initCollectionCombo(this->m_oFilterGUI.cbo_collection);
	this->refreshEventGroup();
	this->refreshAnnotationGroup(0);
	RETURN_NOERROR;
}
tResult ReviewModeFilter:: on_btn_RL0_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listVinSelected, this->selectedVinModel, m_oFilterGUI.listVin, this->vinModel);
	this->initCollectionCombo(this->m_oFilterGUI.cbo_collection);
	this->refreshEventGroup();
	this->refreshAnnotationGroup(0);
	RETURN_NOERROR;
}


tResult ReviewModeFilter::on_btn_LR1_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listFeature, this->featureModel, m_oFilterGUI.listFeatureSelected, this->selectedFeatureModel);
	this->refreshEventGroup();
	this->refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_RL1_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listFeatureSelected, this->selectedFeatureModel, m_oFilterGUI.listFeature, this->featureModel);
	refreshEventGroup();
	refreshAnnotationGroup(0);
	RETURN_NOERROR;
}


tResult ReviewModeFilter::on_btn_LR2_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listEvent, this->eventModel, m_oFilterGUI.listEventSelected, this->selectedEventModel);
	refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_RL2_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listEventSelected, this->selectedEventModel, m_oFilterGUI.listEvent, this->eventModel);
	refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_LR3_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listAnnotation, this->annotationModel, m_oFilterGUI.listAnnotationSelected, this->selectedAnnotationModel);
	refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_RL3_clicked()
{
	this->listhandle->moveSelectedItemWithDelete(m_oFilterGUI.listAnnotationSelected, this->selectedAnnotationModel, m_oFilterGUI.listAnnotation, this->annotationModel);
	refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_LR4_clicked()
{
	this->listhandle->moveSelectedItems(m_oFilterGUI.listAI, this->AIModel, m_oFilterGUI.listAISelected, this->selectedAIModel);
	refreshAnnotationGroup(0);
	RETURN_NOERROR;
}

tResult ReviewModeFilter::on_btn_RL4_clicked()
{
	this->listhandle->removeSelectedItems(m_oFilterGUI.listAISelected, this->selectedAIModel);
	refreshAnnotationGroup(0);
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
	refreshAnnotationGroup(0);
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
		return false;
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
		if(getCurClipText().compare(dat)==0){
			MagnaUtil::show_message("The selected event is recorded in the current clip");
			return false;
		}

		t->update("adtf:project", "configurations", "Review", att_name, att_value, dat);
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
	
	this->m_oFilterGUI.cbo_collection->setVisible(!mode);
	
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

void ReviewModeFilter::toAIMode(bool mode)
{
	this->m_oFilterGUI.chk_AI->setChecked(mode);
	this->m_oFilterGUI.cbo_AI->setEnabled(!mode);
	this->selectedAIModel = new QStandardItemModel();
	this->m_oFilterGUI.listAISelected->setModel(this->selectedAIModel);
	this->m_oFilterGUI.listAISelected->setEnabled(!mode);
	this->m_oFilterGUI.listAI->setEnabled(!mode);
	this->m_oFilterGUI.btn_LR4->setEnabled(!mode);
	this->m_oFilterGUI.btn_RL4->setEnabled(!mode);
}

void ReviewModeFilter::toPlayListMode(bool mode)
{
	this->m_oFilterGUI.grp_playlist->setVisible(!mode);
	if(mode==true) this->m_oFilterGUI.btn_show_playlist->setText(QString(">"));
	else this->m_oFilterGUI.btn_show_playlist->setText(QString("<"));
	QRect rect = this->m_oFilterGUI.grp_annotation->geometry();
	if(mode==true)
	{
		this->m_oFilterGUI.btn_show_playlist->setGeometry(10, 22, 20, 211);
		this->m_oFilterGUI.grp_annotation->setGeometry(35, 20, 861, 211);
	}else{
		this->m_oFilterGUI.btn_show_playlist->setGeometry(345, 22, 20, 211);
		this->m_oFilterGUI.grp_annotation->setGeometry(370, 20, 861, 211);
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
	this->m_oFilterGUI.cbo_collection->setVisible(!mode);
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
	this->m_oFilterGUI.cbo_collection->setVisible(!mode);
	if(mode==true) this->m_oFilterGUI.txt_m->setFocus(Qt::OtherFocusReason);
}

void ReviewModeFilter::restoreCurrentStatus(const char* tmpFile)
{

	this->m_oFilterGUI.chk_eyeq_event->setChecked(false);
	this->m_oFilterGUI.chk_fcm_event->setChecked(false);
	this->m_oFilterGUI.chk_user_event->setChecked(false);
	this->m_oFilterGUI.chk_radar->setChecked(false);

	this->m_oFilterGUI.chk_day_1->setChecked(false);
	this->m_oFilterGUI.chk_day_2->setChecked(false);
	this->m_oFilterGUI.chk_day_3->setChecked(false);

	this->m_oFilterGUI.chk_weather_1->setChecked(false);
	this->m_oFilterGUI.chk_weather_2->setChecked(false);
	this->m_oFilterGUI.chk_weather_3->setChecked(false);
	this->m_oFilterGUI.chk_weather_4->setChecked(false);

	this->m_oFilterGUI.chk_road_1->setChecked(false);
	this->m_oFilterGUI.chk_road_2->setChecked(false);
	this->m_oFilterGUI.chk_road_3->setChecked(false);

	this->m_oFilterGUI.chk_event_status_1->setChecked(false);
	this->m_oFilterGUI.chk_event_status_2->setChecked(false);
	this->m_oFilterGUI.chk_event_status_3->setChecked(false);
	this->m_oFilterGUI.chk_event_status_4->setChecked(false);
	this->m_oFilterGUI.chk_event_status_5->setChecked(false);

	this->m_oFilterGUI.lbl_reportid->setVisible(false);
	this->m_oFilterGUI.lbl_table->setVisible(false);
	this->m_oFilterGUI.txt_annotation->setVisible(false);

	XMLHandler *xmlHandle = new XMLHandler(tmpFile);


	this->offset = MagnaUtil::stringTointeger(xmlHandle->getNodeValue("offset", "value"));

	vector<string> project_list = xmlHandle->getNodeListAtSecondLevel("project_list");
	vector<string> selected_project_list = xmlHandle->getNodeListAtSecondLevel("selected_project_list");

	vector<string> vin_list = xmlHandle->getNodeListAtSecondLevel("vin_list");
	vector<string> selected_vin_list = xmlHandle->getNodeListAtSecondLevel("selected_vin_list");

	vector<string> collection_list = xmlHandle->getNodeListAtSecondLevel("collection_list");

	vector<string> feature_list = xmlHandle->getNodeListAtSecondLevel("feature_list");
	vector<string> selected_feature_list = xmlHandle->getNodeListAtSecondLevel("selected_feature_list");

	vector<string> event_list = xmlHandle->getNodeListAtSecondLevel("event_list");
	vector<string> selected_event_list = xmlHandle->getNodeListAtSecondLevel("selected_event_list");

	vector<string> annotation_list = xmlHandle->getNodeListAtSecondLevel("annotation_list");
	vector<string> selected_annotation_list = xmlHandle->getNodeListAtSecondLevel("selected_annotation_list");

	vector<string> AI_type_list = xmlHandle->getNodeListAtSecondLevel("AI_type_list");
	vector<string> AI_list = xmlHandle->getNodeListAtSecondLevel("AI_list");
	vector<string> selected_AI_list = xmlHandle->getNodeListAtSecondLevel("selected_AI_list");
	vector<string> play_list = xmlHandle->getNodeListAtSecondLevel("play_list");
	vector<string> event_annotation_list = xmlHandle->getNodeListAtSecondLevel("event_annotation_list");

	vector<string> event_category_list = xmlHandle->getNodeListAtSecondLevel("event_category_list");
	vector<string> day_list = xmlHandle->getNodeListAtSecondLevel("day_list");
	vector<string> weather_list = xmlHandle->getNodeListAtSecondLevel("weather_list");
	vector<string> road_list = xmlHandle->getNodeListAtSecondLevel("road_list");
	vector<string> event_status_list = xmlHandle->getNodeListAtSecondLevel("event_status_list");

	string clip_directory = xmlHandle->getNodeValue("clip_directory", "value");
	string selected_event = xmlHandle->getNodeValue("selected_event", "value");
	string selected_collection = xmlHandle->getNodeValue("collection_selected", "value");
	string selected_AI_type = xmlHandle->getNodeValue("AI_type_selected", "value");
	string set_directory = xmlHandle->getNodeValue("set_directory", "value");

	string chk_collection = xmlHandle->getNodeValue("collection_check", "value");
	string chk_AI = xmlHandle->getNodeValue("AI_check", "value");
	string chk_date = xmlHandle->getNodeValue("date_check", "value");
	string chk_annotation = xmlHandle->getNodeValue("annotation_check", "value");
	string chk_text_annotation = xmlHandle->getNodeValue("text_annotation_check", "value");
	string text_annotation = xmlHandle->getNodeValue("text_annotation", "value");
	string status = xmlHandle->getNodeValue("status", "value");

	string start_date = xmlHandle->getNodeValue("start_date", "value");
	string end_date = xmlHandle->getNodeValue("end_date", "value");
	
	this->m_oFilterGUI.lbl_status->setText(QString(status.c_str()));
	this->initAnnotationCombo(this->m_oFilterGUI.cbo_annotation, selected_feature_list);
	this->listhandle->addItems(this->m_oFilterGUI.listProject, this->projectModel, project_list);
	this->listhandle->addItems(this->m_oFilterGUI.listProjectSelected, this->selectedProjectModel, selected_project_list);
	this->listhandle->addItems(this->m_oFilterGUI.listVin, this->vinModel, vin_list);
	this->listhandle->addItems(this->m_oFilterGUI.listVinSelected, this->selectedVinModel, selected_vin_list);

	this->listhandle->addItems(this->m_oFilterGUI.listFeature, this->featureModel, feature_list);
	this->listhandle->addItems(this->m_oFilterGUI.listFeatureSelected, this->selectedFeatureModel, selected_feature_list);
	this->listhandle->addItems(this->m_oFilterGUI.listEvent, this->eventModel, event_list);
	this->listhandle->addItems(this->m_oFilterGUI.listEventSelected, this->selectedEventModel, selected_event_list);
	this->listhandle->addItems(this->m_oFilterGUI.listAnnotation, this->annotationModel, annotation_list);	
	this->listhandle->addItems(this->m_oFilterGUI.listAnnotationSelected, this->selectedAnnotationModel, selected_annotation_list);
	this->listhandle->addItems(this->m_oFilterGUI.listAI, this->AIModel, AI_list);	
	this->listhandle->addItems(this->m_oFilterGUI.listAISelected, this->selectedAIModel, selected_AI_list);

	this->listhandle->addItems(this->m_oFilterGUI.listEventAnnotation, this->eventListModel, event_annotation_list);

	this->listhandle->addItems(this->m_oFilterGUI.list_playlist, this->playlistModel, play_list);

	combo_handle->initCombo(this->m_oFilterGUI.cbo_collection, collection_list);
	combo_handle->initCombo(this->m_oFilterGUI.cbo_AI, AI_type_list);

	int indexOfColelction = this->m_oFilterGUI.cbo_collection->findText(QString(selected_collection.c_str()));
	this->m_oFilterGUI.cbo_collection->setCurrentIndex(indexOfColelction);
	int indexOfAI = this->m_oFilterGUI.cbo_AI->findText(QString(selected_AI_type.c_str()));
	this->m_oFilterGUI.cbo_AI->setCurrentIndex(indexOfAI);

	this->m_oFilterGUI.txt_directory->setText(QString(set_directory.c_str()));

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
	if(chk_collection.compare(str_true)) {
		this->m_oFilterGUI.chk_tour->setChecked(false);
		this->on_chk_tour_clicked();
	}

	for(unsigned int idx = 0 ; idx < event_category_list.size() ; idx++)
	{
		string tmp = event_category_list.at(idx);

		if(tmp.compare("eyeq")==0) this->m_oFilterGUI.chk_eyeq_event->setChecked(true);
		if(tmp.compare("fcm")==0) this->m_oFilterGUI.chk_fcm_event->setChecked(true);
		if(tmp.compare("user")==0) this->m_oFilterGUI.chk_user_event->setChecked(true);
		if(tmp.compare("radar")==0) this->m_oFilterGUI.chk_user_event->setChecked(true);
	}

	for(unsigned int idx = 0 ; idx < day_list.size() ; idx++)
	{
		string tmp = day_list.at(idx);
		if(tmp.compare("day")==0) this->m_oFilterGUI.chk_day_1->setChecked(true);
		if(tmp.compare("dusk")==0) this->m_oFilterGUI.chk_day_2->setChecked(true);
		if(tmp.compare("night")==0) this->m_oFilterGUI.chk_day_3->setChecked(true);
	}

	for(unsigned int idx = 0 ; idx < weather_list.size() ; idx++)
	{
		string tmp = weather_list.at(idx);
		if(tmp.compare("clear")==0) this->m_oFilterGUI.chk_weather_1->setChecked(true);
		if(tmp.compare("rain")==0) this->m_oFilterGUI.chk_weather_2->setChecked(true);
		if(tmp.compare("overcast")==0) this->m_oFilterGUI.chk_weather_3->setChecked(true);
		if(tmp.compare("snow")==0) this->m_oFilterGUI.chk_weather_4->setChecked(true);
	}

	for(unsigned int idx = 0 ; idx < road_list.size() ; idx++)
	{
		string tmp = road_list.at(idx);
		if(tmp.compare("highway")==0) this->m_oFilterGUI.chk_road_1->setChecked(true);
		if(tmp.compare("urban")==0) this->m_oFilterGUI.chk_road_2->setChecked(true);
		if(tmp.compare("both")==0) this->m_oFilterGUI.chk_road_3->setChecked(true);
	}

	for(unsigned int idx = 0 ; idx < event_status_list.size() ; idx++)
	{
		string tmp = event_status_list.at(idx);
		if(tmp.compare("unconfirmed")==0) this->m_oFilterGUI.chk_event_status_1->setChecked(true);
		if(tmp.compare("accepted")==0) this->m_oFilterGUI.chk_event_status_2->setChecked(true);
		if(tmp.compare("annotated")==0) this->m_oFilterGUI.chk_event_status_3->setChecked(true);
		if(tmp.compare("declined")==0) this->m_oFilterGUI.chk_event_status_4->setChecked(true);
		if(tmp.compare("missed")==0) this->m_oFilterGUI.chk_event_status_5->setChecked(true);
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
	bool AI_checker= this->m_oFilterGUI.chk_AI->isChecked();
	bool collection_checker= this->m_oFilterGUI.chk_tour->isChecked();
	string offset = MagnaUtil::integerToString(this->offset);
	string sdate;
	string edate;
	string text_annotation;
	string clip_directory;
	string txt_status = this->m_oFilterGUI.lbl_status->text().toStdString();
	vector<string> project_list;
	vector<string> selected_project_list;
	vector<string> vin_list;
	vector<string> selected_vin_list;
	vector<string> AI_list;
	vector<string> selected_AI_list;
	vector<string> play_list;
	vector<string> featureList;
	vector<string> selectedFeatureList;
	vector<string> eventList;
	vector<string> selectedEventList;
	vector<string> annotationList;
	vector<string> selectedAnnotationList;
	vector<string> eventAnnotationList;

	vector<string> event_category;
	vector<string> days;
	vector<string> weathers;
	vector<string> roads;
	vector<string> event_statuses;

	vector<string> collection_list;
	vector<string> AI_type_list;


	string selectedAIType = this->m_oFilterGUI.cbo_AI->currentText().toStdString();
	string selectedCollection = this->m_oFilterGUI.cbo_collection->currentText().toStdString();
	string selectedEvent;
	string set_directory;
	
	if(this->m_oFilterGUI.chk_eyeq_event->isChecked()) event_category.push_back("eyeq");
	if(this->m_oFilterGUI.chk_fcm_event->isChecked()) event_category.push_back("fcm");
	if(this->m_oFilterGUI.chk_user_event->isChecked()) event_category.push_back("user");
	if(this->m_oFilterGUI.chk_radar->isChecked()) event_category.push_back("radar");

	if(this->m_oFilterGUI.chk_day_1->isChecked()) days.push_back("day");
	if(this->m_oFilterGUI.chk_day_2->isChecked()) days.push_back("dusk");
	if(this->m_oFilterGUI.chk_day_3->isChecked()) days.push_back("night");

	if(this->m_oFilterGUI.chk_weather_1->isChecked()) weathers.push_back("clear");
	if(this->m_oFilterGUI.chk_weather_2->isChecked()) weathers.push_back("rain");
	if(this->m_oFilterGUI.chk_weather_3->isChecked()) weathers.push_back("overcast");
	if(this->m_oFilterGUI.chk_weather_4->isChecked()) weathers.push_back("snow");

	if(this->m_oFilterGUI.chk_road_1->isChecked()) roads.push_back("highway");
	if(this->m_oFilterGUI.chk_road_2->isChecked()) roads.push_back("urban");
	if(this->m_oFilterGUI.chk_road_3->isChecked()) roads.push_back("both");

	if(this->m_oFilterGUI.chk_event_status_1->isChecked()) event_statuses.push_back("unconfirmed");
	if(this->m_oFilterGUI.chk_event_status_2->isChecked()) event_statuses.push_back("accepted");
	if(this->m_oFilterGUI.chk_event_status_3->isChecked()) event_statuses.push_back("annotated");
	if(this->m_oFilterGUI.chk_event_status_4->isChecked()) event_statuses.push_back("declined");
	if(this->m_oFilterGUI.chk_event_status_5->isChecked()) event_statuses.push_back("missed");
	
	text_annotation = this->m_oFilterGUI.txt_annotation->toPlainText().toStdString();
	clip_directory = this->m_oFilterGUI.txt_clip_directory->text().toStdString();
	set_directory = this->m_oFilterGUI.txt_directory->text().toStdString();

	sdate = this->getDate(this->m_oFilterGUI.dateEdit, "/");
	edate = this->getDate(this->m_oFilterGUI.dateEdit_2, "/");


	for(int idx = 0 ; idx < this->m_oFilterGUI.cbo_collection->model()->rowCount() ; idx++)
		collection_list.push_back(this->m_oFilterGUI.cbo_collection->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.cbo_AI->model()->rowCount() ; idx++)
		AI_type_list.push_back(this->m_oFilterGUI.cbo_AI->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());


	for(int idx = 0 ; idx < this->m_oFilterGUI.listProject->model()->rowCount() ; idx++)
		project_list.push_back(this->m_oFilterGUI.listProject->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listProjectSelected->model()->rowCount() ; idx++)
		selected_project_list.push_back(this->m_oFilterGUI.listProjectSelected->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listVin->model()->rowCount() ; idx++)
		vin_list.push_back(this->m_oFilterGUI.listVin->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listVinSelected->model()->rowCount() ; idx++)
		selected_vin_list.push_back(this->m_oFilterGUI.listVinSelected->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

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


	for(int idx = 0 ; idx < this->m_oFilterGUI.listAI->model()->rowCount() ; idx++)
		AI_list.push_back(this->m_oFilterGUI.listAI->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.listAISelected->model()->rowCount() ; idx++)
		selected_AI_list.push_back(this->m_oFilterGUI.listAISelected->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());



	for(int idx = 0 ; idx < this->m_oFilterGUI.listEventAnnotation->model()->rowCount() ; idx++)
		eventAnnotationList.push_back(this->m_oFilterGUI.listEventAnnotation->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	for(int idx = 0 ; idx < this->m_oFilterGUI.list_playlist->model()->rowCount() ; idx++)
		play_list.push_back(this->m_oFilterGUI.list_playlist->model()->index(idx,0).data(Qt::DisplayRole).toString().toStdString());

	
	vector<string> v_selectedEventList = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listEventAnnotation, this->eventListModel);
	if(!v_selectedEventList.empty())
		selectedEvent = v_selectedEventList.at(0);

	XMLHandler xmlHandle(TEMP_FILE + "tmp.xml");
	xmlHandle.open();
	xmlHandle.addHeader("backup");

	xmlHandle.addItem("offset", "value", offset);
	xmlHandle.addItem("clip_directory", "value", clip_directory);
	xmlHandle.addItem("set_directory", "value", set_directory);
	xmlHandle.addItem("date_check", "value", !datechecker);
	xmlHandle.addItem("text_annotation_check", "value", !text_annotation_checker);
	xmlHandle.addItem("AI_check", "value", !AI_checker);
	xmlHandle.addItem("AI_type_selected", "value", selectedAIType);
	xmlHandle.addItem("collection_check", "value", !collection_checker);
	xmlHandle.addItem("collection_selected", "value", selectedCollection);
	xmlHandle.addItem("status", "value", txt_status);

	xmlHandle.addItem("annotation_check", "value", !annotation_checker);
	xmlHandle.addItem("text_annotation", "value", text_annotation);
	xmlHandle.addItem("start_date", "value", sdate);
	xmlHandle.addItem("end_date", "value", edate);
	xmlHandle.addItem("selected_event", "value", selectedEvent);
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

	xmlHandle.addItems("collection_list", "item", collection_list);
	xmlHandle.addItems("AI_type_list", "item", AI_type_list);

	xmlHandle.addItems("project_list", "item", project_list);
	xmlHandle.addItems("selected_project_list", "item", selected_project_list);
	xmlHandle.addItems("vin_list", "item", vin_list);
	xmlHandle.addItems("selected_vin_list", "item", selected_vin_list);
	xmlHandle.addItems("feature_list", "item", featureList);
	xmlHandle.addItems("selected_feature_list", "item", selectedFeatureList);
	xmlHandle.addItems("event_list", "item", eventList);
	xmlHandle.addItems("selected_event_list", "item", selectedEventList);
	xmlHandle.addItems("annotation_list", "item", annotationList);
	xmlHandle.addItems("selected_annotation_list", "item", selectedAnnotationList);
	
	xmlHandle.addItems("AI_type_list", "item", AI_type_list);
	xmlHandle.addItems("AI_list", "item", AI_list);
	xmlHandle.addItems("selected_AI_list", "item", selected_AI_list);
	xmlHandle.addItems("event_annotation_list", "item", eventAnnotationList);
	
	xmlHandle.addItems("play_list", "item", play_list);
	xmlHandle.addItems("event_category_list", "item", event_category);
	xmlHandle.addItems("day_list", "item", days);
	xmlHandle.addItems("weather_list", "item", weathers);
	xmlHandle.addItems("road_list", "item", roads);
	xmlHandle.addItems("event_status_list", "item", event_statuses);

	xmlHandle.addFooter("backup");
	xmlHandle.close();
}

void ReviewModeFilter::refreshEventGroup()
{

	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);	
	vector<string> vin_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listVinSelected, 0);
	vector<string> days = this->getDayTypes();
	vector<string> weathers = this->getWeatherTypes();
	vector<string> roads = this->getRoadTypes();
	

	string selected_clip_cluster = ReviewModeFilter::localtime_clipid_map[this->m_oFilterGUI.cbo_collection->currentText().toStdString()];
	string start_clip = "";
	string end_clip = "";

	bool chk_tour = this->m_oFilterGUI.chk_tour->isChecked();
	if(selected_clip_cluster!= "")
	{
		start_clip = MagnaUtil::stringTokenizer(selected_clip_cluster, '-').at(0);
		end_clip = MagnaUtil::stringTokenizer(selected_clip_cluster, '-').at(1);
	}


	if(project_id.empty() || vin_id.empty() || days.empty() || weathers.empty()|| roads.empty())
	{
		initEventGroup();

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
		refreshEvents(progress, project_id, vin_id, chk_tour, start_clip, end_clip, days, weathers, roads);
		refreshAnnotation(project_id, progress);
		refreshAICombo(project_id, progress);

		progress->hide();
	}
}

void ReviewModeFilter::refreshAICombo(vector<string> project_id, QProgressDialog * progress)
{
	vector<string> vstr_fetures = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listFeatureSelected, 1);	
	this->m_oFilterGUI.cbo_AI->clear();

	if(!vstr_fetures.empty()){
		string query = "select name, id from additional_event_type ";
		query = this->queryFactory->addFieldsViaInStatement("featureid", vstr_fetures, query, 1, false);
		query += ";";

		vector<string> fields;
		fields.push_back("name");
		fields.push_back("id");

		map<string, vector<string>> container = (new Retriever(fields, query))->getData();
		vector<string> ai_list = listhandle->getConcatenatedText(container, fields);

		combo_handle->initCombo(this->m_oFilterGUI.cbo_AI, ai_list);
	}	
}

void ReviewModeFilter::refreshAnnotation(vector<string> project_id, QProgressDialog * progress)
{
	vector<string> vstr_fetures = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listFeatureSelected, 1);	
	this->m_oFilterGUI.cbo_annotation->clear();
	annotationModel = new QStandardItemModel();
	selectedAnnotationModel = new QStandardItemModel();

	this->m_oFilterGUI.listAnnotation->setModel(this->annotationModel);
	this->m_oFilterGUI.listAnnotationSelected->setModel(this->selectedAnnotationModel);
	this->m_oFilterGUI.chk_search->setChecked(false);
	this->on_chk_search_clicked();

	if(vstr_fetures.size()>0)
	{
		string query4Annotation = this->queryFactory->getAnnotationCategoryQuery(vstr_fetures, project_id);
		if(!query4Annotation.empty())
		{
			this->listhandle->addItemsFromDB(m_oFilterGUI.listAnnotation, annotationModel, this->getListField4Annotation(), query4Annotation);
			progress->setValue(100);
		}
	}

}

void ReviewModeFilter::refreshEvents(QProgressDialog * progress, vector<string> project_id, 
									 vector<string> vin_id, bool chk_tour, 
									 string start_clip, string end_clip, vector<string> days, vector<string> weathers, vector<string> roads)
{
	eventModel = new QStandardItemModel();
	selectedEventModel = new QStandardItemModel();

	this->m_oFilterGUI.listEvent->setModel(this->eventModel);
	this->m_oFilterGUI.listEventSelected->setModel(this->selectedEventModel);

	vector<string> vstr_fetures = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listFeatureSelected, 1);	
	vector<string> categories = this->getEventCategories();
	vector<string> status = this->getEventStatus();

	progress->setValue(20);

	string stime, etime;
	if(!this->m_oFilterGUI.chk_date->isChecked())
	{
		vector<string> dateRange= getDateRange();
		stime = dateRange.at(0);
		etime = dateRange.at(1);
	}

	if(!(vstr_fetures.empty() || categories.empty() || status.empty() || vin_id.empty() || weathers.empty() || days.empty() || roads.empty()))
	{
		string query = this->queryFactory->getEventTypeQuery(this->getListField4Event(), 
			" event_report b, clip_info c", categories, stime, etime, vstr_fetures, project_id,
			vin_id, chk_tour, start_clip, end_clip, days, weathers, roads, status);
		this->listhandle->addItemsFromDB(m_oFilterGUI.listEvent, eventModel, this->getListField4Event(), query);
	}

	progress->setValue(90);
}

void ReviewModeFilter::refreshAnnotationGroup(int current_offset)
{

	vector<string> project_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listProjectSelected, 1);
	vector<string> vin_id = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listVinSelected, 0);
	vector<string> ai_typeid = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listAISelected, 2);
	vector<string> ai_value = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listAISelected, 3);

	vector<string> days = this->getDayTypes();
	vector<string> weathers = this->getWeatherTypes();
	vector<string> roads = this->getRoadTypes();
	vector<string> event_status = this->getEventStatus();
	vector<string> event_categories = getEventCategories();

	vector<string> events = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listEventSelected, 1);
	vector<string> vstr_fetures = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listFeatureSelected, 1);	
	vector<string> annotations = this->listhandle->getSelectedItemTextList(this->m_oFilterGUI.listAnnotationSelected, 1);

	string selected_clip_cluster = ReviewModeFilter::localtime_clipid_map[this->m_oFilterGUI.cbo_collection->currentText().toStdString()];
	string search_condition = this->m_oFilterGUI.txt_search->text().toStdString();
	bool chk_search = this->m_oFilterGUI.chk_search->isChecked();
	bool chk_annotation = this->m_oFilterGUI.chk_annotation->isChecked();
	string start_clip = "";
	string end_clip = "";

	bool chk_tour = this->m_oFilterGUI.chk_tour->isChecked();
	if(selected_clip_cluster!= "")
	{
		start_clip = MagnaUtil::stringTokenizer(selected_clip_cluster, '-').at(0);
		end_clip = MagnaUtil::stringTokenizer(selected_clip_cluster, '-').at(1);
	}

	string stime, etime;
	if(!this->m_oFilterGUI.chk_date->isChecked())
	{
		vector<string> dateRange= getDateRange();
		stime = dateRange.at(0);
		etime = dateRange.at(1);
	}

	if(vstr_fetures.empty()|| events.empty()|| event_categories.empty() || 
		project_id.empty() || vin_id.empty() || days.empty() || weathers.empty()|| roads.empty() || event_status.empty() 
		|| (this->m_oFilterGUI.chk_AI->isChecked()==false && ai_typeid.empty())|| (annotations.size()==0 && chk_annotation))
	{
		this->m_oFilterGUI.lbl_status->setText("");
		initAnnotationGroup();
	}
	else
	{
		if(current_offset==0) this->offset = 0;
		else this->offset = current_offset;

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
		this->m_oFilterGUI.chk_text_annotation->setChecked(true);

		this->on_chk_text_annotation_clicked();
		this->m_oFilterGUI.txt_annotation->clear();
		progress->setValue(30);


		this->m_oFilterGUI.lbl_status->setText("");
		this->m_oFilterGUI.lbl_clip->setText("");
		this->m_oFilterGUI.lbl_adtftime->setText("");
		this->m_oFilterGUI.cbo_status->setCurrentIndex(0);
		this->m_oFilterGUI.cbo_annotation->setCurrentIndex(0);

		progress->setValue(20);
		
		string query = this->queryFactory->getEventListQuery(this->offset,
			this->getListField4EventList(), ReviewModeFilter::ID , project_id, events, stime, etime, event_categories, annotations, search_condition, chk_search, 
			vin_id, chk_tour, start_clip, end_clip, days, weathers, roads, event_status, ai_typeid, ai_value, true);
		
		int recordsize = this->listhandle->addItemsFromDB(m_oFilterGUI.listEventAnnotation, eventListModel, this->getListField4EventList(), query);
		progress->setValue(30);
		
		if(this->offset==0)
		{
			progress->setLabelText(QString("Refreshing PlayList..."));
			string queryPlaylist = this->queryFactory->getEventListQuery(this->offset,
				this->getListField4PlayList(), ReviewModeFilter::ID , project_id, events, stime, etime, event_categories, annotations, search_condition, chk_search, 
				vin_id, chk_tour, start_clip, end_clip, days, weathers, roads, event_status, ai_typeid, ai_value, false);
			
			int recordsizedPlayList = this->listhandle->addItemsFromDB(m_oFilterGUI.list_playlist, playlistModel, this->getListField4PlayList(), queryPlaylist);

		}
		
		progress->setValue(60);
		

		this->initAnnotationCombo(this->m_oFilterGUI.cbo_annotation);
		string text = MagnaUtil::integerToString(recordsize) + " events have been selected in the " + MagnaUtil::integerToString(this->offset/100 + 1) + " page.";
		this->m_oFilterGUI.lbl_status->setText(QString(text.c_str()));
		progress->setValue(90);

		this->saveCurrentStatus(LOGOUT);
		progress->hide();

		if(current_offset>0 && recordsize==0)
		{
			refreshAnnotationGroup(current_offset-100);
			MagnaUtil::show_message("No Next Event!");
		}
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

vector<string> ReviewModeFilter::getDayTypes()
{
	vector<string> days;
	if(this->m_oFilterGUI.chk_day_1->isChecked()) days.push_back("1");
	if(this->m_oFilterGUI.chk_day_2->isChecked()) days.push_back("2");
	if(this->m_oFilterGUI.chk_day_3->isChecked()) days.push_back("3");

	return days;
}

vector<string> ReviewModeFilter::getWeatherTypes()
{
	vector<string> weather;
	if(this->m_oFilterGUI.chk_weather_1->isChecked()) weather.push_back("1");
	if(this->m_oFilterGUI.chk_weather_2->isChecked()) weather.push_back("2");
	if(this->m_oFilterGUI.chk_weather_3->isChecked()) weather.push_back("3");
	if(this->m_oFilterGUI.chk_weather_4->isChecked()) weather.push_back("4");

	return weather;
}

vector<string> ReviewModeFilter::getEventStatus()
{
	vector<string> event_status;
	if(this->m_oFilterGUI.chk_event_status_1->isChecked()) event_status.push_back("1");
	if(this->m_oFilterGUI.chk_event_status_2->isChecked()) event_status.push_back("2");
	if(this->m_oFilterGUI.chk_event_status_3->isChecked()) event_status.push_back("3");
	if(this->m_oFilterGUI.chk_event_status_4->isChecked()) event_status.push_back("4");
	if(this->m_oFilterGUI.chk_event_status_5->isChecked()) event_status.push_back("5");

	return event_status;
}

vector<string> ReviewModeFilter::getRoadTypes()
{
	vector<string> roads;
	if(this->m_oFilterGUI.chk_road_1->isChecked()) roads.push_back("1");
	if(this->m_oFilterGUI.chk_road_2->isChecked()) roads.push_back("2");
	if(this->m_oFilterGUI.chk_road_3->isChecked()) roads.push_back("3");

	return roads;
}

vector<string> ReviewModeFilter::get_condition_for_ai()
{
	vector<string> field;
	field.push_back("projectid");
	field.push_back("featureid");
	field.push_back("vin");
	field.push_back("eventcategoryid");

	return field;
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


vector<string> ReviewModeFilter::getListField4Project()
{
	vector<string> itemsFromDB;
	itemsFromDB.push_back("name");
	itemsFromDB.push_back("id");

	return itemsFromDB;
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

vector<string> ReviewModeFilter::getListField4PlayList()
{
	vector<string> itemsFromDB;
	itemsFromDB.push_back("clipname");

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
vector<string> ReviewModeFilter::getListField4AI()
{
	vector<string> itemsFromDB;
	itemsFromDB.push_back("typename");
	itemsFromDB.push_back("name");
	itemsFromDB.push_back("typeid");
	itemsFromDB.push_back("id");

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