/*********************************************************
 *
 *
 *                                                       *
 *********************************************************/
// main_Census_Sequence.cpp

// STL:

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
// OpenCV:
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
// Eigene:
#include "../Library/global_defines_UOC.h"
#include "../Library/Optical_Flow/storage64b.h"
#include "../Library/config_file_handle/config_file_handle.h"
#include "../Library/Optical_Flow/CensusFlow_vs2.h"
//#include "Library/global_defines_UOC.h"
//#include "Library/Optical_Flow/storage64b.h"
//#include "Library/config_file_handle/config_file_handle.h"
//#include "Library/Optical_Flow/CensusFlow_vs2.h"
using namespace std;

#define REG_WRITE32(addr, off, val) (*(volatile unsigned int*)(addr+off)=(val))
#define REG_READ32(addr,off) (*(volatile unsigned int*)(addr+off))
#define REG_WRITE8(addr, off, val) (*(volatile uint8_t*)(addr+off)=(val))
#define REG_READ8(addr, off) (*(volatile uint8_t*)(addr+off))
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

//define the address for CE_TOP
#define CE_TOP_BASEADDR	0x6F800000
#define CE_TOP_REG_LENGTH 0x48 //72 bytes

#define CE_REG_CONTROL_OFFSET	0x00
#define CE_MASK_START	0x01
#define CE_MASK_DONE	0x02
#define CE_MASK_IDLE	0x04
#define CE_MASK_READY	0x08
#define CE_MASK_AUTORESTART	0x80
#define CE_REG_GLOBAL_INTR_EN_OFFSET	0x04
#define CE_MASK_GLOBAL_INTR_EN	0x01
#define CE_REG_IP_INTER_EN_OFFSET	0x08
#define CE_MASK_DONE_INTR_EN	0x01
#define CE_MASK_READY_INTR_EN	0x02
#define CE_REG_IP_INTR_STATUS_OFFSET	0x0c
#define CE_MASK_DONE_INTR_STATUS	0x01
#define CE_MASK_READY_INTR_STATUS	0x02
#define CE_REG_WIDTH_OFFSET	0x14
#define CE_REG_HEIGHT_OFFSET	0x1c
#define CE_REG_CSD1_OFFSET	0x24
#define CE_REG_CSD2_OFFSET	0x2c
#define CE_REG_EPSILON_OFFSET	0x34
#define CE_REG_READADDRESS_OFFSET	0x3c
#define CE_REG_WRITEADDRESS_OFFSET	0x44

//define the address for ME_TOP
#define ME_TOP_BASEADDR	0x7D600000
#define ME_TOP_REG_LENGTH 0x3c //60 bytes

#define ME_REG_CONTROL_OFFSET	0x00
#define ME_MASK_START	0x01
#define ME_MASK_DONE	0x02
#define ME_MASK_IDLE	0x04
#define ME_MASK_READY	0x08
#define ME_MASK_AUTORESTART	0x80
#define ME_REG_GLOBAL_INTR_EN_OFFSET	0x04
#define ME_MASK_GLOBAL_INTR_EN	0x01
#define ME_REG_IP_INTER_EN_OFFSET	0x08
#define ME_MASK_DONE_INTR_EN	0x01
#define ME_MASK_READY_INTR_EN	0x02
#define ME_REG_IP_INTR_STATUS_OFFSET	0x0c
#define ME_MASK_DONE_INTR_STATUS	0x01
#define ME_MASK_READY_INTR_STATUS	0x02
#define ME_REG_WIDTH_OFFSET	0x1c
#define ME_REG_HEIGHT_OFFSET	0x14
#define ME_REG_READADDRESS_1_OFFSET	0x24
#define ME_REG_READADDRESS_2_OFFSET	0x2C
#define ME_REG_WRITEADDRESS_OFFSET	0x34
#define ME_REG_RETURN_OFFSET	0x38

//define the address for EEE_TOP
#define EEE_TOP_BASEADDR	0x68E00000
#define EEE_TOP_REG_LENGTH 0x50 //80 bytes

#define EEE_REG_CONTROL_OFFSET	0x00
#define EEE_MASK_START	0x01
#define EEE_MASK_DONE	0x02
#define EEE_MASK_IDLE	0x04
#define EEE_MASK_READY	0x08
#define EEE_MASK_AUTORESTART	0x80
#define EEE_REG_GLOBAL_INTR_EN_OFFSET	0x04
#define EEE_MASK_GLOBAL_INTR_EN	0x01
#define EEE_REG_IP_INTER_EN_OFFSET	0x08
#define EEE_MASK_DONE_INTR_EN	0x01
#define EEE_MASK_READY_INTR_EN	0x02
#define EEE_REG_IP_INTR_STATUS_OFFSET	0x0c
#define EEE_MASK_DONE_INTR_STATUS	0x01
#define EEE_MASK_READY_INTR_STATUS	0x02
#define EEE_REG_MATCHCOUNT_OFFSET	0x14
#define EEE_REG_FADDR_OFFSET	0x1C
#define EEE_REG_VECOUTADDR_OFFSET	0x24
#define EEE_REG_MODELADDR_OFFSET	0x2C
#define EEE_REG_MODELCOUNT_OFFSET	0x34
#define EEE_REG_ERRTHRES_OFFSET	0x3C
#define EEE_REG_RESULTADDR_OFFSET	0x44
#define EEE_REG_MODE_OFFSET	0x4C

//define the address for ECE_TOP
#define ECE_TOP_BASEADDR	0x68E20000
#define ECE_TOP_REG_LENGTH 0x3c //60 bytes

#define ECE_REG_CONTROL_OFFSET	0x00
#define ECE_MASK_START	0x01
#define ECE_MASK_DONE	0x02
#define ECE_MASK_IDLE	0x04
#define ECE_MASK_READY	0x08
#define ECE_MASK_AUTORESTART	0x80
#define ECE_REG_GLOBAL_INTR_EN_OFFSET	0x04
#define ECE_MASK_GLOBAL_INTR_EN	0x01
#define ECE_REG_IP_INTER_EN_OFFSET	0x08
#define ECE_MASK_DONE_INTR_EN	0x01
#define ECE_MASK_READY_INTR_EN	0x02
#define ECE_REG_IP_INTR_STATUS_OFFSET	0x0c
#define ECE_MASK_DONE_INTR_STATUS	0x01
#define ECE_MASK_READY_INTR_STATUS	0x02
#define ECE_REG_BESTMODELADDR_OFFSET	0x14
#define ECE_REG_FULLVECTORADDR_OFFSET	0x1C
#define ECE_REG_VECCOUNT_OFFSET	0x24
#define ECE_REG_RESULTVECTORADDR_OFFSET	0x2C
#define ECE_REG_ERRTHRES_OFFSET	0x34
#define ECE_REG_RETURN_OFFSET	0x38

//define for general settings
#define IMG_WIDTH	640
#define IMG_HEIGHT	480
#define CSD1	3
#define CSD2	7
#define EPSILON	20
#define THRESHOLD 2
#define CONVERGTHRESHOLD 0.75f
#define READ_ADDR	0x1F000000
#define WRITE_ADDR_0	0x1E200000
#define WRITE_ADDR_1	0x1F300000
#define FEATURE_ADDR	0x1F100000
#define COMPRESS_VECOTR_ADDR	0x1F300000
#define MODEL_ADDR	0x1F400000
#define BEST_MODEL_ADDR 0x1F500000
#define RESULT_ADDR	0x1F600000
#define RESULT_VEC_ADDR	0x1F700000

/*-------------------------------------------------------------------------
* Hardware Initialization
-------------------------------------------------------------------------*/
int initHardware(unsigned long phyaddress){

	unsigned long int PhysicalAddress = phyaddress;
	int map_len;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* base_address;
	
	if ((PhysicalAddress != CE_TOP_BASEADDR) && (PhysicalAddress != ME_TOP_BASEADDR) && (PhysicalAddress != EEE_TOP_BASEADDR) && (PhysicalAddress != ECE_TOP_BASEADDR)){
		perror("invalid hardware!!\n");
		return 1;
	}

	switch (PhysicalAddress){
		case CE_TOP_BASEADDR:{
			map_len = CE_TOP_REG_LENGTH;
			break;
		}
		case ME_TOP_BASEADDR:{	
			map_len = ME_TOP_REG_LENGTH;
			break;
		}
		case EEE_TOP_BASEADDR:{	
			map_len = EEE_TOP_REG_LENGTH;
			break;
		}
		case ECE_TOP_BASEADDR:{	
			map_len = ECE_TOP_REG_LENGTH;
			break;
		}
	}
	
	base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(base_address == MAP_FAILED)
	{
		switch (PhysicalAddress){
			case CE_TOP_BASEADDR:{
				perror("ce_top Mapping memory for absolute memory access failed.\n");
				break;
			}
			case ME_TOP_BASEADDR:{	
				perror("me_top Mapping memory for absolute memory access failed.\n");
				break;
			}
			case EEE_TOP_BASEADDR:{	
				perror("eee_top Mapping memory for absolute memory access failed.\n");
				break;
			}
			case ECE_TOP_BASEADDR:{	
				perror("ece_top Mapping memory for absolute memory access failed.\n");
				break;
			}
		}
		return 1;
	}

	switch (PhysicalAddress){
		case CE_TOP_BASEADDR:{
			//set width
			REG_WRITE32(base_address,CE_REG_WIDTH_OFFSET,IMG_WIDTH);

			//set height
			REG_WRITE32(base_address,CE_REG_HEIGHT_OFFSET,IMG_HEIGHT);

			//set csd1
			REG_WRITE32(base_address,CE_REG_CSD1_OFFSET,CSD1);

			//set csd2
			REG_WRITE32(base_address,CE_REG_CSD2_OFFSET,CSD2);

			//set epsilon
			REG_WRITE32(base_address,CE_REG_EPSILON_OFFSET,EPSILON);
			
			//set read address
			REG_WRITE32(base_address,CE_REG_READADDRESS_OFFSET,READ_ADDR);
			
			break;
		}
		case ME_TOP_BASEADDR:{	
			//set width
			REG_WRITE32(base_address,ME_REG_WIDTH_OFFSET,IMG_WIDTH);

			//set height
			REG_WRITE32(base_address,ME_REG_HEIGHT_OFFSET,IMG_HEIGHT);
			
			//set read address 1
			REG_WRITE32(base_address,ME_REG_READADDRESS_1_OFFSET,WRITE_ADDR_0);
			
			//set read address 2
			REG_WRITE32(base_address,ME_REG_READADDRESS_2_OFFSET,WRITE_ADDR_1);
			
			//set write address
			REG_WRITE32(base_address,ME_REG_WRITEADDRESS_OFFSET,FEATURE_ADDR);
			
			break;
		}
		case EEE_TOP_BASEADDR:{	
			//set feature input
			REG_WRITE32(base_address,EEE_REG_FADDR_OFFSET,FEATURE_ADDR);
			
			//set compressed vector output
			REG_WRITE32(base_address,EEE_REG_VECOUTADDR_OFFSET,COMPRESS_VECOTR_ADDR);
			
			//set model input
			REG_WRITE32(base_address,EEE_REG_MODELADDR_OFFSET,MODEL_ADDR);
			
			//set result address
			REG_WRITE32(base_address,EEE_REG_RESULTADDR_OFFSET,RESULT_ADDR);
			
			break;
		}
		case ECE_TOP_BASEADDR:{	
			//set best model input
			REG_WRITE32(base_address,ECE_REG_BESTMODELADDR_OFFSET,BEST_MODEL_ADDR);
			
			//set full vectors input
			REG_WRITE32(base_address,ECE_REG_FULLVECTORADDR_OFFSET,FEATURE_ADDR);
			
			//set result vectors input
			REG_WRITE32(base_address,ECE_REG_RESULTVECTORADDR_OFFSET,RESULT_VEC_ADDR);
			
			break;
		}
	}
	
	munmap((void *)base_address, map_len);
	close(fd);
	
	switch (PhysicalAddress){
		case CE_TOP_BASEADDR:{
			printf("hardware ce initialization successful!!\n");
			break;
		}
		case ME_TOP_BASEADDR:{	
			printf("hardware me initialization successful!!\n");
			break;
		}
		case EEE_TOP_BASEADDR:{	
			printf("hardware eee initialization successful!!\n");
			break;
		}
		case ECE_TOP_BASEADDR:{	
			printf("hardware ece initialization successful!!\n");
			break;
		}
	}

	return 0;
}

