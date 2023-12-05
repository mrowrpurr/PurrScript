set_languages("c++20")

add_rules("mode.debug", "mode.releasedbg")

add_repositories("SkyrimScripting https://github.com/SkyrimScripting/Packages.git")
add_repositories("MrowrLib https://github.com/MrowrLib/Packages.git")

includes("Papyrus.lua")
includes("SkyScripts.lua")
includes("Papyrus/*/xmake.lua")
includes("SkyrimScripting/*/xmake.lua")
includes("SkyrimScripting/*/*/xmake.lua")
includes("PurrScript/*/xmake.lua")
includes("SkyScript/*/xmake.lua")
includes("Resources/*/xmake.lua")
includes("Examples/*/xmake.lua")
