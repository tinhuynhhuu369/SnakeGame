#include "LCD.h"
#include "GPIO.h"
#include "TimerDriver.h"

#define LCD_CMD_ADDR           (*(volatile uint16_t *)0x68000000UL)
#define LCD_DATA_ADDR          (*(volatile uint16_t *)0x68000002UL)

static inline void LCD_WRITE_CMD(uint8_t c)     { LCD_CMD_ADDR  = c; }
static inline void LCD_WRITE_DATA(uint16_t d)   { LCD_DATA_ADDR  = d; }
static inline uint16_t LCD_READ_DATA(void)      { return LCD_DATA_ADDR; }

SRAM_HandleTypeDef hsram1;

/* FMC initialization function */
void MX_FMC_Init(void)
{
  FMC_NORSRAM_TimingTypeDef Timing = {0};

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK3;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.NBLSetupTime = 0;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  hsram1.Init.MaxChipSelectPulse = DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 3;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 4;
  Timing.DataHoldTime = 3;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }
}

void ILI9806G_LCD_Fill_Rect(uint16_t x0, uint16_t y0,
                            uint16_t x1, uint16_t y1,
                            uint16_t color)
{
    LCD_WRITE_CMD(0x2A); // Column Address Set
    LCD_WRITE_DATA(x0 >> 8); LCD_WRITE_DATA(x0 & 0xFF);
    LCD_WRITE_DATA(x1 >> 8); LCD_WRITE_DATA(x1 & 0xFF);

    LCD_WRITE_CMD(0x2B); // Page (Row) Address Set
    LCD_WRITE_DATA(y0 >> 8); LCD_WRITE_DATA(y0 & 0xFF);
    LCD_WRITE_DATA(y1 >> 8); LCD_WRITE_DATA(y1 & 0xFF);

    LCD_WRITE_CMD(0x2C); // Memory Write

    uint32_t n = (uint32_t)(x1 - x0 + 1) * (y1 - y0 + 1);

    while (n--) LCD_WRITE_DATA(color);
}

void LCD_Fill_Screen(uint16_t color)
{
    ILI9806G_LCD_Fill_Rect(0, 0, ILI9806G_WIDTH - 1, ILI9806G_HEIGHT - 1, color);
}

