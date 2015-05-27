#ifndef __EQ_CAN_SELECTION_HEADER__
#define __EQ_CAN_SELECTION_HEADER__

#pragma pack(push, 8)
typedef struct
{
    tUInt8 VA1level; // signal=VD_alert.VA1level;
    tUInt8 VA1syncFrameIndex; // signal=VD_alert.VA1syncFrameIndex;
    tBool VA1alert; // signal=VD_alert.VA1alert;
    tBool VA1RTA; // signal=VD_alert.VA1RTA;
    tBool VA1NRTA_reasonSpdAbvThresh; // signal=VD_alert.VA1NRTA_reasonSpdAbvThresh;
    tBool VA1NRTA_reasonSpdBlwThresh; // signal=VD_alert.VA1NRTA_reasonSpdBlwThresh;
    tUInt8 VA1reservedVDAlert1; // signal=VD_alert.VA1reservedVDAlert1;
    tUInt16 VA1FCWSuppressionReasons; // signal=VD_alert.VA1FCWSuppressionReasons;
    tUInt32 VA1reservedVDAlert2; // signal=VD_alert.VA1reservedVDAlert2;
} tVD_alert;


typedef struct
{
    tUInt8 VA1level__2; // signal=VD_alert_02.VA1level__2;
    tUInt8 VA1syncFrameIndex__2; // signal=VD_alert_02.VA1syncFrameIndex__2;
    tBool VA1alert__2; // signal=VD_alert_02.VA1alert__2;
    tBool VA1RTA__2; // signal=VD_alert_02.VA1RTA__2;
    tBool VA1NRTA_reasonSpdAbvThresh__2; // signal=VD_alert_02.VA1NRTA_reasonSpdAbvThresh__2;
    tBool VA1NRTA_reasonSpdBlwThresh__2; // signal=VD_alert_02.VA1NRTA_reasonSpdBlwThresh__2;
    tUInt8 VA1reservedVDAlert1__2; // signal=VD_alert_02.VA1reservedVDAlert1__2;
    tUInt16 VA1FCWSuppressionReasons__2; // signal=VD_alert_02.VA1FCWSuppressionReasons__2;
    tUInt32 VA1reservedVDAlert2__2; // signal=VD_alert_02.VA1reservedVDAlert2__2;
} tVD_alert_02;


typedef struct
{
    tUInt8 VA1level__3; // signal=VD_alert_03.VA1level__3;
    tUInt8 VA1syncFrameIndex__3; // signal=VD_alert_03.VA1syncFrameIndex__3;
    tBool VA1alert__3; // signal=VD_alert_03.VA1alert__3;
    tBool VA1RTA__3; // signal=VD_alert_03.VA1RTA__3;
    tBool VA1NRTA_reasonSpdAbvThresh__3; // signal=VD_alert_03.VA1NRTA_reasonSpdAbvThresh__3;
    tBool VA1NRTA_reasonSpdBlwThresh__3; // signal=VD_alert_03.VA1NRTA_reasonSpdBlwThresh__3;
    tUInt8 VA1reservedVDAlert1__3; // signal=VD_alert_03.VA1reservedVDAlert1__3;
    tUInt16 VA1FCWSuppressionReasons__3; // signal=VD_alert_03.VA1FCWSuppressionReasons__3;
    tUInt32 VA1reservedVDAlert2__3; // signal=VD_alert_03.VA1reservedVDAlert2__3;
} tVD_alert_03;


