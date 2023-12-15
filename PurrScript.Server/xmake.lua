add_requires("websocketpp", "cxxopts", "underscore_log", "collections", "string_format")

target("PurrScript.Server")
    set_kind("headeronly")
    add_includedirs("include", { public = true })
    add_deps(
        "PurrScript.Server.API",
        "PurrScript.Implementations"
    )
    add_packages("websocketpp", "cxxopts", "underscore_log", "collections", "string_format", { public = true })
