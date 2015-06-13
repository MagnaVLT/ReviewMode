#pragma once
#include <string>
#include <stdio.h>
#include <sstream>
#include <istream>
#include <iostream>
#include <fstream>
#include <iterator>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTime>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <process.h>
#include <windows.h>
#include <ShellAPI.h>
#include <direct.h>
#define GetCurrentDir _getcwd

using namespace std;

class MagnaUtil
{

public:
	static string integerToString(int i) 
	{
		stringstream s;
		s << i;
		return s.str();
	}

	static string bigIntegerToString(tInt64 i) 
	{
		stringstream s;
		s << i;
		return s.str();
	}

	static string boolToString(bool i) 
	{
		stringstream s;
		if(i==0) s << "true";
		else s << "false";
		
		return s.str();
	}

	static string longToString(unsigned long i) 
	{
		stringstream s;
		s << i;
		return s.str();
	}

	static string longLongIntToString(__int64 i) 
	{
		stringstream s;
		s << i;
		return s.str();
	}

	static int stringTointeger(string s)
	{		
		std::istringstream is(s); 
		int i; 
		is >> i; 
		return i;
	}

	static unsigned long stringToLong(string s)
	{
		std::istringstream is(s);
		unsigned long i;
		is >> i; 
		return i;
	}

	static __int64 stringToLongLongInt(string s)
	{
		std::istringstream is(s);
		__int64 i;
		is >> i; 
		return i;
	}

	static string removeURLOfFile(string URL)
	{
		
		replace( URL.begin(), URL.end(), '\\', '/' );
		
		//if(URL.at(0)=='.')
		//{
		vector<string> tokens = MagnaUtil::stringTokenizer(URL, '/');
		URL = tokens.at(tokens.size()-1);

		/*}else
		{
			char* buf = 0;
			size_t sz = 0;
			_dupenv_s(&buf, &sz, envName.c_str());
			string magna_home = string(buf);
			URL.replace(0, magna_home.size()+1, "");
		}*/


		return URL;
	}
	static vector<string> getTokenedVector(vector<string> list, char delim, int loc)
	{
		vector<string> tokened_vector;
		for each(string i in list)
		{
			tokened_vector.push_back(stringTokenizer(i, delim).at(loc));
		}

		return tokened_vector;
	}