typedef struct
{
    tUInt8 VA1level__4; // signal=VD_alert_04.VA1level__4;
    tUInt8 VA1syncFrameIndex__4; // signal=VD_alert_04.VA1syncFrameIndex__4;
    tBool VA1alert__4; // signal=VD_alert_04.VA1alert__4;
    tBool VA1RTA__4; // signal=VD_alert_04.VA1RTA__4;
    tBool VA1NRTA_reasonSpdAbvThresh__4; // signal=VD_alert_04.VA1NRTA_reasonSpdAbvThresh__4;
    tBool VA1NRTA_reasonSpdBlwThresh__4; // signal=VD_alert_04.VA1NRTA_reasonSpdBlwThresh__4;
    tUInt8 VA1reservedVDAlert1__4; // signal=VD_alert_04.VA1reservedVDAlert1__4;
    tUInt16 VA1FCWSuppressionReasons__4; // signal=VD_alert_04.VA1FCWSuppressionReasons__4;
    tUInt32 VA1reservedVDAlert2__4; // signal=VD_alert_04.VA1reservedVDAlert2__4;
} tVD_alert_04;


typedef struct
{
    tUInt8 VA1level__5; // signal=VD_alert_05.VA1level__5;
    tUInt8 VA1syncFrameIndex__5; // signal=VD_alert_05.VA1syncFrameIndex__5;
    tBool VA1alert__5; // signal=VD_alert_05.VA1alert__5;
    tBool VA1RTA__5; // signal=VD_alert_05.VA1RTA__5;
    tBool VA1NRTA_reasonSpdAbvThresh__5; // signal=VD_alert_05.VA1NRTA_reasonSpdAbvThresh__5;
    tBool VA1NRTA_reasonSpdBlwThresh__5; // signal=VD_alert_05.VA1NRTA_reasonSpdBlwThresh__5;
    tUInt8 VA1reservedVDAlert1__5; // signal=VD_alert_05.VA1reservedVDAlert1__5;
    tUInt16 VA1FCWSuppressionReasons__5; // signal=VD_alert_05.VA1FCWSuppressionReasons__5;
    tUInt32 VA1reservedVDAlert2__5; // signal=VD_alert_05.VA1reservedVDAlert2__5;
} tVD_alert_05;


typedef struct
{
    tUInt8 VH1SyncFrameIndex; // signal=VD_header.VH1SyncFrameIndex;
    tBool VH1CIPV_exist; // signal=VD_header.VH1CIPV_exist;
    tBool VH1VehicleAheadAlertInd; // signal=VD_header.VH1VehicleAheadAlertInd;
    tBool VH1LeftCloseRangeCut_in; // signal=VD_header.VH1LeftCloseRangeCut_in;
    tBool VH1RightCloseRangeCut_in; // signal=VD_header.VH1RightCloseRangeCut_in;
    tUInt8 VH1goNotification; // signal=VD_header.VH1goNotification;
    tBool VH1friendOrFoe; // signal=VD_header.VH1friendOrFoe;
    tBool VH1threatAssesment; // signal=VD_header.VH1threatAssesment;
    tBool VH1Reserved_VD_header_1; // signal=VD_header.VH1Reserved_VD_header_1;
    tBool VH1CIPV_lostIn; // signal=VD_header.VH1CIPV_lostIn;
    tBool VH1CIPV_lostOut; // signal=VD_header.VH1CIPV_lostOut;
    tUInt8 VH1numberOfObjects; // signal=VD_header.VH1numberOfObjects;
    tUInt64 VH1Reserved_VD_header_2; // signal=VD_header.VH1Reserved_VD_header_2;
} tVD_header;


