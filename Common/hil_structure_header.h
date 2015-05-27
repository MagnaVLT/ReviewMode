/*****************************************************************************
 *     ADTF STRUCTURE HEADER for VLT 3.0
 *****************************************************************************
 *  
 *	@file
 *	Copyright &copy; Magna Electronics in Auburn Hills. All rights reserved
 *	
 *	$Author: Mo Zhao (E38658)	Sangsig Kim (E37039)$ 
 *	$Comany: Magna Electronics $
 *	$Reference: Magana_HIL_Source (VLT 2.5, revision 1.13) by Jin Jin$
 *	$Date: 2015/01/29 00:17:00 EST $
 *	$Revision: 3.0 $
 *
 *	@remarks
 *
 ******************************************************************************/


/* This defines the way of Byte Alignment of the structured elements */ 
#pragma pack(push,8)

int const MAGNA_VERSION_ID = 3;				// sEnvironmentalDataThe main change of the system: Demo - 1, GEN2.0 & GEN2.5 Related Release - 2, GEN3.0 Related Release - 3
int const MAGNA_VERSION_MAIN = 1;			// sEnvironmentalDataMassive Update version.
int const MAGNA_VERSION_MINOR = 0;

/* Set Customer */
#define _Chrysler 1
#define _Mazda 0

/* Macro functions to match PINs of a specific customer*/
#define MACRO2STR0(name) #name
#define MACRO2STR(name) MACRO2STR0(name)
#define TYPE_CAN0(name) t ## name
#define TYPE_CAN(name) TYPE_CAN0(name)
#define STRUCT_CAN(name) s ## name
#define STRUCT_DECLARE(name) TYPE_CAN(name) STRUCT_CAN(name)

#include "../Common/Bosch_CAN_Selection.h"

/* Event Data structure is used for a media sample between Event Observer filter and SQL filter */
typedef struct
{
	tUInt64				ui64ReportID;				// Corresponding ReportID from SQL DB
	tUInt64				ui64ClipID;					// Corresponding SampleID from SQL DB
	tUInt				nEventID;					// Event ID
	string				strEventName;				// Event name; needed for GUI
	tUInt64				ui64LocalPCTime;			// Local PC absolute time
	tInt64				i64EventADTFTime;			// ADTF returns signed integer
	string				strUserAnnotation;
	tUInt				uiEventStatus;
	tFloat64			f64GPSLongitude;
	tFloat64			f64GPSLatitude;
}tEventData;

// TODO	need to check signed or unsigned for each data
typedef struct
{
	tInt16				nYawRaw;
	tInt16				nHorizonRaw;
	tInt16				nRollRaw;
	tInt8				nAutofixYaw;
	tInt8				nAutofixHorizon;
	tInt8				nDrivingside;
}tInitConfigData;

typedef struct
{
	tUInt64				ui64LowVIN;
	tUInt64				ui64MedVIN;
	tUInt64				ui64HighVIN;
}tVINData;

typedef struct
{
	string				strDriverName;
	string				strRecPurpose;
	string				strDay;
	string				strWeather;
	string				strRoadType;
}tInitPropertyData;

/* The size of each char array has to be defined here! */
#define SIZEGUISTRUCT 3

/* 1 - Maskcheck Enabled; 2 - Maskcheck Disabled */
#define MASKINIT 2

#define ADTF210

/* Complete Set of relevant GPS Data from GPS Filter to SQL Filter */
typedef struct
{
	/* Content of RMC Message */
	tFloat64    f64PositionLat;                 // converted to decimal degrees
	tFloat64    f64PositionLon;                 // converted to decimal degrees
	tFloat64    f64SpeedOverGroundKnots;        // Speed over Ground in Knots 
	tFloat64    f64TrackMadeGoodDegreesTrue;    // Heading direction in decimal degrees
	tChar       chModus[2];                     // RMC: A-Autonomous, D-Differential, E-Estimated, N-NotValid, S-Simulated
	tInt        intInvalidCounter;              // Counter for RMC Signal being INVALID
	tChar       chTimestamp[16];                // Contains complete Time information; format: YYYYMMDDhhmmss

	/* Additional Information from GGA Message */
	tFloat64    f64Elevation;                   // Elevation above sea level in meter
	tInt        nNumberOfSatellitesInUse;       // GGA: Number of Satellites in use (0-12)
	tInt        nGPSQuality;                    // GGA: 0-NotValid, 1-GPS, 2-DGPS, 3-GPSPPS, 6-Estimated
	tFloat64    f64InclineSlope;                // Incline Slope in percent
	tFloat64    f64InclineAngle;                // Incline Slope in degrees
}tGpsData;

