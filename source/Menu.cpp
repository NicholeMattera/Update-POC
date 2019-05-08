/*
 * UpdatePOC
 * Copyright (c) 2019 Steven Mattera
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above 
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Menu.hpp"
#include <iostream>
#include <string>

using namespace std;

namespace UpdatePOC {
    Menu::Menu(string title, vector<string> items, function<void(u64)> itemSelectedCallback, function<void()> backCallback) {
        _title = title;
        _items = items;
        _itemSelectedCallback = itemSelectedCallback;
        _backCallback = backCallback;
        _hasDrawn = false;
        _itemSelected = 0;
    }

    void Menu::draw(u64 kDown) {
        u64 itemSelectedChanged = 0;

        if (kDown & KEY_A) {
            _itemSelectedCallback(_itemSelected);
        } else if (kDown & KEY_B) {
            _backCallback();
        } else if (kDown & KEY_UP && _itemSelected > 0) {
            itemSelectedChanged = -1;
        } else if (kDown & KEY_DOWN && _itemSelected < _items.size() - 1) {
            itemSelectedChanged = 1;
        }

        if (!_hasDrawn) {
            _itemSelected += itemSelectedChanged;

            consoleClear();

            cout << "\x1b[0;0H" << _title << "\n\n";

            u64 index = 0;
            for (auto const& item : _items) {
                if (index == _itemSelected) {
                    cout << " > " << item << "\n";
                } else {
                    cout << "   " << item << "\n";
                }

                index++;
            }

            _hasDrawn = true;
        } else if (itemSelectedChanged != 0) {
            u64 previousItemIndex = _itemSelected;
            _itemSelected += itemSelectedChanged;

            printf("\x1b[%lu;0H   ", previousItemIndex + 3);
            printf("\x1b[%lu;0H > ", _itemSelected + 3);
        }
    }

    void Menu::reset() {
        _hasDrawn = false;
        _itemSelected = 0;
    }
}
