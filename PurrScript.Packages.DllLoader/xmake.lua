target("PurrScript.Packages.DllLoader")
    set_kind("headeronly")
    add_deps("PurrScript.Client")
    add_includedirs("include", { public = true })
