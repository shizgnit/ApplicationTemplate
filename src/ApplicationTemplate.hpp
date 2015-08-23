void on_startup(void *asset_manager=NULL);
void on_resize(int width, int height);
void on_draw();
void on_touch_press(float normalized_x, float normalized_y);
void on_touch_release(float normalized_x, float normalized_y);
void on_touch_drag(float normalized_x, float normalized_y);
void on_touch_zoom_in();
void on_touch_zoom_out();
