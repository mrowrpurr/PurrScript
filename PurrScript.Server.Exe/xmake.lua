add_requires("unordered_dense", "spdlog")

target("PurrScript.Server.Exe")
    set_kind("binary")
    set_basename("PurrScript.Server")
    add_files("*.cpp")
    add_deps("PurrScript.Server")
    add_packages("unordered_dense", "spdlog")
