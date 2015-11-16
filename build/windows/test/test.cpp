#include "mylo.hpp"

void main(int argc, char **argv) {

//  my::vector<my::string> relative_vector_storage;

//  relative_vector_storage[3] = "foo";
//  relative_vector_storage[6] = "bar";
//  relative_vector_storage[1] = "baz";

  my::map<my::string, my::string> map;

  map["foo1"] = "foobar1";
  map["f_2"] = "foobar2";
  map["f_3"] = "foobar3";

//  printf("map: %s\n", map["foobarbaz"].c_str());

  my::map<my::string, my::string>::iterator it = map.begin();
  for (; it != map.end(); it++) {
    printf("%s: %s\n", it->first.c_str(), it->second.c_str());
  }

  printf("done\n");
}