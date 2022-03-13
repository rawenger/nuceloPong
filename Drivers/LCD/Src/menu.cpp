/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "menu.h"

void menu::init()
{
//    auto resume = std::make_shared<menu_item>("Resume");
//    auto options = std::make_shared<menu_item>("Options");
//    auto quit = std::make_shared<menu_item>("Quit");
//    auto restart = std::make_shared<menu_item>("Restart");

    title_pos.x = (DISP_X_SIZE - title.size() * MENU_TITLE_FONT[0]) / 2;
    title_pos.y = MENU_TITLE_Y;

    auto resume = new menu_item("Resume", 0, MENU_ITEMS_Y);
    resume->center();
    auto options = new menu_item("Options", resume);
    constexpr int i = DISP_Y_SIZE - 40 - menu_item::font_dim.y;
    auto quit = new menu_item("Quit", 20, DISP_Y_SIZE - 40 - menu_item::font_dim.y);
    auto restart = new menu_item("Restart", nullptr, quit);

    options->join_below(quit);
    // only want to join these in one direction
    restart->links[UP] = options;

    selection = first = resume;
}


menu::~menu() {
    LOG("Deleting menu\r\n");
    delete first;
}

void menu::show() const {
    MENU_TITLE_COLOR;
    LCD_setFont(MENU_TITLE_FONT);
    LCD_print(title.c_str(), title_pos.x, title_pos.y);

    selection->highlight();

    MENU_UNSELECTED_COLOR;
    LCD_setFont(MENU_ITEMS_FONT);
    display_items(first->links[DOWN]);
    display_items(first->links[RIGHT]);
}

void menu::move_cursor(menu::Direction dir) {
    if (selection->links[dir]) {
        selection->un_highlight();
        selection = selection->links[dir];
        selection->highlight();
    }
}

void menu::display_items(item_link start) const {
    if (!start)
        return;

    LCD_print(start->display_text.c_str(), start->pos.x, start->pos.y);

    display_items(start->links[DOWN]);
    display_items(start->links[RIGHT]);
}


menu::menu_item::menu_item(std::string &&name, const _menuitem& up, const _menuitem& left)
        : display_text(name),
          links{up, nullptr, left, nullptr}
{
    this->pos.x = (DISP_X_SIZE - display_text.size() * font_dim.x) / 2;
    if (up) {
        up->links[DOWN] = this;
        this->pos.y = up->pos.y + font_dim.y + MENU_ITEM_SPACING;
    } else if (left) {
        left->links[RIGHT] = this;
        this->pos.x = DISP_X_SIZE - left->pos.x - font_dim.x * display_text.size();
        this->pos.y = left->pos.y;
    }
}

menu::menu_item::~menu_item() {
    // as of C++ 17, deleting nullptr has no effect, so this *should* be safe to do...
    // (as long as two items don't share the same "down" or "right")
    delete links[DOWN];
    delete links[RIGHT];
    LOG("Deleted menu item \"%s\"\r\n", this->display_text.c_str());
}

void menu::menu_item::join_below(_menuitem const &lower) {
    this->links[DOWN] = lower;
    lower->links[UP] = this;
}

void menu::menu_item::join_right(_menuitem const &right) {
    this->links[RIGHT] = right;
    right->links[UP] = this;
}


void menu::menu_item::highlight() const {
    MENU_SELECTED_COLOR;
    LCD_setFont(MENU_ITEMS_FONT);
    LCD_print(display_text.c_str(), pos.x, pos.y);
}

void menu::menu_item::un_highlight() const {
    MENU_UNSELECTED_COLOR;
    LCD_setFont(MENU_ITEMS_FONT);
    LCD_print(display_text.c_str(), pos.x, pos.y);
}
