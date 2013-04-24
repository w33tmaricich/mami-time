#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xE1, 0x4A, 0x6F, 0xED, 0x0B, 0xF9, 0x43, 0x15, 0x88, 0xC3, 0x75, 0xFE, 0xF0, 0x48, 0xEE, 0xC1 }
PBL_APP_INFO(MY_UUID,
             "Mami Time", "Alexander Maricich",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
BmpContainer mami_container;
TextLayer timeLayer;

char timeText[32];

void handle_tick(AppContextRef ctx, PebbleTickEvent *event);
void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Mami Time");
  window_stack_push(&window, true /* Animated */);
  resource_init_current_app(&APP_RESOURCES);

  bmp_init_container(RESOURCE_ID_MAMI, &mami_container);
  layer_add_child(&window.layer, &mami_container.layer.layer);

  text_layer_init(&timeLayer, GRect(3, 134, 144-60 /*width*/, 168-50 /*height*/));
  text_layer_set_text_color(&timeLayer, GColorWhite);
  text_layer_set_background_color(&timeLayer, GColorClear);
  text_layer_set_font(&timeLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

  handle_tick(ctx, (PebbleTickEvent *) NULL);
  layer_add_child(&window.layer, &timeLayer.layer);
}

void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&mami_container);
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *event) {
  PblTm currentTime;
  get_time(&currentTime);

  string_format_time(timeText, sizeof(timeText), "%I:%M", &currentTime);
  text_layer_set_text(&timeLayer, timeText);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
