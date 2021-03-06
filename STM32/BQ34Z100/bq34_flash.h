/*
 * bq34_flash.h
 *
 *  Created on: 26 march 2021
 *      Author: IDJGB0
 */

#ifndef BQ34_FLASH_H_
#define BQ34_FLASH_H_

#include <stdio.h>
#include "cmsis_os.h"
#include "string.h"

#include "bq34z100g1.h"
#include "bq34_standard.h"

/*	Size of DataTypes */
#define F4	4
#define H1	1
#define H2	2
#define I1	1
#define I2	2
#define S5	5
#define S12	12
#define U1	1
#define U2	2

typedef enum {
	/* Data Flash Sub-Class */
	SAFETY				= 2,
	CHRG_INHIBIT_CNFG	= 32,
	CHRG				= 34,
	CHRG_TERMN			= 36,
	CFG_DATA			= 48,
	DISCHRG				= 49,
	MNF_DATA			= 56,
	MNF_INFO			= 58,
	LIFE_DATA			= 59,
	LIFE_TEMP_SAMP		= 60,
	REGISTERS			= 64,
	LIFE_RESOL			= 66,
	LED_DSPLY			= 67,
	POWER				= 68,
	IT_CFG				= 80,
	CRNT_THRSHLD		= 81,
	STATE				= 82,
	R_A0				= 88,
	R_AX				= 89,
	CAL_DATA			= 104,
	CAL_CURR			= 107,
	CODES				= 112,
	EOF_CONFIG			= 255,
} t_DFSubClass;

/* Define parameters of DataFlash subClass Safety in structure */
typedef struct tagbq34z100Config_s
{
/*********************************************SAFETY***********************************************/
	int16_t OTChg;
	uint8_t OTChgTime;
	int16_t OTChgRecovery;
	int16_t OTDsg;
	uint8_t OTDsgTime;
	int16_t OTDsgRecovery;

/************************************Charge Inhibit configuration**********************************/
	int16_t ChgInhibitTempLow;
	int16_t ChgInhibitTempHigh;
	int16_t TempHys;

/***********************************************Charge ********************************************/
	int16_t SpndLowTemp;
	int16_t SpndHighTemp;
	uint8_t PBEFFEffncy;
	float   PBTempComp;
	uint8_t PBDropOffPer;
	float   PBRedctRate;

/******************************************Charge Termination**************************************/
	int16_t  TaprCurrent;
	int16_t  MinTaprCap;
	int16_t  CellTaprVolt;
	uint8_t  CurTaprWindw;
	int8_t 	 TCASet;
	int8_t   TCAClear;
	int8_t   FCSet;
	int8_t   FCClear;
	int16_t	 DODatEOCDeltaT;
	int16_t  NiMHDeltaTemp;
	uint16_t NiMHDeltaTempTime;
	uint16_t NiMHHoldOffTime;
	int16_t  NiMHHoldOffCurrent;
	int16_t  NiMHHoldOffTemp;
	uint8_t  NiMHCellNegDeltaVolt;
	uint8_t  NiMHCellNegDeltaTime;
	int16_t  NiMHCellNegDeltaQualVolt;

/***********************************************Data**********************************************/
	uint16_t Reserved1;
	uint16_t ManufactureDate;
	uint16_t SerialNum;
	uint16_t CycleCount;
	int16_t  CCThreshold;
	uint8_t MaxErrorLimit;
	int16_t DesignCapacity;
	int16_t DesignEnergy;
	int16_t SOHLoadI;
	uint16_t CellChargeVoltageT1T2;
	uint16_t CellChargeVoltageT2T3;
	uint16_t CellChargeVoltageT3T4;
	uint8_t ChargeCurrentT1T2;
	uint8_t ChargeCurrentT2T3;
	uint8_t ChargeCurrentT3T4;
	int8_t JEITA_T1;
	int8_t JEITA_T2;
	int8_t JEITA_T3;
	int8_t JEITA_T4;
	uint8_t DesignEnergyScale;
	char DeviceName[12];
	char ManufacturerName[12];
	char DeviceChemistry[5];

/*******************************************DISCHARGE*******************************************/
	uint16_t SOC1SetThreshold;
	uint16_t SOC1ClearThreshold;
	uint16_t SOCFSetThreshold;
	uint16_t SOCFClearThreshold;
	int16_t CellBLSetVoltThreshold;
	uint8_t CellBLSetVoltTime;
	int16_t CellBLClearVoltThreshold;
	int16_t CellBHSetVoltThreshold;
	uint8_t CellBHVoltTime;
	int16_t CellBHClearVoltThreshold;
	uint8_t Reserved2[3];
	uint8_t CycleDelta;

/****************************************MANUFACTURER DATA**************************************/
	uint16_t PackLotCode;
	uint16_t PCBLotCode;
	uint16_t FirmwareVersion;
	uint16_t HardwareRevision;
	uint16_t CellRevision;
	uint16_t DFConfigVersion;

/******************************************LIFETIME DATA***************************************/
	int16_t LifetimeMaxTemp;
	int16_t LifetimeMinTemp;
	int16_t LifetimeMaxChgCurrent;
	int16_t LifetimeMaxDsgCurrent;
	uint16_t LifetimeMaxPackVoltage;
	uint16_t LifetimeMinPackVoltage;

/*************************************LIFETIME TEMP SAMPLES************************************/
	uint16_t LTFlashCnt;

/********************************************REGISTERS*****************************************/
	uint16_t PackConfiguration;
	uint8_t PackConfigurationB;
	uint8_t PackConfigurationC;
	uint8_t LED_CommConfiguration;
	uint16_t ALERTConfiguration;
	uint8_t NumberOfCells;

/*************************************LIFETIME RESOLUTION**********************************/
	uint8_t LTTempRes;
	uint8_t LTCurRes;
	uint8_t LTVRes;
	uint16_t LTUpdateTime;

/****************************************LED DISPLAY***************************************/
	uint8_t LEDHoldTime;

/*******************************************POWER******************************************/
	int16_t FlashUpdateOKCellVolt;
	int16_t SleepCurrent;
	uint8_t Reserved3[7];
	uint8_t FSWait;

/************************************MANUFCTURING INFO*************************************/
	uint8_t BlockA[32];

/*****************************************IT Cfg*******************************************/
	uint8_t LoadSelect;
	uint8_t LoadMode;
	uint8_t Reserved4[8];
	int16_t ResCurrent;
	uint8_t Reserved5[2];
	uint8_t MaxResFactor;
	uint8_t MinResFactor;
	uint8_t Reserved6;
	uint16_t RaFilter;
	uint8_t Reserved7[28];
	uint8_t MinPassedChgNiMH_LA1stQmax;
	uint8_t Reserved8;
	uint8_t MaximumQmaxChange;
	uint8_t Reserved9[3];
	int16_t CellTerminateVoltage;
	int16_t CellTermVDelta;
	uint8_t Reserved10;
	uint16_t ResRelaxTime;
	uint8_t Reserved11[2];
	int16_t UserRatemA;
	int16_t UserRatePwr;
	int16_t ReserveCapmAh;
	int16_t ReserveEnergy;
	uint8_t Reserved12[2];
	uint8_t MaxScaleBackGrid;
	uint16_t CellMinDeltaV;
	uint8_t RaMaxDelta;
	int16_t DesignResistance;
	uint8_t ReferenceGrid;
	uint8_t QmaxMaxDeltaPer;
	uint16_t MaxResScale;
	uint16_t MinResScale;
	uint8_t FastScaleStartSOC;
	uint8_t Reserved13[4];
	int16_t ChargeHysVShift;
	int16_t SmoothRelaxTime;

/**********************************Current Threshold*****************************************/
	int16_t DsgCurrentThreshold;
	int16_t ChgCurrentThreshold;
	int16_t QuitCurrent;
	uint16_t DsgRelaxTime;
	uint8_t ChgRelaxTime;
	uint16_t CellMaxIRCorrect;

/****************************************State**********************************************/
	int16_t QmaxCell0;
	uint16_t StateCycleCount;
	uint8_t UpdateStatus;
	int16_t CellVatChgTerm;
	int16_t AvgILastRun;
	int16_t AvgPLastRun;
	int16_t CellDeltaVoltage;
	int16_t TRise;
	int16_t TTimeConstant;

}__attribute__((packed, aligned(1))) tagbq34z100Config;

