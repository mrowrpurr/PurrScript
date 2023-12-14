set_languages("c++20")

add_rules("mode.debug", "mode.releasedbg", "mode.release")

add_repositories("MrowrLib https://github.com/MrowrLib/Packages.git")

includes("*/xmake.lua")
includes("Resources/*/xmake.lua")
