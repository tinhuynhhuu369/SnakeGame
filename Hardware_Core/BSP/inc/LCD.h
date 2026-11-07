#ifndef LCD_H
#define LCD_H

#include "main.h"

extern SRAM_HandleTypeDef hsram1;

// Color definitions
#define	ILI9806G_BLACK   0x0000
#define	ILI9806G_BLUE    0x001F
#define	ILI9806G_RED     0xF800
#define	ILI9806G_GREEN   0x07E0
#define ILI9806G_CYAN    0x07FF
#define ILI9806G_MAGENTA 0xF81F
#define ILI9806G_YELLOW  0xFFE0
#define ILI9806G_WHITE   0xFFFF

//  Orientation params
#define ILI9806G_MADCTL_MY  0x0080 // Row Address Order 
#define ILI9806G_MADCTL_MX  0x0040 // Column Address Order
#define ILI9806G_MADCTL_MV  0x0020 // Row/Column Exchange
#define ILI9806G_MADCTL_ML  0x0010 // Vertical Refresh Order
#define ILI9806G_MADCTL_BGR 0x0008 // RGB-BGR Order
#define ILI9806G_MADCTL_MH  0x0004 // Horizontal Refresh Order
#define ILI9806G_MADCTL_SS  0x0002 // Flip Horizontal(SS)
#define ILI9806G_MADCTL_GS  0x0001 // Flip Vertical (GS)

#define ILI9806G_ROTATION (ILI9806G_MADCTL_MY | ILI9806G_MADCTL_MV)

#define ILI9806G_WIDTH   800
#define ILI9806G_HEIGHT  480

void MX_FMC_Init(void);

void LCD_Init(void);
void LCD_Fill_Screen(uint16_t color);

#endif /* LCD_H */