/*****************************************************************************
 *     ADTF Template Project Filter
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


#include "stdafx.h"
#include "SimpleReviewModeFilter.h"
#include "XML/XMLHandler.h"
#include <QtGui/QFileDialog>
#include <QtGui/QTreeView>
#include <QtCore/QDir>

extern const std::string SYSTEM_RELEASE = "./config/system_release.xml";
extern const std::string REVIEW_WORKSPACE = "./review_workspace/";
extern const std::string TEMP_WORKSPACE = REVIEW_WORKSPACE + "temp/";
extern const std::string TEMP_FILE = TEMP_WORKSPACE + "tmp_simple_playback.xml";

// Create filter shell
ADTF_FILTER_PLUGIN("SimpleReviewModeFilter", OID_ADTF_HIL_SimpleReviewModeFilter, SimpleReviewModeFilter);

SimpleReviewModeFilter::SimpleReviewModeFilter(const tChar* __info):cBaseQtFilter(__info)
{
}

SimpleReviewModeFilter::~SimpleReviewModeFilter()
{
}

tResult SimpleReviewModeFilter::Init(tInitStage eStage, __exception)
{
    RETURN_IF_FAILED(cBaseQtFilter::Init(eStage, __exception_ptr))
    
    if (eStage == StageFirst)
    {
	}
	else if (eStage == StageNormal)
    {
		
    }
    else if (eStage == StageGraphReady)
    {
    }

    RETURN_NOERROR;
}

tResult SimpleReviewModeFilter::Start(__exception)
{
	return cBaseQtFilter::Start(__exception_ptr);
}

tResult SimpleReviewModeFilter::Stop(__exception)
{
    return cBaseQtFilter::Stop(__exception_ptr);
}

tResult SimpleReviewModeFilter::Shutdown(tInitStage eStage, __exception)
{
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

tResult SimpleReviewModeFilter::OnPinEvent(IPin *pSource, tInt nEventCode, tInt nParam1, tInt nParam2, IMediaSample *pMediaSample)
{
	RETURN_NOERROR;
}

tResult SimpleReviewModeFilter::Run(tInt nActivationCode, const tVoid* pvUserData, tInt szUserDataSize, ucom::IException** __exception_ptr/* =NULL */)
{
	return cBaseQtFilter::Run(nActivationCode, pvUserData, szUserDataSize, __exception_ptr);
}














tHandle SimpleReviewModeFilter::CreateView()
{
	m_pFilterWidget = new QWidget();
    m_oFilterGUI.setupUi(m_pFilterWidget);
	
	this->registerEventHandler();

	if(QFile().exists(QString(TEMP_FILE.c_str()))){
		this->restoreCurrentStatus();
	}else{
		this->initGUI();
	}
	
	
	return (tHandle)m_pFilterWidget;
}

void SimpleReviewModeFilter::initGUI()
{
	//XMLHandler *xmlHandle = new XMLHandler(tmpFile);
	this->m_oFilterGUI.btn_showclip->setEnabled(false);
	this->m_oFilterGUI.btn_next->setEnabled(false);
	this->m_oFilterGUI.btn_previous->setEnabled(false);
}

tResult SimpleReviewModeFilter::ReleaseView()
{
	MagnaUtil::delete_file(TEMP_FILE);
	RETURN_NOERROR;
}


void SimpleReviewModeFilter::registerEventHandler()
{
	connect(m_oFilterGUI.btn_clip, SIGNAL(clicked()), this, SLOT(on_btn_clip_clicked()));
	connect(m_oFilterGUI.btn_showclip, SIGNAL(clicked()), this, SLOT(on_btn_showclip_clicked()));

	connect(m_oFilterGUI.btn_next, SIGNAL(clicked()), this, SLOT(on_btn_next_clicked()));
	connect(m_oFilterGUI.btn_previous, SIGNAL(clicked()), this, SLOT(on_btn_prev_clicked()));
}

