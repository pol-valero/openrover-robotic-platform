#include <Arduino_GFX_Library.h>
#include <lvgl.h>

#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "screenConfig.h"

//NOTICE: The code present in this file was mostly extracted from the configuration example that the ESP32-8048 touchscreen manufacturer provided: http://pan.jczn1688.com/directlink/1/ESP32%20module/4.3inch_ESP32-8048S043.zip.
//The provided configuration example had the necessary screen driver and bus configurations to display a LVGL example.

#define TFT_BL 2
#define GFX_BL DF_GFX_BL // default backlight pin

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
);

//  ST7262 IPS LCD 800x480
Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
    bus,
    800 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
    480 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 14000000 /* prefer_speed */, true /* auto_flush */);

#include "touch.h"


static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    if (touch_has_signal())
    {
        if (touch_touched())
        {
        data->state = LV_INDEV_STATE_PR;

        data->point.x = touch_last_x;
        data->point.y = touch_last_y;
        }
        else if (touch_released())
        {
        data->state = LV_INDEV_STATE_REL;
        }
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

void initScreen() {

    // Init Display
    gfx->begin();

    //We set the backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->fillScreen(BLACK);
    delay(500);
    lv_init();
    delay(10);
    touch_init();

    screenWidth = gfx->width();
    screenHeight = gfx->height();

    // Allocate memory for display buffer
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 4, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

    if (!disp_draw_buf)
    {
    //Serial.println("LVGL disp_draw_buf allocate failed!");
    }
    else
    {
        lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight / 4);

        /* Initialize the display */
        lv_disp_drv_init(&disp_drv);

        disp_drv.hor_res = screenWidth;
        disp_drv.ver_res = screenHeight;
        disp_drv.flush_cb = my_disp_flush;
        disp_drv.draw_buf = &draw_buf;
        lv_disp_drv_register(&disp_drv);

        /* Initialize the (dummy) input device driver */
        static lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = my_touchpad_read;
        lv_indev_drv_register(&indev_drv);

        //UI FUNCTIONS TO CALL AND SHOW THE LVGL UI

        //lv_demo_widgets();
        ui_init();

        //////////////////////////////

        //Change to home tab
        lv_tabview_set_act(ui_TabView, 1, LV_ANIM_OFF);

        //Put initial notifications label message
        lv_label_set_text(ui_NotificationsLabel, "REMINDER: Check RcValues before enabling radio RC.");

  }

}

void runScreen() {
    //Runs the screen, calling the lv_task_handler function every 5ms

    static unsigned long previousMillis = 0;

    if (millis() - previousMillis >= 5) {
        previousMillis = millis();
        lv_task_handler();  /* let the GUI do its work */
    }
}