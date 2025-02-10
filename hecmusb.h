#pragma once
#include<stdio.h.>
#include<windows.h>
//#include<cstdint>

// Define types and a structure to handling CAMAC
// 


//Type Definition

typedef BYTE (* TUSBONLINE)();
typedef BYTE (* TCMINIT)(byte);
typedef unsigned long long (* TCAMAC)(byte, byte, byte, byte, byte, byte, byte);
typedef unsigned long long (* TCLAM)(byte);
typedef BYTE (* TCTLAM)(byte, byte);
typedef void (* TCMEND)();


// Q and X are most important, while others are not so much important
// Q is signal from a module you handling, if Q=1 then there is something, Q=0 there nothing
// X is also signal from a module and it means if X=1 the module is capable for commands, X=0 then it is not

typedef struct{
	BYTE REQ;
	BYTE DEM;
	BYTE LE;
	BYTE I;
	BYTE X;
	BYTE Q;
} CAMAC_STATUS;


typedef struct{
	//fundamental infomation about camac

	byte crate_number; //crate_number should be 1<= crate_number <=7
	HMODULE hmodule; //hmodule is something like dll file path

	//functions about camac
	//if you want to know about more, read ome introduction PDF about CCP-USB

	
	// initialization 
	TUSBONLINE usbonline;
	TCMINIT camac_init;


	//communication to module via controller
	TCAMAC camac;

	// smallest number of module whose LAM is set
	TCLAM check_lam;

	// tells the module's LAM is set or not and the module is capable or not
	TCTLAM test_lam;

	// close camac
	TCMEND camac_end;

	// status from camac's function and it means vary
	CAMAC_STATUS status;

} CAMAC;


// read data from camac module
typedef struct{
	int low;// low byte data
	int middle;// middle byte data
	int high;// high byte data
} MODULE_DATA;



//Function Prototype declaration

HMODULE LoadDLL(LPCTSTR dll_path);
TUSBONLINE GetUsbOnline(HMODULE);
TCMINIT GetCamacInit(HMODULE);
TCAMAC GetCamac(HMODULE);
TCLAM GetClam(HMODULE);
TCMEND GetCmend(HMODULE);
CAMAC_STATUS CamacStatus(byte);

CAMAC_STATUS InitFunc(CAMAC tmp);
MODULE_DATA CommunicateCamac(CAMAC *, BYTE, BYTE, BYTE, int);
byte Smallest_Lam(CAMAC *);
int Test_Zero_Lam(CAMAC *, BYTE);
MODULE_DATA Read_Register(CAMAC *, BYTE, BYTE);
MODULE_DATA Read_And_Clear_Register(CAMAC *, BYTE, BYTE);
void Clear_Register(CAMAC *, BYTE, BYTE);
int Test_Lam(CAMAC *, BYTE, BYTE);
void Clear_Lam(CAMAC *, BYTE, BYTE);
MODULE_DATA Wait_Lam_Read_Clear(CAMAC*, BYTE, BYTE);
void status_print(CAMAC);









