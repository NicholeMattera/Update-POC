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

#include "AtmosphereView.hpp"
#include <iostream>

using namespace std;

namespace UpdatePOC {
    AtmosphereView::AtmosphereView(function<void()> backCallback) : View(backCallback) {}

    void AtmosphereView::draw(u64 kDown) {
        if (!_hasDrawn) {
            consoleClear();

            cout << "\x1b[0;0HGet Atmosphere Version\n";
            cout << "----------------------\n\n";
            
            if (_getVersion()) {
                cout << "Current Version Installed: " << _major << "." << _minor << "." << _micro << "\n\n";
            }
            else {
                cout << "Failed to get current version installed.\n\n";
            }

            cout << "Press A to continue.";

            _hasDrawn = true;
            _hasFinished = true;
        }

        View::draw(kDown);
    }

    bool AtmosphereView::_getVersion() {
        // We already have the version no need to call again.
        if (_micro != 0 || _minor != 0 || _major != 0) {
            return true;
        }

        // Get the Atmosphere version number from Exosphere.
        u64 exosphere_cfg;
        if (R_FAILED(splGetConfig((SplConfigItem)65000, &exosphere_cfg))) {
            return false;
        }
        
        _micro = (u32)((exosphere_cfg >> 16ull) & 0xFF);
        _minor = (u32)((exosphere_cfg >> 24ull) & 0xFF);
        _major = (u32)((exosphere_cfg >> 32ull) & 0xFF);

        return true;
    }
}
