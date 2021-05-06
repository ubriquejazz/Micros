#include "bq34_flash.h"

uint8_t flash_block_data[32];

typedef struct $ {
    t_DFSubClass    DFSubClass;         
    uint8_t         Size;
    uint8_t         NoOfparam; 
} tagBQ_DFConfigSubClass;

tagBQ_DFConfigSubClass DFConfigSubClass[]=
{// {SubClass           Size,   NoOfparam},
/****************** Class: Configuration *********************/
    {SAFETY,            10,     6},
    {CHRG_INHIBIT_CNFG, 6,      3},
    {CHRG,              14,     6},
    {CHRG_TERMN,        27,     17},
    {CFG_DATA,          60,     23},
    {DISCHRG,           22,     11},
    {MNF_DATA,          12,     6},
    {LIFE_DATA,         12,     6},
    {LIFE_TEMP_SAMP,    2,      1},
    {REGISTERS,         8,      6},
    {LIFE_RESOL,        5,      4},
    {LED_DSPLY,         1,      1},
    {POWER,             12,     4},
/*************** Class: Manufacturing Info. ******************/
    {MNF_INFO,          32,     1},
/******************* Class: Gas Gauging **********************/
    {IT_CFG,            93,     36},
    {CRNT_THRSHLD,      11,     6},
    {STATE,             17,     9},
    {EOF_CONFIG,        0,      0},
};

