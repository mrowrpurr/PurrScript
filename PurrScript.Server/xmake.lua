add_requires("websocketpp", "cxxopts", "underscore_log", "collections")

target("PurrScript.Server")
    set_kind("headeronly")
    add_includedirs("include", { public = true })
    add_deps("PurrScript.Implementations")
    add_packages("websocketpp", "cxxopts", "underscore_log", "collections", { public = true })
