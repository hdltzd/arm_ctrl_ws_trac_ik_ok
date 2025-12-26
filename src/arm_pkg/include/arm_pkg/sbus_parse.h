#ifndef SBUS_PARSE_H
#define SBUS_PARSE_H    

#include <stdint.h>

#define SWITCH_UP 326
#define SWITCH_MIDDLE 992
#define SWITCH_DOWN 1659


class sbus_app
{
private:
    /* data */
public:
    sbus_app(/* args */);
    ~sbus_app();

    //遥杆
    double left_x; 
    double left_y;
  
    double right_x;
    double right_y;

    //拨杆
    uint8_t left_putter_a; 
    uint8_t right_putter_b;

    //圆形按钮
    double left_button_vra;
    double right_button_vrb;
    
    uint8_t connected; //0: not connected, 1: connected
};

sbus_app::sbus_app(/* args */)
{
}

sbus_app::~sbus_app()
{
}





#endif