typedef struct $ {
    uint8_t Offset;
    uint8_t DataType;
} tagBQ_DFConfig;
//Note: Your cannot skip inbetween variable of subclass
//      Start of varaible should always be from zero offset
//      No. of Parameters of kstBQ_DFConfig[] must be inline with the other structure
tagBQ_DFConfig kstBQ_DFConfig[]=
{
/*********************************************SAFETY***********************************************/
//  {Offset,DataType
    {0,     I2},        //OT Chg
    {2,     U1},        //OT Chg Time
    {3,     I2},        //OT Check Recovery
    {5,     I2},        //OT Dsg
    {7,     U1},        //OT Dsg Time
    {8,     I2},        //OT Dsg Recovery

/************************************Charge Inhibit configuration**********************************/
    {0,     I2},        //Chg Inhibit Temp Low
    {2,     I2},        //Chg Inhibit Temp High
    {4,     I2},        //Temp Hysterisis

/***********************************************Charge ********************************************/
    {0,     I2},        //Suspend Low Temp
    {2,     I2},        //Suspend High Temp
    {4,     U1},        //Pb EFF Efficiency
    {5,     F4},        //Pb Temp Comp
    {9,     U1},        //Pb Drop Off Percent
    {10,    F4},        //Pb Reduction Rate

/******************************************Charge Termination**************************************/
    {0,     I2},        //Taper Current
    {2,     I2},        //Min Taper Capacity
    {4,     I2},        //Cell Taper Voltage
    {6,     U1},        //Current Taper Window
    {7,     I1},        //TCA Set %
    {8,     I1},        //TCA Clear %
    {9,     I1},        //FC Set %
    {10,    I1},        //FC Clear %
    {11,    I2},        //DODatEOC Delta T
    {13,    I2},        //NiMH Delta Temp
    {15,    U2},        //NiMH Delta Temp Time
    {17,    U2},        //NiMH Hold Off Time
    {19,    I2},        //NiMH Hold Off Current
    {21,    I2},        //NiMH Hold Off Temp
    {23,    U1},        //NiMH Cell Negative Delta Volt
    {24,    U1},        //NiMH Cell Negative Delta Time
    {25,    I2},        //NiMH Cell Neg Delta Qual Volt

/*********************************************DATA**********************************************/
    {0,     2},         //RESERVED1
    {2,     U2},        //Manufacture Date
    {4,     H2},        //Serial Number
    {6,     U2},        //Cycle Count
    {8,     I2},        //CC Threshold
    {10,    U1},        //Max Error Limit
    {11,    I2},        //Design Capacity
    {13,    I2},        //Design Energy
    {15,    I2},        //SOH Load I
    {17,    U2},        //Cell Charge Voltage T1-T2
    {19,    U2},        //Cell Charge Voltage T2-T3
    {21,    U2},        //Cell Charge Voltage T3-T4
    {23,    U1},        //Charge Current T1-T2
    {24,    U1},        //Charge Current T2-T3
    {25,    U1},        //Charge Current T3-T4
    {26,    I1},        //JEITA T1
    {27,    I1},        //JEITA T2
    {28,    I1},        //JEITA T3
    {29,    I1},        //JEITA T4
    {30,    U1},        //Design Energy Scale
    {31,    S12},       //Device Name
    {43,    S12},       //Manufacturer Name
    {55,    S5},        //Device Chemistry

/*******************************************DISCHARGE*******************************************/
    {0,     U2},        //SOC1 Set Threshold
    {2,     U2},        //SOC1 Clear Threshold
    {4,     U2},        //SOCF Set Threshold
    {6,     U2},        //SOCF Clear Threshold
    {8,     I2},        //Cell BL Set Volt Threshold
    {10,    U1},        //Cell BL Set Volt Time
    {11,    I2},        //Cell BL Clear Volt Threshold
    {13,    I2},        //Cell BH Set Volt Threshold
    {15,    U1},        //Cell BH Volt Time
    {16,    I2},        //Cell BH Clear Volt Threshold
    {18,    3},         //RESERVED2
    {21,    U1},        //Cycle Delta

/****************************************MANUFACTURER DATA**************************************/
    {0,     H2},        //Pack Lot Code
    {2,     H2},        //PCB Lot Code
    {4,     H2},        //Firmware Version
    {6,     H2},        //Hardware Revision
    {8,     H2},        //Cell Revision
    {10,    H2},        //DF Config Version

/******************************************LIFETIME DATA***************************************/
    {0,     I2},        //Lifetime Max Temp
    {2,     I2},        //Lifetime Min Temp
    {4,     I2},        //Lifetime Max Chg Current
    {6,     I2},        //Lifetime Max Dsg Current
    {8,     U2},        //Lifetime Max Pack Voltage
    {10,    U2},        //Lifetime Min Pack Voltage

/*************************************LIFETIME TEMP SAMPLES************************************/
    {0,     U2},        //LT Flash Cnt

/********************************************REGISTERS*****************************************/
    {0,     H2},        //Pack Configuration
    {2,     H1},        //Pack Configuration B
    {3,     H1},        //Pack Configuration C
    {4,     H1},        //LED_Comm Configuration
    {5,     H2},        //Alert Configuration
    {7,     U1},        //Number of series cell

/*************************************LIFETIME RESOLUTION**********************************/
    {0,     U1},        //LT Temp Res
    {1,     U1},        //LT Cur Res
    {2,     U1},        //LT V Res
    {3,     U2},        //LT Update Time

/****************************************LED DISPLAY***************************************/
    {0,     U1},        //LED Hold Time

/*******************************************POWER******************************************/
    {0,     I2},        //Flash Update OK Cell Volt
    {2,     I2},        //Sleep Current
    {4,     7},         //RESERVED3
    {11,    U1},        //FS Wait

/************************************MANUFACTURING INFO************************************/
    {0,     32},        //BLOCK A

/*******************************************IT CFG*****************************************/
    {0,     U1},        //Load Select
    {1,     U1},        //Load Mode
    {2,     8},         //RESERVED4
    {10,    I2},        //Res Current
    {12,    2},         //RESERVED5
    {14,    U1},        //Max Res Factor
    {15,    U1},        //Min Res Factor
    {16,    1},         //RESERVED6
    {17,    U2},        //Ra Filter
    {19,    28},        //RESERVED7
    {47,    U1},        //Min PassedChg NiMH-LA 1st Qmax
    {48,    1},         //RESERVED8
    {49,    U1},        //Maximum Qmax Change
    {50,    3},         //RESERVED9
    {53,    I2},        //Cell Terminate Voltage
    {55,    I2},        //Cell Term V Delta
    {57,    1},         //RESERVED10
    {58,    U2},        //ResRelax Time
    {60,    2},         //RESERVED11
    {62,    I2},        //User Rate-mA
    {64,    I2},        //User Rate-Pwr
    {66,    I2},        //Reserve Cap-mAh
    {68,    I2},        //Reserve Energy
    {70,    2},         //RESERVED12
    {72,    U1},        //Max Scale Back Grid
    {73,    U2},        //Cell Min DeltaV
    {75,    U1},        //Ra Max Delta
    {76,    I2},        //Design Resistance
    {78,    U1},        //Reference Grid
    {79,    U1},        //Qmax Max Delta %
    {80,    U2},        //Max Res Scale
    {82,    U2},        //Min Res Scale
    {84,    U1},        //Fast Scale Start SOC
    {85,    4},         //RESERVED13
    {89,    I2},        //Charge Hys V Shift
    {91,    I2},        //Smooth Relax Time

/********************************************Current Threshold*****************************************/
    {0,     I2},        //Dsg Current Threshold
    {2,     I2},        //Chg Current Threshold
    {4,     I2},        //Quit Current
    {6,     U2},        //Dsg Relax Time
    {8,     U1},        //Chg Relax Time
    {9,     U2},        //Cell Max IR Correct

/***********************************************State*************************************************/
    {0,     I2},        //Qmax Cell 0
    {2,     U2},        //Cycle Count
    {4,     H1},        //Update Status
    {5,     I2},        //Cell V at Chg TermS
    {7,     I2},        //Avg I Last Run
    {9,     I2},        //Avg P Last Run
    {11,    I2},        //Cell Delta Voltage
    {13,    I2},        //T Rise
    {15,    I2},        //T Time Constant
};

