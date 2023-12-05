add_requires("collections")

target("PurrScript.Implementations")
    set_kind("headeronly")
    add_deps("PurrScript.API")
    add_packages("collections", { public = true })
    add_includedirs("include", { public = true })
