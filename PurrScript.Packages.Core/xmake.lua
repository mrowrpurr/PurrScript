add_requires("string_format")

target("PurrScript.Packages.Core")
    set_kind("headeronly")
    add_deps("PurrScript.Client")
    add_packages("string_format", { public = true })
    add_includedirs("include", { public = true })
