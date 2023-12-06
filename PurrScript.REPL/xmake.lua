add_requires("luajit")

target("PurrScript.REPL")
    set_kind("binary")
    add_files("*.cpp")
    add_deps(
        "PurrScript.Lua",
        "PurrScript.Runtime",
        "PurrScript.Packages.Console"
    )
    add_packages("luajit")
