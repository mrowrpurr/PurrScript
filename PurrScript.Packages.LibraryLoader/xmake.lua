target("PurrScript.Packages.LibraryLoader")
    set_kind("headeronly")
    add_deps(
        "PurrScript.DLL",
        "PurrScript.Client"
    )
    add_includedirs("include", { public = true })
