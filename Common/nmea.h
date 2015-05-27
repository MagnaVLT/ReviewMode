/**********************************
** ================================
** $Revision: 1.11 $
** $Date: 2013/08/30 10:19:05EDT $
** $CompanyInfo$
** $Author: Jin Jin (E31083) $
** ================================
**********************************/

/**
 *
 * ADTF NMEA-0183 Format (National Marine Electronics Association).
 * http://www.kowoma.de/gps/zusatzerklaerungen/NMEA.htm
 *
 * @file
 * Copyright &copy;  Audi Electronics Venture GmbH. All rights reserved
 *
 * @author               $Author: Jin Jin (E31083) $
 * @date                 $Date: 2013/08/30 10:19:05EDT $
 * @version              $Revision: 1.11 $
 *
 */

#ifndef _NMEA_H_
#define _NMEA_H_

#pragma pack(push, 1)

typedef enum 
{
    North = 0,
    South = 1

} tNorthSouth;

typedef enum 
{
    East = 0,
    West = 1

} tEastWest;

/**
 * GGA - Global Positioning System Fix Data
 * Time, Position and fix related data for a GPS receiver.
 *
 *        1          2         3 4          5 6 7  8   9  10 11 12 13  14   15
 *        |          |         | |          | | |  |   |   | |   | |   |    |
 * $--GGA,hhmmss.sss,ddmm.mmmm,N,dddmm.mmmm,W,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
 *
 * Field Number: 
 *  1) Universal Time Coordinated (UTC)
 *  2) Latitude
 *  3) N or S (North or South)
 *  4) Longitude
 *  5) E or W (East or West)
 *  6) GPS Quality Indicator,
 *     0 - Fix not available or invalid
 *     1 - GPS SPS Mode, fix valid
 *     2 - Differential GPS, SPS Mode, fix valid
 *     3 - GPS PPS Mode, fix valid
 *     6 - Estimated (since NMEA-0183 Version 2.3) 
 *  7) Number of satellites in view, 00 - 12
 *  8) Horizontal Dilution of precision
 *  9) Antenna Altitude above/below mean-sea-level (geoid) 
 * 10) Units of antenna altitude, meters
 * 11) Geoidal separation, the difference between the WGS-84 earth
 *     ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level below ellipsoid
 * 12) Units of geoidal separation, meters
 * 13) Age of differential GPS data, time in seconds since last SC104
 *     type 1 or 9 update, null field when DGPS is not used
 * 14) Differential reference station ID, 0000-1023
 * 15) Checksum
 *
 */
typedef struct
{
    tInt        nLatitudeDegree;     // Degree
    tFloat64    f64LatitudeMinutes;  // Minute Degree
    tNorthSouth eNSOrientation;      // North / South

    tInt        nLongitudeDegree;    // Degree
    tFloat64    f64LongitudeMinutes; // Minute Degree
    tEastWest   eEWOrientation;      // East / West

    tUInt16     ui16UTCTimeHour;
    tUInt16     ui16UTCTimeMinute;
    tUInt16     ui16UTCTimeSecond;
    tUInt32     ui32UTCTimeMilliseconds;

    tInt        nGPSQuality;
    tInt        nNumberOfSatellitesInUse;
    tFloat64    f64HorizontalDilutionOfPrecision;
    tFloat64    f64AntennaAltitudeMeters;
    tFloat64    f64GeoidSeparationMeters;
    tFloat64    f64AgeOfDifferentialGPSDataSeconds;
    tInt        nDifferentialReferenceStationID;

    tFloat64 GetLatitude() const
    {
        tFloat64 f64Latitude = (tFloat64)this->nLatitudeDegree + (this->f64LatitudeMinutes / 60.0);
        return (this->eNSOrientation == North ? f64Latitude : -f64Latitude);
    }

    tFloat64 GetLongitude() const
    {
        tFloat64 f64Longitude = (tFloat64)this->nLongitudeDegree + (this->f64LongitudeMinutes / 60.0);
        return (this->eEWOrientation == East ? f64Longitude : f64Longitude);
    }

    tBool IsValid()
    {
        return (this->nGPSQuality > 0 && this->nGPSQuality < 6);
    }

} tNMEAGGA;