	static void start_process(string URL)
	{
		string temp = "/C " + URL;
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("cmd.exe"), LPCSTR(temp.c_str()), NULL, SW_HIDE);
	}

	static int start_process_with_waiting(string URL)
	{
		return system(URL.c_str());
	}

	static int get_process_id()
	{
		return _getpid();
	}

	static void kill_by_pid(int pid)
	{
		HANDLE handy;
		handy =OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, TRUE,pid);
		TerminateProcess(handy,0);
	}


	static char* getFileURLOf(string fileName, string envName)
	{
		char* buf = 0;
		size_t sz = 0;
		_dupenv_s(&buf, &sz, envName.c_str());

		string env = string(buf);
		string filename = env + "\\" + fileName;

		char *coorFileURL = new char[filename.length() + 1];
		strcpy_s(coorFileURL, filename.length()+1, filename.c_str());

		return coorFileURL;
	}

	static bool FileExists(const char *fileName)
	{	
		bool file_exists;
		FILE * f = fopen(fileName, "rb"); 
		if (f == NULL) { 
			file_exists = false;
		} else { 
			file_exists = true; 
			fclose(f);
		}
		return file_exists;
	}

	static char* concatenateCharArray(const char* c1, const char* c2)
	{
		char* full_text= (char*) malloc(strlen(c1)+strlen(c2)+1); 
		strcpy(full_text, c1); 
		strcat(full_text, c2);

		return full_text;
	}

	static void copyfile(string from, string to, QWidget *parent) {
		FILE * filer, * filew;
		int numr,numw;
		char buffer[4096];
		
		if(isExist(from, to))
		{
			QProgressDialog *progress = new QProgressDialog(parent);
			progress->setWindowTitle(QString("Copying DAT Clips..."));
			progress->autoClose();
			string fileName = MagnaUtil::removeURLOfFile(from);
			fileName += " is copying...";
			progress->setLabelText(QString(fileName.c_str()));
			progress->setWindowModality(Qt::WindowModal);
			QFile src(from.c_str());
			qint64 len = src.bytesAvailable();
			src.close();
			progress->setRange(0,len);
			progress->show();

			if((filer=fopen(from.c_str(),"rb"))==NULL){
				fprintf(stderr, "open read file error.\n");
				exit(1);
			}

			if((filew=fopen(to.c_str(),"wb"))==NULL){
				fprintf(stderr,"open write file error.\n");
				exit(1);
			}
			bool isCancel = false;
			while(feof(filer)==0){	
				if((numr=fread(buffer,1,4096,filer))!=4096){
					if(ferror(filer)!=0){
						fprintf(stderr,"read file error.\n");
						exit(1);
					}
//					else if(feof(filer)!=0);
				}

				if((numw=fwrite(buffer,1,numr,filew))!=numr){
					fprintf(stderr,"write file error.\n");
					exit(1);
				}

				QApplication::processEvents();
				progress->setValue(progress->value()+1);
				if(progress->wasCanceled())
				{
					show_message("The copying has been canceled. \n The temporal clip will be deleted.");
					isCancel=true;
					break;
				}
			}

			progress->setValue(len);
			progress->hide();
			fclose(filer);
			fclose(filew);
			if(isCancel) delete_file(to);
		}
		
	}
	static void make_directory(string url)
	{
		if(!QDir(QString(url.c_str())).exists())
			QDir().mkdir(QString(url.c_str()));
	}

	static bool isExist(string check_file)
	{
		ifstream tmp_file(check_file.c_str());
		return tmp_file.is_open();
	}
	static bool isExist(string from, string to)
	{
		ifstream from_file(from.c_str());
		ifstream to_file(to.c_str());

		if(!from_file.is_open())
		{
			show_message("There is no clip.");
			return false;
		}

		if(to_file.is_open())
		{
			if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "CAUTION!!", "There is already the file having the same name. \n Remove it?", QMessageBox::Yes|QMessageBox::No).exec()) 
			{
				return true;
			}else{
				return false;
			}
		}

		from_file.close();
		to_file.close();

		return true;
		
	}

	static string charToString(char* data, std::size_t length)
	{
		data[length] = '\0';
		std::string str = std::string(data);
		return str;
	}

	static std::vector<std::string> stringTokenizer(std::string s, char delim)
	{	
		vector<string> tokens;
		stringstream ss(s);
		string token;

		
		while (getline(ss, token, delim)) {
			tokens.push_back(token);
		}

		return tokens;
	}

	

	static string convertTimestampToDate(string unixtime){
		int i_unixtime;
		sscanf(unixtime.c_str(), "%d", &i_unixtime);
		QDateTime timestamp;
		timestamp.setTime_t(i_unixtime);
		QString date_time = timestamp.toString(Qt::SystemLocaleShortDate);

		return date_time.toStdString();
	}

	static string convertMicroSecondToTime(string str_timestamp)
	{
		
		__int64 timestamp = MagnaUtil::stringToLongLongInt(str_timestamp);
		__int64 milliseconds = (__int64) (timestamp / 1000);
		__int64 seconds = (__int64) ((milliseconds / 1000) % 60);
		__int64 minutes = (__int64) (((milliseconds / 1000) / 60) % 60);
		__int64 hours = (__int64) ((((milliseconds / 1000) / 60) / 60) % 24);
		milliseconds = milliseconds - ((hours * 3600000) + (minutes * 60000) + (seconds * 1000));

		string sec, min, hrs;
		if(seconds<10)  sec="0"+MagnaUtil::longLongIntToString(seconds);
		else            sec= ""+MagnaUtil::longLongIntToString(seconds);
		if(minutes<10)  min="0"+MagnaUtil::longLongIntToString(minutes);
		else            min= ""+MagnaUtil::longLongIntToString(minutes);
		if(hours<10)    hrs="0"+MagnaUtil::longLongIntToString(hours);
		else            hrs= ""+MagnaUtil::longLongIntToString(hours);

		string r_time = hrs + ":" + min + ":" + sec + ":" + MagnaUtil::longLongIntToString(milliseconds);

		return r_time;
	}

	static tUInt64 MagnaUtil::getCurrentSystemTime()
	{
		cMutex m_oTimeStampMutex;
		volatile WORD m_wPreMilli;
		volatile WORD m_wPreSec;
		SYSTEMTIME systemTime;
		GetSystemTime(&systemTime);
		
		m_oTimeStampMutex.Enter();
		
		GetSystemTime(&systemTime);		
		m_wPreMilli = systemTime.wMilliseconds;
		m_wPreSec = systemTime.wSecond;

		m_oTimeStampMutex.Leave();
		
		tUInt64 result = systemTime.wYear;
		result = (((((result*100 + systemTime.wMonth)*100 + systemTime.wDay)*100 + systemTime.wHour)*100 + systemTime.wMinute)*100 + systemTime.wSecond)*1000 + systemTime.wMilliseconds;
		
		return result;
	}

	static tUInt64 MagnaUtil::getCurrentLocalTime()
	{
		SYSTEMTIME localTime;

		GetLocalTime(&localTime);

		tUInt64 result = localTime.wYear;
		result = (((((result*100 + localTime.wMonth)*100 + localTime.wDay)*100 + localTime.wHour)*100 + localTime.wMinute)*100 + localTime.wSecond)*1000 + localTime.wMilliseconds;

		return result;
	}


	static string MagnaUtil::convertSystemTimeToTime(string sysTime){
		string y = sysTime.substr(2, 2);
		string m = sysTime.substr(4, 2);
		string d = sysTime.substr(6, 2);
		string h = sysTime.substr(8, 2);
		string min = sysTime.substr(10, 2);
		string s = sysTime.substr(12, 2);
		string ms = sysTime.substr(14, 3);
		return y + "/" + m + "/" + d + " " + h + ":" + min + ":" + s + ":" + ms;
	}

	static __int64 MagnaUtil::convertSystemTimeToSecond(string sysTime){
		__int64 y = MagnaUtil::stringTointeger(sysTime.substr(2, 2));
		__int64 m = MagnaUtil::stringTointeger(sysTime.substr(4, 2));
		__int64 d = MagnaUtil::stringTointeger(sysTime.substr(6, 2));
		__int64 h = MagnaUtil::stringTointeger(sysTime.substr(8, 2));
		__int64 min = MagnaUtil::stringTointeger(sysTime.substr(10, 2));
		__int64 s = MagnaUtil::stringTointeger(sysTime.substr(12, 2));
		
		s += (y* 365* 30 * 24* 60 * 60) + (m * 30 * 24 * 60* 60) + (d * 24 * 60*60) + (h * 60 * 60) + (min *60);
		return s;
	}

	static string MagnaUtil::getMicroSecond(string m, string s, string ms)
	{
		string timeStamp = "";


		long i_m = MagnaUtil::stringToLong(m);
		long i_s = MagnaUtil::stringToLong(s);
		long i_ms = MagnaUtil::stringToLong(ms);
		long result = (i_m * 60 * 1000) + (i_s * 1000) + i_ms;

		timeStamp = MagnaUtil::longToString(result) + "000";


		return timeStamp;
	}


	static void show_message(string message)
	{
		QMessageBox box;
		box.setText(QString(message.c_str()));
		box.exec();
	}
	
	static void delete_file(string file_name)
	{
		remove(file_name.c_str());
		//if( remove( file_name.c_str() ) != 0 )
		//	MagnaUtil::show_message("Error deleting temp file");
	}

	static bool Contains(vector<string> v, string str )
	{
		if(v.size()==0) return false;
		for(unsigned int i = 0 ; i < v.size() ; i++)
		{
			if(v.at(i).compare(str)==0) {
				return true;
			}
		}
		return false;
	}

	static bool Contains(map<string, string> m, string str )
	{
		if(m.size()==0) return false;
		if(m.find(str)==m.end()) return false;
		else return true;
	}

	static map<string, string> convertVectorToMap(vector<string> data)
	{
		map<string, string> converted_map;
		for each(string d in data)
		{
			converted_map[d.c_str()] = d.c_str();
		}

		return converted_map;

	}

	static bool compare_string_vector(vector<string> a, vector<string> b) 
	{
		if(a.size() != b.size()) {
			return false;
		}

		for(unsigned int i = 0 ; i < a.size() ; i++)
		{
			if(Contains(b, a.at(i))==false)
			{
				return false;
			}
		}
		return true;
	}

	static void restartADTF()
	{
		MagnaUtil::start_process("Review.bat");
		MagnaUtil::kill_by_pid(MagnaUtil::get_process_id());
	}

	static void log_to_file(string log, string fileName)
	{
		std::ofstream out(fileName.c_str());
		out << log;
		out.close();
	}


	static string getCurPath() {
		char filePath[256];
		GetCurrentDir(filePath, sizeof(filePath));
		string str_path (filePath);
		replace( str_path.begin(), str_path.end(), '\\', '/' );
		return str_path;
	}

};