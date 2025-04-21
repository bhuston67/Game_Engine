//
//  File.m
//  game_engine
//
//  Created by Ben Huston on 1/28/25.
//


struct Actor
{
public:
    std::string actor_name;
    char view;
    glm::ivec2 position;
    glm::ivec2 velocity;
    bool blocking;
    std::string nearby_dialogue;
    std::string contact_dialogue;

    Actor(std::string actor_name, char view, glm::ivec2 position, glm::ivec2 initial_velocity,
        bool blocking, std::string nearby_dialogue, std::string contact_dialogue)
        : actor_name(actor_name), view(view), position(position), velocity(initial_velocity), blocking(blocking), nearby_dialogue(nearby_dialogue), contact_dialogue(contact_dialogue) {}

    Actor() {}
};