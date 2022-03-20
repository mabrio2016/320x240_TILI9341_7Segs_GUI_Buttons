// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       320x240_TILI9341_7Segs_GUI_Buttons.ino
    Created:	3/20/2022 9:38:27 AM
    Author:     OFFICEPC\Marco Flag
*/

// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       320x240_TILI9341_GUI_Test.ino
    Created:	3/18/2022 7:33:36 PM
    Author:     OFFICEPC\Marco Flag
*/

//#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "XPT2046_Touchscreen.h"
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include "Math.h"

#define BLACK			ILI9341_BLACK      	//0x0000 /*   0,   0,   0 */
#define NAVY			ILI9341_NAVY       	//0x000F /*   0,   0, 128 */
#define DARKGREEN		ILI9341_DARKGREEN  	//0x03E0 /*   0, 128,   0 */
#define DARKCYAN		ILI9341_DARKCYAN   	//0x03EF /*   0, 128, 128 */
#define MAROON			ILI9341_MAROON     	//0x7800 /*   128, 0,   0 */
#define PURPLE			ILI9341_PURPLE     	//0x780F /*   128,  0, 128 */
#define OLIVE			ILI9341_OLIVE      	//0x7BE0 /*   128, 128,  0 */
#define LIGHTGREY		ILI9341_LIGHTGREY  	//0xC618 /*   192, 192, 192 */
#define DARKGREY		ILI9341_DARKGREY   	//0x7BEF /*   128, 128, 128 */
#define BLUE			ILI9341_BLUE       	//0x001F /*   0,     0, 255 */
#define GREEN			ILI9341_GREEN      	//0x07E0 /*   0,   255,   0 */
#define CYAN			ILI9341_CYAN       	//0x07FF /*   0,   255, 255 */
#define RED				ILI9341_RED        	//0xF800 /*   255,   0,   0 */
#define MAGENTA			ILI9341_MAGENTA    	//0xF81F /*   255,   0, 255 */
#define YELLOW			ILI9341_YELLOW     	//0xFFE0 /*   255, 255,   0 */
#define WHITE			ILI9341_WHITE      	//0xFFFF /*   255, 255, 255 */
#define ORANGE			ILI9341_ORANGE     	//0xFD20 /*   255, 165,   0 */
#define GREENYELLOW		ILI9341_GREENYELLOW //0xAFE5 /*   173, 255,  47 */
#define PINK			ILI9341_PINK       	//0xF81F


#define displyBackground BLACK


//TFT SPI display - TouchScreen wiring//
#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11 //Also connected Touchscreen "T_DIN" pin
#define TFT_CLK 13  //Also connected Touchscreen "T_CLK" pin
#define TFT_RST 8
#define TFT_MISO 12 //Also connected Touchscreen "T_DO" pin

#define T_CS 7    //Touchscreen T_CS pin (Chip select)
#define T_IRQ 2  //Touchscreen T_IRQ pin (Chip interrupt request)

#define ROTATION 3

/*Start Touch Paramiters*/
#define MINPRESSURE 150
#define MAXPRESSURE 1000
    // calibration values // These values were captured after Touchscreen calibration.
float xCalM = -0.09, yCalM = 0.07; // gradients
float xCalC = 332.57, yCalC = -14.94; // y axis crossing points
TS_Point p; // This Class is defined in the XPT2046 library for Touch Controller
/*End Touch Paramiters*/

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST); // Initiate the TFT disply
XPT2046_Touchscreen ts(T_CS); // Initiate the TouchScreen.

/*Start of GUI Objects Paremiters*/
    //7 Sements rendering position//
#define SEG_FRAME_X 20
#define SEG_FRAME_Y 30 
    //Reset button parameters//
#define rst_BUTTON_X 285
#define rst_BUTTON_Y 205
#define rst_BUTTON_W 70
#define rst_BUTTON_H 40
boolean clickButton = false;
/*End of GUI Objects Paremiters*/

uint32_t Number = 999800;
int Steep = 1;

