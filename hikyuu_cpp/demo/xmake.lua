target("demo")
    set_kind("binary")
    set_default(false)
    
    add_options("hdf5", "mysql", "sqlite", "tdx", "feedback", "stacktrace", "spend_time")
    
    add_packages("boost", "spdlog", "fmt", "flatbuffers")
    add_includedirs("..")

    if is_plat("windows") then
        add_cxflags("-wd4267")
        add_cxflags("-wd4251")
    end

    if is_plat("windows") and get_config("kind") == "shared" then
        add_defines("HKU_API=__declspec(dllimport)")
        add_defines("SQLITE_API=__declspec(dllimport)")
    end
   
    -- add files
    add_files("./*.cpp")

    add_deps("hikyuu")
    
target_end()