/**
 * RMC - Recommended Minimum Navigation Information
 *                                                                12 
 *        1          2 3         4 5          6 7   8   9    10 11 | 13 
 *        |          | |         | |          | |   |   |    |   | | |
 * $--RMC,hhmmss.sss,A,ddmm.mmmm,N,dddmm.mmmm,W,x.x,x.x,xxxx,x.x,E,W*hh<CR><LF>
 *
 * Field Number: 
 *  1) UTC Time
 *  2) Status, V = Navigation receiver warning
 *  3) Latitude
 *  4) N or S
 *  5) Longitude
 *  6) E or W
 *  7) Speed over ground, knots
 *  8) Track made good, degrees true
 *  9) Date, ddmmyy
 * 10) Magnetic Variation, degrees
 * 11) E or W
 * 12) Modus (A,D,E,N,S)
 *      - A = Autonom
 *      - D = Differentiell
 *      - E = geschaetzt (Estimated) 
 *      - N = ungueltig (Not valid)
 *      - S = Simulator
 * 13) Checksum
 */
typedef struct
{
    tInt        nLatitudeDegree;     // Degree
    tFloat64    f64LatitudeMinutes;  // Minute
    tNorthSouth eNSOrientation;      // North / South

    tInt        nLongitudeDegree;    // Degree
    tFloat64    f64LongitudeMinutes; // Minute
    tEastWest   eEWOrientation;      // East / West

    tUInt16     ui16UTCTimeHour;
    tUInt16     ui16UTCTimeMinute;
    tUInt16     ui16UTCTimeSecond;
    tUInt32     ui32UTCTimeMilliseconds;

    tUInt16     ui16Year;
    tUInt16     ui16Month;
    tUInt16     ui16Day;

    tFloat64    f64SpeedOverGroundKnots;
    tFloat64    f64TrackMadeGoodDegreesTrue;
    tFloat64    f64MagneticVariation;

    tBool       bIsStatusValid;
    tChar       cModus;

    tFloat64 GetLatitude() const
    {
        tFloat64 f64Latitude = (tFloat64)this->nLatitudeDegree + (this->f64LatitudeMinutes / 60.0);
        return (this->eNSOrientation == North ? f64Latitude : -f64Latitude);
    }

    tFloat64 GetLongitude() const
    {
        tFloat64 f64Longitude = (tFloat64)this->nLongitudeDegree + (this->f64LongitudeMinutes / 60.0);
        return (this->eEWOrientation == East ? f64Longitude : f64Longitude);
    }

    tBool IsValid()
    {
        return (this->bIsStatusValid && this->cModus != 'N');
    }

} tNMEARMC;

#pragma pack(pop)

/**
 * 
 * NMEA parser helper.
 * 
 */
class cNMEA
{
public:
    /**
     * Protocols
     **/
    typedef enum
    {
        SIRF = 0, // SiRF Binary
        NMEA = 1

    } tProtocol;

    /**
     * NMEA message types
     **/
    typedef enum
    {
        UNKNOWN = -1,
        GGA     = 0,    // Time, position and fix type data.
        GLL     = 1,    // Latitude, longitude, UTC time of position fix and status.
        GSA     = 2,    // GPS receiver operating mode, satellites used in the position solution and DOP values.
        GSV     = 3,    // The number of GPS satellites in view, satellite ID numbers, elevation, azimuth, and SNR values. 
        RMC     = 4,    // Time, date, position, course and speed data. 
        VTG     = 5,    // Course and speed information relative to the ground 
        MSS     = 6,    // Signal-to-noise ratio, signal strength, frequency, and bit rate from a radio-beacon receiver. 
        ZDA     = 7     // Date and time 

    } tMessageType;

    /**
     * Reset Configuration
     **/
    typedef enum 
    {
        HotStart        = 0x01, // All data valid
        WarmStart       = 0x02, // Ephemeris cleared
        HotWarmStart    = 0x03, // Ephemeris cleared, initialization data loaded
        ColdStart       = 0x04, // Clear all data in memory
        ClearMemory     = 0x08  // Clear all data in memory and reset receiver back to factory defaults

    } tResetConfig;

    /**
     * XYZ Navigation Initialization Data Format
     **/
    typedef struct
    {
        tFloat64     f64X;          // Meters.  X coordinate position
        tFloat64     f64Y;          // Meters.  Y coordinate position
        tFloat64     f64Z;          // Meters.  Z coordinate position
        tInt         nClockOffset;  // Hz.      Clock Offset of the Evaluation Unit
        tInt         nTimeOfWeek;   // Seconds. GPS Time Of Week
        tInt         nWeekNo;       // GPS Week Number
        tInt         nChannelCount; // Range 1 to 12
        tResetConfig eResetConfig;  // See enum tResetConfig

        tVoid Init(tResetConfig eResetConfig = HotStart)
        {
            this->f64X          = 0.0;
            this->f64Y          = 0.0;
            this->f64Z          = 0.0;
            this->nClockOffset  = 96000;
            this->nTimeOfWeek   = 237759;
            this->nWeekNo       = 922;
            this->nChannelCount = 12;
            this->eResetConfig  = eResetConfig;
        }

    } tXYZNavigationInit;