/*-------------------------------------------------------------------------
* Hardware Access functions
-------------------------------------------------------------------------*/
void setCETopWriteAddr(unsigned int writeaddr){

	unsigned long int PhysicalAddress = CE_TOP_BASEADDR;
	int map_len = CE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ce_base_address;

	ce_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ce_base_address == MAP_FAILED)
	{
		perror("ce_top Mapping memory for absolute memory access failed.\n");
		return;
	}

	REG_WRITE32(ce_base_address,CE_REG_WRITEADDRESS_OFFSET,writeaddr);

	munmap((void *)ce_base_address, map_len);
	close(fd);
}

unsigned int getMEResult(){

	unsigned long int PhysicalAddress = ME_TOP_BASEADDR;
	int map_len = ME_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* me_base_address;
	unsigned int reg;

	me_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(me_base_address == MAP_FAILED)
	{
		perror("me_top Mapping memory for absolute memory access failed.\n");
		return 0;
	}
	
	reg = REG_READ32(me_base_address,ME_REG_RETURN_OFFSET);
	munmap((void *)me_base_address, map_len);
	close(fd);
	
	return reg;
}

void setEEEMatchcount(unsigned int value){

	unsigned long int PhysicalAddress = EEE_TOP_BASEADDR;
	int map_len = EEE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* eee_base_address;

	eee_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(eee_base_address == MAP_FAILED)
	{
		perror("eee_top Mapping memory for absolute memory access failed.\n");
		return;
	}
	
	REG_WRITE32(eee_base_address,EEE_REG_MATCHCOUNT_OFFSET,value);
	
	munmap((void *)eee_base_address, map_len);
	close(fd);
}

void setEEEModelcount(unsigned int value){

	unsigned long int PhysicalAddress = EEE_TOP_BASEADDR;
	int map_len = EEE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* eee_base_address;

	eee_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(eee_base_address == MAP_FAILED)
	{
		perror("eee_top Mapping memory for absolute memory access failed.\n");
		return;
	}
	
	REG_WRITE32(eee_base_address,EEE_REG_MODELCOUNT_OFFSET,value);
	
	munmap((void *)eee_base_address, map_len);
	close(fd);
}

void setEEEThreshold(unsigned int value){

	unsigned long int PhysicalAddress = EEE_TOP_BASEADDR;
	int map_len = EEE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* eee_base_address;

	eee_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(eee_base_address == MAP_FAILED)
	{
		perror("eee_top Mapping memory for absolute memory access failed.\n");
		return;
	}
	
	REG_WRITE32(eee_base_address,EEE_REG_ERRTHRES_OFFSET,value);
	
	munmap((void *)eee_base_address, map_len);
	close(fd);
}

void setEEEMode(unsigned int value){

	unsigned long int PhysicalAddress = EEE_TOP_BASEADDR;
	int map_len = EEE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* eee_base_address;

	eee_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(eee_base_address == MAP_FAILED)
	{
		perror("eee_top Mapping memory for absolute memory access failed.\n");
		return;
	}
	
	REG_WRITE32(eee_base_address,EEE_REG_MODE_OFFSET,value);
	
	munmap((void *)eee_base_address, map_len);
	close(fd);
}

void getEEEresult(uint32_t* result,float* model_param_est){
	unsigned long int PhysicalAddress = RESULT_ADDR;
	int map_len = 8;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* virtual_addr_in;
	unsigned int Reg;

	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);
	
	if(virtual_addr_in== MAP_FAILED)
	{
		perror("get eee result mapping memory for absolute memory access failed.\n");
		return;
	}
	
	Reg = REG_READ32(virtual_addr_in,0);
	result[0] = Reg>>16; //valid vector number
	result[1] = Reg & 0xffff;//best outlier count
	result[2] = REG_READ32(virtual_addr_in,4);// model id
	
	munmap((void *)virtual_addr_in , map_len);
	close(fd);
	
	PhysicalAddress = MODEL_ADDR;
	map_len = 4800;
	fd = open( "/dev/mem", O_RDWR);
	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);
	
	if(virtual_addr_in== MAP_FAILED)
	{
		perror("get eee result mapping memory for absolute memory access failed.\n");
		return;
	}
	
	model_param_est[0] = ((float) REG_READ32(virtual_addr_in,result[2]*16))/ 8.0;
	model_param_est[1] = ((float) REG_READ32(virtual_addr_in,result[2]*16+4)) / 8.0;
	model_param_est[2] = ((float) REG_READ32(virtual_addr_in,result[2]*16+8)) / 2147483648.0;
	model_param_est[3] = ((float) REG_READ32(virtual_addr_in,result[2]*16+12))/ 2147483648.0;
	
	printf(" XC: %f \n\r",model_param_est[0]);
	printf(" YC: %f \n\r",model_param_est[1]);
	printf(" D : %f \n\r",model_param_est[2]);
	printf(" R : %f \n\r",model_param_est[3]);
	
	printf(" Total iterations : %d \n\r", 300);
	printf(" Outlier count    : %d \n\r", result[1]);
	printf(" Total count      : %d \n\r", result[0]);
	printf(" Outlier Ratio    : %f \n\r",
			((float) result[1]) / ((float) result[0]));
			
	munmap((void *)virtual_addr_in , map_len);
	close(fd);
}
void setECEBestmodel(float* model_param_est){
	unsigned long int PhysicalAddress = BEST_MODEL_ADDR;
	int map_len = 16;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* virtual_addr_in;
	
	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);
	
	if(virtual_addr_in== MAP_FAILED)
	{
		perror("set ece best model mapping memory for absolute memory access failed.\n");
		return;
	}
	REG_WRITE32(virtual_addr_in,0,(uint32_t)(model_param_est[0]*8));
	REG_WRITE32(virtual_addr_in,4,(uint32_t)(model_param_est[1]*8));
	REG_WRITE32(virtual_addr_in,8,(uint32_t)(model_param_est[2]*2147483648));
	REG_WRITE32(virtual_addr_in,12,(uint32_t)(model_param_est[3]*2147483648));
	
	munmap((void *)virtual_addr_in , map_len);
	close(fd);
}
void setECEVectorcount(unsigned int value){

	unsigned long int PhysicalAddress = ECE_TOP_BASEADDR;
	int map_len = ECE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ece_base_address;

	ece_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ece_base_address == MAP_FAILED)
	{
		perror("ece_top Mapping memory for absolute memory access failed.\n");
		return;
	}
	
	REG_WRITE32(ece_base_address,ECE_REG_VECCOUNT_OFFSET,value);
	
	munmap((void *)ece_base_address, map_len);
	close(fd);
}

void setECEThreshold(unsigned int value){

	unsigned long int PhysicalAddress = ECE_TOP_BASEADDR;
	int map_len = ECE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ece_base_address;

	ece_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ece_base_address == MAP_FAILED)
	{
		perror("ece_top Mapping memory for absolute memory access failed.\n");
		return;
	}
	
	REG_WRITE32(ece_base_address,ECE_REG_ERRTHRES_OFFSET,value);
	
	munmap((void *)ece_base_address, map_len);
	close(fd);
}

unsigned int getECEResult(){

	unsigned long int PhysicalAddress = ECE_TOP_BASEADDR;
	int map_len = ECE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ece_base_address;
	unsigned int reg;

	ece_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ece_base_address == MAP_FAILED)
	{
		perror("ece_top Mapping memory for absolute memory access failed.\n");
		return 0;
	}
	
	reg = REG_READ32(ece_base_address,ECE_REG_RETURN_OFFSET);
	munmap((void *)ece_base_address, map_len);
	close(fd);
	
	return reg;
}

/*-------------------------------------------------------------------------
* Hardware Control functions
-------------------------------------------------------------------------*/
void startCETop(){

	unsigned long int PhysicalAddress = CE_TOP_BASEADDR;
	int map_len = CE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ce_base_address;
	unsigned int Reg;

	ce_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ce_base_address == MAP_FAILED)
	{
		perror("ce_top Mapping memory for absolute memory access failed.\n");
		return ;
	}

	Reg = REG_READ32(ce_base_address,CE_REG_CONTROL_OFFSET) & CE_MASK_AUTORESTART;
	REG_WRITE32(ce_base_address,CE_REG_CONTROL_OFFSET,Reg|CE_MASK_START);

	munmap((void *)ce_base_address, map_len);
	close(fd);
}

