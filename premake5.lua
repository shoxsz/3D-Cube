OUTPUTDIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
name = "CUBOMAGICO"

workspace "CUBO_MAGICO"
   location "output/build"
   architecture "x64"
   configurations {
      "Debug",
      "Release"
   }

   project(name)
   location("output/build/" .. name)
   kind "ConsoleApp"
   language "C++"

   targetdir("output/bin/" .. OUTPUTDIR .. "/%{prj.name}")
   objdir("output/bin-int/" .. OUTPUTDIR .. "/%{prj.name}")

   links {
       "Opengl32.lib"
   }

   files{
      "src/**.hpp",
      "src/**.cpp"
   }

   filter "system:windows"
      cppdialect "C++17"
      staticruntime "on"
      systemversion "latest"

      defines{
         "EG_PLATFORM_WINDOWS"
      }

   filter "configurations:Debug"
      defines { "EG_DEBUG" }
      symbols "On"
      debugdir "output/run"

   filter "configurations:Release"
      defines { "EG_RELEASE" }
      optimize "On"