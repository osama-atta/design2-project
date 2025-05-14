/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef hlpuart1;
UART_HandleTypeDef huart2;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
//serial monitor printing related variables
const uint8_t configs[][2] = {
		{0xff, 0x0},
		{0x2c, 0xff},
		{0x2e, 0xdf},
		{0xff, 0x1},
		{0x3c, 0x32},
		{0x11, 0x0},
		{0x09, 0x2},
		{0x04, 0xa8},
		{0x13, 0xe5},
		{0x14, 0x48},
		{0x2c, 0xc},
		{0x33, 0x78},
		{0x3a, 0x33},
		{0x3b, 0xfb},
		{0x3e, 0x0},
		{0x43, 0x11},
		{0x16, 0x10},
		{0x39, 0x2},
		{0x35, 0x88},
		{0x22, 0xa},
		{0x37, 0x40},
		{0x23, 0x0},
		{0x34, 0xa0},
		{0x06, 0x2},
		{0x06, 0x88},
		{0x07, 0xc0},
		{0x0d, 0xb7},
		{0x0e, 0x1},
		{0x4c, 0x0},
		{0x4a, 0x81},
		{0x21, 0x99},
		{0x24, 0x40},
		{0x25, 0x38},
		{0x26, 0x82},
		{0x5c, 0x0},
		{0x63, 0x0},
		{0x46, 0x22},
		{0x0c, 0x3a},
		{0x5d, 0x55},
		{0x5e, 0x7d},
		{0x5f, 0x7d},
		{0x60, 0x55},
		{0x61, 0x70},
		{0x62, 0x80},
		{0x7c, 0x5},
		{0x20, 0x80},
		{0x28, 0x30},
		{0x6c, 0x0},
		{0x6d, 0x80},
		{0x6e, 0x0},
		{0x70, 0x2},
		{0x71, 0x94},
		{0x73, 0xc1},
		{0x3d, 0x34},
		{0x12, 0x4},
		{0x5a, 0x57},
		{0x4f, 0xbb},
		{0x50, 0x9c},
		{0xff, 0x0},
		{0xe5, 0x7f},
		{0xf9, 0xc0},
		{0x41, 0x24},
		{0xe0, 0x14},
		{0x76, 0xff},
		{0x33, 0xa0},
		{0x42, 0x20},
		{0x43, 0x18},
		{0x4c, 0x0},
		{0x87, 0xd0},
		{0x88, 0x3f},
		{0xd7, 0x3},
		{0xd9, 0x10},
		{0xd3, 0x82},
		{0xc8, 0x8},
		{0xc9, 0x80},
		{0x7c, 0x0},
		{0x7d, 0x0},
		{0x7c, 0x3},
		{0x7d, 0x48},
		{0x7d, 0x48},
		{0x7c, 0x8},
		{0x7d, 0x20},
		{0x7d, 0x10},
		{0x7d, 0xe},
		{0x90, 0x0},
		{0x91, 0xe},
		{0x91, 0x1a},
		{0x91, 0x31},
		{0x91, 0x5a},
		{0x91, 0x69},
		{0x91, 0x75},
		{0x91, 0x7e},
		{0x91, 0x88},
		{0x91, 0x8f},
		{0x91, 0x96},
		{0x91, 0xa3},
		{0x91, 0xaf},
		{0x91, 0xc4},
		{0x91, 0xd7},
		{0x91, 0xe8},
		{0x91, 0x20},
		{0x92, 0x0},
		{0x93, 0x6},
		{0x93, 0xe3},
		{0x93, 0x3},
		{0x93, 0x3},
		{0x93, 0x0},
		{0x93, 0x2},
		{0x93, 0x0},
		{0x93, 0x0},
		{0x93, 0x0},
		{0x93, 0x0},
		{0x93, 0x0},
		{0x93, 0x0},
		{0x93, 0x0},
		{0x96, 0x0},
		{0x97, 0x8},
		{0x97, 0x19},
		{0x97, 0x2},
		{0x97, 0xc},
		{0x97, 0x24},
		{0x97, 0x30},
		{0x97, 0x28},
		{0x97, 0x26},
		{0x97, 0x2},
		{0x97, 0x98},
		{0x97, 0x80},
		{0x97, 0x0},
		{0x97, 0x0},
		{0xa4, 0x0},
		{0xa8, 0x0},
		{0xc5, 0x11},
		{0xc6, 0x51},
		{0xbf, 0x80},
		{0xc7, 0x10},
		{0xb6, 0x66},
		{0xb8, 0xa5},
		{0xb7, 0x64},
		{0xb9, 0x7c},
		{0xb3, 0xaf},
		{0xb4, 0x97},
		{0xb5, 0xff},
		{0xb0, 0xc5},
		{0xb1, 0x94},
		{0xb2, 0xf},
		{0xc4, 0x5c},
		{0xa6, 0x0},
		{0xa7, 0x20},
		{0xa7, 0xd8},
		{0xa7, 0x1b},
		{0xa7, 0x31},
		{0xa7, 0x0},
		{0xa7, 0x18},
		{0xa7, 0x20},
		{0xa7, 0xd8},
		{0xa7, 0x19},
		{0xa7, 0x31},
		{0xa7, 0x0},
		{0xa7, 0x18},
		{0xa7, 0x20},
		{0xa7, 0xd8},
		{0xa7, 0x19},
		{0xa7, 0x31},
		{0xa7, 0x0},
		{0xa7, 0x18},
		{0x7f, 0x0},
		{0xe5, 0x1f},
		{0xe1, 0x77},
		{0xdd, 0x7f},
		{0xc2, 0xe},
		{0xff, 0x0},
		{0xe0, 0x4},
		{0xc0, 0xc8},
		{0xc1, 0x96},
		{0x86, 0x3d},
		{0x51, 0x90},
		{0x52, 0x2c},
		{0x53, 0x0},
		{0x54, 0x0},
		{0x55, 0x88},
		{0x57, 0x0},
		{0x50, 0x92},
		{0x5a, 0x50},
		{0x5b, 0x3c},
		{0x5c, 0x0},
		{0xd3, 0x4},
		{0xe0, 0x0},
		{0xff, 0x0},
		{0x05, 0x0},
		{0xda, 0x8},
		{0xd7, 0x3},
		{0xe0, 0x0},
		{0x05, 0x0}		};


