//
//  Client.cpp
//  game_engine
//
//  Created by Ben Huston on 6/19/25.
//

#include "Client.hpp"


void Client::setAddress(std::string host, int port) {
    client = enet_host_create(NULL, 1, 2, 0, 0);
    enet_address_set_host(&address, host.c_str());
    address.port = port;
    
    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
            printf("Connection to server failed.\n");
        }
    ENetEvent event;
        if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            printf("Connected to server!\n");
        } else {
            printf("Connection failed or timed out.\n");
            enet_peer_reset(peer);
        }
    
}

void Client::sendPackage(std::string msg) {
    ENetPacket* packet = enet_packet_create(msg.c_str(), msg.length(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);
    
}


void Client::processEvents() {
    ENetEvent event;
    while (enet_host_service(client, &event, 0) > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            const char* data = reinterpret_cast<const char*>(event.packet->data);
            size_t len = event.packet->dataLength;
            std::string json(data, len);
            //std::cout << "Client received packet: " << json << "\n";
            luabridge::LuaRef packetTable = decodeJsonToLuaTable(json);
            
            if (!packetTable["eventType"].isNil() && packetTable["eventType"].isString()) {
                std::string type = packetTable["eventType"].cast<std::string>();
                std::vector<std::pair<luabridge::LuaRef,luabridge::LuaRef>> refs = subs[type];
                for (auto comp : refs) {
                    if (std::find(unsubs[type].begin(), unsubs[type].end(), comp) == unsubs[type].end()) {
                        luabridge::LuaRef func = comp.second;
                        luabridge::LuaRef self = comp.first;
                        if (func.isFunction()) {
                            try {
                                func(self,packetTable);
                            } catch (const luabridge::LuaException& e) {
                                
                            }
                        }
                    }
                }

            } else {
                std::cerr << "Warning: 'eventType' is missing or not a string\n";
                // Handle default case, ignore, or send error back to client
            }
            
            
            enet_packet_destroy(event.packet);
            break;
        }
        if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            std::cout << "Disconnected from server\n";
            break;
        }
    }
}

luabridge::LuaRef Client::decodeJsonToLuaTable(const std::string& jsonString) {
    // Push cjson.decode onto the stack
    lua_getglobal(ComponentDB::luaState, "cjson");
    lua_getfield(ComponentDB::luaState, -1, "decode");  // cjson.decode

    // Push the JSON string argument
    lua_pushlstring(ComponentDB::luaState, jsonString.c_str(), jsonString.length());

    // Call the function with 1 argument, expecting 1 return
    if (lua_pcall(ComponentDB::luaState, 1, 1, 0) != LUA_OK) {
        std::cerr << "JSON decode error: " << lua_tostring(ComponentDB::luaState, -1) << std::endl;
        lua_pop(ComponentDB::luaState, 2); // pop error + cjson
        return luabridge::LuaRef(ComponentDB::luaState); // return nil ref
    }

    // Return the Lua table
    luabridge::LuaRef table = luabridge::LuaRef::fromStack(ComponentDB::luaState, -1);
    lua_pop(ComponentDB::luaState, 2); // pop table and cjson

    return table;
}

void Client::Subscribe(std::string event, luabridge::LuaRef comp, luabridge::LuaRef function) {
    lateSubs.push_back({event,{comp,function}});
}

void Client::Unsubscribe(std::string event, luabridge::LuaRef comp, luabridge::LuaRef function) {
    lateUnsubs.push_back({event,{comp,function}});
}

void Client::lateHandleEvents() {
    for (auto comp : lateSubs) {
        if (std::find(subs[comp.first].begin(), subs[comp.first].end(), comp.second) == subs[comp.first].end()) {
            subs[comp.first].push_back(comp.second);
        }
    }
    for (auto comp : lateUnsubs) {
        if (std::find(unsubs[comp.first].begin(), unsubs[comp.first].end(), comp.second) == unsubs[comp.first].end()) {
            unsubs[comp.first].push_back(comp.second);
        }
        //unsubs.insert(comp);
    }
}


