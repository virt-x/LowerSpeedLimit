-- include subprojects
includes("lib/commonlibsse-ng")

-- set project constants
set_project("LowerSpeedLimit")
set_version("0.0.0")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")
add_requires("xbyak")
add_requires("simpleini")

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- define targets
target("LowerSpeedLimit")
    add_rules("commonlibsse-ng.plugin", {
        name = "LowerSpeedLimit",
        author = "Gerald",
        description = "Plugin to enforce a lower effective limit for speedmult actorvalue"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
    add_packages("xbyak")
    add_packages("simpleini")

    if is_plat("windows") then
        add_defines("NOMINMAX")
    end
