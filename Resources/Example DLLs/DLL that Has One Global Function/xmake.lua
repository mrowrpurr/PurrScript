target("DLL that Has One Global Function")
    set_kind("shared")
    add_files("*.cpp")
    add_deps(
        "PurrScript.DLL",
        "PurrScript.Client"
    )
