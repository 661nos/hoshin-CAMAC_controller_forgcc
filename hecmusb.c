#pragma once
#include"hecmusb.h"
#include"loaddllfunction.c"
#include<windows.h>

// return camac status
// 7:REQ 6:DEM 5:o 4:o 3:LE 2:I 1:Q 0:X:
// if you want to know more, read the comment in header file

// Q:1 -> Yes, 0 -> No
// X:1 -> Yes, 0 -> No 
CAMAC_STATUS CamacStatus(BYTE NF){
	CAMAC_STATUS ret_camac_status;
	ret_camac_status.Q = NF%2;NF/=2;
	ret_camac_status.X = NF%2;NF/=2;
	ret_camac_status.I = NF%2;NF/=2;
	ret_camac_status.LE = NF%2;NF/=8;
	ret_camac_status.DEM = NF%2;NF/=2;
	ret_camac_status.REQ = NF%2;NF/=2;

	return ret_camac_status;
}

//initializing function about USB and CAMAC
//in rust, there was a bug about CAMAC initialization

CAMAC_STATUS InitFunc(CAMAC tmp){
	BYTE ret_usbonline = tmp.usbonline();
	if(ret_usbonline == 255){
		puts("Failed to USBONLINE");
		exit(1);
	}
	else
		puts("USBONLINE success");

	BYTE ret_camac_init = tmp.camac_init(tmp.crate_number);
	if(ret_camac_init == 255){
		puts("Failed to initialize CAMAC");
		exit(1);
	}
	else
		puts("Initializing CAMAC success");
	return CamacStatus(ret_camac_init);

}


CAMAC InitAll_and_GetControl(byte crate_number, LPCTSTR dll_path){
	//CAMAC camac_tmp;// = {0,0,0,0,0,0,0,0,{0,0,0,0,0,0}};
	HMODULE hmodule = LoadDll(dll_path);
	CAMAC camac_tmp = {
		crate_number,
		hmodule,
		GetUsbOnline(hmodule),
		GetCamacInit(hmodule),
		GetCamac(hmodule),
		GetClam(hmodule),
		GetCtlam(hmodule),
		GetCmend(hmodule)
	};
	//BYTE ret_c = CmamacInit(hmodule);
	//Update_CAMAC_status(hmodule);
	//
	
	
	camac_tmp.status = InitFunc(camac_tmp);
	
	puts("initialized all\n\n");
	
	return camac_tmp;
}
