add_requires("function_pointer")

target("PurrScript.Server.API")
    set_kind("headeronly")
    add_includedirs("include", { public = true })
    add_packages("function_pointer", { public = true })
