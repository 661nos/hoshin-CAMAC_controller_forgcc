#include<stdio.h>
#include<windows.h>
#include<time.h>
#include"hecmusb.h"
#include"hecmusb.c"
#include"usefulsubroutine.c"

int main(int argc, char** argv){
	//argv[1] = save folder name
	//argv[2] = measure minute
	if(argc != 3){
		puts("FOLDER NAME AND MEASURE TIME");
		exit(1);
	}

	char *foldername = argv[1];
	unsigned int me_time_in_sec = chars_to_second(argv[2]);

	CAMAC temp = InitAll_and_GetControl(1, "HECMUSB3.dll");
	
	
	time_t start_time;
	time_t now_time;
	
	time(&start_time);
	time(&now_time);
	
	//puts("OK1");
	char file1[100], file2[100];
	sprintf(file1,".\\%s\\PMT1.txt",foldername);
	sprintf(file2,".\\%s\\PMT2.txt",foldername);
	//sprintf(filename, ".\\measurement_data\\%s",argv[1]);
	printf("START TIME:%s\n", ctime(&start_time));
	while(now_time - start_time < me_time_in_sec){
		time(&now_time);
		MODULE_DATA data1, data2;
		if(Test_Lam(&temp, 22, 0)*Test_Lam(&temp, 22, 15)){
			data1 = Read_And_Clear_Register(&temp, 22, 0);
			data2 = Read_And_Clear_Register(&temp, 22, 15);
		//printf("%d\n", Value_Register(data1));
		//printf("%d\n", Value_Register(data2));
	
		FILE *fp;
		fp = fopen(file1, "a");
		fprintf(fp, "%d\n", Value_Register(data1));
		fclose(fp);
		
		fp = fopen(file2, "a");
		fprintf(fp, "%d\n", Value_Register(data2));
		fclose(fp);

		Clear_Register(&temp, 22, 0);
		Clear_Lam(&temp, 22, 0);
		Clear_Register(&temp, 22, 15);
		Clear_Lam(&temp, 22, 15);
		}
		//MODULE_DATA data = Wait_Lam_Read_Clear(&temp, 22, 0);

	}



	//(*temp.test_lam) (temp.crate_number, 22);

	CloseCamac(&temp);
	
	return 0;
}