/**************************************************************************************
* BREIF:    This method swap the bytes of the variable depending on its size
* PARAM:    *source:    Start Address.
*           Offset:     Offset value from start address
*           size:       Size of the variable
* RETVAL:   None
***************************************************************************************/
static void SwapBytes(void *source, int Offset ,int size)
{
    typedef uint8_t TwoBytes[2];
    typedef uint8_t FourBytes[4];
    typedef uint8_t EightBytes[8];

    source = source + Offset;
    unsigned char temp;

    if(size == 2)
    {
        TwoBytes *src = (TwoBytes *)source;
        temp = (*src)[0];
        (*src)[0] = (*src)[1];
        (*src)[1] = temp;
        return;
    }

    if(size == 4)
    {
        FourBytes *src = (FourBytes *)source;
        temp = (*src)[0];
        (*src)[0] = (*src)[3];
        (*src)[3] = temp;

        temp = (*src)[1];
        (*src)[1] = (*src)[2];
        (*src)[2] = temp;
        return;
    }

    if(size == 8)
    {
        EightBytes *src = (EightBytes *)source;
        temp = (*src)[0];
        (*src)[0] = (*src)[7];
        (*src)[7] = temp;

        temp = (*src)[1];
        (*src)[1] = (*src)[6];
        (*src)[6] = temp;

        temp = (*src)[2];
        (*src)[2] = (*src)[5];
        (*src)[5] = temp;

        temp = (*src)[3];
        (*src)[3] = (*src)[4];
        (*src)[4] = temp;
        return;
    }
}