typedef struct
{
    tUInt8 VO1syncFrameIndex; // signal=VDPD_obj_A.VO1syncFrameIndex;
    tUInt8 VO1ID; // signal=VDPD_obj_A.VO1ID;
    tBool VO1CIPV_flag; // signal=VDPD_obj_A.VO1CIPV_flag;
    tBool VO1pedsExist; // signal=VDPD_obj_A.VO1pedsExist;
    tUInt8 VO1type; // signal=VDPD_obj_A.VO1type;
    tBool VO1Reserved_VDPD_obj_A_2; // signal=VDPD_obj_A.VO1Reserved_VDPD_obj_A_2;
    tUInt8 VO1Lane_Movement_Type; // signal=VDPD_obj_A.VO1Lane_Movement_Type;
    tBool VO1BrakeLights; // signal=VDPD_obj_A.VO1BrakeLights;
    tBool VO1direction; // signal=VDPD_obj_A.VO1direction;
    tUInt8 VO1laneAssignment; // signal=VDPD_obj_A.VO1laneAssignment;
    tUInt8 VO1blinkerInfo; // signal=VDPD_obj_A.VO1blinkerInfo;
    tFloat64 VO1overlapAngleEgoLaneFromRight; // signal=VDPD_obj_A.VO1overlapAngleEgoLaneFromRight;
    tUInt8 VO1Reserved_VDPD_obj_A_3; // signal=VDPD_obj_A.VO1Reserved_VDPD_obj_A_3;
    tFloat64 VO1overlapAngleEgoLaneFromLeft; // signal=VDPD_obj_A.VO1overlapAngleEgoLaneFromLeft;
    tUInt8 VO1Reserved_VDPD_obj_A_4; // signal=VDPD_obj_A.VO1Reserved_VDPD_obj_A_4;
} tVDPD_obj_A;


typedef struct
{
    tUInt8 VO1syncFrameIndex__2; // signal=VDPD_obj_A_02.VO1syncFrameIndex__2;
    tUInt8 VO1ID__2; // signal=VDPD_obj_A_02.VO1ID__2;
    tBool VO1CIPV_flag__2; // signal=VDPD_obj_A_02.VO1CIPV_flag__2;
    tBool VO1pedsExist__2; // signal=VDPD_obj_A_02.VO1pedsExist__2;
    tUInt8 VO1type__2; // signal=VDPD_obj_A_02.VO1type__2;
    tBool VO1Reserved_VDPD_obj_A_2__2; // signal=VDPD_obj_A_02.VO1Reserved_VDPD_obj_A_2__2;
    tUInt8 VO1Lane_Movement_Type__2; // signal=VDPD_obj_A_02.VO1Lane_Movement_Type__2;
    tBool VO1BrakeLights__2; // signal=VDPD_obj_A_02.VO1BrakeLights__2;
    tBool VO1direction__2; // signal=VDPD_obj_A_02.VO1direction__2;
    tUInt8 VO1laneAssignment__2; // signal=VDPD_obj_A_02.VO1laneAssignment__2;
    tUInt8 VO1blinkerInfo__2; // signal=VDPD_obj_A_02.VO1blinkerInfo__2;
    tFloat64 VO1overlapAngleEgoLaneFromRight__2; // signal=VDPD_obj_A_02.VO1overlapAngleEgoLaneFromRight__2;
    tUInt8 VO1Reserved_VDPD_obj_A_3__2; // signal=VDPD_obj_A_02.VO1Reserved_VDPD_obj_A_3__2;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__2; // signal=VDPD_obj_A_02.VO1overlapAngleEgoLaneFromLeft__2;
    tUInt8 VO1Reserved_VDPD_obj_A_4__2; // signal=VDPD_obj_A_02.VO1Reserved_VDPD_obj_A_4__2;
} tVDPD_obj_A_02;


typedef struct
{
    tUInt8 VO1syncFrameIndex__3; // signal=VDPD_obj_A_03.VO1syncFrameIndex__3;
    tUInt8 VO1ID__3; // signal=VDPD_obj_A_03.VO1ID__3;
    tBool VO1CIPV_flag__3; // signal=VDPD_obj_A_03.VO1CIPV_flag__3;
    tBool VO1pedsExist__3; // signal=VDPD_obj_A_03.VO1pedsExist__3;
    tUInt8 VO1type__3; // signal=VDPD_obj_A_03.VO1type__3;
    tBool VO1Reserved_VDPD_obj_A_2__3; // signal=VDPD_obj_A_03.VO1Reserved_VDPD_obj_A_2__3;
    tUInt8 VO1Lane_Movement_Type__3; // signal=VDPD_obj_A_03.VO1Lane_Movement_Type__3;
    tBool VO1BrakeLights__3; // signal=VDPD_obj_A_03.VO1BrakeLights__3;
    tBool VO1direction__3; // signal=VDPD_obj_A_03.VO1direction__3;
    tUInt8 VO1laneAssignment__3; // signal=VDPD_obj_A_03.VO1laneAssignment__3;
    tUInt8 VO1blinkerInfo__3; // signal=VDPD_obj_A_03.VO1blinkerInfo__3;
    tFloat64 VO1overlapAngleEgoLaneFromRight__3; // signal=VDPD_obj_A_03.VO1overlapAngleEgoLaneFromRight__3;
    tUInt8 VO1Reserved_VDPD_obj_A_3__3; // signal=VDPD_obj_A_03.VO1Reserved_VDPD_obj_A_3__3;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__3; // signal=VDPD_obj_A_03.VO1overlapAngleEgoLaneFromLeft__3;
    tUInt8 VO1Reserved_VDPD_obj_A_4__3; // signal=VDPD_obj_A_03.VO1Reserved_VDPD_obj_A_4__3;
} tVDPD_obj_A_03;


