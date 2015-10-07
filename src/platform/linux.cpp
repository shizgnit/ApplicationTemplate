#include "mylo.hpp"

my::shared_ptr<my::object> gbackground;

static my::trace::console cs;

int main(int argc, char **argv) {

  DEBUG_SCOPE;

  DEBUG_TRACE<<"starting application"<<my::endl;

  char *assets = "/home/codeneko/Dropbox/Development/projects/ApplicationTemplate/assets/";
  my::asset::manager((void*)assets);


//  gbackground = my::primitive::quad(256, 256);
//  my::image *img2 = new my::png;
//  *img2 << my::asset("textures/landscape2.png");
//  gbackground->xy_projection(img2, 0, 0, 256, 256);	

  my::obj gapple;
  gapple.search_path = "models/obj";
  gapple << my::asset("models/obj/apple.obj");
	
  exit(0);
}
