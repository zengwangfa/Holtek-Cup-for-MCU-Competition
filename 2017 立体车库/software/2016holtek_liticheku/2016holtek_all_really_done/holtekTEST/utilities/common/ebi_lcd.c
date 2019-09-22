/*********************************************************************************************************//**
 * @file    ebi_lcd.c
 * @version $Rev:: 923          $
 * @date    $Date:: 2015-09-16 #$
 * @brief   This file provides a set of functions needed to manage the
 *          communication between EBI peripheral and LCD.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ebi_lcd.h"
#include "font.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup EBI_LCD EBI_LCD
  * @brief This file provides a set of functions needed to manage the
  *        communication between EBI peripheral and LCD.
  * @{
  */


/* Private constants ---------------------------------------------------------------------------------------*/
/** @defgroup EBI_LCD_Private_Define EBI LCD private definitions
  * @{
  */
#if (LCD_DIRECTION == LCD_NORMAL)
  #define EC (320)
  #define EP (240)
  #define MAC (0x60)
#elif (LCD_DIRECTION == LCD_LEFT_90)
  #define EC (240)
  #define EP (320)
  #define MAC (0x00)
#elif (LCD_DIRECTION == LCD_RIGHT_90)
  #define EC (240)
  #define EP (320)
  #define MAC (0xC0)
#elif (LCD_DIRECTION == LCD_ROTATE_180)
  #define EC (320)
  #define EP (240)
  #define MAC (0xA0)
#endif
/**
  * @}
  */

/* Private variables ---------------------------------------------------------------------------------------*/
/** @defgroup EBI_LCD_Private_Variable EBI LCD private variables
  * @{
  */
static u16 Color_Text = 0x0000;  // default: Black
static u16 Color_Back = 0xFFFF;  // default: White
static vu32 DelayTime = 0;
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------------------------------------*/
static void _delay(u32 nCount);

/* Global functions ----------------------------------------------------------------------------------------*/
/** @defgroup EBI_LCD_Exported_Functions EBI LCD exported functions
  * @{
  */
/*********************************************************************************************************//**
  * @brief  Initialize peripherals used by the EBI LCD driver.
  * @retval None
  ***********************************************************************************************************/
