add_requires("websocketpp", "unordered_dense", "spdlog", "cxxopts", "underscore_log")

target("PurrScript.Server")
    set_kind("binary")
    add_files("*.cpp")
    add_packages("websocketpp", "unordered_dense", "spdlog", "cxxopts", "underscore_log")