/* Start of Classes definition*/
class ScreenPoint {
public:
    int16_t x;
    int16_t y;

    ScreenPoint() {
    } //Default contructor

    ScreenPoint(int16_t xIn, int16_t yIn) {
        x = xIn;
        y = yIn;
    }
};
class SevenSegDisplay {
private:
    const int digit_array[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},  // 0
    {0, 1, 1, 0, 0, 0, 0},  // 1
    {1, 1, 0, 1, 1, 0, 1},  // 2
    {1, 1, 1, 1, 0, 0, 1},  // 3
    {0, 1, 1, 0, 0, 1, 1},  // 4
    {1, 0, 1, 1, 0, 1, 1},  // 5
    {1, 0, 1, 1, 1, 1, 1},  // 6
    {1, 1, 1, 0, 0, 0, 0},  // 7
    {1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 0, 1, 1}   // 9
    };
    int previous_digit[6] = { 0,0,0,0,0,0 }; // Used to confirm if new digit is difernt compared with the old one.

    uint32_t digit;
    uint16_t color;
    int size = 2;
    uint8_t spacing_x = 30;

protected:
    int pos_x;
    int pos_y;

    //void render_digit(uint16_t pos_x, uint16_t pos_y, uint32_t digit, uint16_t color) {  //Small size not implemented yet
    //    // loop through 7 segments
    //    for (uint8_t i = 0; i < 7; i++) {
    //        bool seg_on = digit_array[digit][i];
    //        // if seg_on is true draw segment
    //        if (seg_on) {
    //            switch (i) {
    //            case 0:
    //                tft.fillRoundRect(2 + pos_x, 0 + pos_y, 9, 3, 2, color); // SEG a
    //                break;
    //            case 1:
    //                tft.fillRoundRect(10 + pos_x, 2 + pos_y, 3, 9, 2, color); // SEG b
    //                break;
    //            case 2:
    //                tft.fillRoundRect(10 + pos_x, 12 + pos_y, 3, 9, 2, color); // SEG c
    //                break;
    //            case 3:
    //                tft.fillRoundRect(2 + pos_x, 20 + pos_y, 9, 3, 2, color); // SEG d
    //                break;
    //            case 4:
    //                tft.fillRoundRect(0 + pos_x, 12 + pos_y, 3, 9, 2, color); // SEG e
    //                break;
    //            case 5:
    //                tft.fillRoundRect(0 + pos_x, 2 + pos_y, 3, 9, 2, color); // SEG f
    //                break;
    //            case 6:
    //                tft.fillRoundRect(2 + pos_x, 10 + pos_y, 9, 3, 2, color); // SEG g
    //                break;
    //            }
    //            seg_on = false;
    //        }
    //    }
    //}

