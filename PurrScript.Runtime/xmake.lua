target("PurrScript.Runtime")
    set_kind("headeronly")
    add_deps(
        -- Main PurrScript classes
        "PurrScript.Implementations",

        -- Standard Library of PurrScript packages
        "PurrScript.Packages.Core",
        "PurrScript.Packages.Reflection"
    )