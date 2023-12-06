add_requires("specs_cpp", "vcpkg::snowhouse", "_Log_", "spdlog", "luajit", "unordered_dense", "string_format")

target("PurrScript.Specs")
    set_kind("binary")
    add_deps(
        "PurrScript.Client",
        "PurrScript.Implementations",
        "PurrScript.Packages.Core",
        "PurrScript.Lua"
    )
    add_packages("specs_cpp", "vcpkg::snowhouse", "_Log_", "spdlog", "luajit", "unordered_dense", "string_format")
    add_files("*.cpp")
    add_includedirs(".")