void startMETop(){

	unsigned long int PhysicalAddress = ME_TOP_BASEADDR;
	int map_len = ME_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* me_base_address;
	unsigned int Reg;

	me_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(me_base_address == MAP_FAILED)
	{
		perror("me_top Mapping memory for absolute memory access failed.\n");
		return ;
	}

	Reg = REG_READ32(me_base_address,ME_REG_CONTROL_OFFSET) & ME_MASK_AUTORESTART;
	REG_WRITE32(me_base_address,ME_REG_CONTROL_OFFSET,Reg|ME_MASK_START);

	munmap((void *)me_base_address, map_len);
	close(fd);
}

void startEEETop(){

	unsigned long int PhysicalAddress = EEE_TOP_BASEADDR;
	int map_len = EEE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* eee_base_address;
	unsigned int Reg;

	eee_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(eee_base_address == MAP_FAILED)
	{
		perror("eee_top Mapping memory for absolute memory access failed.\n");
		return ;
	}

	Reg = REG_READ32(eee_base_address,EEE_REG_CONTROL_OFFSET) & EEE_MASK_AUTORESTART;
	REG_WRITE32(eee_base_address,EEE_REG_CONTROL_OFFSET,Reg|EEE_MASK_START);

	munmap((void *)eee_base_address, map_len);
	close(fd);
}

void startECETop(){

	unsigned long int PhysicalAddress = ECE_TOP_BASEADDR;
	int map_len = ECE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ece_base_address;
	unsigned int Reg;

	ece_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ece_base_address == MAP_FAILED)
	{
		perror("ece_top Mapping memory for absolute memory access failed.\n");
		return ;
	}

	Reg = REG_READ32(ece_base_address,ECE_REG_CONTROL_OFFSET) & ECE_MASK_AUTORESTART;
	REG_WRITE32(ece_base_address,ECE_REG_CONTROL_OFFSET,Reg|ECE_MASK_START);

	munmap((void *)ece_base_address, map_len);
	close(fd);
}

/*-------------------------------------------------------------------------
* Hardware Status detection functions
-------------------------------------------------------------------------*/
unsigned int isCEReady(){
	unsigned long int PhysicalAddress = CE_TOP_BASEADDR;
	int map_len = CE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ce_base_address;
	unsigned int Reg;

	ce_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ce_base_address == MAP_FAILED)
	{
		perror("ce_top Mapping memory for absolute memory access failed.\n");
		return 0xff;
	}

	Reg = REG_READ32(ce_base_address,CE_REG_CONTROL_OFFSET);

	munmap((void *)ce_base_address, map_len);
	close(fd);
	
	return !(Reg & CE_MASK_START);
}

unsigned int isCEDone(){
	unsigned long int PhysicalAddress =  CE_TOP_BASEADDR;
	int map_len = CE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ce_base_address;
	unsigned int Reg;

	ce_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ce_base_address == MAP_FAILED)
	{
		perror("ce_top Mapping memory for absolute memory access failed.\n");
		return 0xff;
	}

	Reg = REG_READ32(ce_base_address,CE_REG_CONTROL_OFFSET);

	munmap((void *)ce_base_address, map_len);
	close(fd);
	
	return (Reg>>1)&0x1;
}

unsigned int isMEReady(){
	unsigned long int PhysicalAddress =  ME_TOP_BASEADDR;
	int map_len = ME_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* me_base_address;
	unsigned int Reg;

	me_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(me_base_address == MAP_FAILED)
	{
		perror("me_top Mapping memory for absolute memory access failed.\n");
		return 0xff;
	}

	Reg = REG_READ32(me_base_address,ME_REG_CONTROL_OFFSET);

	munmap((void *)me_base_address, map_len);
	close(fd);
	
	return !(Reg & ME_MASK_START);
}

unsigned int isMEDone(){
	unsigned long int PhysicalAddress = ME_TOP_BASEADDR;
	int map_len = ME_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* me_base_address;
	unsigned int Reg;

	me_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(me_base_address == MAP_FAILED)
	{
		perror("me_top Mapping memory for absolute memory access failed.\n");
		return 0xff;
	}

	//set width
	Reg = REG_READ32(me_base_address,ME_REG_CONTROL_OFFSET);

	munmap((void *)me_base_address, map_len);
	close(fd);
	
	return (Reg>>1)&0x1;
}

unsigned int isEEEReady(){
	unsigned long int PhysicalAddress =  EEE_TOP_BASEADDR;
	int map_len = EEE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* eee_base_address;
	unsigned int Reg;

	eee_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(eee_base_address == MAP_FAILED)
	{
		perror("eee_top Mapping memory for absolute memory access failed.\n");
		return 0xff;
	}

	Reg = REG_READ32(eee_base_address,EEE_REG_CONTROL_OFFSET);

	munmap((void *)eee_base_address, map_len);
	close(fd);
	
	return !(Reg & EEE_MASK_START);
}

unsigned int isEEEDone(){
	unsigned long int PhysicalAddress = EEE_TOP_BASEADDR;
	int map_len = EEE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* eee_base_address;
	unsigned int Reg;

	eee_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(eee_base_address == MAP_FAILED)
	{
		perror("eee_top Mapping memory for absolute memory access failed.\n");
		return 0xff;
	}

	//set width
	Reg = REG_READ32(eee_base_address,EEE_REG_CONTROL_OFFSET);

	munmap((void *)eee_base_address, map_len);
	close(fd);
	
	return (Reg>>1)&0x1;
}

unsigned int isECEReady(){
	unsigned long int PhysicalAddress =  ECE_TOP_BASEADDR;
	int map_len = ECE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ece_base_address;
	unsigned int Reg;

	ece_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ece_base_address == MAP_FAILED)
	{
		perror("ece_top Mapping memory for absolute memory access failed.\n");
		return 0xff;
	}

	Reg = REG_READ32(ece_base_address,ECE_REG_CONTROL_OFFSET);

	munmap((void *)ece_base_address, map_len);
	close(fd);
	
	return !(Reg & ECE_MASK_START);
}

unsigned int isECEDone(){
	unsigned long int PhysicalAddress = ECE_TOP_BASEADDR;
	int map_len = ECE_TOP_REG_LENGTH;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* ece_base_address;
	unsigned int Reg;

	ece_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	if(ece_base_address == MAP_FAILED)
	{
		perror("ece_top Mapping memory for absolute memory access failed.\n");
		return 0xff;
	}

	//set width
	Reg = REG_READ32(ece_base_address,ECE_REG_CONTROL_OFFSET);

	munmap((void *)ece_base_address, map_len);
	close(fd);
	
	return (Reg>>1)&0x1;
}
/*-------------------------------------------------------------------------
* Memory device transferring functions
-------------------------------------------------------------------------*/
void copy_mem2dev_8(cv::Mat img, unsigned int in_buffer){
	unsigned long int map_len = IMG_WIDTH*IMG_HEIGHT;
	int fd = open("/dev/mem", O_RDWR);
	unsigned char* virtual_addr_in;

	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)in_buffer);

	if(virtual_addr_in == MAP_FAILED)
	{
		perror("copying memory map failed\n");
		return;
	}

	for (int i=0; i<IMG_HEIGHT;i++)
	{
		for (int j=0; j<IMG_WIDTH;j++)
		{
			REG_WRITE8(virtual_addr_in,(i*IMG_WIDTH+j),img.at<unsigned char>(i,j));
		}
	}
	munmap((void *)virtual_addr_in, map_len);
	close(fd);
}

void copy_dev2mem_32(cv::Mat img, unsigned int in_buffer){
	unsigned long int map_len = IMG_WIDTH*IMG_HEIGHT*4;
	int fd = open("/dev/mem", O_RDWR);
	unsigned char* virtual_addr_in;
	unsigned int source;

	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)in_buffer);

	if(virtual_addr_in == MAP_FAILED)
	{
		perror("copying memory map failed\n");
		return;
	}

	for (int i=0; i<IMG_HEIGHT;i++)
	{
		for (int j=0; j<IMG_WIDTH;j++)
		{
			source = REG_READ32(virtual_addr_in,(i*IMG_WIDTH+j)*4);
			img.at<int>(i,j)=(int)source;
		}
	}
	munmap((void *)virtual_addr_in, map_len);
	close(fd);
}

void copy_mem2dev_32(cv::Mat img, unsigned int in_buffer){
	unsigned long int map_len = IMG_WIDTH*IMG_HEIGHT*4;
	int fd = open("/dev/mem", O_RDWR);
	unsigned char* virtual_addr_in;
	unsigned int source;

	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)in_buffer);

	if(virtual_addr_in == MAP_FAILED)
	{
		perror("copying memory map failed\n");
		return;
	}

	for (int i=0; i<IMG_HEIGHT;i++)
	{
		for (int j=0; j<IMG_WIDTH;j++)
		{
			source = (unsigned int)img.at<int>(i,j);
			REG_WRITE32(virtual_addr_in,(i*IMG_WIDTH+j)*4,source);
		}
	}
	munmap((void *)virtual_addr_in, map_len);
	close(fd);
}

void copy_mem2dev_32_array(uint32_t* array, int num, unsigned int in_buffer){
	unsigned long int map_len = num*4;
	int fd = open("/dev/mem", O_RDWR);
	unsigned char* virtual_addr_in;
	unsigned int source;

	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)in_buffer);

	if(virtual_addr_in == MAP_FAILED)
	{
		perror("copying memory map failed\n");
		return;
	}

	for (int i=0; i<num;i++)
	{
		source = array[i];
		REG_WRITE32(virtual_addr_in,i*4,source);
	}
	munmap((void *)virtual_addr_in, map_len);
	close(fd);
}

void copy_dev2mem_32_array(uint32_t* array, int num, unsigned int in_buffer){
	unsigned long int map_len = num*4;
	int fd = open("/dev/mem", O_RDWR);
	unsigned char* virtual_addr_in;
	unsigned int source;

	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)in_buffer);

	if(virtual_addr_in == MAP_FAILED)
	{
		perror("copying memory map failed\n");
		return;
	}

	for (int i=0; i<num;i++)
	{
		source = REG_READ32(virtual_addr_in,i*4);
		array[i] = source;
	}
	munmap((void *)virtual_addr_in, map_len);
	close(fd);
}
/*-------------------------------------------------------------------------
* Software application functions
-------------------------------------------------------------------------*/
inline uint32_t ct32(unsigned char data,uint32_t in,unsigned char central_low,unsigned char central_high) {
    in = in << 2;
    if ( data > central_high)
        in = in | 0x01;
    else if ( data < central_low)
        in = in | 0x02;
    return in;
}

