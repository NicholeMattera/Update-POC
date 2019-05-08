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

#include "SystemModule.hpp"
#include <jansson.h>

using namespace std;

namespace UpdatePOC {
    SystemModule::SystemModule(string path) {
        ifstream file(path, ios::binary);
        if (file.fail())
            return;

        Pfs0Header header = _getHeader(&file);
        if (header.magic != PFS0HEADER_MAGIC) {
            file.close();
            return;
        }

        Pfs0FileEntry fileEntry = _findFileEntry(&file, header, "info.json");
        if (fileEntry.fileSize == 0) {
            file.close();
            return;
        }

        auto content = _getFile(&file, header, fileEntry);
        file.close();
        _parseInfoJSON(content);
    }

    bool SystemModule::hasInfo() {
        return _info;
    }

    string SystemModule::getName() {
        return _name;
    }

    string SystemModule::getVersionNumber() {
        return _version;
    }

    bool SystemModule::getRequiresReboot() {
        return _requiresReboot;
    }

    Pfs0Header SystemModule::_getHeader(ifstream * file) {
        Pfs0Header header;
        file->seekg(0);
        file->read((char*)&header, sizeof(Pfs0Header));
        return header;
    }

    Pfs0FileEntry SystemModule::_findFileEntry(ifstream * file, Pfs0Header header, string fileName) {
        for (u32 i = 0; i < header.numberOfFiles - 1; i++) {
            file->seekg(sizeof(Pfs0Header) + (sizeof(Pfs0FileEntry) * i));

            Pfs0FileEntry curFileEntry;
            u32 curFilenameSize;

            Pfs0FileEntry nextFileEntry;

            file->read((char*)&curFileEntry, sizeof(Pfs0FileEntry));
            file->read((char*)&nextFileEntry, sizeof(Pfs0FileEntry));

            curFilenameSize = nextFileEntry.filenameOffset - curFileEntry.filenameOffset;
            string curFileName = _getFileName(file, header, curFileEntry.filenameOffset, curFilenameSize);
            if (curFileName == fileName) {
                return curFileEntry;
            }
            
            if (i == header.numberOfFiles - 2) {
                u32 nextFileNameSize = (header.sizeOfStringTable - 5) - nextFileEntry.filenameOffset;
                string nextFileName = _getFileName(file, header, nextFileEntry.filenameOffset, nextFileNameSize);
                if (nextFileName == fileName) {
                    return nextFileEntry;
                }
            }
        }

        return { 0, 0, 0, 0 };
    }
    
    string SystemModule::_getFileName(ifstream * file, Pfs0Header header, u32 offset, u32 size) {
        u32 stringTableStart = (u32) sizeof(Pfs0Header) + (sizeof(Pfs0FileEntry) * header.numberOfFiles);

        char fileName[size];
        file->seekg(stringTableStart + offset);
        file->read((char*)&fileName, size);

        return string(fileName);
    }

    string SystemModule::_getFile(ifstream * file, Pfs0Header header, Pfs0FileEntry fileEntry) {
        u32 dataStart = (u32) sizeof(Pfs0Header) + (sizeof(Pfs0FileEntry) * header.numberOfFiles) + header.sizeOfStringTable;

        char fileContent[fileEntry.fileSize + 1] = "";
        file->seekg(dataStart + fileEntry.fileOffset);
        file->read((char*)&fileContent, fileEntry.fileSize);

        return string(fileContent);
    }

    void SystemModule::_parseInfoJSON(string content) {
        json_t * root;
        json_error_t error;

        root = json_loads(content.c_str(), 0, &error);
        if (root) {
            auto nameJsonObject = json_object_get(root, "name");
            auto versionJsonObject = json_object_get(root, "version");
            auto requireRebootJsonObject = json_object_get(root, "requires_reboot");

            _info = true;
            _name = json_string_value(nameJsonObject);
            _version = json_string_value(versionJsonObject);
            _requiresReboot = json_boolean_value(requireRebootJsonObject);

            json_decref(root);
        }
    }
}