    /**
     * LLA Navigation Initialization Data Format
     **/
    typedef struct
    {
        tFloat64     f64Latitude;   // Degree.  Latitude position (Range 90 to -90)
        tFloat64     f64Longitude;  // Degree.  Longitude position (Range 180 to -180)
        tFloat64     f64Altitude;   // Meters.  Altitude position
        tInt         nClockOffset;  // Hz.      Clock Offset of the Evaluation Unit
        tInt         nTimeOfWeek;   // Seconds. GPS Time Of Week
        tInt         nWeekNo;       // GPS Week Number
        tInt         nChannelCount; // Range 1 to 12
        tResetConfig eResetConfig;  // See enum tResetConfig

        tVoid Init(tResetConfig eResetConfig = HotStart)
        {
            this->f64Latitude   = 0.0;
            this->f64Longitude  = 0.0;
            this->f64Altitude   = 0.0;
            this->nClockOffset  = 96000;
            this->nTimeOfWeek   = 237759;
            this->nWeekNo       = 922;
            this->nChannelCount = 12;
            this->eResetConfig  = eResetConfig;
        }

    } tLLANavigationInit;
    
    /**
     *
     * Calculate checksum for NMEA message
     * The checksum is the 8-bit exclusive OR of all the characters after $ and before *. (not including $ and *)
     *
     * @param   strMessage      [in]  NMEA message
     * @param   ui8Checksum     [out] Calculate checksum value
     *
     * @return true on success.
     *
     **/
    static tBool CalculateChecksum(const tChar* strMessage, tUInt8& ui8Checksum);

    /**
     *
     * Check checksum for NMEA message
     * The checksum is the 8-bit exclusive OR of all the characters after $ and before *. (not including $ and *)
     *
     * @param   strMessage      [in]  NMEA message
     *
     * @return true on valid checksum.
     *
     **/
    static tBool HasValidCheckSum(const tChar* strMessage);

    /**
     *
     * Return message type for NMEA message
     *
     * @param   strMessage      [in]  NMEA message
     *
     * @return  Return cNMEA::UNKNOWN if NMEA message can't be identified.
     *
     **/
    static tMessageType GetMessageType(const tChar* strMessage);

    /**
     *
     * Parse NMEA GGA message
     *
     * @param   strMessage          [in] NMEA message
     * @param   pGGA                [out] GGA structure
     *
     * @return  Return true on successfull parsing.
     *
     **/
    static tBool ParseMessageGGA(const tChar* strMessage, tNMEAGGA* pGGA);

    /**
     *
     * Parse NMEA RMC message
     *
     * @param   strMessage          [in]  NMEA message
     * @param   pRMC                [out] RMC structure
     *
     * @return  Return true on successfull parsing.
     *
     */
    static tBool ParseMessageRMC(const tChar* strMessage, tNMEARMC* pRMC);

    /**
     * 
     * Set Serial Port
     *
     * This command message is used to set the protocol (SiRF Binary or NMEA) and/or the communication
     * parameters (baud , data bits, stop bits, parity). Generally, this command is used to switch the 
     * module back to SiRF Binary protocol mode where a more extensive command message set is available.
     * When a valid message is received, the parameters are stored in battery-backed SRAM and then the 
     * Evaluation Unit restarts using the saved parameters.
     *
     * @param         nProtocol [in] SiRF Binary or NMEA
     * @param         nBaudRate [in] 1200, 2400, 4800, 9600, 19200, 38400, 57600 and 115200
     * @param         nDataBits [in] 8,7
     * @param         nStopBits [in] 0,1
     * @param         nParity   [in] 0 = None, 1 = Odd, 2 = Even
     * 
     * @return example: $PSRF100,0,9600,8,1,0*0C<CR><LF>
     *
     **/
    static cString BuildSerialPortMessage(tProtocol eProtocol, tInt nBaudRate = 4800, tInt nDataBits = 8, tInt nStopBits = 1, tInt nParity = 0);

    /**
     * 
     * XYZ Navigation Initialization
     *
     * This command is used to initialize the module for a warm start, which provide current position 
     * (in X, Y, Z coordinates), clock offset, and time. This enables the Evaluation Unit to search 
     * for the correct satellite signals at the correct signal parameters . Correct initialization
     * parameters enable the Evaluation Unit to acquire signals quickly.
     * 
     * @param         sNaviInit [in] See structure tXYZNavigationInit
     * 
     * @return example: $PSRF 101,-2686700,-4304200, 3851624, 95000, 497260, 921, 12, 3*22<CR><LF>
     *
     **/
    static cString BuildXYZNavigationInitMessage(tXYZNavigationInit& sNaviInit);

