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
#include "views/HekateView.hpp"
#include "views/AtmosphereView.hpp"
#include "views/SysModuleView.hpp"
#include "views/HomebrewView.hpp"

#pragma once

namespace UpdatePOC {
    class Application {
        public:
            Application();
            ~Application();

            void start();

        private:
            int _state = 0;
            Menu * _mainMenu = NULL;
            HekateView * _hekateView = NULL;
            AtmosphereView * _atmosView = NULL;
            SysModuleView * _sysModuleView = NULL;
            HomebrewView * _homebrewView = NULL;

            void _menuItemSelected(int itemSelected);
            void _back();

    };
}
