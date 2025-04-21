//
//  ENGINEUTIL.h
//  game_engine
//
//  Created by Ben Huston on 1/27/25.
//

#ifndef ENGINEUTIL_h
#define ENGINEUTIL_h


#include <string>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <iostream>


#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "glm/glm.hpp"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

class EngineUtil {
public:
    static void ReadJsonFile(const std::string& path, rapidjson::Document & out_document)
    {
        FILE* file_pointer = nullptr;
    #ifdef _WIN32
        fopen_s(&file_pointer, path.c_str(), "rb");
    #else
        file_pointer = fopen(path.c_str(), "rb");
    #endif
        char buffer[65536];
        rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
        out_document.ParseStream(stream);
        std::fclose(file_pointer);

        if (out_document.HasParseError()) {
            rapidjson::ParseErrorCode errorCode = out_document.GetParseError();
            std::cout << "error parsing json at [" << path << "]" << std::endl;
            exit(0);
        }
    }
    
    static std::string obtain_word_after_phrase(const std::string& input, const std::string& phrase) {
        size_t pos = input.find(phrase);
        if (pos == std::string::npos) return "";
        pos += phrase.length();
        while (pos < input.size() && std::isspace(input[pos])) {
            ++pos;
        }
        if (pos == input.size()) return "";
        size_t endPos = pos;
        while (endPos < input.size() && !std::isspace(input[endPos])) {
            ++endPos;
        }
        return input.substr(pos,endPos-pos);
    }
    
    static uint64_t create_composite_key(int x, int y) {
        uint32_t ux = static_cast<uint32_t>(x);
        uint32_t uy = static_cast<uint32_t>(y);
        uint64_t result = static_cast<uint64_t>(ux);
        result = result << 32;
        result = result | static_cast<uint64_t>(uy);
        return result;
    }
    
    static void ReportError(const std::string & actor_name, const luabridge::LuaException &e) {
        std::string errorMessage = e.what();
        std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
        std::cout << "\033[31m" << actor_name << " : " << errorMessage << "\033[0m" << std::endl;
    }
    
};


#endif /* ENGINEUTIL_h */
