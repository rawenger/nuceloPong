/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_MENU_H
#define NUCLEOPONG_MENU_H

#include <string>
#include <array>
#include <functional>

#include "nunchuk_controller.h"
#include "util.h"
#include "display.h"

class menu {
public:
    enum Direction : uint8_t {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT,
    };
    struct menu_item;

private:
    typedef const menu_item* item_link;
    typedef menu_item* _menuitem;

public:
    class menu_item {
    public:
        static constexpr point font_dim{.x = MENU_ITEMS_FONT[0], .y = MENU_ITEMS_FONT[1]};
        friend class menu;

        menu_item(std::string &&name, uint16_t x, uint16_t y)
                : display_text(name),
                  pos{x,y},
                  links{nullptr, nullptr, nullptr, nullptr}
        {}

        explicit menu_item(std::string &&name, const _menuitem& up=nullptr, const _menuitem& left=nullptr);

        ~menu_item();

        void center() { pos.x = (DISP_X_SIZE - display_text.size() * font_dim.x) / 2; }

        /// Single-direction joins
        void set_upper(item_link upper) { links[UP] = upper; }
        /// bidirectional joins
        void join_below(_menuitem const &lower);
        void join_right(_menuitem const &right);

        void assign_action(const std::function<void (void)> &func) { action = func; }

    private:
        void highlight() const;
        void un_highlight() const;


        point pos;
        const std::string display_text;

        std::array<item_link, 4> links;

        std::function<void (void)> action{[this]() { LOG("Error: menu item %s called select on default action handler\r\n", display_text.c_str()); }};
    };

    menu() : title("Menu") { init(); }
    menu(const std::string &title) : title(title) { init(); } // NOLINT(modernize-pass-by-value)
    menu(std::string &&title) : title(title) { init(); }
    ~menu(); // delete all menu_items

    void set_first(_menuitem first) { selection = this->first = first; }

    void show() const;
    void move_cursor(menu::Direction dir);
    void select();
    void hide() const;

private:
    void init();
    void display_items(item_link start) const;
    void erase_items(item_link start) const;


    const std::string title;
    point title_pos;

    item_link selection;

    _menuitem first;

    current_font item_font;

};




#endif //NUCLEOPONG_MENU_H
