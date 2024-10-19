#include "backend/register.h"
#include"rpc.h"
#include <chrono>
#include "backend/handler.h"
#include "../invoker/Natives.hpp"
#pragma comment(lib, "discord-rpc.lib")

discordhandler g_Discord;

void discordhandler::initialize() {
    DiscordEventHandlers handlers;
    std::memset(&handlers, 0, sizeof(handlers));
    Discord_Initialize("1200814010866606080", &handlers, 1, "0");
}

void discordhandler::update() {


    DiscordRichPresence discordPresence;
    std::memset(&discordPresence, 0, sizeof(discordPresence));

    discordPresence.largeImageKey = "copie_de_v";
    discordPresence.smallImageKey = "gta";
    discordPresence.smallImageText = "V1.0.2";
    discordPresence.largeImageText = "sans_titre83";
    discordPresence.details = "Virtual Engine Service";

    if (NETWORK::NETWORK_IS_SESSION_STARTED()) {
        discordPresence.state = "Playing in online";
    }
    else {
        discordPresence.state = "Playing in story mode";
    }
    discordPresence.instance = 1;
    Discord_UpdatePresence(&discordPresence);
}

void discordhandler::cleanup() {
    Discord_ClearPresence();
    Discord_Shutdown();
}

discordhandler* get_discord() { return &g_Discord; }