typedef struct
{
    tUInt8 VO1syncFrameIndex__4; // signal=VDPD_obj_A_04.VO1syncFrameIndex__4;
    tUInt8 VO1ID__4; // signal=VDPD_obj_A_04.VO1ID__4;
    tBool VO1CIPV_flag__4; // signal=VDPD_obj_A_04.VO1CIPV_flag__4;
    tBool VO1pedsExist__4; // signal=VDPD_obj_A_04.VO1pedsExist__4;
    tUInt8 VO1type__4; // signal=VDPD_obj_A_04.VO1type__4;
    tBool VO1Reserved_VDPD_obj_A_2__4; // signal=VDPD_obj_A_04.VO1Reserved_VDPD_obj_A_2__4;
    tUInt8 VO1Lane_Movement_Type__4; // signal=VDPD_obj_A_04.VO1Lane_Movement_Type__4;
    tBool VO1BrakeLights__4; // signal=VDPD_obj_A_04.VO1BrakeLights__4;
    tBool VO1direction__4; // signal=VDPD_obj_A_04.VO1direction__4;
    tUInt8 VO1laneAssignment__4; // signal=VDPD_obj_A_04.VO1laneAssignment__4;
    tUInt8 VO1blinkerInfo__4; // signal=VDPD_obj_A_04.VO1blinkerInfo__4;
    tFloat64 VO1overlapAngleEgoLaneFromRight__4; // signal=VDPD_obj_A_04.VO1overlapAngleEgoLaneFromRight__4;
    tUInt8 VO1Reserved_VDPD_obj_A_3__4; // signal=VDPD_obj_A_04.VO1Reserved_VDPD_obj_A_3__4;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__4; // signal=VDPD_obj_A_04.VO1overlapAngleEgoLaneFromLeft__4;
    tUInt8 VO1Reserved_VDPD_obj_A_4__4; // signal=VDPD_obj_A_04.VO1Reserved_VDPD_obj_A_4__4;
} tVDPD_obj_A_04;


typedef struct
{
    tUInt8 VO1syncFrameIndex__5; // signal=VDPD_obj_A_05.VO1syncFrameIndex__5;
    tUInt8 VO1ID__5; // signal=VDPD_obj_A_05.VO1ID__5;
    tBool VO1CIPV_flag__5; // signal=VDPD_obj_A_05.VO1CIPV_flag__5;
    tBool VO1pedsExist__5; // signal=VDPD_obj_A_05.VO1pedsExist__5;
    tUInt8 VO1type__5; // signal=VDPD_obj_A_05.VO1type__5;
    tBool VO1Reserved_VDPD_obj_A_2__5; // signal=VDPD_obj_A_05.VO1Reserved_VDPD_obj_A_2__5;
    tUInt8 VO1Lane_Movement_Type__5; // signal=VDPD_obj_A_05.VO1Lane_Movement_Type__5;
    tBool VO1BrakeLights__5; // signal=VDPD_obj_A_05.VO1BrakeLights__5;
    tBool VO1direction__5; // signal=VDPD_obj_A_05.VO1direction__5;
    tUInt8 VO1laneAssignment__5; // signal=VDPD_obj_A_05.VO1laneAssignment__5;
    tUInt8 VO1blinkerInfo__5; // signal=VDPD_obj_A_05.VO1blinkerInfo__5;
    tFloat64 VO1overlapAngleEgoLaneFromRight__5; // signal=VDPD_obj_A_05.VO1overlapAngleEgoLaneFromRight__5;
    tUInt8 VO1Reserved_VDPD_obj_A_3__5; // signal=VDPD_obj_A_05.VO1Reserved_VDPD_obj_A_3__5;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__5; // signal=VDPD_obj_A_05.VO1overlapAngleEgoLaneFromLeft__5;
    tUInt8 VO1Reserved_VDPD_obj_A_4__5; // signal=VDPD_obj_A_05.VO1Reserved_VDPD_obj_A_4__5;
} tVDPD_obj_A_05;