void LCD_Init(void)
{
  EBI_InitTypeDef  EBI_InitStructure;

  /*  Enable AFIO clock                                                                                     */
  /*  Enable EBI & EBI CS port clock                                                                        */
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  LCD_EBI_RST_CLK(CKCUClock) = 1;
  CKCUClock.Bit.EBI          = 1;
  CKCUClock.Bit.AFIO         = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Configure EBI pins & LCD RESET pin                                                                     */
  HT32F_DVB_GPxConfig(LCD_EBI_CS_GPIO_ID, LCD_EBI_CS_AFIO_PIN, LCD_EBI_CS_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_OE_GPIO_ID, LCD_EBI_OE_AFIO_PIN, LCD_EBI_OE_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_WE_GPIO_ID, LCD_EBI_WE_AFIO_PIN, LCD_EBI_WE_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_RS_GPIO_ID, LCD_EBI_RS_AFIO_PIN, LCD_EBI_RS_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD0_GPIO_ID, LCD_EBI_AD0_AFIO_PIN, LCD_EBI_AD0_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD1_GPIO_ID, LCD_EBI_AD1_AFIO_PIN, LCD_EBI_AD1_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD2_GPIO_ID, LCD_EBI_AD2_AFIO_PIN, LCD_EBI_AD2_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD3_GPIO_ID, LCD_EBI_AD3_AFIO_PIN, LCD_EBI_AD3_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD4_GPIO_ID, LCD_EBI_AD4_AFIO_PIN, LCD_EBI_AD4_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD5_GPIO_ID, LCD_EBI_AD5_AFIO_PIN, LCD_EBI_AD5_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD6_GPIO_ID, LCD_EBI_AD6_AFIO_PIN, LCD_EBI_AD6_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD7_GPIO_ID, LCD_EBI_AD7_AFIO_PIN, LCD_EBI_AD7_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD8_GPIO_ID, LCD_EBI_AD8_AFIO_PIN, LCD_EBI_AD8_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD9_GPIO_ID, LCD_EBI_AD9_AFIO_PIN, LCD_EBI_AD9_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD10_GPIO_ID, LCD_EBI_AD10_AFIO_PIN, LCD_EBI_AD10_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD11_GPIO_ID, LCD_EBI_AD11_AFIO_PIN, LCD_EBI_AD11_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD12_GPIO_ID, LCD_EBI_AD12_AFIO_PIN, LCD_EBI_AD12_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD13_GPIO_ID, LCD_EBI_AD13_AFIO_PIN, LCD_EBI_AD13_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD14_GPIO_ID, LCD_EBI_AD14_AFIO_PIN, LCD_EBI_AD14_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_AD15_GPIO_ID, LCD_EBI_AD15_AFIO_PIN, LCD_EBI_AD15_AFIO_MODE);
  HT32F_DVB_GPxConfig(LCD_EBI_RST_GPIO_ID, LCD_EBI_RST_GPIO_PIN, LCD_EBI_RST_AFIO_MODE);

  /* Set output data of RESET as high                                                                       */
  GPIO_SetOutBits(GPIO_PORT[LCD_EBI_RST_GPIO_ID], LCD_EBI_RST_GPIO_PIN);

  /* Configure the direction of RESET as output mode                                                        */
  GPIO_DirectionConfig(GPIO_PORT[LCD_EBI_RST_GPIO_ID], LCD_EBI_RST_GPIO_PIN, GPIO_DIR_OUT);

  /*  EBI Configuration                                                                                     */
  EBI_InitStructure.EBI_Bank = EBI_BANK_0;
  EBI_InitStructure.EBI_Mode = EBI_MODE_D16;
#if (EBI_FUN_BYTELAND == 1)
  EBI_InitStructure.EBI_ByteLane = EBI_BYTELANE_DISABLE;
  EBI_InitStructure.EBI_ByteLanePolarity = EBI_BYTELANEPOLARITY_LOW;
#endif
  EBI_InitStructure.EBI_IdleCycle = EBI_IDLECYCLE_DISABLE;
#if(EBI_FUN_ASYNCREADY == 1)
  EBI_InitStructure.EBI_AsynchronousReady = EBI_ASYNCHRONOUSREADY_DISABLE;
  EBI_InitStructure.EBI_ARDYTimeOut = EBI_ARDYTIMEOUT_DISABLE;
  EBI_InitStructure.EBI_ReadySignalPolarity = EBI_READYSIGNALPOLARITY_LOW;
#endif
  EBI_InitStructure.EBI_ChipSelectPolarity = EBI_CHIPSELECTPOLARITY_LOW;
  EBI_InitStructure.EBI_AddressLatchPolarity = EBI_ADDRESSLATCHPOLARITY_LOW;
  EBI_InitStructure.EBI_WriteEnablePolarity = EBI_WRITEENABLEPOLARITY_LOW;
  EBI_InitStructure.EBI_ReadEnablePolarity = EBI_READENABLEPOLARITY_LOW;
  EBI_InitStructure.EBI_IdleCycleTime = 0;
  EBI_InitStructure.EBI_AddressSetupTime = 0;
  EBI_InitStructure.EBI_AddressHoldTime = 0;
#if defined USE_HT32F1654_DVB // reduce speed since pin shared by analog switch
  EBI_InitStructure.EBI_WriteSetupTime = 3;
  EBI_InitStructure.EBI_WriteStrobeTime = 3;
  EBI_InitStructure.EBI_WriteHoldTime = 3;
#else
  EBI_InitStructure.EBI_WriteSetupTime = 2;
  EBI_InitStructure.EBI_WriteStrobeTime = 2;
  EBI_InitStructure.EBI_WriteHoldTime = 1;
#endif
  EBI_InitStructure.EBI_ReadSetupTime = 2;
  EBI_InitStructure.EBI_ReadStrobeTime = 5;
  EBI_InitStructure.EBI_ReadHoldTime = 1;
  EBI_InitStructure.EBI_PageMode = EBI_PAGEMODE_DISABLE;
  EBI_InitStructure.EBI_PageLength = EBI_PAGELENGTH_4;
  EBI_InitStructure.EBI_PageHitMode = EBI_PAGEHITMODE_ADDINC;
  EBI_InitStructure.EBI_PageAccessTime = 0xF;
  EBI_InitStructure.EBI_PageOpenTime = 0xFF;
  EBI_Init(&EBI_InitStructure);

  EBI_Cmd(EBI_BANK_0, ENABLE);
}

