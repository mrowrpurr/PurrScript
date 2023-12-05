add_requires("function_pointer", "void_pointer")

target("PurrScript.API")
    set_kind("headeronly")
    add_packages(
        "function_pointer",
        "void_pointer",
        { public = true }
    )
    add_includedirs("include", { public = true })