typedef struct
{
    tUInt8 VO1syncFrameIndex__6; // signal=VDPD_obj_A_06.VO1syncFrameIndex__6;
    tUInt8 VO1ID__6; // signal=VDPD_obj_A_06.VO1ID__6;
    tBool VO1CIPV_flag__6; // signal=VDPD_obj_A_06.VO1CIPV_flag__6;
    tBool VO1pedsExist__6; // signal=VDPD_obj_A_06.VO1pedsExist__6;
    tUInt8 VO1type__6; // signal=VDPD_obj_A_06.VO1type__6;
    tBool VO1Reserved_VDPD_obj_A_2__6; // signal=VDPD_obj_A_06.VO1Reserved_VDPD_obj_A_2__6;
    tUInt8 VO1Lane_Movement_Type__6; // signal=VDPD_obj_A_06.VO1Lane_Movement_Type__6;
    tBool VO1BrakeLights__6; // signal=VDPD_obj_A_06.VO1BrakeLights__6;
    tBool VO1direction__6; // signal=VDPD_obj_A_06.VO1direction__6;
    tUInt8 VO1laneAssignment__6; // signal=VDPD_obj_A_06.VO1laneAssignment__6;
    tUInt8 VO1blinkerInfo__6; // signal=VDPD_obj_A_06.VO1blinkerInfo__6;
    tFloat64 VO1overlapAngleEgoLaneFromRight__6; // signal=VDPD_obj_A_06.VO1overlapAngleEgoLaneFromRight__6;
    tUInt8 VO1Reserved_VDPD_obj_A_3__6; // signal=VDPD_obj_A_06.VO1Reserved_VDPD_obj_A_3__6;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__6; // signal=VDPD_obj_A_06.VO1overlapAngleEgoLaneFromLeft__6;
    tUInt8 VO1Reserved_VDPD_obj_A_4__6; // signal=VDPD_obj_A_06.VO1Reserved_VDPD_obj_A_4__6;
} tVDPD_obj_A_06;


typedef struct
{
    tUInt8 VO1syncFrameIndex__7; // signal=VDPD_obj_A_07.VO1syncFrameIndex__7;
    tUInt8 VO1ID__7; // signal=VDPD_obj_A_07.VO1ID__7;
    tBool VO1CIPV_flag__7; // signal=VDPD_obj_A_07.VO1CIPV_flag__7;
    tBool VO1pedsExist__7; // signal=VDPD_obj_A_07.VO1pedsExist__7;
    tUInt8 VO1type__7; // signal=VDPD_obj_A_07.VO1type__7;
    tBool VO1Reserved_VDPD_obj_A_2__7; // signal=VDPD_obj_A_07.VO1Reserved_VDPD_obj_A_2__7;
    tUInt8 VO1Lane_Movement_Type__7; // signal=VDPD_obj_A_07.VO1Lane_Movement_Type__7;
    tBool VO1BrakeLights__7; // signal=VDPD_obj_A_07.VO1BrakeLights__7;
    tBool VO1direction__7; // signal=VDPD_obj_A_07.VO1direction__7;
    tUInt8 VO1laneAssignment__7; // signal=VDPD_obj_A_07.VO1laneAssignment__7;
    tUInt8 VO1blinkerInfo__7; // signal=VDPD_obj_A_07.VO1blinkerInfo__7;
    tFloat64 VO1overlapAngleEgoLaneFromRight__7; // signal=VDPD_obj_A_07.VO1overlapAngleEgoLaneFromRight__7;
    tUInt8 VO1Reserved_VDPD_obj_A_3__7; // signal=VDPD_obj_A_07.VO1Reserved_VDPD_obj_A_3__7;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__7; // signal=VDPD_obj_A_07.VO1overlapAngleEgoLaneFromLeft__7;
    tUInt8 VO1Reserved_VDPD_obj_A_4__7; // signal=VDPD_obj_A_07.VO1Reserved_VDPD_obj_A_4__7;
} tVDPD_obj_A_07;


