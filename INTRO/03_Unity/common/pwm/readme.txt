Code to control IrDA Comms:

    - Beacons: BTx (ATx), BCx (ACx)
    - Messages: MSx (ASx), MRx (ARx)

irda_pic.c - Tested IRDA functions:

    (test_IRDA_Add_Beacon_BT);
    (test_IRDA_Add_Beacon_BC);
    (test_IRDA_Add_BeaconAndMessage_MS);
    (test_IRDA_Add_BeaconAndMessage_MR);

irda_dummy.c - Tested different scenarios:

    Beacons received from the IrDA Pic:
    \nBTx CRC\r         argc = 2            \nMSx CRC\r         argc = 2
    \nBCx 1234 CRC\r    argc = 3            \nMRx 1234 CRC\r    argc = 3

    Acknowledges them and gives an answer:
    \nATx 1234 CRC\r    argc = 4            \nASx 1234 CRC\r    argc = 4
    \nACx 1234 CRC\r    argc = 4            \nARx 1234 CRC\r    argc = 4

irda_ack.c - Tested Dummy Pic's answer and report to the UART0:

    "IRDA_Uart0Message: ACx, ATx, ASx, ARx "
        (test_IRDA_Uart0Message_AT_ResultOk);
        (test_IRDA_Uart0Message_AC_ResultOk);
        (test_IRDA_Uart0Message_AS_ResultOk);
        (test_IRDA_Uart0Message_AR_ResultOk);

    "BeaconPrecheck "
        (test_IRDA_BeaconPrecheck_OldId);
        (test_IRDA_BeaconPrecheck_IdFFFF);
        (test_IRDA_BeaconPrecheck_NullStatus);

    "IRDA_UpdateTable & IRDA_ConfirmID "
        (test_IRDA_UpdateTable_FourIterationsAndCalibrated);
        (test_IRDA_ConfirmID_irdaStable_ResultOk);

test_base.c - Tested ID of the blades & blade detection:
    
    (test_BLADE_WriteReadState);
    (test_HB_ConsecutiveBeacons_Calibrated);

pwm_functions.c - Tested the format of PWM commands:

    {"SET_SPEED",   Set_Speed},             // FAN Speed
    {"GET_SPEED",   Get_Speed},             // 
    {"RMT_MR",      Set_Addr_BeaconMR},     // Addresses
    {"RMT_MS",      Set_Addr_BeaconMS},    
    {"SET_POWER",   Set_Power},             // IRDA Tx Level
    {"GET_POWER",   Get_Power},             //
    {"LCL_MR",      Set_Ch_BeaconMR},       // Channels
    {"LCL_MS",      Set_Ch_BeaconMS},  