public:
    //Contructor
    SevenSegDisplay(int pos_x_, int pos_y_, uint16_t color) {
        this->pos_x = pos_x_;
        this->pos_y = pos_y_;
        this->digit = digit;
        this->color = color;
    };
    void render_digit_segments_big(int pos_x, int pos_y, uint16_t digit, uint16_t color) {
        // loop through 7 segments
        for (uint8_t i = 0; i < 7; i++) {
            bool seg_on = digit_array[digit][i];
            // if seg_on is true draw segment
            if (seg_on) {
                switch (i) {
                case 0:
                    tft.fillRoundRect(4 + pos_x, 0 + pos_y, 18, 6, 4, color); // SEG a;
                    break;
                case 1:
                    tft.fillRoundRect(20 + pos_x, 4 + pos_y, 6, 20, 4, color); // SEG b
                    break;
                case 2:
                    tft.fillRoundRect(20 + pos_x, 26 + pos_y, 6, 20, 4, color); // SEG c
                    break;
                case 3:
                    tft.fillRoundRect(4 + pos_x, 44 + pos_y, 18, 6, 4, color); // SEG d
                    break;
                case 4:
                    tft.fillRoundRect(0 + pos_x, 26 + pos_y, 6, 20, 4, color); // SEG e
                    break;
                case 5:
                    tft.fillRoundRect(0 + pos_x, 4 + pos_y, 6, 20, 4, color); // SEG f
                    break;
                case 6:
                    tft.fillRoundRect(4 + pos_x, 22 + pos_y, 18, 6, 4, color); // SEG g
                    break;
                }
                seg_on = false;
            }
        }
    }
    void render_digit_segments(uint32_t digit) {
        uint8_t digit_array[] = { digit / 100000 % 10, digit / 10000 % 10, digit / 1000 % 10, digit / 100 % 10, digit / 10 % 10, digit % 10 };
        if (digit > 99999) {
            if (previous_digit[6] != digit_array[0])
            {
                tft.fillRect(pos_x, pos_y - 5, 30, 60, WHITE);
                render_digit_segments_big(pos_x, pos_y, digit_array[0], color);
                previous_digit[6] = digit_array[0];
            }
        }
        else {
            tft.fillRect(pos_x, pos_y - 5, 30, 60, WHITE);
        }
        if (digit > 9999) {
            if (previous_digit[5] != digit_array[1]) {
                tft.fillRect(pos_x + spacing_x, pos_y - 5, 45, 60, WHITE);
                render_digit_segments_big(pos_x + spacing_x, pos_y, digit_array[1], color);
                previous_digit[5] = digit_array[1];
            }
        }
        else {
            tft.fillRect(pos_x + spacing_x, pos_y - 5, 30, 60, WHITE);
        }
        if (digit > 999) {
            if (previous_digit[4] != digit_array[2]) {
                tft.fillRect(pos_x + (spacing_x * 2), pos_y - 5, 45, 60, WHITE);
                render_digit_segments_big(pos_x + (spacing_x * 2), pos_y, digit_array[2], color);
                previous_digit[4] = digit_array[2];
                tft.fillRoundRect(32 + (pos_x + (spacing_x * 2)), 42 + pos_y, 8, 8, 4, RED); // Decimal point
            }
        }
        else {
            tft.fillRect(pos_x + (spacing_x * 2), pos_y - 5, 30, 60, WHITE);
            tft.fillRect(30 + pos_x + (spacing_x * 2), pos_y - 5, 15, 60, WHITE); // Used to fill the Decimal point space.
        }
        if (digit > 99) {
            if (previous_digit[3] != digit_array[3]) {
                tft.fillRect(pos_x + (spacing_x * 3 + (spacing_x / 2)), pos_y - 5, 30, 60, WHITE);
                render_digit_segments_big(pos_x + (spacing_x * 3 + (spacing_x / 2)), pos_y, digit_array[3], color);
                previous_digit[3] = digit_array[3];
            }
        }
        else {
            tft.fillRect(pos_x + (spacing_x * 3 + (spacing_x / 2)), pos_y - 5, 30, 60, WHITE);
        }
        if (digit > 9) {
            if (previous_digit[2] != digit_array[4]) {
                tft.fillRect(pos_x + (spacing_x * 4 + (spacing_x / 2)), pos_y - 5, 30, 60, WHITE);
                render_digit_segments_big(pos_x + (spacing_x * 4 + (spacing_x / 2)), pos_y, digit_array[4], color);
                previous_digit[2] = digit_array[4];
            }
        }
        else {
            tft.fillRect(pos_x + (spacing_x * 4 + (spacing_x / 2)), pos_y - 5, 30, 60, WHITE);
        }
        if (previous_digit[1] != digit_array[5]) {
            tft.fillRect(pos_x + (spacing_x * 5 + (spacing_x / 2)), pos_y - 5, 30, 60, WHITE);
            render_digit_segments_big(pos_x + (spacing_x * 5 + (spacing_x / 2)), pos_y, digit_array[5], color);
            previous_digit[1] = digit_array[5];  // previous_Number must be an array with digits 
        }
    }
    void drawFrame(uint16_t frameColor) //7 segment frame//
    {
        tft.fillRect(SEG_FRAME_X - 4, SEG_FRAME_Y - 9, 15 + 8 + 30 * 6, 60 + 8, WHITE);
        tft.drawRect(SEG_FRAME_X - 4, SEG_FRAME_Y - 9, 15 + 8 + 30 * 6, 60 + 8, frameColor); // (x, y, w, h)
        tft.drawRect(SEG_FRAME_X - 2, SEG_FRAME_Y - 7, 15 + 4 + 30 * 6, 60 + 4, frameColor);
    }
};
class up_downButton {
private:

