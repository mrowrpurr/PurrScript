#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/Package_Impl.h>
#include <_Log_.h>
#include <collections.h>

#include <memory>
#include <string>

namespace PurrScript::Client {

    class PurrScriptAPI_Client {
        PurrScriptAPI* _api;
        collections_map<std::string, std::unique_ptr<Implementations::Package_Impl>>
            _definedPackages;

    public:
        PurrScriptAPI_Client(PurrScriptAPI* api) : _api(api) {}

        PurrScriptAPI* GetRawAPI() { return _api; }

        std::unique_ptr<Implementations::Package_Impl>& NewPackage(
            std::string_view name, std::string_view version = "", std::string_view author = ""
        ) {
            if (_definedPackages.contains(name.data())) {
                _Log_("Client already has defined package '{}'", name);
                return _definedPackages[name.data()];
            }
            auto package = std::make_unique<Implementations::Package_Impl>(name, version, author);
            _definedPackages.emplace(name, std::move(package));
            return _definedPackages[name.data()];
        }
    };
}