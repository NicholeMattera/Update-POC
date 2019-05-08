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

#include "Homebrew.hpp"

using namespace std;

namespace UpdatePOC {
    Homebrew::Homebrew(string path) {
        ifstream file(path, ios::binary);
        if (file.fail())
            return;

        NroHeader header = _getHeader(&file);
        if (header.magic != NROHEADER_MAGIC) {
            file.close();
            return;
        }

        NroAssetHeader assetHeader = _getAssetHeader(&file, header);
        if (assetHeader.magic != NROASSETHEADER_MAGIC ||
            assetHeader.version > NROASSETHEADER_VERSION ||
            assetHeader.nacp.offset == 0 ||
            assetHeader.nacp.size < sizeof(NacpStruct)) {

            file.close();
            return;
        }

        NacpStruct nacp = _getNacp(&file, header, assetHeader);
        file.close();

        _version = nacp.version;

        NacpLanguageEntry * langEntry = NULL;
        Result rc = nacpGetLanguageEntry(&nacp, &langEntry);
        if (R_FAILED(rc))
            return;

        _name = langEntry->name;
    }
    
    string Homebrew::getName() {
        return _name;
    }

    string Homebrew::getVersion() {
        return _version;
    }

    NroHeader Homebrew::_getHeader(ifstream * file) {
        NroHeader header;
        file->seekg(sizeof(NroStart));
        file->read((char *) &header, sizeof(NroHeader));
        return header;
    }

    NroAssetHeader Homebrew::_getAssetHeader(ifstream * file, NroHeader header) {
        NroAssetHeader assetHeader;
        file->seekg(header.size);
        file->read((char *) &assetHeader, sizeof(NroAssetHeader));
        return assetHeader;
    }

    NacpStruct Homebrew::_getNacp(ifstream * file, NroHeader header, NroAssetHeader assetHeader) {
        NacpStruct nacp;
        file->seekg(header.size + assetHeader.nacp.offset);
        file->read((char *) &nacp, sizeof(NacpStruct));
        return nacp;
    }
}