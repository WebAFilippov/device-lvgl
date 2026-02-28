SET(SOURCES ui_ScreenWeather.c
    ui_ScreenTable.c
    ui_theme_manager.c
    ui_themes.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_0_png.c
    fonts/ui_font_Geologica14.c
    fonts/ui_font_Geologica16.c
    fonts/ui_font_Geologica64.c
    fonts/ui_font_Geologica78.c
    fonts/ui_font_Geologica24.c)

add_library(ui ${SOURCES})