void CensusEngine8to32(int width,int height,unsigned char epsilon,unsigned char csd1,unsigned char csd2,cv::Mat ims,cv::Mat imc){
		int min_w, max_w, min_h, max_h;
	    int iiw,iih;

	    unsigned char cz,co2l, co2m, co2r, cm2l, cm2r, cu2l, cu2m, cu2r,
        co1l,co1m, co1r, cm1l, cm1r, cu1l, cu1m, cu1r;
	    unsigned char cl, ch;

	    uint32_t c;

	    min_w = csd2;
	    max_w = width-csd2-1;
	    min_h = csd2;
	    max_h = height-csd2-1;

	   for (iiw=0; iiw<width; iiw++) {
	           for (iih=0; iih<height; iih++) {
	               // Census Transformation:
	        	   if (iiw>=min_w && iiw<=max_w && iih >= min_h && iih <= max_h){
					   cz =ims.at<unsigned char>(iih,iiw);
					   cl = cz - epsilon;
					   if (cl < 0)
						cl = 0;
					   ch = cz + epsilon;
					   if (ch > 255)
					ch =255;
					   co2l = ims.at<unsigned char>(iih-csd2,iiw-csd2);
					   co2m = ims.at<unsigned char>(iih-csd2,iiw);
					   co2r = ims.at<unsigned char>(iih-csd2,iiw+csd2);
					   cm2l = ims.at<unsigned char>(iih,iiw-csd2);
					   cm2r = ims.at<unsigned char>(iih,iiw+csd2);
					   cu2l = ims.at<unsigned char>(iih+csd2,iiw-csd2);
					   cu2m = ims.at<unsigned char>(iih+csd2,iiw);
					   cu2r = ims.at<unsigned char>(iih+csd2,iiw+csd2);
					   co1l = ims.at<unsigned char>(iih-csd1,iiw-csd1);
					   co1m = ims.at<unsigned char>(iih-csd1,iiw);
					   co1r = ims.at<unsigned char>(iih-csd1,iiw+csd1);
					   cm1l = ims.at<unsigned char>(iih,iiw-csd2);
					   cm1r = ims.at<unsigned char>(iih,iiw+csd2);
					   cu1l = ims.at<unsigned char>(iih+csd1,iiw-csd1);
					   cu1m = ims.at<unsigned char>(iih+csd1,iiw);
					   cu1r = ims.at<unsigned char>(iih+csd1,iiw+csd1);
					   c = 0x00000000;
					   c = ct32(co2l,c,cl,ch);c = ct32(co2m,c,cl,ch);c = ct32(co2r,c,cl,ch);
					   c = ct32(cm2l,c,cl,ch);c = ct32(cm2r,c,cl,ch);
					   c = ct32(cu2l,c,cl,ch);c = ct32(cu2m,c,cl,ch);c = ct32(cu2r,c,cl,ch);
					   c = ct32(co1l,c,cl,ch);c = ct32(co1m,c,cl,ch);c = ct32(co1r,c,cl,ch);
					   c = ct32(cm1l,c,cl,ch);c = ct32(cm1r,c,cl,ch);
					   c = ct32(cu1l,c,cl,ch);c = ct32(cu1m,c,cl,ch);c = ct32(cu1r,c,cl,ch);
					   imc.at<int>(iih,iiw)=(int)c;
	        	   }
	        	   else
	        	   {
	        		   imc.at<int>(iih,iiw)=0;
	        	   }
	           }
	       }
}

int MatchingEngine32(int width,int height,int w_search,int h_search,cv::Mat imc0,cv::Mat imc1,uint32_t* feature){
	int min_w, max_w, min_h, max_h,ct_corr;
	int short cpos_x1,cpos_y1, cpos_x2,cpos_y2;
	int total_matches=0;
	int iih,iiw,kkh,kkw;
	int ox1=0,oy1=0,ox2=0,oy2=0;
	uint32_t cens_1, cens_2;

	min_w = w_search;
	max_w = width - (w_search+1);
	min_h = h_search;
	max_h = height - (h_search+1);

	for (iih=min_h; iih<=max_h; iih++) {
		for (iiw=min_w; iiw<=max_w; iiw++) {
			cens_1 = (uint32_t)imc0.at<int>(iih,iiw);
			ct_corr=0;
			cpos_x1=0,cpos_y1=0, cpos_x2=0,cpos_y2=0;
			for (kkh= -h_search; kkh<=h_search; kkh++) {
				for (kkw= -w_search; kkw<=w_search; kkw++) {
					cens_2 = (uint32_t)imc1.at<int>(iih+kkh,iiw+kkw);
					if(cens_1==cens_2) {
						ct_corr++;
						cpos_x1=iiw;
						cpos_y1=iih;
						cpos_x2=kkw;
						cpos_y2=kkh;
					}
					if(ct_corr>1)
						goto afterloop;
				}
			}
			afterloop:
			if (1==ct_corr) {
				feature[total_matches]=(((uint32_t)cpos_x1)<<22)+(((uint32_t)cpos_y1)<<12)+((((int)cpos_x2)&0x3f)<<6)+(((int)cpos_y2)&0x3f);
				total_matches++;
			}
		}
	}
	return total_matches;
}

int vis_flow(cv::Mat& target_im,unsigned int featureaddr, uint featurecnt,uint32_t* feature,uint16_t* feature16, int mode,vector<CvScalar> color_LUT)
{
	unsigned long int map_len;
	int fd;
	unsigned char* virtual_addr_in;
	unsigned int source;

 	CvPoint p,q;
    int line_thickness = 1;
    double hypotenuse; 
    CvScalar line_color;
    
    int flag=0;
    
    if (mode == 0){
	  	if ((featureaddr == 0) && feature){
	  	 	flag = 0;
	   		printf("software drawing\n");	
		}else if ((featureaddr != 0) && (!feature)){
			flag = 1;
			printf("hardware drawing\n");
		}else{
			perror("drawing invalid arguments\n");
			return 1;
		}
    }else if (mode == 1){
    	if (feature16){
    		printf("software feature 16 drawing\n");	
    	}
    	else{
    		perror("invalid argument\n");
    		return 1;
    	}
    }else{
    	perror("invalid mode\n");
		return 1;
    }
  

	if (flag){
		map_len = featurecnt*4;
		fd = open("/dev/mem", O_RDWR);
		virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)featureaddr);
		if(virtual_addr_in == MAP_FAILED)
		{
			perror("drawing memory mapping failed\n");
			return 1;
		}
	}


	for (int i=0; i<featurecnt;i++)
	{
		if (mode == 0){
			if (flag)
				source = REG_READ32(virtual_addr_in,i*4);
			else
				source = feature[i];
		
			q.x = (source & 0xffc00000)>>22;
			q.y = (source & 0x003ff000)>>12;

			if ((source & 0x00000800) == 0x00000800){
				p.x = q.x-(((~source & 0x000007c0)>>6)+1);
			}else{
				p.x = q.x+((source & 0x000007c0)>>6);
			}
			if ((source & 0x00000020) == 0x00000020){
				p.y = q.y-((~source & 0x0000001f)+1);
			}else{
				p.y = q.y+(source & 0x0000001f);
			}
		}else if (mode == 1){
			q.x = feature16[4*i+0];
			q.y = feature16[4*i+1];
			p.x = feature16[4*i+2];
			p.y = feature16[4*i+3];
		}else{
			perror("invalid mode\n");
			return 1;
		}
		if (abs(p.y-q.y)<=15 && abs(p.x-q.x)<=15 && p.x<=IMG_WIDTH-1 && p.x >=0 && q.x<=IMG_WIDTH-1 && q.x >=0 && p.y<=IMG_HEIGHT-1 && p.y >=0 && q.y<=IMG_HEIGHT-1 && q.y >=0){
			hypotenuse = sqrt( (double)( (p.y - q.y)*(p.y - q.y)+(p.x - q.x)*(p.x - q.x) ) );
			int num = color_LUT.size();
			if( ((unsigned)hypotenuse+1) < num) {
				line_color = color_LUT.at( ((int)hypotenuse) );
			} else {
				line_color = color_LUT.at( num-1 ) ;
			}
			cv::line( target_im, p, q, line_color, line_thickness, 0, 0 );
		}

	}
	if (flag){
		munmap((void *)virtual_addr_in, map_len);
		close(fd);
	}
	
	return 0;
}

int presample(unsigned int featureaddr,uint32_t* featurearray, uint16_t* p_featureimg,int matchcount,int width, int height){
	int i,j;
	uint32_t feature;
	uint16_t x1,y1,x2,y2;
	
	unsigned long int map_len;
	int fd;
	unsigned char* virtual_addr_in;
	
	int flag;
	
	if ((featureaddr == 0) && featurearray){
		flag = 0;
		printf("software presample\n");
	}else if ((featureaddr != 0) && (!featurearray)){
		flag = 1;
    	printf("hardware presample\n");
	}else{
		perror("presample invalid arguments\n");
		return 1;
	}
	
	if (flag){
		map_len = matchcount*4;
		fd = open("/dev/mem", O_RDWR);
		virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)featureaddr);

		if(virtual_addr_in == MAP_FAILED)
		{
			perror("presample memory mapping failed\n");
			return 1;
		}
	}


	for (i=0;i<width;i++){
		for (j=0;j<height;j++){
			p_featureimg[(j*width+i)*4]=0xffff;
			p_featureimg[(j*width+i)*4+1]=0xffff;
			p_featureimg[(j*width+i)*4+2]=0xffff;
			p_featureimg[(j*width+i)*4+3]=0xffff;
		}
	}
	for (i=0;i<matchcount;i++){
		if (flag)
			feature = REG_READ32(virtual_addr_in,i*4);
		else
			feature = featurearray[i];
			
		x1 = (feature & 0xffc00000)>>22;
		y1 = (feature & 0x003ff000)>>12;
		if ((feature & 0x00000800) == 0x00000800){
			x2 = x1-(((~feature & 0x000007c0)>>6)+1);
		}
		else
		{
			x2 = x1+((feature & 0x000007c0)>>6);
		}

		if ((feature & 0x00000020) == 0x00000020){
			y2 = y1-((~feature & 0x0000001f)+1);
		}
		else
		{
			y2 = y1+(feature & 0x0000001f);
		}
		if (x1>=0 && x1<IMG_WIDTH && y1>=0 && y1<IMG_HEIGHT && x2>=0 && x2<IMG_WIDTH && y2>=0 && y2<IMG_HEIGHT) {
			p_featureimg[(y2*width+x2)*4]=x1;
			p_featureimg[(y2*width+x2)*4+1]=y1;
			p_featureimg[(y2*width+x2)*4+2]=x2;
			p_featureimg[(y2*width+x2)*4+3]=y2;
		}

	}
	
	if (flag){
		munmap((void *)virtual_addr_in, map_len);
		close(fd);
	}
	return 0;
}

