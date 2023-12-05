add_requires("_Log_", "collections", "void_pointer")

target("PurrScript.Client")
    set_kind("headeronly")
    add_deps("PurrScript.API", "PurrScript.Implementations")
    add_packages("_Log_", "collections", "void_pointer", { public = true })
    add_includedirs("include", { public = true })
