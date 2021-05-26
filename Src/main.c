
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "usb_host.h"

/* USER CODE BEGIN Includes */

#include "stm32f4_discovery_audio.h"
#include "ansi.h"
#include "term_io.h"
#include "dbgu.h"
#include "mp3dec.h"


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;

RNG_HandleTypeDef hrng;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_I2C1_Init(void);

static void MX_I2S3_Init(void);

static void MX_SPI1_Init(void);

static void MX_RNG_Init(void);

static void MX_TIM6_Init(void);

static void MX_USART2_UART_Init(void);

void StartDefaultTask(void const *argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_I2S3_Init();
    MX_SPI1_Init();
    MX_RNG_Init();
    MX_TIM6_Init();
    MX_USART2_UART_Init();
    /* USER CODE BEGIN 2 */
    debug_init(&huart2);
    xprintf("F4 player test!\n");
    xprintf("printf test!\n");
    /* USER CODE END 2 */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */


    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

    }
    /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
    PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the Systick interrupt time 
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick 
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void) {

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

}

/* I2S3 init function */
static void MX_I2S3_Init(void) {

    hi2s3.Instance = SPI3;
    hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
    hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
    hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_96K;
    hi2s3.Init.CPOL = I2S_CPOL_LOW;
    hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
    if (HAL_I2S_Init(&hi2s3) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

}

/* RNG init function */
static void MX_RNG_Init(void) {

    hrng.Instance = RNG;
    if (HAL_RNG_Init(&hrng) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

}

/* SPI1 init function */
static void MX_SPI1_Init(void) {

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
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

}

/* TIM6 init function */
static void MX_TIM6_Init(void) {

    TIM_MasterConfigTypeDef sMasterConfig;

    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 0;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = 0;
    if (HAL_TIM_Base_Init(&htim6) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void) {

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PC3   ------> I2S2_SD
     PB10   ------> I2S2_CK
*/
static void MX_GPIO_Init(void) {

    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin
                             | Audio_RST_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : CS_I2C_SPI_Pin */
    GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
    GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PDM_OUT_Pin */
    GPIO_InitStruct.Pin = PDM_OUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PA0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : BOOT1_Pin */
    GPIO_InitStruct.Pin = BOOT1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : CLK_IN_Pin */
    GPIO_InitStruct.Pin = CLK_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                             Audio_RST_Pin */
    GPIO_InitStruct.Pin = LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin
                          | Audio_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
    GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : MEMS_INT2_Pin */
    GPIO_InitStruct.Pin = MEMS_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
FIL file;


extern ApplicationTypeDef Appli_state;
extern USBH_HandleTypeDef hUsbHostHS;
#define AUDIO_OUT_BUFFER_SIZE 8192
#define MAX_SONG_NAME_SIZE 100
#define MAX_NUMBER_SONGS 50
#define FW_PATH "0:/"
enum {
    BUFFER_OFFSET_NONE = 0,
    BUFFER_OFFSET_HALF,
    BUFFER_OFFSET_FULL,
};
static uint8_t buf_offs = BUFFER_OFFSET_NONE;
static uint32_t fpos = 0;
typedef enum {
    FINISHED = 0,
    PLAYING,
    PAUSED,
} PLAYER_STATE;
static PLAYER_STATE player_state1 = FINISHED;
int volume = 70;
FRESULT res;
char play_buffer[AUDIO_OUT_BUFFER_SIZE];
unsigned char input_buffer[AUDIO_OUT_BUFFER_SIZE];
unsigned char *input_buffer_pointer;
short spare_buffer[
        AUDIO_OUT_BUFFER_SIZE / 2]; // short ma 16 bitów a char 8, wiec pamięciowo taka sama wielkość jak play_buffer
short *spare_buffer_pointer;
int spare_buffer_offset = 0;
int bytes_in_input_buffer = 0;
MP3FrameInfo mp3_frame_info;
HMP3Decoder decoder;
int title_offset = 0;
char songs[MAX_NUMBER_SONGS][MAX_SONG_NAME_SIZE];
int songs_number = 0;

void decode(int play_offset) {
    uint32_t bytes_read = 0;
    //zczytujemy dane z pliku do momentu zapełnienia input_buffera
    if (f_read(&file, input_buffer_pointer, AUDIO_OUT_BUFFER_SIZE - bytes_in_input_buffer, (void *) &bytes_read) !=
        FR_OK) {
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        xprintf("file reading error\n");
    }
    //aktualizujemy ilość bajtów w input_bufferze
    bytes_in_input_buffer += bytes_read;
    //przesuwamy wskaźnik na początek
    input_buffer_pointer = input_buffer;
    //dopóki offset jest mniejszy niż połowa długości
    while (spare_buffer_offset < AUDIO_OUT_BUFFER_SIZE / 4) {
        int offset = MP3FindSyncWord((unsigned char *) input_buffer_pointer, bytes_in_input_buffer);
        if (offset == -1) {
            xprintf("Offset is -1 :(");
        }
        //Przesuwamy wskaźnik -> szukamy początku ramki
        bytes_in_input_buffer -= (offset);
        input_buffer_pointer += (offset);
        //dekodujemy ramkę (dane w input_buffer) do spare_buffer
        int is_error = MP3Decode(decoder, (unsigned char **) &input_buffer_pointer, (int *) &bytes_in_input_buffer,
                                 spare_buffer_pointer, 0);
        if (is_error) {
            xprintf("Error while decoding %d.\n", is_error);
            break;
        }
        //aktualizujemy ilość przetworzonych bajtów
        MP3GetLastFrameInfo(decoder, &mp3_frame_info);
        spare_buffer_offset += mp3_frame_info.outputSamps;
        spare_buffer_pointer = spare_buffer + spare_buffer_offset;

    }
    //przesuwamy dane w input_buffer na początek
    memcpy(input_buffer, input_buffer_pointer, bytes_in_input_buffer);
    //aktualizujemy położenie input_pointera tak, aby wskazywał tam, gdzie możemy już umiescić dane
    input_buffer_pointer = input_buffer + bytes_in_input_buffer;

    //wrzucamy dane ze spare_buffera do którego zbieraliśmy dekodowane mp3 do play_buffera odpowiedzialnego za granie
    memcpy(play_buffer + play_offset, spare_buffer, AUDIO_OUT_BUFFER_SIZE / 2);
    //po wrzuceniu danych do celu aktualizujemy spare_buffer, wysłaliśmy do play_buffera audio_out_buffer_size / 4 danych z powodu różnicy formatów, to co zostało to wrzucamy na początek
    memcpy(spare_buffer, &spare_buffer[AUDIO_OUT_BUFFER_SIZE / 4],
           (spare_buffer_offset - AUDIO_OUT_BUFFER_SIZE / 4) * 2);
    spare_buffer_offset -= AUDIO_OUT_BUFFER_SIZE / 4;
    spare_buffer_pointer = spare_buffer + spare_buffer_offset;
    buf_offs = BUFFER_OFFSET_NONE;
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void play_directory(const char* path) {
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn; /* This function is assuming non-Unicode cfg. */
    char buffer[MAX_SONG_NAME_SIZE];
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
	fno.lfname = lfn;
	fno.lfsize = sizeof(lfn);
#endif


    res = f_opendir(&dir, path); /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno); /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break; /* Break on error or end of dir */
            if (fno.fname[0] == '.' || fno.fname[0] == '_' ) continue; /* Ignore dot entry and file starting with _  */
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) { /* It is a directory */

            } else { /* It is a file. */
                sprintf(buffer, "%s/%s", path, fn);
                // Check if it is an mp3 file
                if (strcmp("MP3", get_filename_ext(buffer)) == 0) {
                    xprintf("Song2: %s\n", buffer);
                    play_new(buffer);
                }
            }
        }
    }
    return ;
}

void find_mp3_files() {
    FRESULT fr;     /* Return value */
    DIR dj;         /* Directory object */
    FILINFO fno;    /* File information */

    fr = f_findfirst(&dj, &fno, "", "*.MP3"); /* Start to search for photo files */
    while (fr == FR_OK && fno.fname[0]) {         /* Repeat while an item is found */
        // omit filenames with . and _ at the beginning
        xprintf("%s\n", fno.fname);                /* Print the object name */
        if (fno.fname[0] != '.' && fno.fname[0] != '_' ) {
            strcpy(songs[songs_number++], fno.fname);
        }
        fr = f_findnext(&dj, &fno);               /* Search for next item */
    }
    f_closedir(&dj);
}

void play_new(char *filename) {
    xprintf("Song3: %s\n", filename);
    title_offset = 0;
    res = f_open(&file, filename, FA_READ);
    if (res == FR_OK) {
        xprintf("mp3 file open OK\n");
    } else {
        xprintf("mp3 file open ERROR, res = %d\n", res);
        while (1);
    }
    spare_buffer_pointer = spare_buffer;
    input_buffer_pointer = input_buffer;
    BSP_AUDIO_OUT_Play((uint16_t * ) & play_buffer[0], AUDIO_OUT_BUFFER_SIZE);
    decode(0);
    decode(AUDIO_OUT_BUFFER_SIZE / 2);
    player_state1 = PLAYING;
    fpos = 0;
    buf_offs = BUFFER_OFFSET_NONE;

    for(;;) {
        char in = debug_inkey();
        if (in == 'w') {
            volume_up();
        } else if (in == 's') {
            volume_down();
        } else if (in == 'p') {
            pause();
        } else if (in == 'r') {
            resume();
        }

        if (player_state1 == PLAYING) {
            if (buf_offs == BUFFER_OFFSET_HALF) {
                decode(0);
            }
            if (buf_offs == BUFFER_OFFSET_FULL) {
                decode(AUDIO_OUT_BUFFER_SIZE / 2);
            }
            if (bytes_in_input_buffer == 0) {
                end_song();
                return;
            }
        }
    }
}

void end_song() {
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    f_close(&file);
    xprintf("End of song\n");
    player_state1 = FINISHED;
    spare_buffer_offset = 0;
    bytes_in_input_buffer = 0;
    buf_offs = BUFFER_OFFSET_NONE;
}

int volume_up() {
    if (player_state1 == PLAYING) {
        if (BSP_AUDIO_OUT_SetVolume(volume < 71 ? volume += 10 : volume) != AUDIO_OK) {
            xprintf("ERROR: Failed to set audio volume\n");
            return -1;
        }
        xprintf("w: volume up. Current volume: %d\n", volume);
    } else {
        xprintf("cannot resume in this state :( \n");
    }
    return 0;
}

int volume_down() {
    if (player_state1 == PLAYING) {
        if (BSP_AUDIO_OUT_SetVolume(volume > 9 ? volume -= 10 : volume) != AUDIO_OK) {
            xprintf("ERROR: Failed to set audio volume\n");
            return -1;
        }
        xprintf("s: volume down. Current volume: %d\n", volume);
    } else {
        xprintf("cannot resume in this state :( \n");
    }
    return 0;
}

void resume() {
    if (player_state1 == PAUSED) {
        xprintf("resuming...\n");
        player_state1 = PLAYING;
        BSP_AUDIO_OUT_SetVolume(volume);
        BSP_AUDIO_OUT_Resume();
    } else {
        xprintf("cannot resume in this state :( \n");
    }
}

void pause() {
    if (player_state1 == PLAYING) {
        xprintf("pausing...\n");
        player_state1 = PAUSED;
        BSP_AUDIO_OUT_SetVolume(0);
        BSP_AUDIO_OUT_Pause();
    } else {
        xprintf("cannot pause in this state :( \n");
    }
}

static void f_disp_res(FRESULT r) {
    switch (r) {
        case FR_OK:
            printf("FR_OK\n");
            break;
        case FR_DISK_ERR:
            printf("FR_DISK_ERR\n");
            break;
        case FR_INT_ERR:
            printf("FR_INT_ERR\n");
            break;
        case FR_NOT_READY:
            printf("FR_NOT_READY\n");
            break;
        case FR_NO_FILE:
            printf("FR_NO_FILE\n");
            break;
        case FR_NO_PATH:
            printf("FR_NO_PATH\n");
            break;
        case FR_INVALID_NAME:
            printf("FR_INVALID_NAME\n");
            break;
        case FR_DENIED:
            printf("FR_DENIED\n");
            break;
        case FR_EXIST:
            printf("FR_EXIST\n");
            break;
        case FR_INVALID_OBJECT:
            printf("FR_INVALID_OBJECT\n");
            break;
        case FR_WRITE_PROTECTED:
            printf("FR_WRITE_PROTECTED\n");
            break;
        case FR_INVALID_DRIVE:
            printf("FR_INVALID_DRIVE\n");
            break;
        case FR_NOT_ENABLED:
            printf("FR_NOT_ENABLED\n");
            break;
        case FR_NO_FILESYSTEM:
            printf("FR_NO_FILESYSTEM\n");
            break;
        case FR_MKFS_ABORTED:
            printf("FR_MKFS_ABORTED\n");
            break;
        case FR_TIMEOUT:
            printf("FR_TIMEOUT\n");
            break;
        case FR_LOCKED:
            printf("FR_LOCKED\n");
            break;
        case FR_NOT_ENOUGH_CORE:
            printf("FR_NOT_ENOUGH_CORE\n");
            break;
        case FR_TOO_MANY_OPEN_FILES:
            printf("FR_TOO_MANY_OPEN_FILES\n");
            break;
        case FR_INVALID_PARAMETER:
            printf("FR_INVALID_PARAMETER\n");
            break;
        default:
            printf("result code unknown (%d = 0x%X)\n", r, r);
    }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void) {
    buf_offs = BUFFER_OFFSET_HALF;
}

/**
* @brief  Calculates the remaining file size and new position of the pointer.
* @param  None
* @retval None
*/
void BSP_AUDIO_OUT_TransferComplete_CallBack(void) {
    buf_offs = BUFFER_OFFSET_FULL;
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t * ) & play_buffer[0], AUDIO_OUT_BUFFER_SIZE / 2);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
void StartDefaultTask(void const *argument) {
    /* init code for USB_HOST */
    MX_USB_HOST_Init();

    /* init code for FATFS */
    MX_FATFS_Init();

    /* USER CODE BEGIN 5 */

    HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_RESET);

    vTaskDelay(1000);

    xprintf("waiting for USB mass storage\n");

    do {
        xprintf(".");
        vTaskDelay(250);
    } while (Appli_state != APPLICATION_READY);


//    FRESULT res;
    decoder = MP3InitDecoder();

    if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, 44100) == 0) {
        xprintf("audio init OK\n");
    } else {
        xprintf("audio init ERROR\n");
    }

    /* Infinite loop */
    for (;;) {
        find_mp3_files();
        printf("Songs number: %d\n", songs_number);
        for(int i = 0; i < songs_number; i++) xprintf("%s\n", songs[i]);
        play_directory(FW_PATH);
        vTaskDelay(1);
    }
    /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM7) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while (1) {
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