int sample_flow_vectors(uint16_t* p_featureimg,uint16_t* p_compvector,int width, int height,int select, int blk_size_vect_avg)
{
	int i,j,k,l;
	uint16_t veccount,tmp;
	int x1c,y1c;
	int compcount;

	compcount=0;

	if (select == 1){
		for(i=0; i<width; i+=blk_size_vect_avg) {
			for (j=0;j<height;j+=blk_size_vect_avg) {
				x1c = 0;
				y1c = 0;
				veccount = 0;
				for(k=i; k<(i+blk_size_vect_avg); k++) {
					for (l=j; l<(j+blk_size_vect_avg); l++) {
						tmp = p_featureimg[(k+l*width)*4];
						if(tmp != 0xffff) {
							x1c += p_featureimg[(k+l*width)*4];
							y1c += p_featureimg[(k+l*width)*4+1];
							veccount++;
						}
					}
				}
				if(veccount > 0) {
					p_compvector[compcount*4] = x1c/veccount;
					p_compvector[compcount*4+1] = y1c/veccount;
					p_compvector[compcount*4+2] = i;
					p_compvector[compcount*4+3] = j;
					compcount++;
				}
			}
		}
		return compcount;
	}else if (select == 2){
		for(i=0; i<width; i+=blk_size_vect_avg) {
			for (j=0;j<height;j+=blk_size_vect_avg) {
				for(k=i; k<(i+blk_size_vect_avg); k++) {
					for (l=j; l<(j+blk_size_vect_avg); l++) {
						tmp = p_featureimg[(k+l*width)*4];
						if(tmp != 0xffff) {
							break;
						}
					}
					if (tmp != 0xffff) break;
				}
				if (tmp != 0xffff){
					p_compvector[compcount*4] = p_featureimg[(k+l*width)*4];
					p_compvector[compcount*4+1] = p_featureimg[(k+l*width)*4+1];
					p_compvector[compcount*4+2] = i;
					p_compvector[compcount*4+3] = j;
					compcount++;
				}
			}
		}
		return compcount;
	}else if (select == 3){
		for (i=0;i<width;i++){
			for (j=0;j<height;j++){
				tmp = p_featureimg[(i+j*(width/8))*4];
				if (tmp != 0xffff){
					p_compvector[compcount*4] = p_featureimg[(i+j*(width/8))*4];
					p_compvector[compcount*4+1] = p_featureimg[(i+j*(width/8))*4+1];
					p_compvector[compcount*4+2] = p_featureimg[(i+j*(width/8))*4+2];
					p_compvector[compcount*4+3] = p_featureimg[(i+j*(width/8))*4+3];
					compcount++;
				}
			}
		}
		return compcount;
	}else{
		for (i=0;i<width;i++){
			for (j=0;j<height;j++){
				tmp = p_featureimg[(i+(j*width))*4];
				if (tmp != 0xffff){
					p_compvector[compcount*4] = p_featureimg[(i+j*width)*4];
					p_compvector[compcount*4+1] = p_featureimg[(i+j*width)*4+1];
					p_compvector[compcount*4+2] = p_featureimg[(i+j*width)*4+2];
					p_compvector[compcount*4+3] = p_featureimg[(i+j*width)*4+3];
					compcount++;
				}
			}
		}
		return compcount;
	}
	return 0;
}

int gen_model_param(int x1, int y1, int vx1, int vy1, int x2, int y2, int vx2, int vy2, float *model_param)
{
	float temp;

	/* xc -> model_param[0], yc -> model_param[1], D -> model_param[2], R -> model_param[3] */
    temp = (float)((vx1 *(vx1 - (2*vx2))) + (vx2 * vx2) + (vy1 * vy1) - (vy2 * ((2*vy1) - vy2)));
	if(temp == 0.0) { /* Check to prevent division by zero */
		return(0);
	}
	model_param[0] = ((( vx1 * ((-vx2*x1) + (vx1*x2) - (vx2*x2) + (vy2*y1) - (vy2*y2)))  + (vy1 * ((-vy2*x1) + (vy1*x2) - (vy2*x2) - (vx2*y1) + (vx2*y2) )) + (x1 * ((vy2* vy2) + (vx2 * vx2)))) / temp);

	temp = (float)((vx1 *(vx1 - (2*vx2))) + (vx2 * vx2) + (vy1 * vy1) - (vy2 * ((2*vy1) - vy2)));
	if(temp == 0.0) { /* Check to prevent division by zero */
		return(0);
	}
	model_param[1] = (((vx2 * ((vy1*x1) - (vy1*x2) - (vx1*y1) + (vx2*y1) - (vx1*y2))) + (vy2 * ((-vx1*x1) + (vx1*x2) - (vy1*y1) + (vy2*y1) - (vy1*y2))) + (y2 * ((vx1 * vx1) + (vy1 * vy1)))) / temp);

    temp = (float)((x1 * (x1 - (2*x2))) + (x2 * x2) + (y1 * (y1 - (2*y2))) + (y2 * y2));
	if(temp == 0.0) { /* Check to prevent division by zero */
		return(0);
	}
	model_param[2] = ((((x1 - x2) * (vx1 - vx2)) + ((y1 - y2) * (vy1 - vy2))) / temp);

	temp = (float)((x1 * (x1 - (2*x2))) + (x2 * x2) + (y1 * (y1 - (2*y2))) + (y2 * y2));
	if(temp == 0.0) { /* Check to prevent division by zero */
		return(0);
	}
	model_param[3] = ((((x1 - x2) * (vy1 - vy2)) + ((y2 - y1) * (vx1 - vx2))) / temp);

    return(1);
}

int gen_firstOrderFlow_model(int flow_vector_count, uint16_t* flow_vector, float *model_param)
{
	int   rand_num, upper_limit, lower_limit;
	int   x1, x2, y1, y2, vx1, vx2, vy1, vy2, count=0;
	int  ret;

	upper_limit = flow_vector_count - 1;
	lower_limit = 0;
	while(1) {
		/* Select two motion vectors at random */
		rand_num = lower_limit + rand() % (upper_limit - lower_limit); /* Gen random number with the array size */
		x1  = flow_vector[rand_num*4];
		y1  = flow_vector[rand_num*4+1];
		vx1 = flow_vector[rand_num*4+2] - x1;
		vy1 = flow_vector[rand_num*4+3] - y1;


		rand_num = lower_limit + rand() % (upper_limit - lower_limit); /* Gen random number with the array size */
		x2  = flow_vector[rand_num*4];
		y2  = flow_vector[rand_num*4+1];
		vx2 = flow_vector[rand_num*4+2] - x2;
		vy2 = flow_vector[rand_num*4+3] - y2;

		if((vx1==0) && (vy1==0) && (vx2==0) && (vy2==0)) {
			model_param[0] = 0;
			model_param[1] = 0;
			model_param[2] = 0;
			model_param[3] = 0;
			return(1);
		}

		/* Function to generate model parameters according to first order flow (xc, yc, D and R) */
		ret = gen_model_param(x1, y1, vx1, vy1, x2, y2, vx2, vy2, model_param);
		if(ret == 1) {
			return(1);
		}
		else if((ret == 0) && (count > 100)) {
			return(0);
		}
		count++;
	}
}

void gen_firstOrderFlow_vectors_4(float *model_param, int flow_vector_count,uint16_t* flow_vector, uint16_t* ego_vector)
{
	float temp_x, temp_y;
	int i;

	/* Compute new set of motion vectors for each point specified in the flow_vector_array */
    for(i = 0; i < flow_vector_count; i++) {
    	temp_x  = ((float)flow_vector[4*i]) - model_param[0];
        temp_y  = ((float)flow_vector[4*i+1])-model_param[1];
		ego_vector[4*i]=flow_vector[4*i];
		ego_vector[4*i+1]=flow_vector[4*i+1];
		ego_vector[4*i+2]=(uint16_t)(flow_vector[4*i]+ ((temp_x * model_param[2]) - (temp_y * model_param[3])));
		ego_vector[4*i+3]=(uint16_t)(flow_vector[4*i+1]+ ((temp_y * model_param[2]) + (temp_x * model_param[3])));
   }

}

void choose_better_model (uint16_t* flow_vector, uint16_t* ego_vector, int flow_vector_count, float *model_param, float *model_param_betterModel, int *outlier_count_betterModel, int error_threshold)
{
	int outlier_count = 0,i=0;
	int vx1, vy1, vx_ego, vy_ego;

	for(i = 0; i < flow_vector_count; i++) {
		vx1      = flow_vector[4*i+2] - flow_vector[4*i];
		vy1      = flow_vector[4*i+3] - flow_vector[4*i+1];
		vx_ego   = ego_vector[4*i+2] - ego_vector[4*i];
		vy_ego   = ego_vector[4*i+3] - ego_vector[4*i+1];

		if(!((abs(vx1 - vx_ego) < error_threshold)) || !((abs(vy1 - vy_ego) < error_threshold))) {
			outlier_count++;
		}
	}

	if (outlier_count < (*outlier_count_betterModel)) {
		model_param_betterModel[0] = model_param[0];
		model_param_betterModel[1] = model_param[1];
		model_param_betterModel[2] = model_param[2];
		model_param_betterModel[3] = model_param[3];
		(*outlier_count_betterModel) = outlier_count;
	}
}

