void on_startup(void *asset_manager);
void on_resize(int width, int height);
void on_draw();

void on_proc();

void on_touch_press(float x, float y);
void on_touch_release(float x, float y);
void on_touch_drag(float x, float y);
void on_touch_scale(float x, float y, float z);
void on_touch_zoom_in();
void on_touch_zoom_out();

void on_key_down(int key);
void on_key_up(int key);

void on_mouse_move(long int x, long int y);