//SRAM related variabl
const uint8_t SRAM_WRMR = 0x01;
const uint8_t SRAM_SEQ = 0x00;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

unsigned int charToHex(char val){
	if (val == '0'){
		return 0x0;
	}
	else if (val == '1'){
		return 0x1;
	}
	else if (val == '2'){
		return 0x2;
	}
	else if (val == '3'){
		return 0x3;
	}
	else if (val == '4'){
		return 0x4;
	}
	else if (val == '5'){
		return 0x5;
	}
	else if (val == '6'){
		return 0x6;
	}
	else if (val == '7'){
		return 0x7;
	}
	else if (val == '8'){
		return 0x8;
	}
	else if (val == '9'){
		return 0x9;
	}
	else if (val == 'A'){
		return 0xA;
	}
	else if (val == 'B'){
		return 0xB;
	}
	else if (val == 'C'){
		return 0xC;
	}
	else if (val == 'D'){
		return 0xD;
	}
	else if (val == 'E'){
		return 0xE;
	}
	else{
		return 0xF;
	}
}
/*=================================================================================================*/
HAL_StatusTypeDef printToMonitor(uint8_t *data, uint8_t size){
	HAL_StatusTypeDef ret;
	uint8_t printError[16] = "Error Printing";

	ret = HAL_UART_Transmit(&huart2, data, size, HAL_MAX_DELAY);
	if (ret != HAL_OK){
		ret = HAL_UART_Transmit(&huart2, printError, sizeof(printError)-1, HAL_MAX_DELAY);
		return ret;
	}

	return ret;
}
/*=================================================================================================*/
HAL_StatusTypeDef sensor_init(){

	HAL_StatusTypeDef ret;

	int8_t configData[3];

	configData[0]=0x04; //command code
	configData[1]=0x00;
	configData[2]=0x03;

	ret = HAL_I2C_Master_Transmit(&hi2c1, SENSORADDRESS<<1, (uint8_t *)&configData, 3, 2000);
	if (ret != HAL_OK)
	{
		return ret;
	}

	configData[0] = 0x03; //command code
	configData[1] = 0x40;
	configData[2] = 0x08;

	ret = HAL_I2C_Master_Transmit(&hi2c1, SENSORADDRESS<<1, (uint8_t *)&configData, 3, 2000);
	if (ret != HAL_OK)
	{
		return ret;
	}

	return ret;
}
/*=================================================================================================*/
HAL_StatusTypeDef getSensorValue(uint8_t *data){

	HAL_StatusTypeDef ret;
	uint8_t sensorDataBuffer[2];

	ret = HAL_I2C_Mem_Read(&hi2c1,SENSORADDRESS<<1,0x08,1,(uint8_t *)&sensorDataBuffer,2,HAL_MAX_DELAY);
	if (ret != HAL_OK)
		{
			return ret;
		}

	if (sensorDataBuffer[0]<6){
		*data=0x39;
	}else if(sensorDataBuffer[0]<9 && sensorDataBuffer[1] == 0){
		*data=0x38;
	}else if(sensorDataBuffer[0]<35 && sensorDataBuffer[1] == 0){
		*data=0x37;
	}else if(sensorDataBuffer[0]<65 && sensorDataBuffer[1] == 0){
		*data = 0x36;
	}else if(sensorDataBuffer[0]<90 && sensorDataBuffer[1] == 0){
		*data = 0x35;
	}else if(sensorDataBuffer[0]<125 && sensorDataBuffer[1] == 0){
		*data = 0x34;
	}else if(sensorDataBuffer[0]<175 && sensorDataBuffer[1] == 0){
		*data = 0x33;
	}else if(sensorDataBuffer[0] < 225 && sensorDataBuffer[1] == 0){
		*data = 0x32;
	}else{
		*data = 0x31;
	}

	return ret;
}
/*=================================================================================================*/
HAL_StatusTypeDef getGpsData(){
	HAL_StatusTypeDef ret;
	int8_t gpsDataBuffer[256];
	int8_t init = 0xFF;

	ret = HAL_I2C_Master_Transmit(&hi2c1, GPSADDRESS<<1, (uint8_t *)&init, 1, HAL_MAX_DELAY);
		if (ret != HAL_OK)
		{
			return ret;
		}
	HAL_Delay(40);

	do{
		ret = HAL_I2C_Master_Receive(&hi2c1, GPSADDRESS<<1, (uint8_t *)&gpsDataBuffer, 256, HAL_MAX_DELAY);
			if (ret != HAL_OK){
				return ret;
			}
		//HAL_Delay(40);

	}while(gpsDataBuffer[0] == -1);

	HAL_UART_Transmit(&huart2,(uint8_t *)&gpsDataBuffer,162,HAL_MAX_DELAY);

	return ret;
}
/*=================================================================================================*/
void ram_init(){

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&SRAM_WRMR, 1, 100);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&SRAM_SEQ, 1, 100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
}
/*=================================================================================================*/
void ram_write_split(uint8_t high, uint8_t med, uint8_t low, uint8_t value){

	static uint8_t srcBuff[5];

	srcBuff[0] = 0x02;				// write command code
	srcBuff[1] = high; //high address
	srcBuff[2] = med; //middle address
	srcBuff[3] = low; //low address
	srcBuff[4] = value;	//data

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET); //slave select low
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&srcBuff, 5, 100); //command -> addr -> data
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET); //slave select high

}
/*=================================================================================================*/
uint8_t ram_read_split(uint8_t high, uint8_t med, uint8_t low, uint8_t *data){

	static uint8_t srcReadBuff[4];

	srcReadBuff[0] = 0x03;		//Read Command Code
	srcReadBuff[1] = high;	//high address
	srcReadBuff[2] = med;	//middle address
	srcReadBuff[3] = low;	//low address

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);//slave select low

	HAL_SPI_Transmit(&hspi1, (uint8_t *)&srcReadBuff, 4, 100); //command -> addr
	HAL_SPI_Receive(&hspi1, data, 1, 100); //they are definitely trolling with this


	HAL_SPI_Receive(&hspi1, data, 1, 100); //how should we retun the data? -> one by one
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET); //slave select high

	return *data;

}
/*=================================================================================================*/
HAL_StatusTypeDef cam_config(){
	uint8_t i = 0;
	HAL_StatusTypeDef ret;
	uint8_t printError[16];
	int8_t configData[2];


	configData[0] = 0xff;
	configData[1] = 0x01;
	ret = HAL_I2C_Master_Transmit(&hi2c1, CAMADDRESSWRITE, (uint8_t *)&configData, 2, 100);
	if (ret != HAL_OK){
		return ret;
	}


	configData[0] = 0x12;
	configData[1] = 0x80;
	ret = HAL_I2C_Master_Transmit(&hi2c1, 0x60, (uint8_t *)&configData, 2, 100);
	if (ret != HAL_OK){
		return ret;
	}


	HAL_Delay(200);

	for(i = 0; i<193; i++){
		ret = HAL_I2C_Master_Transmit(&hi2c1, 0x60, (uint8_t *)&configs[i], 2, 100);
		if (ret != HAL_OK){
			return ret;
		}
	}

	return ret;
}
/*=================================================================================================*/
void esp_tx(uint8_t data){
	uint8_t txbuff[1];
	txbuff[0] = data;
	HAL_UART_Transmit(&hlpuart1,(uint8_t *) &txbuff, 1,HAL_MAX_DELAY);
}
/*=================================================================================================*/
uint8_t esp_rx(){
	uint8_t rxbuff[1];
	HAL_UART_Receive(&hlpuart1,(uint8_t *)&rxbuff, sizeof(rxbuff),HAL_MAX_DELAY);
	return rxbuff[0];
}
/*=================================================================================================*/
void cam_pic(void){

	uint8_t srcBuff[2];
	char uart_buf_cam[50];
	int uart_buf_len_cam;
	uint8_t readysend;
	uint8_t readyrec;
	uint8_t srcTestBuff = 0x3C;
	uint8_t high = 0x00;
	uint8_t low = 0x00;
	uint8_t cover = 0x00;


	srcBuff[0] = 0x84; //msb is 1 for write
	srcBuff[1]=  0x20; // try 0x33


	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //slave select low
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&srcBuff, 2, HAL_MAX_DELAY); //setup register
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //slave select high


	srcBuff[1]=  0x10;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //slave select low
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&srcBuff, 2, HAL_MAX_DELAY); //setup register
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //slave select high



	srcBuff[1] = 0x02;

	//first capture
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //slave select low
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&srcBuff, 2, HAL_MAX_DELAY); //start the camera capture
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //slave select high


	readysend=0x41;

	do{
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //slave select low
		HAL_SPI_Transmit(&hspi1,&readysend, 1, HAL_MAX_DELAY);
		HAL_SPI_Receive(&hspi1,&readyrec, 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //slave select high
	}while((readyrec & 0x08) == 0);


	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //slave select high

	uart_buf_len_cam = sprintf(uart_buf_cam, "Picture taken!\n");
	printToMonitor((uint8_t *)&uart_buf_cam, uart_buf_len_cam);

	srcTestBuff = 0x3C;


	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //slave select low


	HAL_SPI_Transmit(&hspi1,&srcTestBuff, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &high, 1, HAL_MAX_DELAY); //dummy bit

	HAL_UART_Transmit(&hlpuart1,(uint8_t *) "IMG", 3,HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1,(uint8_t *) &cover, 1,HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1,(uint8_t *) &cover, 1,HAL_MAX_DELAY);

	for(uint8_t t = 0; t < 240; t++){
			for(uint16_t v = 0; v < 320; v++){

				HAL_SPI_Receive(&hspi1, &high, 1, HAL_MAX_DELAY);
				HAL_SPI_Receive(&hspi1, &low, 1, HAL_MAX_DELAY);

				if((t >= 40 && t < 200) && (v >= 100 && v < 220)){

					HAL_UART_Transmit(&hlpuart1,&low, 1,HAL_MAX_DELAY);
					HAL_UART_Transmit(&hlpuart1,&high, 1,HAL_MAX_DELAY);

				}
			}
		}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //slave select high
}
/*=================================================================================================*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  char uart_buf[150];
  int uart_buf_len;


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_LPUART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  uint8_t didntwork[4] = "FAIL\n";
  uint8_t invalidInput[13] = "Invalid input";
  HAL_StatusTypeDef checker;

  uint8_t ramBuff = 0;
  uint8_t espbuff;
  uint8_t sensorValue;


  uint8_t high;
  uint8_t low;

  uint8_t ramCheck;



  //we need to initalize the slave select pins here
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);//slave select 2
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);//slave select 1


  checker = sensor_init();
  if(checker != HAL_OK){
 	  uart_buf_len = sprintf(uart_buf, "sensor init didnt work\n");
 	  printToMonitor((uint8_t *)&uart_buf, uart_buf_len);
   }

  ram_init();

  checker = cam_config();
  if(checker != HAL_OK){
	  uart_buf_len = sprintf(uart_buf, "cam config didnt work\n");
	  printToMonitor((uint8_t *)&uart_buf, uart_buf_len);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
 //*****************************************************************************************
	  uart_buf_len = sprintf(uart_buf,
			  "Hello World!\nCAM for picture from camera\nGPS for Coordinates\nSEN for Proximity Sensor\nRAM for a ram check\n");

	  printToMonitor((uint8_t *)&uart_buf, uart_buf_len);

	  uint8_t response[4] = {0,0,0,0}; //check with board

	  HAL_UART_Receive(&huart2, (uint8_t *)&response, sizeof(response), HAL_MAX_DELAY);

	  uart_buf_len = sprintf(uart_buf,
	 			  "%c%c%c\r\n",response[0],response[1],response[2]);

	  printToMonitor((uint8_t *)&uart_buf, uart_buf_len);



	  if(response[0] == 'G' && response[1] == 'P' && response[2] == 'S'){
		  checker = getGpsData();
	 	  if (checker != HAL_OK){
	 		  HAL_UART_Transmit(&huart2,didntwork,sizeof(didntwork),HAL_MAX_DELAY);
	 	  }
	  }
	  else if(response[0] == 'C' && response[1] == 'A' && response[2] == 'M'){


		  uart_buf_len = sprintf(uart_buf, "Say Cheese!\n");
		  printToMonitor((uint8_t *)&uart_buf, uart_buf_len);


		  do{
			  espbuff = esp_rx();

			  uart_buf_len = sprintf(uart_buf,
								  "value we are getting from esp: %c\n", espbuff);
					  printToMonitor((uint8_t *)&uart_buf, uart_buf_len);


		  }while(espbuff != 'R');


		  cam_pic();


		  uart_buf_len = sprintf(uart_buf,
						  "transmitted!\n");
			  printToMonitor((uint8_t *)&uart_buf, uart_buf_len);

		}
	  else if(response[0] == 'S' && response[1] == 'E' && response[2] == 'N'){


		  while(1){

			  getSensorValue(&sensorValue);

			  uart_buf_len = sprintf(uart_buf,
			  						  "Value from sensor: 0x%x\n", sensorValue);
			  			  printToMonitor((uint8_t *)&uart_buf, uart_buf_len);

			  HAL_Delay(100);

		  }



	  }

	  else if(response[0] == 'R' && response[1] == 'A' && response[2] == 'M'){

		  response[4] = 0;

		  uart_buf_len = sprintf(uart_buf,"Input 2 hex value to check ram with:\n");
		 			  			  printToMonitor((uint8_t *)&uart_buf, uart_buf_len);

		 HAL_UART_Receive(&huart2, (uint8_t *)&response, 3, HAL_MAX_DELAY);

		 high = charToHex(response[0]);
		 low = charToHex(response[1]);

		 ramCheck = (high << 4) | (low) ;

		 uart_buf_len = sprintf(uart_buf,"Your input 0x%x\n", ramCheck);
		printToMonitor((uint8_t *)&uart_buf, uart_buf_len);



		ram_write_split(0x10,0xFF,0xFF, ramCheck);

		ramBuff = 0;

		ram_read_split(0x10,0xFF,0xFF,&ramBuff);

		if(ramBuff != ramCheck){
			uart_buf_len = sprintf(uart_buf,"Ram Check Failed\n");
					printToMonitor((uint8_t *)&uart_buf, uart_buf_len);

		}

		uart_buf_len = sprintf(uart_buf,"Value in address 0x%x\n", ramBuff);
		printToMonitor((uint8_t *)&uart_buf, uart_buf_len);

	  }


	  else{
		  HAL_UART_Transmit(&huart2,invalidInput,sizeof(invalidInput),HAL_MAX_DELAY);
	  }


	  //*****************************************************************************************

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_LPUART1
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000708;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI1_SS1_Pin|SPI1_SS2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SPI1_SS1_Pin SPI1_SS2_Pin */
  GPIO_InitStruct.Pin = SPI1_SS1_Pin|SPI1_SS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : INT_Pin */
  GPIO_InitStruct.Pin = INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