int estimate_ego_motion_first_order_flow( uint16_t* flow_vector, uint16_t* ego_vector,int size_flow_vector_array, float* model_param_est,int max_iter, int min_iter, int error_threshold, float converg_threshold)
{
	int outlier_count_betterModel, loop_count;
	float model_param_betterModel[4];
	int ret;

	if(size_flow_vector_array == 0) {
		return (0);
	}

	outlier_count_betterModel = size_flow_vector_array;

	for(loop_count = 0; loop_count < max_iter; loop_count++) {
		// Obtain model parameters for First Oder FLow
		ret = gen_firstOrderFlow_model(size_flow_vector_array,flow_vector, model_param_est);
		if(ret == 0) {
			break;
		}

		gen_firstOrderFlow_vectors_4(model_param_est, size_flow_vector_array,flow_vector, ego_vector);

		choose_better_model (flow_vector,ego_vector, size_flow_vector_array, model_param_est, model_param_betterModel, &outlier_count_betterModel, error_threshold);
		if( ( loop_count > min_iter ) &&
            ( ((float)outlier_count_betterModel/(float)size_flow_vector_array) <= converg_threshold) ){
			break;
		}
	}

	if((((float)outlier_count_betterModel/(float)size_flow_vector_array)) > converg_threshold) {
		printf(" Model did not converge (outlier count: %d) ! \n\r ", outlier_count_betterModel);
        printf("Outlier Ratio    : %f \n\r",  ((float)outlier_count_betterModel)/((float)size_flow_vector_array));
		ret = 0;
	}
	else {
		model_param_est[0] = model_param_betterModel[0];
		model_param_est[1] = model_param_betterModel[1];
		model_param_est[2] = model_param_betterModel[2];
		model_param_est[3] = model_param_betterModel[3];
        model_param_est[4] = outlier_count_betterModel;
        model_param_est[5] = size_flow_vector_array;
        model_param_est[6] = ((float)outlier_count_betterModel)/((float)size_flow_vector_array);
		printf(" XC: %f \n\r", model_param_betterModel[0]);
		printf(" YC: %f \n\r", model_param_betterModel[1]);
		printf(" D : %f \n\r", model_param_betterModel[2]);
		printf(" R : %f \n\r", model_param_betterModel[3]);
		printf(" Total iterations : %d \n\r", loop_count);
		printf(" Outlier count    : %d \n\r", outlier_count_betterModel);
        printf(" Total count      : %d \n\r", size_flow_vector_array);
        printf(" Outlier Ratio    : %f \n\r",  ((float)outlier_count_betterModel)/((float)size_flow_vector_array));
		ret = 1;
	}
	return ret;

}

int compressvectorHW(unsigned int vector,uint16_t* compvector,int width,int height){
	int i,j;
	uint32_t feature;
	uint16_t x1,y1,x2,y2;
	int count=0;

	unsigned long int map_len;
	int fd;
	unsigned char* virtual_addr_in;
	
	map_len = width*height*4;
	fd = open("/dev/mem", O_RDWR);
	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)vector);

	if(virtual_addr_in == MAP_FAILED)
	{
		perror("hardware vector sample memory mapping failed\n");
		return 0;
	}
	
	for (i=0;i<width;i++){
		for (j=0;j<height;j++){
			feature = REG_READ32(virtual_addr_in,(j*width+i)*4);
			if (feature != 0){
				x1 = (feature >> 15)+(i<<3);
				y1 = ((feature >> 12)& 0x7)+ (j<<3);
				if ((feature & 0x00000800) == 0x00000800){
					x2 = x1-(((~feature & 0x000007c0)>>6)+1);
				}
				else
				{
					x2 = x1+((feature & 0x000007c0)>>6);
				}

				if ((feature & 0x00000020) == 0x00000020){
					y2 = y1-((~feature & 0x0000001f)+1);
				}
				else
				{
					y2 = y1+(feature & 0x0000001f);
				}
				compvector[4*count] = x1;
				compvector[4*count+1] = y1;
				compvector[4*count+2] = x2;
				compvector[4*count+3] = y2;
				count++;
			}
		}
	}
	
	munmap((void *)virtual_addr_in, map_len);
	close(fd);

	return count;
}

int estimate_ego_motion_first_order_flow_HW( uint16_t* flow_vector, int size_flow_vector_array, float* model_param_est, int max_iter, int min_iter)
{
	int loop_count,offset=0;
	int ret;
	
	unsigned long int map_len;
	int fd;
	unsigned char* virtual_addr_in;
	
	map_len = 0x12C0;
	fd = open("/dev/mem", O_RDWR);
	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)MODEL_ADDR);

	if(virtual_addr_in == MAP_FAILED)
	{
		perror("hardware model generating memory mapping failed\n");
		return 0;
	}
	

	if(size_flow_vector_array == 0) {
		return (0);
	}

	for(loop_count = 0; loop_count < max_iter; loop_count++) {
		// Obtain model parameters for First Oder FLow
		ret = gen_firstOrderFlow_model(size_flow_vector_array,flow_vector, model_param_est);
		if(ret == 0) {
			break;
		}

		//---- store the model param for use by ego-motion esitmation engine -----

		REG_WRITE32(virtual_addr_in,offset*4,(uint32_t)(model_param_est[0] * 8));
		REG_WRITE32(virtual_addr_in,offset*4+4,(uint32_t)(model_param_est[1] * 8));
		REG_WRITE32(virtual_addr_in,offset*4+8,(uint32_t)(model_param_est[2] * 2147483648));
		REG_WRITE32(virtual_addr_in,offset*4+12,(uint32_t)(model_param_est[3] * 2147483648));
		offset+=4;

	}
	
	munmap((void *)virtual_addr_in, map_len);
	close(fd);
	
	return ret;
}

void diff_motion_vectors(uint16_t* fullvector, uint16_t* resvector, int vector_count, uint16_t* resvectorout, int error_threshold){

	int vx_error, vy_error,i;

	for(i=0; i<vector_count; i++) {
		vx_error = (((int)resvector[4*i+2]) - ((int)fullvector[4*i+2]));
		vy_error = (((int)resvector[4*i+3])  - ((int)fullvector[4*i+3]));

		if((abs(vx_error) < error_threshold) && (abs(vy_error) < error_threshold)){
			resvectorout[4*i]=0;
			resvectorout[4*i+1]=0;
			resvectorout[4*i+2]=0;
			resvectorout[4*i+3]=0;
		}
		else {
			resvectorout[4*i+2] = (uint16_t)(((int)resvector[4*i])+vx_error);
			resvectorout[4*i+3] = (uint16_t)(((int)resvector[4*i+1])+vy_error);
		}
	}

} 

