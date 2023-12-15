add_requires("global_macro_functions", "collections")

target("PurrScript.DLL")
    set_kind("headeronly")
    add_deps("PurrScript.API")
    add_packages("global_macro_functions", "collections", { public = true })
    add_includedirs("include", { public = true })
