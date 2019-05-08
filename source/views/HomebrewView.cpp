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

#include "HomebrewView.hpp"
#include <iostream>
#include <algorithm>
#include <dirent.h>
#include "Homebrew.hpp"

using namespace std;

namespace UpdatePOC {
    HomebrewView::HomebrewView(function<void()> backCallback) : View(backCallback) {}

    void HomebrewView::draw(u64 kDown) {
        if (!_hasDrawn) {
            consoleClear();

            cout << "\x1b[0;0HGet Homebrew Versions\n";
            cout << "---------------------\n\n";
            
            _processHomebrews("sdmc:/switch");

            cout << "\nPress A to continue.";

            _hasDrawn = true;
            _hasFinished = true;
        }

        View::draw(kDown);
    }

    void HomebrewView::_processHomebrews(string path, int depth) {
        // We only want to go one folder deep.
        if (depth >= 2) 
            return;

        DIR * dir;
        struct dirent * ent;
        if ((dir = opendir(path.c_str())) == NULL) {
            return;
        }

        while ((ent = readdir(dir)) != NULL) {
            // Directories
            if (ent->d_type == DT_DIR) {
                _processHomebrews(path + "/" + string(ent->d_name), depth + 1);
            }
            // Files
            else if (ent->d_type == DT_REG) {
                string filename = string(ent->d_name);
                string ext = filename.substr(filename.length() - 3, 3);
                transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                // Only want files that end with "nro".
                if (ext == "nro") {
                    Homebrew app = Homebrew(path + "/" + filename);
                    if (app.getName() != "" && app.getVersion() != "") {
                        cout << app.getName() << " - " << app.getVersion() << "\n";
                    }
                }
            }
        }

        closedir(dir);
    }
}