/*-------------------------------------------------------------------------
* Main function
-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	//global flags
    unsigned char ceflag=0,meflag=0,eeeflag=0,eceflag=0,ceoutflag=0,meoutflag=0,finaloutflag=0,quitflag=0,cameraflag=0,oflag =1,autoflag=0;
    
    //feature array
    uint32_t feature[50000];
    
    //matchcount
    uint16_t matchcount;
    
    //feature image array
    uint16_t featureimg[IMG_HEIGHT*IMG_WIDTH*4];
    
    //compressed vectors
    uint16_t compvector[50000*4];
    
    //compressed compensated vectors
    uint16_t egovector[50000*4];
    
    //model array
    int model_param_32[300*4];
    
    //result array
    uint32_t result[3];
    
    //return result
    int ret;
    
    //samplecount
    uint16_t samplecount;
	
	//model para
	float model_param_est[8];
    
	//file loading config
//    string  Pname = "sequence1/";
//	string  Pname = "../sequence1/";
    string  Pname = "../data3/sequence1/";
    string  Fprefix = "Test_Seq_00";
    string  Fappendix = ".bmp";

    int epsilon, sizeblur;
   
    //for camera
    cv::VideoCapture cap(0);

	//for time measure
    float hwcetime=0,swcetime=0,hwmetime=0,swmetime=0,hweeetime=0,sweeetime=0,hwecetime=0,swecetime=0;
    int hwceframe=0,swceframe=0,hwmeframe=0,swmeframe=0,hweeeframe=0,sweeeframe=0,hweceframe=0,sweceframe=0;

	//for frame index
    int kk=0;

	//for time measure
    timespec ts;
    uint64_t timestart,timeend;

    //begin
    std::cout<<"In this application"<<std::endl;
    std::cout<<"Press 'v' or 'V' for camera"<<std::endl;
    std::cout<<"Press 'i' or 'I' for still images"<<std::endl;
    std::cout<<"Default mode is still image mode"<<std::endl;
    std::cout<<"You can switch modes between them later"<<std::endl;
	
    epsilon = EPSILON;
    sizeblur = 3;
	
	//Init camera
	std::cout<<std::endl;
	printf("Initializing Camera ...\n");
	if(!cap.isOpened())
	{
	    printf("camera device is not opened \n");
	    return -1;
	}
	else printf("camera device is opened \n");

	//Set Camera Parameters
	cap.set(CV_CAP_PROP_FRAME_WIDTH,  IMG_WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, IMG_HEIGHT);
	cap.set(CV_CAP_PROP_FPS, 30);

    // initialize flower2
    CensusFlower2 flower2(epsilon,IMG_HEIGHT,IMG_WIDTH,IMG_WIDTH, 0, 0, VRMAGIC_HEIGHT, VRMAGIC_WIDTH );
    flower2.Initialize();

 	//Init Hardware
	std::cout<<std::endl;
	std::cout<<"Initializing Hardware ..."<<std::endl;
	if (initHardware(CE_TOP_BASEADDR)) return 1;
	if (initHardware(ME_TOP_BASEADDR)) return 1;
	if (initHardware(EEE_TOP_BASEADDR)) return 1;
	if (initHardware(ECE_TOP_BASEADDR)) return 1;

    // kernel for smothing
    cv::Mat kernel;
    kernel.create(cv::Size(3,3),CV_32F);
    kernel.at<float>(0,0,0) = 1.0/16;
	kernel.at<float>(0,1,0) = 2.0/16;
	kernel.at<float>(0,2,0) = 1.0/16;
	kernel.at<float>(1,0,0) = 2.0/16;
	kernel.at<float>(1,1,0) = 4.0/16;
	kernel.at<float>(1,2,0) = 2.0/16;
	kernel.at<float>(2,0,0) = 1.0/16;
	kernel.at<float>(2,1,0) = 2.0/16;
	kernel.at<float>(2,2,0) = 1.0/16;

	//main loop
	while (!quitflag){
		cv::Mat image_FINAL,image_TMP,image_CEIN,image_MEOUT;
		cv::Mat image_CEOUT1;
		cv::Mat image_CEOUT2;

		if (oflag){
			if (!cameraflag){
				printf("Original still image mode\n");
				char a[2];
				sprintf(a,"%d",kk);
				string tmp(a);
				image_TMP = cv::imread(Pname+Fprefix+(kk<10?"0":"")+tmp+Fappendix,CV_LOAD_IMAGE_GRAYSCALE);
				cv::resize(image_TMP,image_FINAL,cv::Size(IMG_WIDTH,IMG_HEIGHT));
				image_TMP.release();
				cv::imshow("Main Window",image_FINAL);
				cv::waitKey(10);
			}else{
				cap>>image_FINAL;
				cv::imshow("Main Window",image_FINAL);
				cv::waitKey(10);
			}
		}else{
			image_CEOUT1 = cv::Mat::zeros(cv::Size(IMG_WIDTH,IMG_HEIGHT),CV_32SC1);
			image_CEOUT2 = cv::Mat::zeros(cv::Size(IMG_WIDTH,IMG_HEIGHT),CV_32SC1);
			if (!ceflag){
				// SW CE
				if (!cameraflag){
					printf("Still image mode ...\n");
					char a[2];
					sprintf(a,"%d",kk);
					string tmp(a);
					image_TMP = cv::imread(Pname+Fprefix+(kk<10?"0":"")+tmp+Fappendix,CV_LOAD_IMAGE_GRAYSCALE);
					cv::resize(image_TMP,image_CEIN,cv::Size(IMG_WIDTH,IMG_HEIGHT));
					image_TMP.release();
				}else{
					printf("Camera mode ...\n");
					cap >> image_TMP;
					cv::cvtColor(image_TMP, image_CEIN, CV_BGR2GRAY);
					image_TMP.release();
				}

				clock_gettime(CLOCK_REALTIME,&ts);
	    		timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
	    		cv::filter2D(image_CEIN,image_TMP,-1,kernel);
	    		image_CEIN.release();
				CensusEngine8to32(IMG_WIDTH,IMG_HEIGHT,epsilon,3,7,image_TMP,image_CEOUT1);
				image_TMP.release();
				clock_gettime(CLOCK_REALTIME,&ts);
				timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
				swceframe++;
				swcetime += timeend-timestart;
				std::cout<<"Software Census Transformation 1 elapsed time: "<<timeend-timestart<<" ms"<<std::endl;

				if (meflag){
					copy_mem2dev_32(image_CEOUT1, WRITE_ADDR_0);
	 				image_CEOUT1.release();
				}

				if (!cameraflag){
					printf("Still image mode ...\n");
					char a[2];
					sprintf(a,"%d",kk+1);
					string tmp(a);
					image_TMP = cv::imread(Pname+Fprefix+((kk+1)<10?"0":"")+tmp+Fappendix,CV_LOAD_IMAGE_GRAYSCALE);
					cv::resize(image_TMP,image_CEIN,cv::Size(IMG_WIDTH,IMG_HEIGHT));
					image_TMP.release();
					cv::cvtColor(image_CEIN, image_FINAL, CV_GRAY2BGR);
					cv::cvtColor(image_CEIN, image_MEOUT, CV_GRAY2BGR);
				}else{
					printf("Camera mode ...\n");
					cap >> image_TMP;
					cv::cvtColor(image_TMP, image_CEIN, CV_BGR2GRAY);
					image_TMP.release();
					cv::cvtColor(image_CEIN, image_FINAL, CV_GRAY2BGR);
					cv::cvtColor(image_CEIN, image_MEOUT, CV_GRAY2BGR);

				}

				clock_gettime(CLOCK_REALTIME,&ts);
				timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
				cv::filter2D(image_CEIN,image_TMP,-1,kernel);
				image_CEIN.release();
				CensusEngine8to32(IMG_WIDTH,IMG_HEIGHT,epsilon,3,7,image_TMP,image_CEOUT2);
				image_TMP.release();
				clock_gettime(CLOCK_REALTIME,&ts);
				timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
				swceframe++;
				swcetime += timeend-timestart;
				std::cout<<"Software Census Transformation 2 elapsed time: "<<timeend-timestart<<" ms"<<std::endl;

				if (meflag){
					copy_mem2dev_32(image_CEOUT2, WRITE_ADDR_1);
				}
			}else{
				if (!cameraflag){
					printf("Still image mode ...\n");
					char a[2];
					sprintf(a,"%d",kk);
					string tmp(a);
					image_TMP = cv::imread(Pname+Fprefix+(kk<10?"0":"")+tmp+Fappendix,CV_LOAD_IMAGE_GRAYSCALE);
					cv::resize(image_TMP,image_CEIN,cv::Size(IMG_WIDTH,IMG_HEIGHT));
					image_TMP.release();
				}else{
					printf("Camera mode ...\n");
					cap >> image_TMP;
					cv::cvtColor(image_TMP, image_CEIN, CV_BGR2GRAY);
					image_TMP.release();
				}

				copy_mem2dev_8(image_CEIN,READ_ADDR);
				image_CEIN.release();

				if (!(isCEReady() == 1)){
					printf("CE is not ready!\n");
					return 1;
				}

				setCETopWriteAddr(WRITE_ADDR_0);
				clock_gettime(CLOCK_REALTIME,&ts);
				timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
				startCETop();
				while (isCEDone()==0);
				clock_gettime(CLOCK_REALTIME,&ts);
				timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
				hwceframe++;
				hwcetime += timeend-timestart;
				std::cout<<"Hardware Census Transformation 1 elapsed time: "<<timeend-timestart<<" ms"<<std::endl;

				if (!meflag)
					copy_dev2mem_32(image_CEOUT1, WRITE_ADDR_0);
				else
					image_CEOUT1.release();

				if (!cameraflag){
					printf("Still image mode ...\n");
					char a[2];
					sprintf(a,"%d",kk+1);
					string tmp(a);
					image_TMP = cv::imread(Pname+Fprefix+((kk+1)<10?"0":"")+tmp+Fappendix,CV_LOAD_IMAGE_GRAYSCALE);
					cv::resize(image_TMP,image_CEIN,cv::Size(IMG_WIDTH,IMG_HEIGHT));
					image_TMP.release();
					cv::cvtColor(image_CEIN, image_FINAL, CV_GRAY2BGR);
					cv::cvtColor(image_CEIN, image_MEOUT, CV_GRAY2BGR);
				}else{
					printf("Camera mode ...\n");
					cap >> image_TMP;
					cv::cvtColor(image_TMP, image_CEIN, CV_BGR2GRAY);
					image_TMP.release();
					cv::cvtColor(image_CEIN, image_FINAL, CV_GRAY2BGR);
					cv::cvtColor(image_CEIN, image_MEOUT, CV_GRAY2BGR);

				}

				copy_mem2dev_8(image_CEIN,READ_ADDR);
				image_CEIN.release();

				if (!(isCEReady() == 1)){
					printf("CE is not ready!\n");
					return 1;
				}

				setCETopWriteAddr(WRITE_ADDR_1);
				clock_gettime(CLOCK_REALTIME,&ts);
				timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
				startCETop();
				while (isCEDone()==0);
				clock_gettime(CLOCK_REALTIME,&ts);
				timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
				hwceframe++;
				hwcetime += timeend-timestart;
				std::cout<<"Hardware Census Transformation 2 elapsed time: "<<timeend-timestart<<" ms"<<std::endl;

				copy_dev2mem_32(image_CEOUT2, WRITE_ADDR_1);
			}

 			if (!meflag){
 				//SW ME
 				clock_gettime(CLOCK_REALTIME,&ts);
         		timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 				matchcount = MatchingEngine32(IMG_WIDTH, IMG_HEIGHT,7,7,image_CEOUT1,image_CEOUT2,feature);
 				clock_gettime(CLOCK_REALTIME,&ts);
         		timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 				swmeframe++;
 				swmetime += timeend-timestart;
 				printf("matchcount: %d\n",matchcount);
 				std::cout<<"Software signature matching elapsed time: "<<timeend-timestart<<" ms"<<std::endl;
 				image_CEOUT1.release();
 			}else{
 				//HW ME
 				clock_gettime(CLOCK_REALTIME,&ts);
         		timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 				startMETop();
 				while (isMEDone()==0);
 				clock_gettime(CLOCK_REALTIME,&ts);
 				timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 				matchcount = getMEResult();
 				printf("matchcount: %d\n",matchcount);
 				hwmeframe++;
 				hwmetime += timeend-timestart;
 				std::cout<<"Hardware signature matching elapsed time: "<<timeend-timestart<<" ms"<<std::endl;
 			}

 			//Drawing vector and presample
 			if (!meflag){
 				if (vis_flow(image_MEOUT,0,matchcount,feature,NULL,0,flower2.color_LUT) == 1)
 					perror("vector drawing failed\n");
 				if (presample(0,feature,featureimg,matchcount,IMG_WIDTH,IMG_HEIGHT))
 					perror("presample failed\n");
 				if (eeeflag)
 					copy_mem2dev_32_array(feature,matchcount,FEATURE_ADDR);
 			}else{
 				if (vis_flow(image_MEOUT,FEATURE_ADDR,matchcount,NULL,NULL,0,flower2.color_LUT) == 1)
 					perror("vector drawing failed\n");
 				if (presample(FEATURE_ADDR,NULL,featureimg,matchcount,IMG_WIDTH,IMG_HEIGHT))
 					perror("presample failed\n");
 			}

 			if (!eeeflag){
 				//SW EEE
 				clock_gettime(CLOCK_REALTIME,&ts);
         		timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 				samplecount = sample_flow_vectors(featureimg,compvector, IMG_WIDTH, IMG_HEIGHT, 2, 8);
 				ret = estimate_ego_motion_first_order_flow(compvector, egovector, samplecount, model_param_est, 300, 300, THRESHOLD, CONVERGTHRESHOLD);
 				clock_gettime(CLOCK_REALTIME,&ts);
         		timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 				if (ret) {
 					sweeeframe++;
 					sweeetime += timeend-timestart;
 					printf("samplecount: %d\n",samplecount);
 					std::cout<<"Software ego-motion estimation elapsed time: "<<timeend-timestart<<" ms"<<std::endl;
 				} else {
 					printf("samplecount: %d\n",samplecount);
 					std::cout<<"Software ego-motion estimation failed"<<std::endl;
 				}
 			}else{
 				//HW EEE
 				if (!(isEEEReady() == 1)){
 					printf("EEE is not ready!\n");
 					return 1;
 				}
 				clock_gettime(CLOCK_REALTIME,&ts);
         		timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
         		setEEEMatchcount(matchcount);
 				setEEEMode(0);
 				startEEETop();
 				while (isEEEDone()==0);
 				samplecount = compressvectorHW(COMPRESS_VECOTR_ADDR,compvector,80,60);
 				ret = estimate_ego_motion_first_order_flow_HW(compvector,samplecount, model_param_est, 300, 300);
 				if (ret){
 					setEEEModelcount(300);
 					setEEEThreshold(2);
 					setEEEMode(1);
 					startEEETop();
 					while (isEEEDone()==0);
 					getEEEresult(result,model_param_est);
 					clock_gettime(CLOCK_REALTIME,&ts);
 					timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 					hweeeframe++;
 					hweeetime += timeend-timestart;
 					std::cout<<"Hardware ego-motion estimation elapsed time: "<<timeend-timestart<<" ms"<<std::endl;
 				}else{
 					printf("samplecount: %d\n",samplecount);
 					std::cout<<"Hardware ego-motion estimation failed"<<std::endl;
 				}
 			}

 			if (ret){
 				if (!eceflag){
 					clock_gettime(CLOCK_REALTIME,&ts);
         			timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 					samplecount = sample_flow_vectors(featureimg,compvector, IMG_WIDTH, IMG_HEIGHT,0,1);
 					gen_firstOrderFlow_vectors_4(model_param_est, samplecount,compvector,egovector);
 					diff_motion_vectors(compvector, egovector, samplecount, egovector, 2);
 					clock_gettime(CLOCK_REALTIME,&ts);
 					timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 					sweceframe++;
 					swecetime += timeend-timestart;
 					std::cout<<"Software ego-motion compensation elapsed time: "<<timeend-timestart<<" ms"<<std::endl;

 					if (vis_flow(image_FINAL,0,samplecount,NULL,egovector,1,flower2.color_LUT) == 1)
 						perror("vector drawing failed\n");
 				}else{
 					setECEBestmodel(model_param_est);
 					clock_gettime(CLOCK_REALTIME,&ts);
         			timestart = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
         			setECEVectorcount(matchcount);
         			setECEThreshold(2);
 					startECETop();
 					while (isECEDone()==0);
 					samplecount = getECEResult();
 					clock_gettime(CLOCK_REALTIME,&ts);
 					timeend = ((uint64_t)ts.tv_sec)*1000+((uint64_t)ts.tv_nsec)/1000000;
 					hweceframe++;
 					hwecetime += timeend-timestart;
 					std::cout<<"Hardware ego-motion compensation elapsed time: "<<timeend-timestart<<" ms"<<std::endl;

 					if (vis_flow(image_FINAL,RESULT_VEC_ADDR,samplecount,NULL,NULL,0,flower2.color_LUT) == 1)
 						perror("vector drawing failed\n");
 				}
 			}else{
 				perror("No ego-motion compensation due to estimation failure\n");
 			}

 			if (!cameraflag){
 				//still image always show final result after processing
 				ceoutflag = 0;
 				meoutflag = 0;
 				finaloutflag = 1;
 			}

 			if (ceoutflag == 1 && meoutflag == 0 && finaloutflag == 0){
 				cv::imshow("Main Window",image_CEOUT2);
 				cv::waitKey(10);
 			}
 			else if (ceoutflag == 0 && meoutflag == 1 && finaloutflag == 0){
 				cv::imshow("Main Window",image_MEOUT);
 				cv::waitKey(10);
 			}
 			else if (ceoutflag == 0 && meoutflag == 0 && finaloutflag == 1){
 				if (ret){
 					cv::imshow("Main Window",image_FINAL);
 					cv::waitKey(10);
 				}
 				else{
 					cv::imshow("Main Window",image_MEOUT);
 					cv::waitKey(10);
 				}
 			}else{
 				perror("invalid display mode\n");
 			}

		}
		
		if (!cameraflag){
			while (1){
				int c;
				if (autoflag == 0){c = cvWaitKey(0);}else {c = cvWaitKey(30);}
				if (c == 'q'){
					quitflag =1;
					printf("Exiting application ...\n");
					break;
				}else if (c == 'o'){
					if (oflag == 1){
						oflag = 0;
					}
					else{
						oflag =1;
					}
					break;
				}else if (c == 'h'){
					ceflag =1;
					meflag = 1;
					eeeflag = 1;
					eceflag = 1;
					printf("Switch to pure hardware mode\n");
				}else if (c == 'v' || c == 'V'){
					cameraflag = 1;
					printf("Switch to camera mode\n");
					break;
				}else if (c == 's'){
					ceflag = 0;
					meflag = 0;
					eeeflag = 0;
					eceflag = 0;
					printf("Switch to pure software mode\n");
				}else if (c == 'c'){
					if (ceflag == 1){
						ceflag = 0;
						printf("Switch to software CE\n");
					}
					else{
						ceflag =1;
						printf("Switch to hardware CE\n");
					}
				}else if (c == 'm'){
					if (meflag == 1){
						meflag = 0;
						printf("Switch to software ME\n");
					}
					else{
						meflag = 1;
						printf("Switch to hardware ME\n");
					}
				}else if (c == 'e'){
					if (eeeflag == 1){
						eeeflag = 0;
						printf("Switch to software EEE\n");
					}
					else{
						eeeflag =1;
						printf("Switch to hardware EEE\n");
					}
				}else if (c == 'f'){
					if (eceflag == 1) {
						eceflag = 0;
						printf("Switch to software ECE\n");
					}
					else {
						eceflag =1;
						printf("Switch to hardware ECE\n");
					}
				}else if (c == 'a'){
					autoflag =1;
					printf("Switch to auto mode\n");
				}else if (c == 'b'){
					autoflag =0;
					printf("Switch to non-auto mode\n");		
				}else if (autoflag == 1){
					kk++;
 					if (kk>19) kk=0;
					break;
				}else if (c == 65363){
					kk++;
 					if (kk>19) kk=19;
					printf("Go forward\n");
					break;
				}else if (c == 65361){
					kk--;
					if (kk<0) kk=0;
					printf("Go backwrad\n");
					break;
				}else if (c == 65362){
					kk=0;
					printf("Start from the beginning\n");
					break;
				}else if (c == 65364){
 					kk=19;
					printf("Go to the end\n");
					break;
				}else if (c=='1'){
 					cv::imshow("Main Window",image_CEOUT2);
 					cv::waitKey(10);
				}
				else if (c=='2'){
					cv::imshow("Main Window",image_MEOUT);
					cv::waitKey(10);
				}
				else if (c=='3'){
 					if (ret){
 						cv::imshow("Main Window",image_FINAL);
 						cv::waitKey(10);
 					}
 					else{
 						cv::imshow("Main Window",image_MEOUT);
 						cv::waitKey(10);
 					}
				}
			}
		}else{
			char c =cvWaitKey(30);
			if (c == 'q'){
				quitflag =1;
				printf("Exiting application ...\n");
			}else if (c == 'o'){
				if (oflag == 1){
					oflag = 0;
					printf("Switch to non-original camera mode\n");
				}
				else{
					oflag =1;
					printf("Swtich to original camera mode\n");
				}
			}else if (c == 'h'){
				ceflag =1;
				meflag = 1;
				eeeflag = 1;
				eceflag = 1;
				printf("Switch to pure hardware mode\n");
			}else if (c == 'i' || c == 'I'){
				cameraflag = 0;
				printf("Switch to still image mode\n");
			}else if (c == 's'){
				ceflag = 0;
				meflag = 0;
				eeeflag = 0;
				eceflag = 0;
				printf("Switch to pure software mode\n");
			}else if (c == 'c'){
				if (ceflag == 1){
					ceflag = 0;
					printf("Switch to software CE\n");
				}
				else{
					ceflag =1;
					printf("Switch to hardware CE\n");
				}
			}else if (c == 'm'){
				if (meflag == 1){
					meflag = 0;
					printf("Switch to software ME\n");
				}
				else{
					meflag = 1;
					printf("Switch to hardware ME\n");
				}
			}else if (c == 'e'){
				if (eeeflag == 1){
					eeeflag = 0;
					printf("Switch to software EEE\n");
				}
				else{
					eeeflag =1;
					printf("Switch to hardware EEE\n");
				}
			}else if (c == 'f'){
				if (eceflag == 1) {
					eceflag = 0;
					printf("Switch to software ECE\n");
				}
				else {
					eceflag =1;
					printf("Switch to hardware ECE\n");
				}
			}else if (c=='1'){
				ceoutflag = 1;
				meoutflag = 0;
				finaloutflag = 0;	
			}
			else if (c=='2'){
				ceoutflag = 0;
				meoutflag = 1;
				finaloutflag = 0;	
			}
			else if (c=='3'){
				ceoutflag = 0;
				meoutflag = 0;
				finaloutflag = 1;	
			}
		}
		
		if (oflag) {
			image_FINAL.release();
		} else {
			image_CEOUT2.release();
			image_MEOUT.release();
			image_FINAL.release();
		}
	}
	
    printf("--------------Performance Final Report---------------\n");
    printf("-----------------------------------------------------\n");
    printf("-----------------------------------------------------\n");
    printf("SW CE processed frames number: %d\n",swceframe);
    printf("HW CE processed frames number: %d\n",hwceframe);
    printf("SW ME processed frames number: %d\n",swmeframe);
    printf("HW ME processed frames number: %d\n",hwmeframe);
    printf("SW EEE processed frames number: %d\n",sweeeframe);
    printf("HW EEE processed frames number: %d\n",hweeeframe);
    printf("SW ECE processed frames number: %d\n",sweceframe);
    printf("HW ECE processed frames number: %d\n",hweceframe);
    (swceframe == 0)? printf("No SW CE average processing time\n"):printf("SW CE average processing time: %f ms\n",swcetime/swceframe);
    (hwceframe == 0)? printf("No HW CE average processing time\n"):printf("HW CE average processing time: %f ms\n",hwcetime/hwceframe);
    (swmeframe == 0)? printf("No SW ME average processing time\n"):printf("SW ME average processing time: %f ms\n",swmetime/swmeframe);
    (hwmeframe == 0)? printf("No HW ME average processing time\n"):printf("HW ME average processing time: %f ms\n",hwmetime/hwmeframe);
    (sweeeframe == 0)? printf("No SW EEE average processing time\n"):printf("SW EEE average processing time: %f ms\n",sweeetime/sweeeframe);
    (hweeeframe == 0)? printf("No HW EEE average processing time\n"):printf("HW EEE average processing time: %f ms\n",hweeetime/hweeeframe);
    (sweceframe == 0)? printf("No SW ECE average processing time\n"):printf("SW ECE average processing time: %f ms\n",swecetime/sweceframe);
    (hweceframe == 0)? printf("No HW ECE average processing time\n"):printf("HW ECE average processing time: %f ms\n",hwecetime/hweceframe);
    printf("Total processed frames number: %d\n",swceframe+hwceframe);
    printf("-----------------------------------------------------\n");
    printf("-----------------------------------------------------\n");
    
    cv::destroyWindow("Main Window");
    kernel.release();

	return 0;
}



