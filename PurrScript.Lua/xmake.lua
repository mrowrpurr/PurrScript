add_requires("sol2", { configs = { includes_lua = false }})

target("PurrScript.Lua")
    set_kind("headeronly")
    add_deps("PurrScript.Client")
    add_packages("sol2", { public = true })
    add_includedirs("include", { public = true })
