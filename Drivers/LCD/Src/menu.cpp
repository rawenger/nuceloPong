/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "menu.h"

void menu::init()
{
    title_pos.x = (DISP_X_SIZE - title.length() * MENU_TITLE_FONT[0]) / 2;
    title_pos.y = MENU_TITLE_Y;

    selection = first;
}


menu::~menu() {
    CPP_LOG("Deleting menu");
    delete first;
}

void menu::show() {
    MENU_TITLE_COLOR;
    LCD_setFont(MENU_TITLE_FONT);
    LCD_print(title.c_str(), title_pos.x, title_pos.y);
    selection->highlight();

    MENU_UNSELECTED_COLOR;
    LCD_setFont(MENU_ITEMS_FONT);
    display_items(first->links[DOWN]);
    display_items(first->links[RIGHT]);

    hidden = false;
}

void menu::move_cursor(menu::Direction dir) {
    if (selection->actions.count(dir)) {
        selection->actions.at(dir)(selection);
    } else if (selection->links[dir]) {
        selection->un_highlight();
        selection = selection->links[dir];
        selection->highlight();
    }
}

void menu::select() {
    CPP_LOG("Performing action on selected item " << selection->display_text);
    selection->actions.at(CLICK)(selection);
}

void menu::hide() {
    // erase title
    COLOR_BLACK;
    LCD_fillRect(title_pos.x, title_pos.y,
                 title_pos.x + title.length() * MENU_TITLE_FONT[0],
                 title_pos.y + MENU_TITLE_FONT[1]);

    // erase items
    erase_items(first);

    hidden = true;
}

void menu::display_items(_cmenuitem start) const {
    if (!start)
        return;

    MENU_UNSELECTED_COLOR;
    LCD_print(start->display_text.c_str(), start->pos.x, start->pos.y);
    start->update_value(*(start->value));

    display_items(start->links[DOWN]);
    display_items(start->links[RIGHT]);
}

void menu::erase_items(menu::_cmenuitem start) const {
    if (!start)
        return;

    LCD_fillRect(start->pos.x, start->pos.y,
                 start->pos.x + start->display_text.length() * menu_item::font_dim.x,
                 start->pos.y + menu_item::font_dim.y);

    erase_items(start->links[DOWN]);
    erase_items(start->links[RIGHT]);
}

menu::menu_item::menu_item(std::string &&name, const _menuitem& up, const _menuitem& left)
        : display_text(name),
          links{up, nullptr, left, nullptr}
{
    this->pos.x = (DISP_X_SIZE - display_text.length() * font_dim.x) / 2;
    if (up) {
        up->links[DOWN] = this;
        this->pos.y = up->pos.y + font_dim.y + MENU_ITEM_SPACING;
    } else if (left) {
        left->links[RIGHT] = this;
        this->pos.x = DISP_X_SIZE - left->pos.x - font_dim.x * display_text.length();
        this->pos.y = left->pos.y;
    }
}

menu::menu_item::~menu_item() {
    // as of C++ 17, deleting nullptr has no effect, so this *should* be safe to do...
    // (as long as two items don't share the same "down" or "right")
    CPP_LOG("Preparing to delete item \"" << display_text << "\"");
    delete links[DOWN];
    delete links[RIGHT];
    delete valstr;
    delete value;
    CPP_LOG("Deleted menu item \"" << this->display_text << "\"");
}

void menu::menu_item::join_below(const _menuitem &lower) {
    this->links[DOWN] = lower;
    lower->links[UP] = this;
}

void menu::menu_item::join_right(const _menuitem &right) {
    this->links[RIGHT] = right;
    right->links[UP] = this;
}

void menu::menu_item::default_value(int val) {
    if (!has_value) {
        value = new int{val};
        has_value = true;
        valstr = new std::string(display_text);
    }
}

void menu::menu_item::update_value(int val) const {
    if (!has_value)
        return;
    // erase old string
    *value = val;
    int old_len = valstr->length();
    // TODO: check for off-by-1 in placement of valstr
    int startx = display_text.length() * font_dim.x + pos.x;
    *valstr = ": " + std::to_string(val);
    std::string erase = std::string(" ", old_len - valstr->length());
    LCD_print(erase.c_str(), startx + old_len * font_dim.x, pos.y);
    LCD_print(valstr->c_str(), startx, pos.y);
//    LCD_fillRect(startx, pos.y, startx + old_len * font_dim.x, pos.y + font_dim.y);

}

void menu::menu_item::highlight() const {
    MENU_SELECTED_COLOR;
    LCD_setFont(MENU_ITEMS_FONT);
    LCD_print(display_text.c_str(), pos.x, pos.y);
    update_value(*value);
}

void menu::menu_item::un_highlight() const {
    MENU_UNSELECTED_COLOR;
    LCD_setFont(MENU_ITEMS_FONT);
    LCD_print(display_text.c_str(), pos.x, pos.y);
    update_value(*value);
}


