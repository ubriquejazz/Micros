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

typedef struct BMSConfig_CBalancing_s
{
	int8_t		minCellTemp;				/* Min cell Temp */
	int8_t		maxCellTemp;				/* Max cell Temp */
	uint8_t		maxCellsBalancing;			/* ... */
	int16_t		minCellVoltage;				/* ... */
	uint8_t		minDelta;					/* ... */
	uint8_t		stopDelta;					/* Stop delta */
}__attribute__((packed, aligned(1))) BMSConfig_CBalancing_t;

typedef struct BMSConfig_Configuration_s
{
	uint8_t		balanceConfig;				/* ... */
	uint8_t		mfgStatusInit;				/* ... */
	uint16_t	vcellMode;					/* ... */
	uint8_t		regulatorEnableREG0;		/* ... */
	uint8_t		regulatorEnableREG12;		/* ... */
	uint8_t		fetOptions;					/* ... */
	uint8_t		chgPumpCtrl;				/* ... */
	uint8_t		pinConfigCFETOFF;			/* ... */
	uint8_t		pinConfigDFETOFF;			/* ... */
	uint8_t		pinConfigALERT;				/* ... */
	uint8_t		pinConfigTS1;				/* ... */
	uint8_t		pinConfigTS2;				/* ... */
	uint8_t		pinConfigTS3;				/* ... */
	uint8_t		pinConfigHDG;				/* ... */
	uint8_t		pinConfigDCHG;				/* ... */
	uint8_t		pinConfigDDSG;				/* ... */
}__attribute__((packed, aligned(1))) BMSConfig_Configuration_t;

typedef struct BMSConfig_Configuration2_s
{
	uint8_t 	enableProtectionA;         /* ... */
	uint8_t 	enableProtectionB;         /* ... */
	uint8_t 	enableProtectionC;         /* ... */
	uint8_t 	CFET_ProtectionA;		   /* ... */
	uint8_t 	CFET_ProtectionB;		   /* ... */
	uint8_t 	CFET_ProtectionC;		   /* ... */
	uint8_t 	DFET_ProtectionA;		   /* ... */
	uint8_t 	DFET_ProtectionB;		   /* ... */
	uint8_t 	DFET_ProtectionC;		   /* ... */
	uint8_t 	enablePFailA;              /* ... */
	uint8_t 	enablePFailB;              /* ... */
	uint8_t 	enablePFailC;              /* ... */
	uint8_t 	enablePFailD;              /* ... */
	uint16_t 	alarmEnable;               /* ... */
	uint16_t	alarmDefaultMask;		   /* ... */
	uint16_t	powerConfig;               /* ... */
	uint8_t		recoveryTime;              /* ... */
	float		CCGain;                    /* ... */
	float		CapacityGain;              /* ... */
	uint16_t	lowCurrent;                /* ... */
	float		lowVbat;                   /* ... */
	float 		deltaChargerAtt;		   /* ... */
	float 		threshChargerAtt;		   /* ... */
	uint16_t	dsgCurrentRelax;           /* ... */
	uint16_t	chgCurrentRelax;           /* ... */
}__attribute__((packed, aligned(1))) BMSConfig_Configuration2_t;

typedef struct BMSConfig_Protection_s
{
	/* Overvoltage I */
	float		thresholdCOV;           /* ... */
	uint16_t	msCOV;                  /* ... */
	float		recoveryCOV;            /* ... */
	float		thresholdCUV;           /* ... */
	uint16_t	msCUV;                  /* ... */
	float		recoveryCUV;            /* ... */
	uint8_t		secCOW;                 /* ... */
	/* OverTemperature I */
	int8_t		thresholdOTD;           /* ... */
	int8_t		recoveryOTD;            /* ... */
	uint8_t		secOTD;                 /* ... */
	int8_t		thresholdOTC;           /* ... */
	int8_t		recoveryOTC;            /* ... */
	uint8_t		secOTC;                 /* ... */
	int8_t		thresholdOTInt;         /* ... */
	int8_t		recoveryOTInt;          /* ... */
	uint8_t		secOTInt;               /* ... */
	/* UnderTemperture I */
	int8_t		thresholdUTD;           /* ... */
	int8_t		recoveryUTD;            /* ... */
	uint8_t		secUTD;                 /* ... */
	int8_t		thresholdUTC;           /* ... */
	int8_t		recoveryUTC;            /* ... */
	uint8_t		secUTC;                 /* ... */
	int8_t		thresholdUTInt;         /* ... */
	int8_t		recoveryUTInt;          /* ... */
	uint8_t		secUTInt;               /* ... */
	/* OverCurrent I */
	float		thresholdOCC;           /* ... */
	uint8_t		msOCC;                  /* ... */
	int16_t		recovery1OCC;		    /* ... */
	int16_t		recovery2OCC;		    /* ... */
	/* Discharge I */
	float		thresholdOCD1;          /* ... */
	uint8_t		msOCD1;                 /* ... */
	float		thresholdOCD2;          /* ... */
	uint8_t		msOCD2;                 /* ... */
	uint8_t		thresholdSCD;           /* ... */
	uint8_t		usSCD;                  /* ... */
	uint8_t		recoverySCD;            /* ... */
	/* Overvoltage II */
	int16_t		thresholdSUV;			/* ... */
	uint8_t		secSUV;					/* ... */
	int16_t		thresholdSOV;			/* ... */
	uint8_t		secSOV;					/* ... */
	int16_t		thresholdTOS;			/* ... */
	uint8_t		secTOS;					/* ... */
	/* Temperature II */
	int8_t		thresholdSOT;			/* ... */
	uint8_t		secSOT;					/* ... */
	int8_t		thresholdSOTF;			/* ... */
	uint8_t		secSOTF;				/* ... */
	/* OverCurrent II */
	int16_t 	thresholdSOCC;			/* ... */
	uint8_t 	secSOCC;				/* ... */
	int16_t		thresholdSOCD;			/* ... */
	uint8_t 	secSOCD;				/* ... */
	/* FETs II */
	int16_t		thresholdCFET;			/* ... */
	uint8_t		secCFET;				/* ... */
	int16_t		thresholdDFET;			/* ... */
	uint8_t		secDFET;				/* ... */
	/* Imbalance II */
	int16_t		threshVIMR;				/* ... */
	uint8_t 	secVIMR;				/* ... */
	int16_t		checkVoltageVIMR;		/* ... */
	int16_t		maxCurrentVIMR;			/* ... */
	uint16_t	durationVIMR;			/* ... */

}__attribute__((packed, aligned(1))) BMSConfig_Protection_t;

typedef struct BMSConfig_Info_s
{
	uint32_t ConfigDateLast;		/* UTC time when last configured */
	uint32_t ConfigDateFirst;		/* UTC time when first configured */
}__attribute__((packed, aligned(1))) BMSConfig_Info_t;

typedef struct BMSConfig_Status_s
{
	uint8_t						configured;		// YES/NO
	uint8_t						state;			// RUN, CONFIG, SLEEP, ...
	// Todo: More
}__attribute__((packed, aligned(1))) BMSConfig_Status_t;


typedef struct BMSConfig_s
{
	BMSConfig_Status_t			status;

	BMSConfig_CBalancing_t		bal;

	BMSConfig_Configuration_t	conf;

	BMSConfig_Configuration2_t	conf2;

	BMSConfig_Protection_t		protect;

	BMSConfig_Info_t			info;

}__attribute__((packed, aligned(1))) BMSConfig_t;

#endif /* BMS_CONFIG_H_ */