/*********************************************************************************************************//**
  * @brief  Configures the LCD by the EBI lcd driver.
  * @retval None
  ***********************************************************************************************************/
void LCD_Config(void)
{
  _delay(10);

  /* RESET LCD                                                                                              */
  GPIO_ClearOutBits(GPIO_PORT[LCD_EBI_RST_GPIO_ID], LCD_EBI_RST_GPIO_PIN);
  _delay(10);
  GPIO_SetOutBits(GPIO_PORT[LCD_EBI_RST_GPIO_ID], LCD_EBI_RST_GPIO_PIN);
  _delay(15);

  EBI_LCD->EBI_LCD_REG = 0xE0;  // PLL enable
  EBI_LCD->EBI_LCD_RAM = 0x01;
  _delay(10);

  /* Driving ability Setting                                                                                */
  LCD_WriteReg(0xE2, 0x00);
  LCD_WriteReg(0xE4, 0x01);
  LCD_WriteReg(0xE5, 0x10);
  LCD_WriteReg(0xE6, 0x01);
  LCD_WriteReg(0xE7, 0x10);
  LCD_WriteReg(0xE8, 0x70);
  LCD_WriteReg(0xF2, 0x00);
  LCD_WriteReg(0xEA, 0x00);
  LCD_WriteReg(0xEB, 0x20);
  LCD_WriteReg(0xEC, 0x3C);
  LCD_WriteReg(0xED, 0xC8);
  LCD_WriteReg(0xE9, 0x38);
  LCD_WriteReg(0xF1, 0x01);
  LCD_WriteReg(0x2E, 0x89);
  LCD_WriteReg(0x29, 0x8F);
  LCD_WriteReg(0x2B, 0x02);

  /* Gamma 2.2 Setting                                                                                      */
  LCD_WriteReg(0x40, 0x00);
  LCD_WriteReg(0x41, 0x00);
  LCD_WriteReg(0x42, 0x00);
  LCD_WriteReg(0x43, 0x15);
  LCD_WriteReg(0x44, 0x13);
  LCD_WriteReg(0x45, 0x3f);
  LCD_WriteReg(0x47, 0x55);
  LCD_WriteReg(0x48, 0x00);
  LCD_WriteReg(0x49, 0x12);
  LCD_WriteReg(0x4A, 0x19);
  LCD_WriteReg(0x4B, 0x19);
  LCD_WriteReg(0x4C, 0x16);
  LCD_WriteReg(0x50, 0x00);
  LCD_WriteReg(0x51, 0x2c);
  LCD_WriteReg(0x52, 0x2a);
  LCD_WriteReg(0x53, 0x3F);
  LCD_WriteReg(0x54, 0x3F);
  LCD_WriteReg(0x55, 0x3F);
  LCD_WriteReg(0x56, 0x2a);
  LCD_WriteReg(0x57, 0x7e);
  LCD_WriteReg(0x58, 0x09);
  LCD_WriteReg(0x59, 0x06);
  LCD_WriteReg(0x5A, 0x06);
  LCD_WriteReg(0x5B, 0x0d);
  LCD_WriteReg(0x5C, 0x1F);
  LCD_WriteReg(0x5D, 0xFF);

  /* Power Voltage Setting                                                                                  */
  LCD_WriteReg(0x1B, 0x1A);
  LCD_WriteReg(0x1A, 0x02);
  LCD_WriteReg(0x24, 0x61);
  LCD_WriteReg(0x25, 0x5C);

  /* VCOM offset                                                                                            */
  LCD_WriteReg(0x18, 0x36);
  LCD_WriteReg(0x19, 0x01);
  LCD_WriteReg(0x23, 0x62);
  LCD_WriteReg(0x1F, 0x88);
  _delay(5);
  LCD_WriteReg(0x1F, 0x80);
  _delay(5);
  LCD_WriteReg(0x1F, 0x90);
  _delay(5);
  LCD_WriteReg(0x1F, 0xD4);
  _delay(5);

  /* 262k or 65k color selection                                                                            */
  LCD_WriteReg(0x17, 0x05);

  LCD_WriteReg(0x36, 0x09);

  /* Display ON Setting                                                                                     */
  LCD_WriteReg(0x28, 0x38);
  _delay(40);
  LCD_WriteReg(0x28, 0x3C);

  /* Set GRAM Area                                                                                          */
  // Column Start
  LCD_WriteReg(0x02, 0x00);
  LCD_WriteReg(0x03, 0x00);

  // Column End
  LCD_WriteReg(0x04, ((EC - 1) & 0xFF00) >> 8);
  LCD_WriteReg(0x05, ((EC - 1) & 0xFF));

  // Row Start
  LCD_WriteReg(0x06, 0x00);
  LCD_WriteReg(0x07, 0x00);

  // Row End
  LCD_WriteReg(0x08, ((EP - 1) & 0xFF00) >> 8);
  LCD_WriteReg(0x09, ((EP - 1) & 0xFF));

  /* Show Direction                                                                                         */
  LCD_WriteReg(0x16, MAC);

  LCD_Clear(Black);

  _delay(200);
}

