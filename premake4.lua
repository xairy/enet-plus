newaction {
  trigger = 'clean',
  description = 'Cleans up the project.',
  shortname = "clean",
  
  execute = function()
    os.rmdir("bin")
    os.rmdir("build")
  end
}

solution "enet-plus"
  configurations { "debug", "release" }
  platforms { "x32", "x64" }

  location "build"
  targetdir "bin"
  
  flags { "FatalWarnings", "NoRTTI" }
  
  configuration { "linux" }
    flags { "NoExceptions" }
    
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
    files { "src/base/**.cpp",
            "src/base/**.hpp" }
     
    configuration "linux"
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
