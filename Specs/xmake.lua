add_requires(
    "specs", "unordered_dense", "fmt", "spdlog"
)

target("Specs")
    set_kind("binary")
    add_files("Specs.cpp")
    add_packages(
        "specs", "unordered_dense", "fmt", "spdlog"
    )

includes("*/xmake.lua")

target("Run Specs")
    set_kind("phony")
    add_deps(
        "Specs",
        "Specs.Core"
    )
    on_run(function(target)
        os.execv(
            target:dep("Specs"):targetfile()
            .. " --dll " .. target:dep("Specs.Core"):targetfile()
        )
    end)