    /**
     * 
     * Set DGPS Port
     *
     * This command is used to control Serial Port B which is an input only serial port used to receive
     * RTCM differential corrections. Different receivers may output corrections using different 
     * communication parameters. The default communication parameters for PORT B are 9600 baud, 8 data bits, 
     * stop bit, and no parity. If a DGPS received , the parameters are stored in battery backed SRAM and
     * then the receiver restarts using the saved parameters.
     * 
     * @param         nBaudRate [in] 1200, 2400, 4800, 9600, 19200, 38400, 57600 and 115200
     * @param         nDataBits [in] 8,7
     * @param         nStopBits [in] 0,1
     * @param         nParity   [in] 0 = None, 1 = Odd, 2 = Even
     * 
     * @return example: $PSRF102,9600,8,1,0*3C<CR><LF>
     *
     **/
    static cString BuildDGPSMessage(tInt nBaudRate = 4800, tInt nDataBits = 8, tInt nStopBits = 1, tInt nParity = 0);

    /**
     * 
     * Query / Rate Control
     *
     * This command is used to control the output of standard NMEA messages GGA, GLL, GSA, RMC, and VTG.
     * Using this command message, standard NMEA messages may be polled once, or setup for periodic 
     * output. Checksums may also be enabled or disabled depending on the needs of the receiving program.
     * NMEA message settings are saved in battery-backed memory for each entry when the message is accepted.
     * 
     * @param         nType           [in] See structure tMessageType.
     * @param         nMode           [in] 0 = SetRate, 1 = Query.
     * @param         nRate           [in] seconds Output - off = 0, max = 255.
     * @param         bEnableChecksum [in] false = Disable Checksum, true = Enable Checksum
     * 
     * @return example: $PSRF103,05,00,00,01*21<CR><LF>
     *
     **/
    static cString BuildQueryRateControlMessage(tMessageType eType, tInt nMode = 1, tInt nRate = 0, tBool bEnableChecksum = tTrue);

    /**
     * 
     * LLA Navigation Initialization
     *
     * This command is used to initialize the module for a warm start, by providing current position
     * (in latitude, longitude, and altitude coordinates), clock offset, and time. This enables the 
     * receiver to search for the correct satellite signals at the correct signal parameters.
     * Correct initialization parameters enable the receiver to acquire signals quickly.
     * 
     * @param         sNaviInit [in] See structure tLLANavigationInit.
     * 
     * @return example: $PSRF104,0,0,0,96000,237759,922,12,3*3A<CR><LF>
     *
     **/
    static cString BuildLLANavigationInitMessage(tLLANavigationInit& sNaviInit);

    /**
     * 
     * Development Data On/Off
     *
     * Use this command to enable development data information if you can not get the commands accepted. 
     * Invalid commands generate debug information that enables the user to determine the source of the
     * command rejection. Common reasons for input command rejection are invalid checksum of parameter 
     * out of specified range.
     * 
     * @param         bDebugEnable [in] True debug on, False debug off.
     * 
     * @return debug on example: $PSRF105,1*3E<CR><LF>
     *
     **/
    static cString BuildDebugMessage(tBool bDebugEnable);

    /**
     * 
     * Set Low Power Mode
     *
     * This command message is used to set the receiver to one of the low power modes available on GPS 
     * receivers. When a valid message is received, the parameters are stored in battery-backed SRAM 
     * and then the receiver configures itself from the saved parameters.
     * 
     * @param         bPushToFixEnable [in] false = TricklePower or Continuous, true = Push-to-Fix enabled
     * @param         nDutyCycle       [in] 10 times desired duty cycle values are rounded up to 10%, 20%, 30%, 40%, 50% or 100%
     * @param         nTrackingTime    [in] Milliseconds tracking time per TricklePower cycle. Minimum = 200
     * 
     * @return example: $PSRF107,0,200,200*3D<CR><LF>
     *
     **/
    static cString BuildPowerModeMessage(tBool bPushToFixEnable = tFalse, tInt nDutyCycle = 1000, tInt nTrackingTime = 1000);

    /**
     * 
     * WAAS / EGNOS
     *
     * @param         bWAASAndEGNOSEnable [in] true enable WAAS / EGNOS reception, false disable it.
     *
     * @return example: $PSRF108,01*3D<CR><LF>
     *
     **/ 
    static cString BuildWAASEGNOSMessage(tBool bWAASAndEGNOSEnable = tFalse);
};

//*************************************************************************************************
#endif // _NMEA_H_
