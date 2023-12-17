add_requires(
    "specs", "specs_snowhouse", "vcpkg::snowhouse", "unordered_dense", "fmt", "spdlog"
)

target("Specs.Core")
    set_kind("shared")
    add_files("*.cpp", "**/*.cpp")
    add_packages(
        "specs", "specs_snowhouse", "vcpkg::snowhouse", "unordered_dense", "fmt", "spdlog"
    )