typedef struct
{
    tUInt8 VO1syncFrameIndex__8; // signal=VDPD_obj_A_08.VO1syncFrameIndex__8;
    tUInt8 VO1ID__8; // signal=VDPD_obj_A_08.VO1ID__8;
    tBool VO1CIPV_flag__8; // signal=VDPD_obj_A_08.VO1CIPV_flag__8;
    tBool VO1pedsExist__8; // signal=VDPD_obj_A_08.VO1pedsExist__8;
    tUInt8 VO1type__8; // signal=VDPD_obj_A_08.VO1type__8;
    tBool VO1Reserved_VDPD_obj_A_2__8; // signal=VDPD_obj_A_08.VO1Reserved_VDPD_obj_A_2__8;
    tUInt8 VO1Lane_Movement_Type__8; // signal=VDPD_obj_A_08.VO1Lane_Movement_Type__8;
    tBool VO1BrakeLights__8; // signal=VDPD_obj_A_08.VO1BrakeLights__8;
    tBool VO1direction__8; // signal=VDPD_obj_A_08.VO1direction__8;
    tUInt8 VO1laneAssignment__8; // signal=VDPD_obj_A_08.VO1laneAssignment__8;
    tUInt8 VO1blinkerInfo__8; // signal=VDPD_obj_A_08.VO1blinkerInfo__8;
    tFloat64 VO1overlapAngleEgoLaneFromRight__8; // signal=VDPD_obj_A_08.VO1overlapAngleEgoLaneFromRight__8;
    tUInt8 VO1Reserved_VDPD_obj_A_3__8; // signal=VDPD_obj_A_08.VO1Reserved_VDPD_obj_A_3__8;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__8; // signal=VDPD_obj_A_08.VO1overlapAngleEgoLaneFromLeft__8;
    tUInt8 VO1Reserved_VDPD_obj_A_4__8; // signal=VDPD_obj_A_08.VO1Reserved_VDPD_obj_A_4__8;
} tVDPD_obj_A_08;


typedef struct
{
    tUInt8 VO1syncFrameIndex__9; // signal=VDPD_obj_A_09.VO1syncFrameIndex__9;
    tUInt8 VO1ID__9; // signal=VDPD_obj_A_09.VO1ID__9;
    tBool VO1CIPV_flag__9; // signal=VDPD_obj_A_09.VO1CIPV_flag__9;
    tBool VO1pedsExist__9; // signal=VDPD_obj_A_09.VO1pedsExist__9;
    tUInt8 VO1type__9; // signal=VDPD_obj_A_09.VO1type__9;
    tBool VO1Reserved_VDPD_obj_A_2__9; // signal=VDPD_obj_A_09.VO1Reserved_VDPD_obj_A_2__9;
    tUInt8 VO1Lane_Movement_Type__9; // signal=VDPD_obj_A_09.VO1Lane_Movement_Type__9;
    tBool VO1BrakeLights__9; // signal=VDPD_obj_A_09.VO1BrakeLights__9;
    tBool VO1direction__9; // signal=VDPD_obj_A_09.VO1direction__9;
    tUInt8 VO1laneAssignment__9; // signal=VDPD_obj_A_09.VO1laneAssignment__9;
    tUInt8 VO1blinkerInfo__9; // signal=VDPD_obj_A_09.VO1blinkerInfo__9;
    tFloat64 VO1overlapAngleEgoLaneFromRight__9; // signal=VDPD_obj_A_09.VO1overlapAngleEgoLaneFromRight__9;
    tUInt8 VO1Reserved_VDPD_obj_A_3__9; // signal=VDPD_obj_A_09.VO1Reserved_VDPD_obj_A_3__9;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__9; // signal=VDPD_obj_A_09.VO1overlapAngleEgoLaneFromLeft__9;
    tUInt8 VO1Reserved_VDPD_obj_A_4__9; // signal=VDPD_obj_A_09.VO1Reserved_VDPD_obj_A_4__9;
} tVDPD_obj_A_09;