tResult SimpleReviewModeFilter::on_btn_next_clicked()
{
	if(this->fileList.empty()){
		MagnaUtil::show_message("Please select a clip.");
		RETURN_NOERROR;
	}
	
	string current_clip = this->m_oFilterGUI.txt_clip_directory->text().toStdString();
	int current_idx = this->getCurrentIdx(current_clip);
	int next_idx = current_idx+1;
	if(next_idx<this->fileList.size()){
		this->m_oFilterGUI.txt_clip_directory->setText(QString(this->fileList.at(next_idx).c_str()));
	}

	RETURN_NOERROR;
}

int SimpleReviewModeFilter::getCurrentIdx(string current_clip){
	for(int i = 0; i < fileList.size(); i++)
	{
		if(fileList.at(i)==current_clip)
		{
			return i;
		}
	}

	return -1;
}

tResult SimpleReviewModeFilter::on_btn_prev_clicked()
{
	if(this->fileList.empty()){
		MagnaUtil::show_message("Please select a clip.");
		RETURN_NOERROR;
	}

	string current_clip = this->m_oFilterGUI.txt_clip_directory->text().toStdString();
	int current_idx = this->getCurrentIdx(current_clip);
	int prev_idx = current_idx-1;
	if(prev_idx>-1){
		this->m_oFilterGUI.txt_clip_directory->setText(QString(this->fileList.at(prev_idx).c_str()));
	}

	RETURN_NOERROR;
}
tResult SimpleReviewModeFilter::on_btn_clip_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this->m_pFilterWidget,
		tr("Open DAT file"), "", tr(".dat files (*.dat)"));
	this->setFileList(fileName);

	this->m_oFilterGUI.txt_clip_directory->setText(fileName);
	this->m_oFilterGUI.btn_showclip->setEnabled(true);
	this->m_oFilterGUI.btn_next->setEnabled(true);
	this->m_oFilterGUI.btn_previous->setEnabled(true);
	this->m_oFilterGUI.txt_clip_directory->setEnabled(false);

	RETURN_NOERROR;
}

void SimpleReviewModeFilter::setFileList(QString fileName)
{
	this->fileList.clear();
	QDir dir = QFileInfo(fileName).absoluteDir();
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);
	QFileInfoList list = dir.entryInfoList();

	for (int i = 0; i < list.size(); ++i) {
		string current_file_path = list.at(i).absoluteFilePath().toStdString();
		vector<string> tokens = MagnaUtil::stringTokenizer(current_file_path, '.');
		if(tokens.size()>1)
		{
			string ext = tokens.at(1);
			if(ext=="dat"){
				this->fileList.push_back(current_file_path);
			}
		}
		
	}
}

tResult SimpleReviewModeFilter::on_btn_showclip_clicked()
{
	string clip = this->m_oFilterGUI.txt_clip_directory->text().toStdString();

	if(clip.empty())
	{
		MagnaUtil::show_message("No .dat clip.");
	}
	else
	{
		bool result = this->changeDatFile(clip);
		if(result==true)
		{
			this->saveCurrentStatus();
			MagnaUtil::restartADTF();
		}
	}

	RETURN_NOERROR;
}

void SimpleReviewModeFilter::saveCurrentStatus()
{
	XMLHandler xmlHandle(TEMP_FILE);
	xmlHandle.open();
	xmlHandle.addHeader("backup");

	string clip = this->m_oFilterGUI.txt_clip_directory->text().toStdString();
	xmlHandle.addItem("clip_url", "value", clip);
	
	xmlHandle.addFooter("backup");
	xmlHandle.close();
}

void SimpleReviewModeFilter::restoreCurrentStatus()
{
	XMLHandler xmlHandle(TEMP_FILE);

	string clip = xmlHandle.getNodeValue("clip_url", "value");
	this->m_oFilterGUI.txt_clip_directory->setEnabled(false);
	this->m_oFilterGUI.txt_clip_directory->setText(QString(clip.c_str()));
	this->setFileList(QString(clip.c_str()));
}


bool SimpleReviewModeFilter::changeDatFile(string dat)
{

	if(dat.empty())
	{
		MagnaUtil::show_message("Please select the clip-directory");
	}

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