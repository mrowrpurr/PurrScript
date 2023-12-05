#pragma once

#include <memory>

#include "PurrScriptAPI_Client.h"

namespace PurrScript {

    inline std::unique_ptr<Client::PurrScriptAPI_Client> CreateClient(PurrScriptAPI* api) {
        return std::make_unique<Client::PurrScriptAPI_Client>(api);
    }
}