#ifndef __BOS_HEADER__
#define __BOS_HEADER__

#pragma pack(push, 8)
typedef struct
{
    tUInt8 day; // signal=Meas_Status.day;
    tUInt8 month; // signal=Meas_Status.month;
    tUInt8 status; // signal=Meas_Status.status;
    tUInt16 year; // signal=Meas_Status.year;
    tUInt8 second; // signal=Meas_Status.second;
    tUInt8 minute; // signal=Meas_Status.minute;
    tUInt8 hour; // signal=Meas_Status.hour;
    tUInt8 file_counter; // signal=Meas_Status.file_counter;
} tMeas_Status;


typedef struct
{
    tUInt8 trigger_counter; // signal=Meas_Trigger.trigger_counter;
    tInt8 trigger_measurement; // signal=Meas_Trigger.trigger_measurement;
    tInt8 trigger_comment; // signal=Meas_Trigger.trigger_comment;
} tMeas_Trigger;


typedef struct
{
    tUInt8 Command_07; // signal=Meas_Command.Command_07;
    tUInt8 Command_06; // signal=Meas_Command.Command_06;
    tUInt8 Command_05; // signal=Meas_Command.Command_05;
    tUInt8 Command_04; // signal=Meas_Command.Command_04;
    tUInt8 Command_03; // signal=Meas_Command.Command_03;
    tUInt8 Command_02; // signal=Meas_Command.Command_02;
    tUInt8 Command_01; // signal=Meas_Command.Command_01;
    tUInt8 Command_00; // signal=Meas_Command.Command_00;
} tMeas_Command;

#pragma pack(pop)

#endif // __BOS_HEADER__
