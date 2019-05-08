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

#include <switch.h>
#include <string>
#include <vector>
#include <functional>

#pragma once

namespace UpdatePOC {
    class Menu {
        public:
            Menu(std::string title, std::vector<std::string> items, std::function<void(u64)> itemSelectedCallback, std::function<void()> backCallback);

            void draw(u64 kDown);
            void reset();

        private:
            std::string _title;
            std::vector<std::string> _items;
            std::function<void(u64)> _itemSelectedCallback;
            std::function<void()> _backCallback;

            bool _hasDrawn;
            u64 _itemSelected;
            
    };
}
