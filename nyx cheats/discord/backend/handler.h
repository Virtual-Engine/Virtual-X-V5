#pragma once
#include "register.h"
#include "rpc.h"
#include "../../common.hpp"


class discordhandler {
public:
    void initialize();
    void update();
    void cleanup();
};

discordhandler* get_discord();
