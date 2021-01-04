/**
  ******************************************************************************
  * @file    bmsconfig.h
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   BMS Config file
  ******************************************************************************
  */

#ifndef BMS_CONFIG_H_
#define BMS_CONFIG_H_

/* Historical Data ---------------------------------------------------------- */

typedef struct BMS_HistoricalValue_s
{
	timeIdn_dateTime_t	time;
	float				value;
} BMS_HistoricalValue_t;

typedef struct {
	BMS_HistoricalValue_t	TempMax;
	BMS_HistoricalValue_t	TempMin;
	BMS_HistoricalValue_t	TempAvg;
	BMS_HistoricalValue_t	CurrMax;
	BMS_HistoricalValue_t	CurrAvg;
} BMS_HistoricalData_t;

/* Thresholds ---------------------------------------------------------------- */

typedef struct {
	uint8_t 			MaxPkVoltThresh;			// Peak voltage (max)
	uint8_t 				MaxPackVoltRkvry;
	uint8_t 			MinPkVoltThresh;			// Peak voltage (min)
	uint8_t 				MinPkVoltRecovry;
	uint8_t 			MaxCellVoltThresh;			// Cell Voltage (max)
	uint8_t 				MaxCellVoltRkvry;
	uint8_t 			MinCellVoltThresh;			// Cell Voltage (min)
	uint8_t 				MinCellVoltRkvry;	
	uint8_t 			DeltaCellVoltThresh;		// Cell Voltage (delta)
	uint8_t 				DeltaCellVoltRkvry;
	/* currents */
	int32_t 			DsgCurThresh;				// Discharge Current
	int32_t 				DsgCurRkvry;
	int32_t 			ChrgCurThresh;				// Charge Current
	int32_t 				ChrgCurRkvry;	
	/* temperatures */
	uint8_t 			MaxExtTempThresh;			// Ext Temperature (max)
	uint8_t 				MaxExtTempRkvry;
	uint8_t 			MinExtTempThresh;			// Ext Temperature (min)
	uint8_t 				MinExtTempRkvry;
	uint8_t 			MaxIntTempThresh;			// Int Temperature (max)
	uint8_t 				MaxIntTempRkvry;
	uint8_t 			MinIntTempThresh;			// Int Temperature (min)
	uint8_t 				MinIntTempRkvry;
} threshold_t;

typedef struct{
	/************** Structure: Warning Threshold *****************/
	threshold_t			x
} tagBMSWarnThresh;

typedef struct{
	/************** Structure: Error Thresholds *****************/
	threshold_t			x
} tagBMSErrorThresh;

/* System Commands ---------------------------------------------------------- */

typedef struct {
//	uint8_t				numberOfCells;				// 
//	uint8_t				whereAreTheCells;
//	uint8_t				sleepTimeSec;
	uint8_t				PreChrgTimeThresh;
	uint8_t 			BalanceStartVoltage;		// Start Voltage
	uint8_t				CellBalanceTimeThresh;		// Time in Seconds
	int32_t 			ShrtCktThresh;
//	
//	tagBMSInitConfig 	stBMSInitConfig;
//	tagBMSWarnThresh 	stBMSWarnThresh;
//	tagBMSErrorThresh 	stBMSErrorThresh;
//	tagBMSCellBal		stBMSCellBal;;
} tagBMSConfig;

#endif /* BMS_CONFIG_H_ */
