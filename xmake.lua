set_languages("c++20")

add_rules("mode.debug", "mode.releasedbg")

add_repositories("MrowrLib https://github.com/MrowrLib/Packages.git")

includes("*/xmake.lua")