typedef struct {
    int8_t  	set;
    int8_t  	clear;
} voltage_t;

typedef struct {
    int16_t     current;
    int16_t     min_capacity;
    int16_t     cell_voltage;
    uint8_t     window;
    voltage_t   tca;
    voltage_t   fc;
} taper_t;

typedef struct
{
	tagBQ_DFSubClass enBQ_DFSubClass;
	unsigned char SubClassSize;
	unsigned char NoOfParam;
	unsigned short SubClassLocatn;
}tagBQ_DFConfigSubClass;

/* basic */
uint8_t* bq34fl_get_buffer(void);
void bq34fl_read_block(uint8_t sub_class, uint8_t offset);
void bq34fl_write_block(uint8_t sub_class, uint8_t offset);
bool bq34fl_check_checksum(uint8_t delay);

/* advanced */
bool bq34fl_update_design_capacity(int16_t capacity);
bool bq34fl_update_q_max(int16_t capacity)
bool bq34fl_update_design_energy(int16_t energy); 
bool bq34fl_update_cell_charge_voltage_range(uint16_t t1_t2, uint16_t t2_t3, uint16_t t3_t4);
bool bq34fl_update_number_of_series_cells(uint8_t cells);
bool bq34fl_update_pack_configuration(uint16_t config);
bool bq34fl_update_charge_termination_parameters(taper_t);

/* advanced & calibration */
void bq34fl_calibrate_voltage_divider(uint16_t applied_voltage, uint8_t cells_count);
void bq34fl_calibrate_sense_resistor(int16_t applied_current);

#endif /* BQ34_FLASH_H_ */