/* Truncated Set of Position and Time from GPS Filter to Trigger WOEID and SunAngle Filter */
typedef struct
{
	tFloat64    f64PositionLat;                 // converted to decimal degrees
	tFloat64    f64PositionLon;                 // converted to decimal degrees
	tBool		bGpsSignalValid;

}tTruncGps;

/* This Struct contains information about the local time characterized by its UTC Offset and a DST flag */
typedef struct
{
	tInt        intUtcOffset;                   // UTC Offset by timezone
	tChar       chDayligtSavingTime[8];         // Daylight Saving Time?

}tUtcOffset;

/* Filename sent from Trigger to SQL Filter */
typedef struct
{
	/* From Trigger */
	tChar       chFileName[256];                // Path of Filename of the recorded .dat file
	tInt64		nSampleStreamTime;				// Stream Time that gets determined upon arrival at Multiplex Filter
	tChar		chLocalPcTime[16];				// PC Time that gets determined upon arrival at Multiplex Filter

}tTriggerFilename;

/* Sun Angles sent from Sun Angle Filter to SQL Filter */
typedef struct
{
	tFloat64    f64SolarElevation;              // In decimal degrees
	tFloat64    f64SolarAzimuth;                // In decimal degrees
	tInt        intFacingSunCounter;            // How often did we face the sun during the last clip?

}tSunAngles;

typedef struct
{
	tInt	intFacingSunStatus;
}tFacingSunStatus;

/* WOEID passed from Woeid Filter to Weather Filter */
typedef struct
{
	tInt32      intWoeid;
	tInt32      intZipCode; // changed from tChar chZip[16];
	tChar       chHouse[16];
	tChar       chStreet[32];
	tChar       chCounty[32];
	tBool       bInternetConnection;
	tBool		bGpsAvailable;
}tWoeid;

