REM  ===================================================================
REM  run_sys.cmd
REM
REM  This is a diag21k script file that can be run to test the 
REM  multiprocessor example using link port transfers on a Hammerhead board.
REM  The test involves 3 processors:
REM
REM  PROC1 - The generator of the data, paced by a timer to send data packets
REM          via link to PROC2, and via MMS DAM transfer to PROC3.  PROC1 also
REM          initializes the SDRAM source data by writing an incrementing pattern
REM          before starting the transfers.
REM  PROC2 - The receiver of (a portion of) PROC1's data via link ports.
REM  PROC2 - The receiver of (a portion of) PROC1's data via link ports.
REM
REM  At a DOS prompt with executables HH_GENERATOR.DXE and HH_CONSUMER.DXE in 
REM  the local directory MYDIR, type:
REM
REM    C:\MYDIR\>diag21k -xrun_sys
REM
REM  PROVIDED BY:
REM  ------------
REM  BittWare, Inc.
REM  33 North Main Street
REM  Concord, NH  03301
REM  Ph:  603-226-0404
REM  Fax: 603-226-6667
REM  WWW: www.bittware.com
REM  E-mail: support@bittware.com
REM
REM  Copyright (c) 2001
REM  ===================================================================


rem -- reset board, select proc, configure processor
br
bs 3
br
pc

rem -- clear an area of internal memory --
echo
echo Clearing internal mem buffer on PROC3
mw ih 0x58000 0 0x1000
os pause

REM --------------------------------------------
REM Load and start PROC2, consumer
REM --------------------------------------------
bs 2
pc
fl debug\hh2_consumer.dxe
mw li _Frame_Size 32
ps

REM --------------------------------------------
REM Load and start PROC4, consumer
REM --------------------------------------------
bs 4
pc
fl debug\hh4_consumer.dxe
mw li _Frame_Size 32
ps

REM --------------------------------------------
REM Load and start PROC1, the generator
REM --------------------------------------------
bs 1
pc
fl debug\hh1_generator.dxe
mw li _Line_Freq 5000
mw li _Frame_Size 32
ps

REM --------------------------------------------
REM Monitor PROC1 for completion
REM --------------------------------------------
echo Image ready?
os pause
mw li _input 1

echo Image ready?
os pause
mw li _input 1

echo Hit a key to stop:
os pause
mw li _done 1

REM --------------------------------------------
REM Dump PROC1 status variables
REM --------------------------------------------
bs 1
echo PROC1 variables offset and done:
mr lh _offset
mr li _frame_cnt
os pause

REM --------------------------------------------
REM View PROC2 data
REM --------------------------------------------
bs 4
echo Link data on PROC2. beginning of rcvd_data[0]:
mr lh _rcvd_data 20
echo For end of rcvd_data[1],
os pause
mr lh _rcvd_data 0x2af
os pause
bs 2
echo Processing Times:
mr lh _stat 16
os pause

REM --------------------------------------------
REM Check DMA metrics
REM --------------------------------------------
bs 1
echo STAT (TIMER INTR/DMAC10/DMAC11/LINK2/LINK3):
mr lh _stat 30

rem -- exit diag21k without a reset --
x
