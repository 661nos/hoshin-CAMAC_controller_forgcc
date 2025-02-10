#pragma once
#include<windows.h>
#include"hecmusb.h"

//Loading functions from HECUSB3.dll 

HMODULE LoadDll(LPCTSTR dll_path){
	HMODULE temp_module = LoadLibrary(dll_path);
	if(temp_module == NULL){
		puts("Failed to load dll file");
		exit(1);
	}
	else puts("Dll file load success");
	return temp_module;
}

TUSBONLINE GetUsbOnline(HMODULE tmp){
	TUSBONLINE function_usbonline = (TUSBONLINE) GetProcAddress(tmp, "_USBONLINE@0"); 
	if(function_usbonline == NULL){
		puts("Failed to get USBONLINE function");
		exit(1);
	}
	else puts("Get USBONLINE function");
	return function_usbonline;
	
}

TCMINIT GetCamacInit(HMODULE tmp){
	TCMINIT function_camacinit = (TCMINIT) GetProcAddress(tmp, "_CMINIT@4");
	if(function_camacinit == NULL){
		puts("Failed to get CMINIT function");
		exit(1);
	}
	else 	
		puts("Get CMINIT function");
	return function_camacinit;
}

TCAMAC GetCamac(HMODULE tmp){
	//TCAMAC function_camac = GetProcAddress(tmp.hmodule, "_CAMAC@28");
	TCAMAC function_camac = (TCAMAC) GetProcAddress(tmp, "_CAMAC@28");
	if(function_camac == NULL){
		puts("Failed to initialize CAMAC function");
		exit(1);
	}
	else 	
		puts("Get CAMAC function");
	return function_camac;
}

TCLAM GetClam(HMODULE tmp){
	TCLAM function_clam = (TCLAM) GetProcAddress(tmp, "_CLAM@4");
	if(function_clam == NULL){
		puts("Failed to initialize CLAM function");
		exit(1);
	}
	else 	
		puts("Get CLAM function");
	return function_clam;
}

TCTLAM GetCtlam(HMODULE tmp){
	TCTLAM function_ctlam = (TCTLAM) GetProcAddress(tmp, "_CTLAM@8");
	if(function_ctlam == NULL){
		puts("Failed to initialize CTLAM function");
		exit(1);
	}
	else 	
		puts("Get CTLAM function");
	return function_ctlam;
}

TCMEND GetCmend(HMODULE tmp){
	TCMEND function_cmend = (TCMEND) GetProcAddress(tmp, "_CMEND@0");
	if(function_cmend == NULL){
		puts("Failed to initialize CMEND function");
		exit(1);
	}
	else 	
		puts("Get CMEND function");
	return function_cmend;
}