/* WeatherData from Weather Filter to SQL Filter */
typedef struct
{
	/* from Weather Filter */
	tChar       chCity[32];
	tChar       chRegion[32];
	tChar       chCountry[32];
	tChar       chDate[32];
	tChar       chCondition[32];
	tChar       chSunRise[8];              // 10
	tChar       chSunSet[8];               // 10
	tChar       chUnitTemp[8];              // 2
	tChar       chUnitDist[8];              // 3
	tChar       chUnitPress[8];             // 3
	tChar       chUnitSpeed[8];             
	tInt32      intWindChill;
	tInt32      intWindDirection;
	tInt32      intWindSpeed;
	tInt32      intYahooConditionCode;
	tInt32      intHumidity;
	tInt32      intVisibility;
	tInt32      intPressureTrend;
	tInt32      intTemperature;
	tFloat64    f64Pressure;
	tFloat64    f64ObservationPosLat;
	tFloat64    f64ObservationPosLon;
	
	/* from WOEID Filter */
	tInt32      intWoeid;
	tInt32      intZipCode; // changed from tChar chZip[16];
	tChar       chHouse[16];
	tChar       chStreet[32];
	tChar       chCounty[32];
	tBool       bInternetConnection;
	tBool		bGpsAvailable;

	/* from UTC Filter */    
	tInt        intUtcOffset;               // UTC Offset by timezone
	tChar       chDayligtSavingTime[8];     // Daylight Saving Time?

	/* from Multiplex Filter */
	tInt		intEnvClassWeather;			// Possible states: 1 - CLEAR, 2 - CLOUDY, 3 - RAIN, 4 - SNOW, 9999 - UNKNOWN
	tInt		intDayNightTimeIndicator;	// if local time < 6:00 and > 18:00 => 0 - NIGHT, else 1 - DAY

	/* YAHOO's weather conditions: */
	/*******************************
	0 	tornado
	1 	tropical storm
	2 	hurricane
	3 	severe thunderstorms
	4 	thunderstorms
	5 	mixed rain and snow
	6 	mixed rain and sleet
	7 	mixed snow and sleet
	8 	freezing drizzle
	9 	drizzle
	10 	freezing rain
	11 	showers
	12 	showers
	13 	snow flurries
	14 	light snow showers
	15 	blowing snow
	16 	snow
	17 	hail
	18 	sleet
	19 	dust
	20 	foggy
	21 	haze
	22 	smoky
	23 	blustery
	24 	windy
	25 	cold
	26 	cloudy
	27 	mostly cloudy (night)
	28 	mostly cloudy (day)
	29 	partly cloudy (night)
	30 	partly cloudy (day)
	31 	clear (night)
	32 	sunny
	33 	fair (night)
	34 	fair (day)
	35 	mixed rain and hail
	36 	hot
	37 	isolated thunderstorms
	38 	scattered thunderstorms
	39 	scattered thunderstorms
	40 	scattered showers
	41 	heavy snow
	42 	scattered snow showers
	43 	heavy snow
	44 	partly cloudy
	45 	thundershowers
	46 	snow showers
	47 	isolated thundershowers
	3200 	not available
	*******************************/

}tWeatherData;

/* Set of Environmental Data as a composition of all other structs */
typedef struct
{
	tUInt64				EnvironmentalID;
	tGpsData			stGpsData;
	//tTriggerFilename	stTriggerFilename;
	tSunAngles			stSunAngles;
	tWeatherData		stWeatherData;
	//tInt				nTriggered;	
    //tInt64          nCurrentSampleID;
}tEnvironmentals;

/* Set of Diagnostics Data */
typedef struct
{
	tUInt64	DiagnosticsID;
	tInt	nStatusGps;					// 0 - no error, 1 - Signal invalid, 2 - No GPS Signal, 9999 - unknown
	tInt	nStatusVehCan;				// 0 - no error, 1 - Vehicle CAN not received, 9999 - unknown
	tInt	nStatusFusCan;				// 0 - no error, 1 - Fusion CAN not received, 9999 - unknown
	tInt	nStatusPvtCan;				// 0 - no error, 1 - Received negative indicator, 2 - No indicator received, 9999 - unknown
	tInt	nStatusInternet;			// 0 - no error, 1 - Internet connected but environmental Media Sample unavailable, 2 - No Internet connection, 9999 - unknown
	tInt	nStatusDatabase;			// 0 - no error, 1 - no status update received , 2 - exception caught, 9999 - unknown
	tInt	nStatusRAMdisk;				// 0 - no error, 1 - Not Recognized, 2 - Disk Space low, 3 - Disk Space Full, 9999 - unknown
	tInt	nStatusHarddisk;			// 0 - no error, 1 - No Hard disk connected for storage, 2 - one of hard disks has low space, 3 - one of hard disks has no space (almost full), 4 - All of hard disks have low space, 5 - All of hard disks have no space, 9999 - unknown
	string	sFullDisk;					// Each char denotes the name of a hard disk that has no storage space
	string	sLowDisk;					// Each char denotes the name of a hard disk that has low storage space
	//string	sFailDisk;					// Each char denotes the name of a hard disk that has connection problems
	tFloat	fRelFreeSpace;				//
	tInt64	nAbsFreeSpace;				// 
	tInt64	nTimeStampOfStatusChange;	// ADTF time of data submission
	tInt	nSystemHealth;				// 0 - no error (GREEN), 1 - Non-critical (YELLOW), 2 - Critical (RED) , 9999 - unknown
	//tInt	nAutoStatusUpdate;			// 0 - On Change Report, 1 - Automatic status update
	//tInt	nStatusRecording;			// 0 - Recording quality is perfect, 1 - Recording quality is acceptable, 2 - Recording quality is too low, 9999 - unknown
    //tInt64	nCurrentSampleID;
}tDiagnostics;