    int X_arrwsPodition;
    int Y_arrwsPodition;
    int arrwWidth;
    int arrwHeight;
    uint16_t frameColor;
    uint16_t errowColor;

    typedef struct {
        int X;
        int Y;
        int width;
        int height;
    }rect;
    typedef struct {
        int Min;
        int Max;
        int Init;
        int Step;
    }arrowRec;

    rect arrwFrameR;
    rect upArrwR;
    rect dwnArrwR;
    TS_Point upArrwTip, upArrwRBase, upArrwLBase;
    TS_Point dwnArrwTip, dwnArrwRBase, dwnArrwLBase;

public:
    //Constructor
    up_downButton(int X_arrwsPodition, int Y_arrwsPodition, int arrwWidth, int arrwHeight, uint16_t frameColor, uint16_t errowColor) {
        this->X_arrwsPodition = X_arrwsPodition;
        this->Y_arrwsPodition = Y_arrwsPodition;
        this->arrwWidth = arrwWidth;
        this->arrwHeight = arrwHeight;
        this->frameColor = frameColor;
        this->errowColor = errowColor;

        arrwFrameR = { X_arrwsPodition, Y_arrwsPodition, arrwWidth, arrwHeight * 2 };
        upArrwR = { arrwFrameR.X, arrwFrameR.Y, arrwWidth, arrwHeight };
        dwnArrwR = { arrwFrameR.X, arrwFrameR.Y + arrwFrameR.height / 2, arrwWidth, arrwHeight };
    };
    void littleArrows()
    {
        tft.drawRect(arrwFrameR.X + 1, arrwFrameR.Y + 1, arrwFrameR.width, arrwFrameR.height, frameColor);
        //tft.fillRect(arrwFrameR.X + 2, arrwFrameR.Y + 2, arrwFrameR.width - 2, arrwFrameR.height - 2, YELLOW);
        upArrwTip.x = upArrwR.X + upArrwR.width / 2;
        upArrwTip.y = upArrwR.Y + 5;
        upArrwLBase.x = upArrwR.X + 5;
        upArrwLBase.y = upArrwR.Y + upArrwR.height - 5;
        upArrwRBase.x = upArrwR.X + upArrwR.width - 5;
        upArrwRBase.y = upArrwR.Y + upArrwR.height - 5;
        tft.fillTriangle(upArrwTip.x, upArrwTip.y, upArrwLBase.x, upArrwLBase.y, upArrwRBase.x, upArrwRBase.y, errowColor);
        dwnArrwTip.x = dwnArrwR.X + dwnArrwR.width / 2;
        dwnArrwTip.y = dwnArrwR.Y + dwnArrwR.height - 5;
        dwnArrwLBase.x = dwnArrwR.X + 5;
        dwnArrwLBase.y = dwnArrwR.Y + 5;
        dwnArrwRBase.x = dwnArrwR.X + dwnArrwR.width - 5;
        dwnArrwRBase.y = dwnArrwR.Y + 5;
        tft.fillTriangle(dwnArrwTip.x, dwnArrwTip.y, dwnArrwLBase.x, dwnArrwLBase.y, dwnArrwRBase.x, dwnArrwRBase.y, errowColor);
    }

