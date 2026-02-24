#include "display.h"
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "ui/ui.h"
#include "modules/rtc/rtc.h"

static TFT_eSPI tft = TFT_eSPI();

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
void *draw_buf_1;

static uint8_t current_theme = UI_THEME_LIGHT;

#if LV_USE_LOG != 0
static void my_print(lv_log_level_t level, const char *buf)
{
    const char *level_str;

    switch (level)
    {
    case LV_LOG_LEVEL_ERROR:
        level_str = "ERROR";
        break;
    case LV_LOG_LEVEL_WARN:
        level_str = "WARN";
        break;
    case LV_LOG_LEVEL_INFO:
        level_str = "INFO";
        break;
    case LV_LOG_LEVEL_TRACE:
        level_str = "TRACE";
        break;
    case LV_LOG_LEVEL_USER:
        level_str = "USER";
        break;
    default:
        level_str = "UNKNOWN";
        break;
    }

    Serial0.printf("[%s] %s", level_str, buf);
    Serial0.flush();
}
#endif

static void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *pixelmap)
{
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)pixelmap, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

static void my_touchpad_read(lv_indev_t *indev_driver, lv_indev_data_t *data)
{
    data->state = LV_INDEV_STATE_REL;
}

static uint32_t my_tick_get_cb(void)
{
    return millis();
}

void Task_LVGL(void *pvParameters)
{
    tft.begin();
    tft.setRotation(0);

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print);
#endif

    static lv_display_t *disp;
    disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    draw_buf_1 = heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    lv_display_set_buffers(disp, draw_buf_1, NULL, DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(disp, my_disp_flush);

    static lv_indev_t *indev;
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);

    lv_tick_set_cb(my_tick_get_cb);

    ui_init();

    lv_obj_t *sysmon = lv_sysmon_create(lv_display_get_default());
    lv_sysmon_show_performance(NULL);

    lv_timer_create(clock_timer_cb, 1000, NULL);

    while (1)
    {
        lv_timer_handler();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void display_init()
{
    xTaskCreatePinnedToCore(Task_LVGL,
                            "Task_LVGL",
                            1024 * 10,
                            NULL,
                            3,
                            NULL,
                            1);
}

uint8_t display_get_theme()
{
    return current_theme;
}

void display_set_theme(uint8_t theme)
{
    current_theme = theme;
    ui_theme_set(current_theme);
}

void display_toggle_theme()
{
    current_theme = (current_theme == UI_THEME_DARK) ? UI_THEME_LIGHT : UI_THEME_DARK;
    ui_theme_set(current_theme);
}