/*********************************************************************************************************//**
  * @brief  Write value to the selected LCD register.
  * @param  LCD_Reg_Index: address of the selected register.
  * @param  LCD_Reg_Value: value of to write the selected register.
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteReg(u32 LCD_Reg_Index, u32 LCD_Reg_Value)
{
  EBI_LCD->EBI_LCD_REG = LCD_Reg_Index;
  EBI_LCD->EBI_LCD_RAM = LCD_Reg_Value;
}

/*********************************************************************************************************//**
  * @brief  Set the cursor position.
  * @param  X_Location: specify the X position.
  * @param  Y_Location: specify the Y position.
  * @retval None
  ***********************************************************************************************************/
void LCD_StarterSet(u32 X_Location, u32 Y_Location)
{
  LCD_WriteReg(REG6, ((X_Location >> 8) & 0xff));
  LCD_WriteReg(REG7, (X_Location & 0xff));

  LCD_WriteReg(REG2, ((Y_Location >> 8) & 0xff));
  LCD_WriteReg(REG3, (Y_Location & 0xff));
}

/*********************************************************************************************************//**
  * @brief  Prepare to write data to LCD RAM.
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteRAMPrior(void)
{
  EBI_LCD->EBI_LCD_REG = REG34;
}

/*********************************************************************************************************//**
  * @brief  Write to the LCD RAM.
  * @param  RGB_Set: the pixel color in RGB mode (5-6-5).
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteRAM(u32 RGB_Set)
{
  EBI_LCD->EBI_LCD_RAM = RGB_Set;
}

/*********************************************************************************************************//**
  * @brief  Clear the whole LCD.
  * @param  Color: the color to be the background.
  * @retval None
  ***********************************************************************************************************/
void LCD_Clear(u16 Color)
{
  u32 i = LCD_Height * LCD_Width;

  LCD_StarterSet(0, 0);

  /* Prepare to write GRAM                                                                                  */
  LCD_WriteRAMPrior();

  while (i--) EBI_LCD->EBI_LCD_RAM = Color;
}

/*********************************************************************************************************//**
  * @brief  Set the background color.
  * @param  Color: background color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
void LCD_BackColorSet(u16 Color)
{
  Color_Back = Color;
}

/*********************************************************************************************************//**
  * @brief  Set the text color.
  * @param  Color: text color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
void LCD_TextColorSet(u16 Color)
{
  Color_Text = Color;
}

/*********************************************************************************************************//**
  * @brief  Display one character (16 dots width, 24 dots height).
  * @param  Line_Num: Where to display the character, should be LINE0 ~ LINE9.
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  ***********************************************************************************************************/
void LCD_CharDisplay(u32 Line_Num, u32 Column, u32 Ascii)
{
  Ascii -= 32;
  LCD_CharDraw(Line_Num, Column, (u16 *)&ASCII_Font_Table[Ascii * 24]);
}

/*********************************************************************************************************//**
  * @brief  Disable the Display.
  * @retval None
  ***********************************************************************************************************/
void LCD_DisplayOff(void)
{
  /* Display Off                                                                                            */
  LCD_WriteReg(REG40, 0);
}

/*********************************************************************************************************//**
  * @brief  Enable the Display.
  * @retval None
  ***********************************************************************************************************/
void LCD_DisplayOn(void)
{
  /* Display On                                                                                             */
  LCD_WriteReg(REG40, 0x3C);
}

/*********************************************************************************************************//**
  * @brief  Display the maximum 20 characters on one line.
  * @param  Line_Num: Where to display the character, should be LINE0 ~ LINE9.
  * @param  Sptr: point to the string data.
  * @retval None
  ***********************************************************************************************************/
