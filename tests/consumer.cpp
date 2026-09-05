#include <lwcgl/lwcgl.h>
#include <cstring>
int main(){return std::strcmp(lwcglGetVersion(),"3.4.2") == 0 ? 0 : 1;}
