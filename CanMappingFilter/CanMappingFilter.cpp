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
#include "CanMappingFilter.h"
#include "Utils/Utilities.h"
#include "XML/XMLHandler.h"
#include <QtGui/QFileDialog>
#include <QtGui/QTreeView>
#include <QtCore/QDir>

extern const std::string SYSTEM_RELEASE = "./config/system_release.xml";
// Create filter shell
ADTF_FILTER_PLUGIN("CanMappingFilter", OID_ADTF_HIL_CanMappingFilter, CanMappingFilter);

CanMappingFilter::CanMappingFilter(const tChar* __info):cBaseQtFilter(__info)
{
}

CanMappingFilter::~CanMappingFilter()
{
}

tResult CanMappingFilter::Init(tInitStage eStage, __exception)
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

tResult CanMappingFilter::Start(__exception)
{
	return cBaseQtFilter::Start(__exception_ptr);		// TODO	RETURN_IF_FAILED generates a warning [created on 07/02/2014]
}

tResult CanMappingFilter::Stop(__exception)
{
    return cBaseQtFilter::Stop(__exception_ptr);
}

tResult CanMappingFilter::Shutdown(tInitStage eStage, __exception)
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

    return cBaseQtFilter::Shutdown(eStage, __exception_ptr);
}

tResult CanMappingFilter::OnPinEvent(IPin *pSource, tInt nEventCode, tInt nParam1, tInt nParam2, IMediaSample *pMediaSample)
{
	RETURN_NOERROR;
}

tResult CanMappingFilter::Run(tInt nActivationCode, const tVoid* pvUserData, tInt szUserDataSize, ucom::IException** __exception_ptr/* =NULL */)
{
	return cBaseQtFilter::Run(nActivationCode, pvUserData, szUserDataSize, __exception_ptr);
}














tHandle CanMappingFilter::CreateView()
{
	m_pFilterWidget = new QWidget();
    m_oFilterGUI.setupUi(m_pFilterWidget);
	this->Init();
	this->registerEventHandler();

	return (tHandle)m_pFilterWidget;
}

void CanMappingFilter::Init()
{
	this->m_oFilterGUI.txt_vehicle->setText(QString(""));
	this->m_oFilterGUI.txt_fusion->setText(QString(""));
	this->m_oFilterGUI.txt_eyeq->setText(QString(""));
}

void CanMappingFilter::registerEventHandler()
{
	connect(m_oFilterGUI.btn_browse_vehicle, SIGNAL(clicked()), this, SLOT(on_btn_browse_vehicle_clicked()));
	connect(m_oFilterGUI.btn_browse_fusion, SIGNAL(clicked()), this, SLOT(on_btn_browse_fusion_clicked()));
	connect(m_oFilterGUI.btn_browse_eyeq, SIGNAL(clicked()), this, SLOT(on_btn_browse_eyeq_clicked()));
	
	connect(m_oFilterGUI.btn_apply, SIGNAL(clicked()), this, SLOT(on_btn_apply_clicked()));

}

tResult CanMappingFilter::on_btn_browse_vehicle_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this->m_pFilterWidget,
		tr("Open DBC file for Vehicle CAN"), "", tr(".dat files (*.dbc)"));
	this->m_oFilterGUI.txt_vehicle->setText(fileName);

	RETURN_NOERROR;
}

tResult CanMappingFilter::on_btn_browse_fusion_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this->m_pFilterWidget,
		tr("Open DBC file for Fusion CAN"), "", tr(".dbc files (*.dbc)"));
	this->m_oFilterGUI.txt_fusion->setText(fileName);
	
	RETURN_NOERROR;
}

tResult CanMappingFilter::on_btn_browse_eyeq_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this->m_pFilterWidget,
		tr("Open DBC file for EyeQ CAN"), "", tr(".dat files (*.dbc)"));
	this->m_oFilterGUI.txt_eyeq->setText(fileName);

	RETURN_NOERROR;
}

tResult CanMappingFilter::on_btn_apply_clicked()
{
	string txt_vehicle = this->m_oFilterGUI.txt_vehicle->text().toStdString();
	string txt_fusion = this->m_oFilterGUI.txt_fusion->text().toStdString();
	string txt_eyeq = this->m_oFilterGUI.txt_eyeq->text().toStdString();
	bool success = change_dbc_files(txt_vehicle, txt_fusion, txt_eyeq);
	if(success==true)
		MagnaUtil::restartADTF();

	RETURN_NOERROR;
}

bool CanMappingFilter::change_dbc_files(string txt_vehicle, string txt_fusion, string txt_eyeq)
{
	string att_name = "DBCFiles4Channel0";
	bool success;
	XMLHandler *t = new XMLHandler(SYSTEM_RELEASE);
	success = t->update("adtf:project", "general_settings", att_name, txt_vehicle);
	if(success==false) {
		MagnaUtil::show_message("Fail to Update!");
		return false;
	}

	att_name = "DBCFiles4Channel1";
	success = t->update("adtf:project", "general_settings", att_name, txt_fusion);
	if(success==false) {
		MagnaUtil::show_message("Fail to Update!");
		return false;
	}
	att_name = "DBCFiles4Channel2";
	success = t->update("adtf:project", "general_settings", att_name, txt_eyeq);
	if(success==false) {
		MagnaUtil::show_message("Fail to Update!");
		return false;
	}

	return true;
}


tResult CanMappingFilter::ReleaseView()
{

	RETURN_NOERROR;
}