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
#include <fstream>

#pragma once

#define PFS0HEADER_MAGIC 0x30534650

namespace UpdatePOC {
    typedef struct {
        u32 magic;
        u32 numberOfFiles;
        u32 sizeOfStringTable;
        u32 reserved;
    } Pfs0Header;

    typedef struct {
        u64 fileOffset;
        u64 fileSize;
        u32 filenameOffset;
        u32 reserved;
    } Pfs0FileEntry;

    class SystemModule {
        public:
            SystemModule(std::string path);
            
            bool hasInfo();
            std::string getName();
            std::string getVersionNumber();
            bool getRequiresReboot();
        
        private:
            bool _info = false;
            std::string _name = "";
            std::string _version = "";
            bool _requiresReboot = false;

            Pfs0Header _getHeader(std::ifstream * file);
            Pfs0FileEntry _findFileEntry(std::ifstream * file, Pfs0Header header, std::string fileName);
            std::string _getFileName(std::ifstream * file, Pfs0Header header, u32 offset, u32 size);
            std::string _getFile(std::ifstream * file, Pfs0Header header, Pfs0FileEntry fileEntry);
            void _parseInfoJSON(std::string content);

    };
}