void LCD_StringLineDisplay(u32 Line_Num, char *Sptr)
{
  u32 i = 0;
  u16 reference_column = 0;

  /* Send character by character on LCD                                                                     */
  while ((*Sptr != 0) & (i < 20))
  {
    LCD_CharDisplay(Line_Num, reference_column, *Sptr);
    /* Increment the column position by 16, because character is size of 16x24                              */
    reference_column += 16;
    /* Point to the next character                                                                          */
    Sptr++;

    /* Increase the character counter                                                                       */
    i++;
  }
}

/*********************************************************************************************************//**
  * @brief  Draw one character.
  * @param  X_Location: the position of display line, should be LINE0 ~ LINE9.
  * @param  Y_Location: the start position of character.
  * @param  Cptr: point to font data of character.
  * @retval None
  ***********************************************************************************************************/
void LCD_CharDraw(u32 X_Location, u32 Y_Location, u16 *Cptr)
{
  u32 xid = X_Location;
  u32 i = 0, j = 0;

  LCD_StarterSet(X_Location, Y_Location);

  for (i = 0; i < Font_Raw; i++)
  {
    LCD_WriteRAMPrior();  // Get ready to write GRAM
    for (j = 0; j < Font_Column; j++)
    {
      if ((Cptr[i] & (1 << j)) == 0x00)
      {
        LCD_WriteRAM(Color_Back);
      }
      else
      {
        LCD_WriteRAM(Color_Text);
      }
    }
    xid++;
    LCD_StarterSet(xid, Y_Location);
  }
}

/*********************************************************************************************************//**
  * @brief  Display a line.
  * @param  X_Location: specify the X position.
  * @param  Y_Location: specify the Y position.
  * @param  Length: line length.
  * @param  Direction: line direction.
  *   This parameter can be one of the following values:
  *     @arg Vertical
  *     @arg Horizontal
  * @retval None
  ***********************************************************************************************************/
void LCD_LineDraw(u32 X_Location, u32 Y_Location, u32 Length, u32 Direction)
{
  s32 i = 0;

  LCD_StarterSet(X_Location, Y_Location);

  if (Direction == Horizontal)
  {
    LCD_WriteRAMPrior();  // Get ready to write GRAM
    for (i = Length; i > 0; i--)
    {
      LCD_WriteRAM(Color_Text);
    }
  }
  else
  {
    for (i = Length; i > 0; i--)
    {
      LCD_WriteRAMWord(Color_Text);
      X_Location++;
      LCD_StarterSet(X_Location, Y_Location);
    }
  }
}

/*********************************************************************************************************//**
  * @brief  Display a circle.
  * @param  X_Location: specify the X position.
  * @param  Y_Location: specify the Y position.
  * @param  Radius: radius of the circle.
  * @retval None
  ***********************************************************************************************************/
void LCD_CircleDraw(u32 X_Location, u32 Y_Location, u32 Radius)
{
  s32  DV;  // Decision Variable
  u32  X_Value;      // Current X Value
  u32  Y_Value;      // Current Y Value

  DV = 3 - (Radius << 1);
  X_Value = 0;
  Y_Value = Radius;

  while (X_Value <= Y_Value)
  {
    LCD_StarterSet(X_Location + X_Value, Y_Location + Y_Value);
    LCD_WriteRAMWord(Color_Text);

    LCD_StarterSet(X_Location + X_Value, Y_Location - Y_Value);
    LCD_WriteRAMWord(Color_Text);

    LCD_StarterSet(X_Location - X_Value, Y_Location + Y_Value);
    LCD_WriteRAMWord(Color_Text);

    LCD_StarterSet(X_Location - X_Value, Y_Location - Y_Value);
    LCD_WriteRAMWord(Color_Text);

    LCD_StarterSet(X_Location + Y_Value, Y_Location + X_Value);
    LCD_WriteRAMWord(Color_Text);

    LCD_StarterSet(X_Location + Y_Value, Y_Location - X_Value);
    LCD_WriteRAMWord(Color_Text);

    LCD_StarterSet(X_Location - Y_Value, Y_Location + X_Value);
    LCD_WriteRAMWord(Color_Text);

    LCD_StarterSet(X_Location - Y_Value, Y_Location - X_Value);
    LCD_WriteRAMWord(Color_Text);

    if (DV < 0)
    {
      DV += (X_Value << 2) + 6;
    }
    else
    {
      DV += ((X_Value - Y_Value) << 2) + 10;
      Y_Value--;
    }
    X_Value++;
  }
}

