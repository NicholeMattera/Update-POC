/*
 * UpdatePOC
 * Copyright (c) 2019 Nichole Mattera
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

#include "HekateView.hpp"
#include <iostream>
#include <fstream>

using namespace std;

namespace UpdatePOC {
    HekateView::HekateView(function<void()> backCallback) : View(backCallback) {}

    void HekateView::draw(u64 kDown) {
        if (!_hasDrawn) {
            consoleClear();

            cout << "\x1b[0;0HGet Hekate Version\n";
            cout << "------------------\n\n";

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

    bool HekateView::_getVersion() {
        // We already have the version no need to call again.
        if (_micro != 0 || _minor != 0 || _major != 0) {
            return true;
        }

        ifstream file("sdmc:/bootloader/update.bin", ios::binary);
        if (file.fail()) {
            return false;
        }

        // Get the size of the file.
        file.seekg(0, ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, ios::beg);

        // Create a string to hold the entire file.
        string content;
        content.reserve(fileSize);

        // Read the file into our string.
        char buffer[16384];
        streamsize bufferSize;
        while (file.read(buffer, sizeof buffer), bufferSize = file.gcount())
            content.append(buffer, bufferSize);

        // Were we able to reach the end of the file?
        if (file.eof()) {
            // Try to find the header string in the binary.
            auto startPos = content.find("ICTC");
            if (startPos == string::npos) {
                file.close();
                return false;
            }
            startPos += 4;

            // Slice out the version number.
            auto version = content.substr(startPos, 3);
            _major = version[0] - 0x30;
            _minor = version[1] - 0x30;
            _micro = version[2] - 0x30;
        }

        file.close();

        return true;
    }
}
