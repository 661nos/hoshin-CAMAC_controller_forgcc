#pragma once
#include<stdio.h>
#include"hecmusb.h"
#include"hecmusb.c"

//define useful subroutine, because the raw function of dll file is not useful


// communicate to modules and return module's data

MODULE_DATA CommunicateCamac(CAMAC *camac_tmp, byte slot_number, byte subaddress, byte function_number, int write_data){
	// slot_number is the number of module you want to handle
	// subaddress is the number of port of the module you want to access
	// function_nuber is the common signals of functions 
	//	the 7th digit of function_number is C(clear)
	//	the 6th digit of function_number is Z(initialize)
	//	other digit < 5 , from 0 to 32 is the CAMAC function code
	// write_data is a data you want to send to module
	//	write_data will use only in the case 16 <= function_number <=31

	byte DL = write_data%256;write_data/=256;
	byte DM = write_data%256;write_data/=256;
	byte DH = write_data%256;write_data/=256;
	
	asm("sub $0x1c, %esp");
	unsigned long long ret_communication = (unsigned long long) camac_tmp->camac(camac_tmp->crate_number, slot_number, subaddress, function_number, DL, DM, DH);

	MODULE_DATA ret_module_data = {0, 0, 0};

	int ret_ST = ret_communication%256;ret_communication/=256;

	ret_module_data.low = ret_communication%256;ret_communication/=256;
	ret_module_data.middle = ret_communication%256;ret_communication/=256;
	ret_module_data.high = ret_communication%256;ret_communication/=256;
	
	camac_tmp->status = CamacStatus(ret_ST);
	//printf("%d,%d,%d\n", ret_module_data.low, ret_module_data.middle, ret_module_data.high);

	return ret_module_data;

}


// returns the smallest number of module whose LAM is set

byte Smallest_Lam(CAMAC *camac_tmp){
	asm("sub $0x4, %esp");
	unsigned int ret_check_lam = (unsigned int) camac_tmp->check_lam(camac_tmp->crate_number);

	int ret_ST = ret_check_lam%256;ret_check_lam/=256;
	byte ret_smallest = ret_check_lam%256;ret_check_lam/=256;
	
	camac_tmp->status = CamacStatus(ret_ST);

	return ret_smallest;

}

// returns the module's subaddress 0 LAM is set or not, and is capable of or not
// if LAM is set and the module is capable of, then return 1
// otherwise, return 0
int Test_Zero_Lam(CAMAC *camac_tmp, byte slot_number){
	asm("sub $0x8, %esp");
	byte ret_test_lam = camac_tmp->test_lam(camac_tmp->crate_number, slot_number);


	int ret_ST = ret_test_lam%256;ret_test_lam/=256;
	camac_tmp->status = CamacStatus(ret_ST);
	
	return (camac_tmp->status).Q * (camac_tmp->status).X;
}



// returns register
MODULE_DATA Read_Register(CAMAC *camac_tmp, byte slot_number, byte subaddress){
	MODULE_DATA ret_register = CommunicateCamac(camac_tmp, slot_number, subaddress, 0, 0);

	return ret_register;
}

// returns register and clear it
MODULE_DATA Read_And_Clear_Register(CAMAC *camac_tmp, byte slot_number, byte subaddress){
	MODULE_DATA ret_register = CommunicateCamac(camac_tmp, slot_number, subaddress, 2, 0);

	return ret_register;
}

// clear register
void Clear_Register(CAMAC *camac_tmp, byte slot_number, byte subaddress){
	CommunicateCamac(camac_tmp, slot_number, subaddress, 9, 0);
}


// test LAM
// returns the module's LAM is set or not, and is capable of or not
// if LAM is set and the module is capable of, then return 1
// otherwise, return 0
int Test_Lam(CAMAC *camac_tmp, BYTE slot_number, BYTE subaddress){
	MODULE_DATA ret_test_lam = CommunicateCamac(camac_tmp, slot_number, subaddress, 8, 0);
	//printf("%d\n",camac_tmp->status.X);

	return camac_tmp->status.Q * camac_tmp->status.X;
}


// clear LAM
void Clear_Lam(CAMAC *camac_tmp, byte slot_number, byte subaddress){
	CommunicateCamac(camac_tmp, slot_number, subaddress, 10, 0);
}

void CloseCamac(CAMAC *camac_tmp){
	camac_tmp->camac_end();
	puts("CAMAC CLOSED");
}

// wait till the LAM is set
// if the LAM is set and is capable, then read the register and clear it, then clear LAM
// returns register if LAM is set
MODULE_DATA Wait_Lam_Read_Clear(CAMAC *camac_tmp, byte slot_number, byte subaddress){
	//int i=0;
	while(1){
		//printf("i:%d\n", i++);
		if(Test_Lam(camac_tmp, slot_number, subaddress) == 1)break;
	}

	MODULE_DATA ret_register = Read_And_Clear_Register(camac_tmp, slot_number, subaddress);
	//MODULE_DATA ret_register = Read_Register(camac_tmp, slot_number, subaddress);
	Clear_Lam(camac_tmp, slot_number, subaddress);
	Clear_Register(camac_tmp, slot_number, subaddress);

	return ret_register;
}

MODULE_DATA Read(CAMAC *camac_tmp, byte slot_number, byte subaddress){
	MODULE_DATA ret_register = Read_And_Clear_Register(camac_tmp, slot_number, subaddress);
	//MODULE_DATA ret_register = Read_Register(camac_tmp, slot_number, subaddress);

	return ret_register;
}



void status_print(CAMAC temp){
	printf("%d,%d,%d,%d,%d,\n",temp.status.Q,temp.status.X,temp.status.I,temp.status.LE,temp.status.DEM,temp.status.REQ);
	

}


unsigned int Value_Register(MODULE_DATA mod_tmp){
	unsigned int ret=0;
	ret += mod_tmp.high;
	ret *= 256;
	ret += mod_tmp.middle;
	ret *= 256;
	ret += mod_tmp.low;

	return ret;
}



unsigned int chars_to_second(char *tmp){
	unsigned int ret = 0;
	for(int i=0;tmp[i]!=0;i++){
		ret *=10;
		ret += tmp[i]-48;
	}

	return ret*60;

}