static void ILI9806G_LCD_115FPC_B_Config(void)
{
	//************* Start Initial Sequence **********//
	LCD_WRITE_CMD(0x00FF); // EXTC Command Set enable register 
	LCD_WRITE_DATA(0x00FF); 
	LCD_WRITE_DATA(0x0098); 
	LCD_WRITE_DATA(0x0006); 
	
    LCD_WRITE_CMD(0x00BA); // SPI Interface Setting 
    LCD_WRITE_DATA(0x0060); 
	
	LCD_WRITE_CMD(0x00BC); // GIP 1 
	LCD_WRITE_DATA(0x0001); 
	LCD_WRITE_DATA(0x000E); 
	LCD_WRITE_DATA(0x0061); 
	LCD_WRITE_DATA(0x00FB); 
	LCD_WRITE_DATA(0x0010); 
	LCD_WRITE_DATA(0x0010); 
	LCD_WRITE_DATA(0x000B); 
	LCD_WRITE_DATA(0x000F); 
	LCD_WRITE_DATA(0x002E); 
	LCD_WRITE_DATA(0x0073); 
	LCD_WRITE_DATA(0x00FF); 
	LCD_WRITE_DATA(0x00FF); 
	LCD_WRITE_DATA(0x000E); 
	LCD_WRITE_DATA(0x000E); 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0003); 
	LCD_WRITE_DATA(0x0066); 
	LCD_WRITE_DATA(0x0063); 
	LCD_WRITE_DATA(0x0001); 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0000); 
	
	LCD_WRITE_CMD(0x00BD); // GIP 2 
	LCD_WRITE_DATA(0x0001); 
	LCD_WRITE_DATA(0x0023); 
	LCD_WRITE_DATA(0x0045); 
	LCD_WRITE_DATA(0x0067); 
	LCD_WRITE_DATA(0x0001); 
	LCD_WRITE_DATA(0x0023); 
	LCD_WRITE_DATA(0x0045); 
	LCD_WRITE_DATA(0x0067); 
	
	LCD_WRITE_CMD(0x00BE); // GIP 3 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0021); 
	LCD_WRITE_DATA(0x00AB); 
	LCD_WRITE_DATA(0x0060); 
	LCD_WRITE_DATA(0x0022); 
	LCD_WRITE_DATA(0x0022); 
	LCD_WRITE_DATA(0x0022); 
	LCD_WRITE_DATA(0x0022); 
	LCD_WRITE_DATA(0x0022); 
	
	LCD_WRITE_CMD(0x00C7); // Vcom 
	LCD_WRITE_DATA(0x006F); 
	
	LCD_WRITE_CMD(0x00ED); // EN_volt_reg 
	LCD_WRITE_DATA(0x007F); 
	LCD_WRITE_DATA(0x000F); 
	LCD_WRITE_DATA(0x0000); 
	
	LCD_WRITE_CMD(0x00C0); // Power Control 1
	LCD_WRITE_DATA(0x0037); 
	LCD_WRITE_DATA(0x000B); 
	LCD_WRITE_DATA(0x000A); 
	
	LCD_WRITE_CMD(0x00FC); // LVGL 
	LCD_WRITE_DATA(0x000A); 
	
	LCD_WRITE_CMD(0x00DF); // Engineering Setting 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0020); 
	
	LCD_WRITE_CMD(0x00F3); // DVDD Voltage Setting 
	LCD_WRITE_DATA(0x0074); 
	
	LCD_WRITE_CMD(0x00B4); // Display Inversion Control 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0000); 
	
	LCD_WRITE_CMD(0x00F7); // 480x800
	LCD_WRITE_DATA(0x0082);
	
	LCD_WRITE_CMD(0x0036);
	LCD_WRITE_DATA(ILI9806G_ROTATION);
	
	LCD_WRITE_CMD(0x00B1); // Frame Rate 
	LCD_WRITE_DATA(0x0000); 
	LCD_WRITE_DATA(0x0012); 
	LCD_WRITE_DATA(0x0013); 
	
	LCD_WRITE_CMD(0x00F2); //Panel Timing Control 
	LCD_WRITE_DATA(0x0080); 
	LCD_WRITE_DATA(0x005B); 
	LCD_WRITE_DATA(0x0040); 
	LCD_WRITE_DATA(0x0028); 
	
	LCD_WRITE_CMD(0x00C1); // Power Control 2 
	LCD_WRITE_DATA(0x0017); 
	LCD_WRITE_DATA(0x007D); 
	LCD_WRITE_DATA(0x007A); 
	LCD_WRITE_DATA(0x0020); 
	
	LCD_WRITE_CMD(0x00E0); 
	LCD_WRITE_DATA(0x0000); //P1 
	LCD_WRITE_DATA(0x0011); //P2 
	LCD_WRITE_DATA(0x001C); //P3 
	LCD_WRITE_DATA(0x000E); //P4 
	LCD_WRITE_DATA(0x000F); //P5 
	LCD_WRITE_DATA(0x000C); //P6 
	LCD_WRITE_DATA(0x00C7); //P7 
	LCD_WRITE_DATA(0x0006); //P8 
	LCD_WRITE_DATA(0x0006); //P9 
	LCD_WRITE_DATA(0x000A); //P10 
	LCD_WRITE_DATA(0x0010); //P11 
	LCD_WRITE_DATA(0x0012); //P12 
	LCD_WRITE_DATA(0x000A); //P13 
	LCD_WRITE_DATA(0x0010); //P14 
	LCD_WRITE_DATA(0x0002); //P15 
	LCD_WRITE_DATA(0x0000); //P16 
	
	LCD_WRITE_CMD(0x00E1); 
	LCD_WRITE_DATA(0x0000); //P1 
	LCD_WRITE_DATA(0x0012); //P2 
	LCD_WRITE_DATA(0x0018); //P3 
	LCD_WRITE_DATA(0x000C); //P4 
	LCD_WRITE_DATA(0x000F); //P5 
	LCD_WRITE_DATA(0x000A); //P6 
	LCD_WRITE_DATA(0x0077); //P7 
	LCD_WRITE_DATA(0x0006); //P8 
	LCD_WRITE_DATA(0x0007); //P9 
	LCD_WRITE_DATA(0x000A); //P10 
	LCD_WRITE_DATA(0x000E); //P11 
	LCD_WRITE_DATA(0x000B); //P12 
	LCD_WRITE_DATA(0x0010); //P13 
	LCD_WRITE_DATA(0x001D); //P14 
	LCD_WRITE_DATA(0x0017); //P15 
	LCD_WRITE_DATA(0x0000); //P16 

    LCD_WRITE_CMD(0x003A);
    LCD_WRITE_DATA(0x0055);
	
	LCD_WRITE_CMD(0x0035); //Tearing Effect ON 
	LCD_WRITE_DATA(0x0000); 
	
	LCD_WRITE_CMD(0x0011); //Exit Sleep 
	HAL_Delay(120); 
	
	LCD_Fill_Screen(ILI9806G_WHITE);
	
	LCD_WRITE_CMD(0x0029); // Display On
	//    LCD_WRITE_CMD(0x0030); // Partial Mode On
}

void LCD_Init(void)
{
    // GPIO configs

    // Config FMC
    MX_FMC_Init();

    // Turn on Backlight
    HAL_GPIO_WritePin(LCD_BackLight_GPIO_Port, LCD_BackLight_Pin, GPIO_PIN_SET);
    
    // Reset LCD
    HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(120);

    ILI9806G_LCD_115FPC_B_Config();
}