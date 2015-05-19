#include "enviroment.h"

#include <strings.h> //bzero()

/*==============公有牌==============*/
Public_cards::Public_cards()
{
    bzero(_point, sizeof(_point));
    bzero(_color, sizeof(_color));
    _card_num = 0;
}

void Public_cards::add_one_card(card_color color, card_point point)
{
    _color[_card_num] = color;
    _point[_card_num] = point;
    _card_num++;
}

const card_color *Public_cards::get_colors()
{
    return _color;
}

const card_point *Public_cards::get_points()
{
    return _point;
}

int Public_cards::get_card_num()
{
    return _card_num;
}


