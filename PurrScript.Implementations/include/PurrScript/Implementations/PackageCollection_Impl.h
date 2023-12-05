#pragma once

#include <PurrScript/API.h>
#include <collections.h>

#include <string>

namespace PurrScript::Implementations {

    class PackageCollection_Impl : public PackageCollection {
        collections_map<std::string, Package*> _packages;
        collections_set<Package*>              _packagePointers;

        bool _IsValidPackageName(const char* packageName) {
            if (packageName == nullptr) return false;
            if (packageName[0] == '\0') return false;
            return true;
        }

    public:
        bool AddPackage(Package* package) override {
            if (package == nullptr) return false;
            auto* packageName = package->GetName();
            if (!_IsValidPackageName(packageName)) return false;
            if (_packages.contains(packageName)) return false;
            _packages[packageName] = package;
            _packagePointers.insert(package);
            return true;
        }

        bool RemovePackage(Package* package) override {
            if (package == nullptr) return false;
            auto* packageName = package->GetName();
            if (!_IsValidPackageName(packageName)) return false;
            if (!_packages.contains(packageName)) return false;
            _packagePointers.erase(package);
            return _packages.erase(packageName) > 0;
        }

        Package* GetPackage(const char* name) override {
            if (!_IsValidPackageName(name)) return nullptr;
            if (!_packages.contains(name)) return nullptr;
            return _packages[name];
        }

        bool HasPackage(const char* name) override {
            if (!_IsValidPackageName(name)) return false;
            return _packages.contains(name);
        }

        bool HasPackage(Package* package) override { return _packagePointers.contains(package); }

        std::uint32_t GetPackageCount() override { return _packagePointers.size(); }

        void ForEachPackage(ReadOnlyPackageCollection::ForEachPackageFn* callback) override {
            for (auto& package : _packagePointers) callback->invoke(package);
        }
    };
}
