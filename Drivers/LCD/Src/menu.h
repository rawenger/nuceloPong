/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_MENU_H
#define NUCLEOPONG_MENU_H

#include <string>
#include <array>
#include <functional>
#include <unordered_map>

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
        CLICK,
    };
    struct menu_item;

    static constexpr Direction get_opposite(Direction dir) { return static_cast<Direction>(dir ^ 0b01); }

private:
    typedef const menu_item* _cmenuitem;
    typedef menu_item* _menuitem;
public:
    class menu_item {
    private:
        typedef const std::function<void (_cmenuitem)> action_type;
    public:
        static constexpr point font_dim{.x = MENU_ITEMS_FONT[0], .y = MENU_ITEMS_FONT[1]};
        friend class menu;

        menu_item(std::string &&name, uint16_t x, uint16_t y)
                : display_text(name),
                  pos{x,y},
                  links{nullptr, nullptr, nullptr, nullptr}
        {CPP_LOG(display_text << ": " << display_text.length());}

        explicit menu_item(std::string &&name, const _menuitem& up=nullptr, const _menuitem& left=nullptr);

        ~menu_item();

        void center() { pos.x = (DISP_X_SIZE - display_text.length() * font_dim.x) / 2; }

        /// Single-direction joins
        void set_upper(_cmenuitem upper) { links[UP] = upper; }
        /// bidirectional joins
        void join_below(const _menuitem &lower);
        void join_right(const _menuitem &right);

        int get_value() const { return *value; }
        void update_value(int val) const;
        void default_value(int val);

        void assign_action(action_type &func, Direction dir=CLICK) { actions.erase(dir); actions.insert({dir, func}); }

    private:
        void highlight() const;
        void un_highlight() const;

        point pos;
        const std::string display_text;
        std::string *valstr{nullptr};

        std::array<_cmenuitem, 4> links;

        // don't have to refactor this into a template, so only ints are supported for now
        bool has_value{false};
        int *value{nullptr}; // also needs to retain const property, so ... pointer (not enough time to refactor lol)

        std::unordered_map<Direction, action_type> actions {
            {CLICK,
             [this](_cmenuitem) {
                 CPP_LOG("menu called select on default action handler for item " << display_text);
                    return;
                }
            },
        };

    };

    menu() : title("Menu") { init(); }
    menu(const std::string &title) : title(title) { init(); } // NOLINT(modernize-pass-by-value)
    menu(std::string &&title) : title(title) { init(); }
    ~menu(); // delete all menu_items

    void set_first(_cmenuitem first) { selection = this->first = first; }

    void show();
    void move_cursor(menu::Direction dir);
    void select();
    void hide();

    bool is_hidden() const { return hidden; }

private:
    void init();
    void display_items(_cmenuitem start) const;
    void erase_items(_cmenuitem start) const;


    const std::string title;
    point title_pos;

    _cmenuitem selection;

    _cmenuitem first;

    bool hidden{true};

//    current_font item_font;

};

typedef const menu::menu_item* menuitem;



#endif //NUCLEOPONG_MENU_H