/**************************************************************************************
* BREIF:    This method Read's/Write's a data flash configuration class memory
*           in a loop.
* PARAM:    mode: Read/Write mode
* RETVAL:   None.
***************************************************************************************/
static void bq34z100DFConfig(int mode, tagbq34z100Config *pbq34z100Config)
{
    int iParamCnt=0;
    int SubClassLocatn = 0;
    for(int k = 0; DFConfigSubClass[k].SubClass!=EOF_CONFIG; k++)
    {
        if(mode == READ)
            bq34z100_read_data_class(   DFConfigSubClass[k].DFSubClass, pbq34z100Config,
                                        DFConfigSubClass[k].SubClassSize, SubClassLocatn);

        for(int i=0 ; i<DFConfigSubClass[k].NoOfParam; i++)
        {
            SwapBytes(pbq34z100Config, 
                        kstBQ_DFConfig[iParamCnt].chOffset + SubClassLocatn,
                        kstBQ_DFConfig[iParamCnt].chDataType);
            iParamCnt++;
        }
        if(mode == WRITE)
            bq34z100_write_data_class(  DFConfigSubClass[k].DFSubClass, pbq34z100Config,
                                        DFConfigSubClass[k].SubClassSize, SubClassLocatn);

        // SubClassLocatn: Provides the start address of the first variable of next subclass
        SubClassLocatn += DFConfigSubClass[k].SubClassSize;
    }

    if(mode == WRITE)
    {
        //  Swapping the bytes of variables to readable format
        iParamCnt=0;
        SubClassLocatn = 0;
        for(int k = 0; DFConfigSubClass[k].DFSubClass!=EOF_CONFIG; k++)
        {
            for(int i=0 ; i<DFConfigSubClass[k].NoOfParam ; i++)
            {
                SwapBytes(pbq34z100Config, 
                            kstBQ_DFConfig[iParamCnt].chOffset + SubClassLocatn,
                            kstBQ_DFConfig[iParamCnt].chDataType);
                iParamCnt++;
            }
            SubClassLocatn += DFConfigSubClass[k].SubClassSize;
        }
    }

/**************************************************************************************
* BREIF:    This method read a dataflash subclass memory BQ34Z100-G1
* PARAM:    nDataClass: data class number
*           pData:      buffer holding the whole data class (all blocks)
*           nLength:    length of data class (all blocks)
* RETVAL:   0 = success
***************************************************************************************/
static int bq34z100_read_data_class(unsigned char nDataClass, void *pData, unsigned char nLength, unsigned short StartAddressLocatn)
{
    unsigned char nRemainder = nLength;
    unsigned char nDataBlock = 0x00;

    pData = pData+StartAddressLocatn;

    if (nLength < 1) return 0;

    do
    {
        nLength = nRemainder;
        if (nLength > 32)
        {
            nRemainder = nLength - 32;
            nLength = 32;
        }
        else nRemainder = 0;

        //nData = (nDataBlock << 8) | nDataClass;
        bq34z100_cmd_write(CMD_BLK_DATA_CNTRL, 0x00U,1);            //Enable Flash x’fer command
        bq34z100_cmd_write(CMD_DATA_FLASH_CLASS, nDataClass,1); //SubClass address
        bq34z100_cmd_write(CMD_DATA_FLASH_BLOCK, nDataBlock,1); //Enable General Purpose Block and its pages
        osDelay(200);   //Delay: To get the initial vale of pData

        if(I2c3_Read(BQ34Z100_ADDR,CMD_BLOCK_DATA, 1,pData, nLength) != 0x01U)  // RD Operation OK
        {
            LogErr_Put(BQ34Z100_MODID, BQ34Z100_COMMUNICATION, Notification);
            return -1;
        }

        osDelay(200);

        pData += nLength;
        nDataBlock++;

    }while (nRemainder > 0);

    return 0;
}


/**************************************************************************************
* BREIF:    This method calculate check sum for block transfer
* PARAM:    pData:      pointer to data block
*           nLength:    length of data block
* RETVAL:   Checksum Value
***************************************************************************************/
static unsigned char check_sum(unsigned char *pData, unsigned char nLength)
{
    unsigned char nSum = 0x00;
    unsigned char n;

    for (n = 0; n < nLength; n++)
        nSum += pData[n];

    nSum = 0xFF - nSum;
    return nSum;
}


/**************************************************************************************
* BREIF:    This method write a dataflash subclass memory of BQ34Z100-G1
* PARAM:    nDataClass: data class number
*           pData:      buffer holding the whole data class (all blocks)
*           nLength:    length of data class (all blocks)
*           StartAddressLocatn: Offset of buffer holding the whole data class
* RETVAL:   0 = success
***************************************************************************************/
static int bq34z100_write_data_class(unsigned char nDataClass, void *pData,unsigned char nLength,unsigned short StartAddressLocatn)
{
    unsigned char nRemainder = nLength;
    unsigned char pCheckSum[2] = {0x00, 0x00};
    unsigned char nDataBlock = 0x00;

    pData = pData+StartAddressLocatn;
    if (nLength < 1) return 0;

    do
     {
        nLength = nRemainder;

        if (nLength > 32)
        {
            nRemainder = nLength - 32;
            nLength = 32;
        }
        else
            nRemainder = 0;

    //nData = (nDataBlock << 8) | nDataClass;
    bq34z100_cmd_write(CMD_BLK_DATA_CNTRL, 0x00U,1);                //Enable Flash x’fer command / Enable block data flash control
    bq34z100_cmd_write(CMD_DATA_FLASH_CLASS, nDataClass,1);     // Sub-Class address
    bq34z100_cmd_write(CMD_DATA_FLASH_BLOCK, nDataBlock,1);     //Enable General Purpose Block and its pages
    osDelay(100);


    if (I2c3_Write(BQ34Z100_ADDR, CMD_BLOCK_DATA,1, pData, nLength) != 0x01U)
    {
        LogErr_Put(BQ34Z100_MODID, BQ34Z100_COMMUNICATION, Notification);
        return -1;
    }

    pCheckSum[0] = check_sum(pData, nLength);
    I2c3_Write(BQ34Z100_ADDR,CMD_CHECK_SUM, 1,pCheckSum, 1);

    osDelay(1000);      // Delay 1,000 mSec
    bq34z100_cmd_write(CMD_DATA_FLASH_CLASS, nDataClass,1);

    I2c3_Read(BQ34Z100_ADDR,CMD_CHECK_SUM, 1,pCheckSum + 1, 1);

    if (pCheckSum[0] != pCheckSum[1])
    {
        LogErr_Put(BQ34Z100_MODID, BQ34Z100_CHKSUM, Notification);
        return -2;
    }

    pData += nLength;
    nDataBlock++;
    } while (nRemainder > 0);
    return 0;
}











/* basic */

void bq34fl_get_buffer() {
    return &flash_block_data[0];
}

void bq34fl_read_block(uint8_t sub_class, uint8_t offset) {
    bq34_write_reg(0x61, 0x00); // Block control
    bq34_write_reg(0x3e, sub_class); // Flash class
    bq34_write_reg(0x3f, offset / 32); // Flash block
    
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(0x40); // Block data
    Wire.requestFrom(BQ34Z100_G1_ADDRESS, 32, true);
    for (uint8_t i = 0; i < 32; i++) {
        flash_block_data[i] = Wire.read(); // Data
    }
}

void bq34fl_write_block(uint8_t sub_class, uint8_t offset) {
    bq34_write_reg(0x61, 0x00); // Block control
    bq34_write_reg(0x3e, sub_class); // Flash class
    bq34_write_reg(0x3f, offset / 32); // Flash block
    
    Wire.beginTransmission(BQ34Z100_G1_ADDRESS);
    Wire.write(0x40); // Block data
    for (uint8_t i = 0; i < 32; i++) {
        Wire.write(flash_block_data[i]); // Data
    }
    Wire.endTransmission(true);
}

uint8_t bq34fl_block_checksum() {
    uint8_t temp = 0;
    for (uint8_t i = 0; i < 32; i++) {
        temp += flash_block_data[i];
    }
    return 255 - temp;
}

bool bq34fl_check_checksum(uint8_t delay)
{
    bq34_write_reg(0x60, bq34fl_block_checksum(););
    OsDelay(delay);
    bq34std_reset();
    OsDelay(delay);
    return true;
}

/* advanced */

bool bq34fl_update_design_capacity(int16_t capacity) {
    bq34_unsealed ();
    bq34fl_read_block(48, 0);    
    flash_block_data[6] = 0; // Cycle Count
    flash_block_data[7] = 0;
    
    flash_block_data[8] = capacity >> 8; // CC Threshold
    flash_block_data[9] = capacity & 0xff;
    
    flash_block_data[11] = capacity >> 8; // Design Capacity
    flash_block_data[12] = capacity & 0xff;
    
    for (uint8_t i = 6; i <= 9; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    
    for (uint8_t i = 11; i <= 12; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34fl_check_checksum(150);
}

bool bq34fl_update_q_max(int16_t capacity) {
    bq34_unsealed ();
    bq34fl_read_block(82, 0);
    flash_block_data[0] = capacity >> 8; // Q Max
    flash_block_data[1] = capacity & 0xff;
    
    flash_block_data[2] = 0; // Cycle Count
    flash_block_data[3] = 0;
    
    for (uint8_t i = 0; i <= 3; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34fl_check_checksum(150);
}

bool bq34fl_update_design_energy(int16_t energy) {
    bq34_unsealed ();
    bq34fl_read_block(48, 0);
    flash_block_data[13] = energy >> 8; // Design Energy
    flash_block_data[14] = energy & 0xff;
    
    for (uint8_t i = 13; i <= 14; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34fl_check_checksum(150);
}

bool bq34fl_update_cell_charge_voltage_range(uint16_t t1_t2, uint16_t t2_t3, uint16_t t3_t4) {
    bq34_unsealed ();
    bq34fl_read_block(48, 0);    
    flash_block_data[17] = t1_t2 >> 8; // Cell Charge Voltage T1-T2
    flash_block_data[18] = t1_t2 & 0xff;
    
    flash_block_data[19] = t2_t3 >> 8; // Cell Charge Voltage T2-T3
    flash_block_data[20] = t2_t3 & 0xff;
    
    flash_block_data[21] = t3_t4 >> 8; // Cell Charge Voltage T3-T4
    flash_block_data[22] = t3_t4 & 0xff;
    
    for (uint8_t i = 17; i <= 22; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34fl_check_checksum(150);
}

bool bq34fl_update_number_of_series_cells(uint8_t cells) {
    bq34_unsealed ();
    bq34fl_read_block(64, 0);    
    flash_block_data[7] = cells; // Number of Series Cell
    
    bq34_write_reg(0x40 + 7, flash_block_data[7]);    
    return bq34fl_check_checksum(150);
}

bool bq34fl_update_pack_configuration(uint16_t config) {
    bq34_unsealed ();
    read_flash_block(64, 0);    
    flash_block_data[0] = config >> 8; // Pack Configuration
    flash_block_data[1] = config & 0xff;
    
    for (uint8_t i = 0; i <= 1; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34fl_check_checksum(150);
}

bool bq34fl_update_charge_termination_parameters(taper_t taper) 
{
    taper_t updated;
    bq34_unsealed ();
    bq34fl_read_block(36, 0);
    
    flash_block_data[0] = taper.current >> 8; // Taper Current
    flash_block_data[1] = taper.current & 0xff;
    
    flash_block_data[2] = taper.min_capacity >> 8; // Min Taper Capacity
    flash_block_data[3] = taper.min_capacity & 0xff;
    
    flash_block_data[4] = taper.cell_voltage >> 8; // Cell Taper Voltage
    flash_block_data[5] = taper.cell_voltage & 0xff;
    
    flash_block_data[6] = taper.window; // Current Taper Window
    
    flash_block_data[7] = taper.tca.set & 0xff; // TCA Set %
    flash_block_data[8] = taper.tca.clear & 0xff; // TCA Clear %
    
    flash_block_data[9] = taper.fc.set & 0xff; // FC Set %
    flash_block_data[10] = taper.fc.clear & 0xff; // FC Clear %
    
    for (uint8_t i = 0; i <= 10; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    return bq34fl_check_checksum(150);
}

/* advanced & calibration */

void bq34fl_calibrate_voltage_divider(uint16_t applied_voltage, uint8_t cells_count) {
    double volt_array[50];
    for (uint8_t i = 0; i < 50; i++) {
        volt_array[i] = voltage();
        delay(150);
    }
    double volt_mean = 0;
    for (uint8_t i = 0; i < 50; i++) {
        volt_mean += volt_array[i];
    }
    volt_mean /= 50.0;
    
    double volt_sd = 0;
    for (uint8_t i = 0; i < 50; i++) {
        volt_sd += pow(volt_array[i] - volt_mean, 2);
    }
    volt_sd /= 50.0;
    volt_sd = sqrt(volt_sd);
    
    if (volt_sd > 100) {
        return;
    }

    bq34_unsealed ();
    bq34fl_read_block(104, 0);
    
    uint16_t current_voltage_divider = flash_block_data[14] << 8;
    current_voltage_divider |= flash_block_data[15];
    
    uint16_t new_voltage_divider = ((double)applied_voltage / volt_mean) * (double)current_voltage_divider;
    
    flash_block_data[14] = new_voltage_divider >> 8;
    flash_block_data[15] = new_voltage_divider & 0xff;
    
    for (uint8_t i = 14; i <= 15; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34fl_check_checksum(150);
    
    bq34_unsealed ();
    bq34fl_read_block(68, 0);
    
    int16_t flash_update_of_cell_voltage = (double)(2800 * cells_count * 5000) / (double)new_voltage_divider;
    
    flash_block_data[0] = flash_update_of_cell_voltage << 8;
    flash_block_data[1] = flash_update_of_cell_voltage & 0xff;
    
    for (uint8_t i = 0; i <= 1; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34fl_check_checksum(150);
}

void bq34fl_calibrate_sense_resistor(int16_t applied_current) {
    double current_array[50];
    for (uint8_t i = 0; i < 50; i++) {
        current_array[i] = current();
        delay(150);
    }
    double current_mean = 0;
    for (uint8_t i = 0; i < 50; i++) {
        current_mean += current_array[i];
    }
    current_mean /= 50.0;

    double current_sd = 0;
    for (uint8_t i = 0; i < 50; i++) {
        current_sd += pow(current_array[i] - current_mean, 2);
    }
    current_sd /= 50.0;
    current_sd = sqrt(current_sd);

    if (current_sd > 100) {
        return;
    }

    bq34_unsealed ();
    bq34fl_read_block(104, 0);

    uint32_t cc_gain = flash_block_data[0] << 24;
    cc_gain |= flash_block_data[1] << 16;
    cc_gain |= flash_block_data[2] << 8;
    cc_gain |= flash_block_data[3];
    
    double gain_resistence = 4.768 / xemics_to_double(cc_gain);

    double temp = (current_mean * gain_resistence) / (double)applied_current;

    uint32_t new_cc_gain = double_to_xemics(4.768 / temp);
    flash_block_data[0] = new_cc_gain >> 24;
    flash_block_data[1] = new_cc_gain >> 16;
    flash_block_data[2] = new_cc_gain >> 8;
    flash_block_data[3] = new_cc_gain & 0xff;

    new_cc_gain = double_to_xemics(5677445.6 / temp);
    flash_block_data[4] = new_cc_gain >> 24;
    flash_block_data[5] = new_cc_gain >> 16;
    flash_block_data[6] = new_cc_gain >> 8;
    flash_block_data[7] = new_cc_gain & 0xff;
    

    for (uint8_t i = 0; i <= 3; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }

    for (uint8_t i = 4; i <= 7; i++) {
        bq34_write_reg(0x40 + i, flash_block_data[i]);
    }
    bq34fl_check_checksum(150);
}