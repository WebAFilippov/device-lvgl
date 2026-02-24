SET(SOURCES ui_Screen.c
    ui_theme_manager.c
    ui_themes.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_0_png.c
    fonts/ui_font_Geologica14.c
    fonts/ui_font_Geologica16.c
    fonts/ui_font_Geologica64.c)

add_library(ui ${SOURCES})