typedef struct
{
    tUInt8 VO1syncFrameIndex__10; // signal=VDPD_obj_A_10.VO1syncFrameIndex__10;
    tUInt8 VO1ID__10; // signal=VDPD_obj_A_10.VO1ID__10;
    tBool VO1CIPV_flag__10; // signal=VDPD_obj_A_10.VO1CIPV_flag__10;
    tBool VO1pedsExist__10; // signal=VDPD_obj_A_10.VO1pedsExist__10;
    tUInt8 VO1type__10; // signal=VDPD_obj_A_10.VO1type__10;
    tBool VO1Reserved_VDPD_obj_A_2__10; // signal=VDPD_obj_A_10.VO1Reserved_VDPD_obj_A_2__10;
    tUInt8 VO1Lane_Movement_Type__10; // signal=VDPD_obj_A_10.VO1Lane_Movement_Type__10;
    tBool VO1BrakeLights__10; // signal=VDPD_obj_A_10.VO1BrakeLights__10;
    tBool VO1direction__10; // signal=VDPD_obj_A_10.VO1direction__10;
    tUInt8 VO1laneAssignment__10; // signal=VDPD_obj_A_10.VO1laneAssignment__10;
    tUInt8 VO1blinkerInfo__10; // signal=VDPD_obj_A_10.VO1blinkerInfo__10;
    tFloat64 VO1overlapAngleEgoLaneFromRight__10; // signal=VDPD_obj_A_10.VO1overlapAngleEgoLaneFromRight__10;
    tUInt8 VO1Reserved_VDPD_obj_A_3__10; // signal=VDPD_obj_A_10.VO1Reserved_VDPD_obj_A_3__10;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__10; // signal=VDPD_obj_A_10.VO1overlapAngleEgoLaneFromLeft__10;
    tUInt8 VO1Reserved_VDPD_obj_A_4__10; // signal=VDPD_obj_A_10.VO1Reserved_VDPD_obj_A_4__10;
} tVDPD_obj_A_10;


typedef struct
{
    tUInt8 VO1syncFrameIndex__11; // signal=VDPD_obj_A_11.VO1syncFrameIndex__11;
    tUInt8 VO1ID__11; // signal=VDPD_obj_A_11.VO1ID__11;
    tBool VO1CIPV_flag__11; // signal=VDPD_obj_A_11.VO1CIPV_flag__11;
    tBool VO1pedsExist__11; // signal=VDPD_obj_A_11.VO1pedsExist__11;
    tUInt8 VO1type__11; // signal=VDPD_obj_A_11.VO1type__11;
    tBool VO1Reserved_VDPD_obj_A_2__11; // signal=VDPD_obj_A_11.VO1Reserved_VDPD_obj_A_2__11;
    tUInt8 VO1Lane_Movement_Type__11; // signal=VDPD_obj_A_11.VO1Lane_Movement_Type__11;
    tBool VO1BrakeLights__11; // signal=VDPD_obj_A_11.VO1BrakeLights__11;
    tBool VO1direction__11; // signal=VDPD_obj_A_11.VO1direction__11;
    tUInt8 VO1laneAssignment__11; // signal=VDPD_obj_A_11.VO1laneAssignment__11;
    tUInt8 VO1blinkerInfo__11; // signal=VDPD_obj_A_11.VO1blinkerInfo__11;
    tFloat64 VO1overlapAngleEgoLaneFromRight__11; // signal=VDPD_obj_A_11.VO1overlapAngleEgoLaneFromRight__11;
    tUInt8 VO1Reserved_VDPD_obj_A_3__11; // signal=VDPD_obj_A_11.VO1Reserved_VDPD_obj_A_3__11;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__11; // signal=VDPD_obj_A_11.VO1overlapAngleEgoLaneFromLeft__11;
    tUInt8 VO1Reserved_VDPD_obj_A_4__11; // signal=VDPD_obj_A_11.VO1Reserved_VDPD_obj_A_4__11;
} tVDPD_obj_A_11;