const tFloat64 SpeedCalib[] = {0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};
const tInt MAX_SPEED_INDEX = 21;

/* Set of Statistics Data */
typedef struct
{
	tUInt64		StatisticsID;
	tFloat64	fSpeedBins[MAX_SPEED_INDEX+1];
	tFloat64	fWeatherKMBins[10];
	tFloat64	fDayNightKMBins[2];
	tFloat64	fFacingSunKM;
	tUInt64		nTimeLength;
	tInt64		nTimeStamp;
	//tInt64      nSampleID;
}tStatistics;

/* Set of GUI Struct Data */
typedef struct
{
	tUInt64				ui64ReportID;				// Corresponding ReportID from SQL DB
	tUInt				nEventID;					// Event ID
	tUInt64				ui64LocalPCTime;			// Local PC absolute time
	tInt64				i64EventADTFTime;			// ADTF returns signed integer
	string				strUserAnnotation;	
	tUInt				uiEventStatus;
}tGUIData;

typedef struct
{
	tInt  nID;
	tChar chFirstName[32];
	tChar chLastName[32];
	tChar chCompany[32];
}tDriver;

typedef struct
{
	tInt  nID;
	tChar chModel[32];
	tChar chMake[32];
	tChar chCompany[32];
	tChar chVIN[32];
	tChar chNum[32];
}tVehicle;

typedef struct
{
	tChar		chEyeQVers[32];
	tChar		chADTFVers[32];
	tChar		chDatDir[32];
	tInt		bIsTestData;
	tDriver		sDriver;
	tVehicle	sVehicle;
}tProperty;

typedef struct
{	
	tChar	header_info[256];
}tMEHeader;

enum tSQL_REQTYPE
{
	NONE = 0,				// Do nothing
	Diag_senddata = 1,		//Data request to Diagnostic filter;
	Stat_calculation = 2,	//Start calculation request to Statistics filter;
	Stat_senddata = 3,		//Data request to Statistics filter;
	Env_senddata = 4,		//Data request to Environmental filter
	GUI_start = 5,			//File Open indication to GUI filter
}; // specifies type of SQL Request Signal



enum eSQL_STATUS
{
	DB_STATUS_CONST_DEFAULT = 0,
	SQL_QUERY_SUCC = 1, 
	SQL_QUERY_FAIL = 2,
	ENV_QUERY_SUCC = 11,
	ENV_QUERY_FAIL = 12,
	DGS_QUERY_SUCC = 21,
	DGS_QUERY_FAIL = 22,
	STATS_QUERY_SUCC = 31,
	STATS_QUERY_FAIL = 32, 
	EVENT_QUERY_SUCC = 41,
	EVENT_QUERY_FAIL = 42
};// specifies type of Database status

typedef tInt tSQL_STATUS;

enum eREC_BTN_STATUS
{
	ENABLE = 0,
	READY_2_REC = 1, 
	RECORDING = 2, 
	STOP_RECORDING = 3
};

typedef tInt tREC_BTN_STATUS;

enum eEVT_STATUS
{
	UNCONFIRMED = 1,		// unconfirmed 
	ACCEPTED = 2,			// confirmed and correct
	ANNOTATED = 3,			// uncertain events, so need to check
	DECLINED = 4,			// false positive
	MISSED = 5				// false negative
};

typedef tInt tEVT_STATUS;

typedef struct
{
	tBool			Trigger_Value;
	tMeas_Status	Meas_Status;
	tMeas_Trigger	Meas_Trigger;
	tEventData		EventData;
}tEvent_Trigger;

#pragma pack(pop)





/*****************************************************************************
 *     Change Log
 *****************************************************************************
 *     
 *
 *     
 *
 *****************************************************************************/