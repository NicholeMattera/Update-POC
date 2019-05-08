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

#include "Application.hpp"

using namespace std;
using namespace std::placeholders;

namespace UpdatePOC {
    Application::Application() {
        consoleInit(NULL);
        splInitialize();

        vector<string> mainMenuItems;
        mainMenuItems.push_back("Get Hekate Version");
        mainMenuItems.push_back("Get Atmosphere Version");
        mainMenuItems.push_back("Get System Module Versions");
        mainMenuItems.push_back("Get Homebrew Versions");
        mainMenuItems.push_back("Exit");

        _mainMenu = new Menu(
            "Update POC",
            mainMenuItems,
            bind(&Application::_menuItemSelected, this, _1),
            bind(&Application::_back, this)
        );

        _hekateView = new HekateView(bind(&Application::_back, this));
        _atmosView = new AtmosphereView(bind(&Application::_back, this));
        _sysModuleView = new SysModuleView(bind(&Application::_back, this));
        _homebrewView = new HomebrewView(bind(&Application::_back, this));
    }

    Application::~Application() {
        if (_mainMenu != NULL) {
            delete _mainMenu;
        }

        splExit();
        consoleExit(NULL);
    }

    void Application::start() {
        while(appletMainLoop())
        {
            hidScanInput();
            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

            switch (_state) {
                case 0:
                    _mainMenu->draw(kDown);
                    break;

                case 1:
                    _hekateView->draw(kDown);
                    break;

                case 2:
                    _atmosView->draw(kDown);
                    break;

                case 3:
                    _sysModuleView->draw(kDown);
                    break;

                case 4:
                    _homebrewView->draw(kDown);
                    break;

                default:
                    return;
            }

            consoleUpdate(NULL);
        }
    }

    void Application::_menuItemSelected(int itemSelected) {
        switch (_state) {
            case 0:
                if (itemSelected == 0) {
                    _state = 1;
                } else if (itemSelected == 1) {
                    _state = 2;
                } else if (itemSelected == 2) {
                    _state = 3;
                } else if (itemSelected == 3) {
                    _state = 4;
                } else {
                    _state = -1;
                }
                break;
        }
    }

    void Application::_back() {
        switch (_state) {
            case 0:
                _state = -1;
                return;

            default:
                _mainMenu->reset();
                _state = 0;
                return;
        }
    }
}