typedef struct
{
    tUInt8 VO1syncFrameIndex__12; // signal=VDPD_obj_A_12.VO1syncFrameIndex__12;
    tUInt8 VO1ID__12; // signal=VDPD_obj_A_12.VO1ID__12;
    tBool VO1CIPV_flag__12; // signal=VDPD_obj_A_12.VO1CIPV_flag__12;
    tBool VO1pedsExist__12; // signal=VDPD_obj_A_12.VO1pedsExist__12;
    tUInt8 VO1type__12; // signal=VDPD_obj_A_12.VO1type__12;
    tBool VO1Reserved_VDPD_obj_A_2__12; // signal=VDPD_obj_A_12.VO1Reserved_VDPD_obj_A_2__12;
    tUInt8 VO1Lane_Movement_Type__12; // signal=VDPD_obj_A_12.VO1Lane_Movement_Type__12;
    tBool VO1BrakeLights__12; // signal=VDPD_obj_A_12.VO1BrakeLights__12;
    tBool VO1direction__12; // signal=VDPD_obj_A_12.VO1direction__12;
    tUInt8 VO1laneAssignment__12; // signal=VDPD_obj_A_12.VO1laneAssignment__12;
    tUInt8 VO1blinkerInfo__12; // signal=VDPD_obj_A_12.VO1blinkerInfo__12;
    tFloat64 VO1overlapAngleEgoLaneFromRight__12; // signal=VDPD_obj_A_12.VO1overlapAngleEgoLaneFromRight__12;
    tUInt8 VO1Reserved_VDPD_obj_A_3__12; // signal=VDPD_obj_A_12.VO1Reserved_VDPD_obj_A_3__12;
    tFloat64 VO1overlapAngleEgoLaneFromLeft__12; // signal=VDPD_obj_A_12.VO1overlapAngleEgoLaneFromLeft__12;
    tUInt8 VO1Reserved_VDPD_obj_A_4__12; // signal=VDPD_obj_A_12.VO1Reserved_VDPD_obj_A_4__12;
} tVDPD_obj_A_12;


typedef struct
{
    tUInt8 PA1SyncFrameIndex; // signal=PD_alert.PA1SyncFrameIndex;
    tBool PA1Reserved_PD_alert_1; // signal=PD_alert.PA1Reserved_PD_alert_1;
    tBool PA1L1; // signal=PD_alert.PA1L1;
    tBool PA1L2; // signal=PD_alert.PA1L2;
    tBool PA1L3; // signal=PD_alert.PA1L3;
    tUInt8 PA1AEB_suppressReasons_1; // signal=PD_alert.PA1AEB_suppressReasons_1;
    tUInt8 PA1AEB_suppressReasons_2; // signal=PD_alert.PA1AEB_suppressReasons_2;
    tUInt8 PA1AEB_suppressReasons_3; // signal=PD_alert.PA1AEB_suppressReasons_3;
    tUInt64 PA1Reserved_PD_alert_2; // signal=PD_alert.PA1Reserved_PD_alert_2;
} tPD_alert;

#pragma pack(pop)

#endif // __EQ_CAN_SELECTION_HEADER__
