newaction {
  trigger = 'clean',
  description = 'Cleans up the project.',
  shortname = "clean",

  execute = function()
    os.rmdir("bin")
    os.rmdir("build")
  end
}

saved_config = {}
function save_config()
  saved_config = configuration().terms
end
function restore_config()
  configuration(saved_config)
end

function windows_libdir(base_path)
  save_config()

  for _, arch in pairs({"x32", "x64"}) do
    for _, conf in pairs({"debug", "release"}) do
      for _, plat in pairs({"vs2008"}) do
        local conf_path = plat .. "/" .. arch .. "/" .. conf
        configuration { "windows", arch, conf, plat }
          libdirs { path.join(base_path, conf_path) }
      end
    end
  end

  restore_config()
end

solution "enet-plus"
  configurations { "debug", "release" }
  platforms { "x32", "x64" }

  location "build"
  targetdir "bin"

  flags { "FatalWarnings", "NoRTTI" }

  configuration { "linux" }
    flags { "NoExceptions" }

  configuration { "windows" }
    includedirs { "src/windows" }
    defines { "WIN32", "_WIN32" }
    defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE" }

  configuration { "debug" }
    defines { "DEBUG" }
    flags { "Symbols" }

  configuration { "release" }
    defines { "NDEBUG" }
    flags { "Optimize" }

  project "enet-plus"
    kind "SharedLib"
    language "C++"

    defines { "ENET_PLUS_DLL" }
    includedirs { "src" }
    files { "src/enet-plus/**.cpp",
            "src/enet-plus/**.hpp" }

    configuration "linux"
      links { "enet" }
    configuration "windows"
      includedirs { "third-party/enet/include" }      
      windows_libdir("third-party/enet/bin")
      links { "ws2_32", "winmm" }
      links { "enet" }

  project "sample-client"
    kind "ConsoleApp"
    language "C++"
    targetname "sample-client"

    includedirs { "src" }
    files { "src/sample-client/**.cpp",
            "src/sample-client/**.hpp" }

    links { "enet-plus" }

  project "sample-server"
    kind "ConsoleApp"
    language "C++"
    targetname "sample-server"

    includedirs { "src" }
    files { "src/sample-server/**.cpp",
            "src/sample-server/**.hpp" }

    links { "enet-plus" }