    boolean isErrowPressed(ScreenPoint sp_) {
        if ((sp_.x > upArrwR.X) && (sp_.x < (upArrwR.X + upArrwR.width))) {
            if ((sp_.y > upArrwR.Y) && (sp_.y <= (upArrwR.Y + upArrwR.height))) {
                Serial.println("up Errow btn hit");
                //upArrowHandler();
                return false;
            }
        }
        if ((sp_.x > dwnArrwR.X) && (sp_.x < (dwnArrwR.X + dwnArrwR.width))) {
            if ((sp_.y > dwnArrwR.Y) && (sp_.y <= (dwnArrwR.Y + dwnArrwR.height))) {
                Serial.println("down Errow btn hit");
                //downArrowHandler();
                return true;
            }
        }
    }
};
/* End of Classes definition*/

/* Start of Classe instances*/
SevenSegDisplay SevenSeg1 = SevenSegDisplay(SEG_FRAME_X, SEG_FRAME_Y, BLACK);
up_downButton up_downButton_1 = up_downButton(223, 22, 32, 32, WHITE, BLUE);
Adafruit_GFX_Button Reset_BTN;
/* End of Classe instances*/

/* Start of Functions definition*/
ScreenPoint getScreenCoords(int16_t x, int16_t y) {
    int16_t xCoord = round((x * xCalM) + xCalC);
    int16_t yCoord = round((y * yCalM) + yCalC);
    if (xCoord < 0) xCoord = 0;
    if (xCoord >= tft.width()) xCoord = tft.width() - 1;
    if (yCoord < 0) yCoord = 0;
    if (yCoord >= tft.height()) yCoord = tft.height() - 1;
    return(ScreenPoint(xCoord, yCoord));
}
boolean buttonPressed_Handler(int xPos_, int yPos_, int buttonWidth_, int buttonHeight_, ScreenPoint sp_) {
    if ((sp_.x > (xPos_ - (buttonWidth_ / 2)) && (sp_.x < (xPos_ + (buttonWidth_ / 2))))) {
        if ((sp_.y > yPos_ - (buttonHeight_ / 2)) && (sp_.y <= (yPos_ + (buttonHeight_ / 2)))) {
            //Serial.println("Click btn hit");
            Reset_BTN.drawButton(true);
            Number = 0;
        }
    }
    else {
        //return(false);
    }
}
/* End of Functions definition*/

void setup()
{
    Serial.begin(9600);
    tft.begin();
    tft.setRotation(ROTATION);
    tft.fillScreen(displyBackground);
    ts.begin();
    ts.setRotation(ROTATION);
    SevenSeg1.drawFrame(RED);
    up_downButton_1.littleArrows();
    Reset_BTN.initButton(&tft, rst_BUTTON_X, rst_BUTTON_Y, rst_BUTTON_W, rst_BUTTON_H, CYAN, BLUE, YELLOW, "Reset", 2, 2);
    Reset_BTN.drawButton();
}

// Add the main program code into the continuous loop() function
void loop()
{
    //Number = Number + Steep;
    //SevenSeg1.render_digit_segments(Number);
    //if (Number >= 999999) {
    //    Number = 000000;
    //}
    ScreenPoint sp;
    if (ts.touched()) {
        p = ts.getPoint();
        sp = getScreenCoords(p.x, p.y);
        Steep = 1;
        SevenSeg1.render_digit_segments(Number);
        if (!up_downButton_1.isErrowPressed(sp)) {
            if (Number > 999998) {
                Number = 999999;
                Steep = 0;
            }
            else Steep = 1;;
            Number = Number + Steep;
        };
        if (up_downButton_1.isErrowPressed(sp)) {
            if (Number < 1) {
                Number = 000000;
                Steep = 0;
            }
            else Steep = 1;
            Number = Number - Steep;
        };
        if (buttonPressed_Handler(rst_BUTTON_X, rst_BUTTON_Y, rst_BUTTON_W, rst_BUTTON_H, sp)) {
            clickButton = true;
        }
    }
    else if (clickButton == true) { //If nothing is touched unclick the button (retur it to the original color)
        Reset_BTN.drawButton();
        clickButton = false;
    }
}