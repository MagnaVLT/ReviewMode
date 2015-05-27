#ifndef __CHRYSLER_CAN_SELECTION_HEADER__
#define __CHRYSLER_CAN_SELECTION_HEADER__

#pragma pack(push, 8)
typedef struct
{
    tUInt8 ISO_TelltaleSts; // signal=HALF_HMI_STATUS.ISO_TelltaleSts;
    tUInt8 TrqIntensitySts; // signal=HALF_HMI_STATUS.TrqIntensitySts;
    tUInt8 SensitivitySts; // signal=HALF_HMI_STATUS.SensitivitySts;
    tUInt8 DisplaySts; // signal=HALF_HMI_STATUS.DisplaySts;
    tUInt8 DispPopupSts; // signal=HALF_HMI_STATUS.DispPopupSts;
    tUInt8 HALF_CHIME_TYPESts; // signal=HALF_HMI_STATUS.HALF_CHIME_TYPESts;
    tUInt8 HALF_CHIME_REP_RATESts; // signal=HALF_HMI_STATUS.HALF_CHIME_REP_RATESts;
    tBool HALF_LF_CHIME_RQSts; // signal=HALF_HMI_STATUS.HALF_LF_CHIME_RQSts;
    tBool HALF_RF_CHIME_RQSts; // signal=HALF_HMI_STATUS.HALF_RF_CHIME_RQSts;
} tHALF_HMI_STATUS;

typedef struct
{
    tUInt8 VIN_MSG; // signal=VIN.VIN_MSG;
    tUInt64 VIN_DATA; // signal=VIN.VIN_DATA;
} tVIN;

typedef struct
{
    tFloat64 StrgVibrAmplReq; // signal=HALF_SWVM_CTRL.StrgVibrAmplReq;
    tUInt8 StrgVibrFreqReq; // signal=HALF_SWVM_CTRL.StrgVibrFreqReq;
    tFloat64 PulseTimeOff; // signal=HALF_SWVM_CTRL.PulseTimeOff;
    tFloat64 PulseTimeOn; // signal=HALF_SWVM_CTRL.PulseTimeOn;
    tBool StrgVibrActv; // signal=HALF_SWVM_CTRL.StrgVibrActv;
    tBool HALFSysSts; // signal=HALF_SWVM_CTRL.HALFSysSts;
    tUInt8 HaLFCntr; // signal=HALF_SWVM_CTRL.HaLFCntr;
    tUInt8 LDW_LEDControlSts; // signal=HALF_SWVM_CTRL.LDW_LEDControlSts;
    tUInt8 TorqueIntensitySts; // signal=HALF_SWVM_CTRL.TorqueIntensitySts;
    tBool HALF_AHB_ACT; // signal=HALF_SWVM_CTRL.HALF_AHB_ACT;
    tBool HALF_AHB_On; // signal=HALF_SWVM_CTRL.HALF_AHB_On;
    tBool SysAvlStat; // signal=HALF_SWVM_CTRL.SysAvlStat;
    tBool SysActv; // signal=HALF_SWVM_CTRL.SysActv;
    tBool SysFail; // signal=HALF_SWVM_CTRL.SysFail;
    tBool AutoSprsActv; // signal=HALF_SWVM_CTRL.AutoSprsActv;
    tUInt8 ConfigSts; // signal=HALF_SWVM_CTRL.ConfigSts;
} tHALF_SWVM_CTRL;

typedef struct
{
    tUInt8 VH_reactionary; // signal=Video_Header.VH_reactionary;
    tUInt8 VH_warn_brake_req; // signal=Video_Header.VH_warn_brake_req;
    tUInt8 VH_CRC; // signal=Video_Header.VH_CRC;
    tUInt8 VH_free_lane_20m; // signal=Video_Header.VH_free_lane_20m;
    tUInt8 VH_free_lane_60m; // signal=Video_Header.VH_free_lane_60m;
    tUInt8 VH_MC; // signal=Video_Header.VH_MC;
    tFloat64 VH_meas_dtime; // signal=Video_Header.VH_meas_dtime;
    tBool VH_near_cutin; // signal=Video_Header.VH_near_cutin;
    tUInt16 VH_update_ctr; // signal=Video_Header.VH_update_ctr;
    tBool VH_vFCW; // signal=Video_Header.VH_vFCW;
    tBool VS_blindness_detected; // signal=Video_Header.VS_blindness_detected;
    tBool VS_degraded_type1; // signal=Video_Header.VS_degraded_type1;
    tBool VS_degraded_type2; // signal=Video_Header.VS_degraded_type2;
    tBool VS_degraded_type3; // signal=Video_Header.VS_degraded_type3;
    tBool VS_degraded_type4; // signal=Video_Header.VS_degraded_type4;
    tBool VS_degraded_type5; // signal=Video_Header.VS_degraded_type5;
    tBool VS_degraded_type6; // signal=Video_Header.VS_degraded_type6;
    tBool VS_degraded_type7; // signal=Video_Header.VS_degraded_type7;
    tBool VS_failure; // signal=Video_Header.VS_failure;
    tBool VS_freeview_not_checked; // signal=Video_Header.VS_freeview_not_checked;
    tBool VS_not_available; // signal=Video_Header.VS_not_available;
} tVideo_Header;

