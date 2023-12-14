add_requires("underscore_log")

target("Shared Example Struct Implementations")
    set_kind("headeronly")
    add_deps("Shared Example Structs")
    add_packages("underscore_log", { public = true })
    add_includedirs(".", { public = true })
