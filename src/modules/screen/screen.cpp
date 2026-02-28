#include "screen.h"
#include "ui/ui.h"

lv_obj_t *current_screen = nullptr;
bool screen_switch_requested = false;

void screen_tick()
{
    if (screen_switch_requested)
    {
        lv_obj_t *current = lv_scr_act();
        if (current == ui_ScreenWeather)
        {
            lv_scr_load_anim(ui_ScreenTable, LV_SCR_LOAD_ANIM_FADE_OUT, 250, 0, false);
        }
        else
        {
            lv_scr_load_anim(ui_ScreenWeather, LV_SCR_LOAD_ANIM_FADE_OUT, 250, 0, false);
        }
        screen_switch_requested = false;
    }
}