/*	Request DID for recording init messages */
typedef struct
{
    tUInt64 D_RQ_HaLF; // signal=D_RQ_HaLF.D_RQ_HaLF;
} tD_RQ_HaLF;

/*	Response DID for recording init messages */
typedef struct
{
    tUInt64 D_RS_HaLF; // signal=D_RS_HaLF.D_RS_HaLF;
} tD_RS_HaLF;

/* Vehicle CAN message */
typedef struct
{
    tFloat64 BrkTrq; // signal=ESP_A8.BrkTrq;
    tBool VehSpSts; // signal=ESP_A8.VehSpSts;
    tFloat64 BrkTrq_D; // signal=ESP_A8.BrkTrq_D;
    tFloat64 VEH_SPEED; // signal=ESP_A8.VEH_SPEED;
    tUInt8 MC_ESP_A8; // signal=ESP_A8.MC_ESP_A8;
    tUInt8 CRC_ESP_A8; // signal=ESP_A8.CRC_ESP_A8;
} tESP_A8;

const static tFloat64	CAN_INTERVAL = 0.02f;

/* EyeQ Private CAN message */
typedef struct
{
    tBool CS1ReverseIndicator; // signal=Car_Signals_A.CS1ReverseIndicator;
    tFloat64 CS1VehicleSpeed; // signal=Car_Signals_A.CS1VehicleSpeed;
    tBool CS1VehicleSpeedValid; // signal=Car_Signals_A.CS1VehicleSpeedValid;
    tFloat64 CS1VehicleYaw; // signal=Car_Signals_A.CS1VehicleYaw;
    tBool CS1VehicleYawValid; // signal=Car_Signals_A.CS1VehicleYawValid;
    tBool CS1Wiper; // signal=Car_Signals_A.CS1Wiper;
    tUInt8 CS1TurnSwitchStatus; // signal=Car_Signals_A.CS1TurnSwitchStatus;
    tFloat64 CS1SteeringWheelAngle; // signal=Car_Signals_A.CS1SteeringWheelAngle;
    tBool CS1SteeringWheelAngVal; // signal=Car_Signals_A.CS1SteeringWheelAngVal;
    tBool CS1TopTrvlClutchSwitchAct; // signal=Car_Signals_A.CS1TopTrvlClutchSwitchAct;
    tBool CS1ClutchStartSwitchAct; // signal=Car_Signals_A.CS1ClutchStartSwitchAct;
    tBool CS1HighBeamStatus; // signal=Car_Signals_A.CS1HighBeamStatus;
    tBool CS1FogLightActivation; // signal=Car_Signals_A.CS1FogLightActivation;
    tUInt8 CS1AmbientLightLevel; // signal=Car_Signals_A.CS1AmbientLightLevel;
    tBool CS1AmbientLightLevelValid; // signal=Car_Signals_A.CS1AmbientLightLevelValid;
    tFloat64 CS1AcceleratorPedalValue; // signal=Car_Signals_A.CS1AcceleratorPedalValue;
} tCar_Signals_A;

/* Fusion CAN message */
typedef struct
{
    tUInt8 AccStatus; // signal=DASM_Status.AccStatus;
    tUInt8 DASM_CRC; // signal=DASM_Status.DASM_CRC;
    tUInt8 DASM_MC; // signal=DASM_Status.DASM_MC;
} tDASM_Status;

/* Odometer */
typedef struct
{
    tFloat64 AvgFuelLvl; // signal=IC_A1.AvgFuelLvl;
    tBool WarnPrio1_Mute_Rq; // signal=IC_A1.WarnPrio1_Mute_Rq;
    tUInt8 PCODE_CTRL; // signal=IC_A1.PCODE_CTRL;
    tUInt8 SRS_LMP_STAT; // signal=IC_A1.SRS_LMP_STAT;
    tBool FuelLvlLow; // signal=IC_A1.FuelLvlLow;
    tBool WTS_LmpRqOn; // signal=IC_A1.WTS_LmpRqOn;
    tUInt8 DPF_DEF_WrnStat; // signal=IC_A1.DPF_DEF_WrnStat;
    tBool Radio_Strng_Rq; // signal=IC_A1.Radio_Strng_Rq;
    tBool FuelSavDispEnbl; // signal=IC_A1.FuelSavDispEnbl;
    tBool ThatchamIC; // signal=IC_A1.ThatchamIC;
    tBool ACC_ScrnActv; // signal=IC_A1.ACC_ScrnActv;
    tBool Trailer_Trp_Rst; // signal=IC_A1.Trailer_Trp_Rst;
    tBool OilChgRst; // signal=IC_A1.OilChgRst;
    tBool FuelFltrRst; // signal=IC_A1.FuelFltrRst;
    tBool ALM_Rq; // signal=IC_A1.ALM_Rq;
    tFloat64 ODO; // signal=IC_A1.ODO;
    tUInt8 PowerMode_RQ; // signal=IC_A1.PowerMode_RQ;
    tBool IC_IntrnlFlt; // signal=IC_A1.IC_IntrnlFlt;
    tUInt8 PTS_MODE_SELECTION; // signal=IC_A1.PTS_MODE_SELECTION;
} tIC_A1;

#pragma pack(pop)

#endif // __CHRYSLER_CAN_SELECTION_HEADER__