/*********************************************************************************************************//**
  * @brief  Draw a rectangle.
  * @param  X_Location: X position.
  * @param  Y_Location: Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @retval None
  ***********************************************************************************************************/
void LCD_RectDraw(u32 X_Location, u32 Y_Location, u32 Height, u32 Width)
{
  LCD_LineDraw(X_Location, Y_Location, Width, Horizontal);
  LCD_LineDraw((X_Location + Height), Y_Location, Width, Horizontal);

  LCD_LineDraw(X_Location, Y_Location, Height, Vertical);
  LCD_LineDraw(X_Location, Y_Location + Width - 1, Height, Vertical);
}

/*********************************************************************************************************//**
  * @brief  Draw a picture.
  * @param  X_Location: X position.
  * @param  Y_Location: Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @param  Pptr: point to pixel data of picture.
  * @retval None
  ***********************************************************************************************************/
void LCD_PicDraw(u8 X_Location, u16 Y_Location, u8 Height, u16 Width, uc8 *Pptr)
{
  u32 xid = 0;
  u32 ImgAdds = 0;
  u32 yid = 0;
  u32 i = 0, j = 0, color = 0;

  xid = Height + X_Location;
  yid = Y_Location;
  LCD_StarterSet(xid, yid);

  for (i = 0; i < Height; i++)
  {
    LCD_WriteRAMPrior();  // Get ready to write GRAM

    for (j = 0; j < Width; j++)
    {
      ImgAdds = (i * Width * 2) + (j * 2);

      color = Pptr[ImgAdds] << 8 | (Pptr[ImgAdds + 1]);

      LCD_WriteRAM(color);
    }
    xid--;
    LCD_StarterSet(xid, yid);
  }
}

/*********************************************************************************************************//**
  * @brief  Write 1 word to the LCD RAM.
  * @param  RGB_Set: the pixel color in RGB mode (5-6-5).
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteRAMWord(u32 RGB_Set)
{
  LCD_WriteRAMPrior();

  LCD_WriteRAM(RGB_Set);
}

/*********************************************************************************************************//**
  * @brief  Set Display Area.
  * @param  Column:
  * @param  Page:
  * @param  Height:
  * @param  Width:
  * @retval None
  ***********************************************************************************************************/
void LCD_SetDisplayArea(u16 Column, u16 Page, u16 Height, u16 Width)
{
  EBI_LCD->EBI_LCD_REG = REG2;
  EBI_LCD->EBI_LCD_RAM = (Page >> 8) & 0xFF;

  EBI_LCD->EBI_LCD_REG = REG3;
  EBI_LCD->EBI_LCD_RAM = (Page & 0xFF);

  EBI_LCD->EBI_LCD_REG = REG4;
  EBI_LCD->EBI_LCD_RAM = ((Page + Width-1) >> 8) & 0xFF;

  EBI_LCD->EBI_LCD_REG = REG5;
  EBI_LCD->EBI_LCD_RAM = (Page + Width-1) & 0xFF;

  EBI_LCD->EBI_LCD_REG = REG6;
  EBI_LCD->EBI_LCD_RAM = (Column >> 8) & 0xFF;

  EBI_LCD->EBI_LCD_REG = REG7;
  EBI_LCD->EBI_LCD_RAM = (Column & 0xFF);

  EBI_LCD->EBI_LCD_REG = REG8;
  EBI_LCD->EBI_LCD_RAM = ((Column + Height-1) >> 8) & 0xFF;

  EBI_LCD->EBI_LCD_REG = REG9;
  EBI_LCD->EBI_LCD_RAM = (Column + Height-1) & 0xFF;
}
/**
  * @}
  */


/* Private functions ---------------------------------------------------------------------------------------*/
/** @defgroup EBI_LCD_Private_Functions EBI LCD private functions
  * @{
  */
/*********************************************************************************************************//**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  ***********************************************************************************************************/
static void _delay(u32 nCount)
{
  vu32 i;

  for (i = 0; i < 10000 * nCount; i++)
  {
  }
}
/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
