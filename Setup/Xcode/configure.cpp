
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <dirent.h>
#include <sys/stat.h>

enum class SolutionFileItemType
{
    folder,
    item
};

class SolutionFileItem
{
public:
    SolutionFileItemType type = SolutionFileItemType::folder;
    std::weak_ptr<SolutionFileItem> parent;
    std::string name;
    std::string path;
    std::vector<std::shared_ptr<SolutionFileItem>> items;
};

class SolutionBuildConfiguration
{
public:
    SolutionBuildConfiguration() { }
    SolutionBuildConfiguration(std::string name) : name(std::move(name)) { }
    
    std::string name;
};

class SolutionProject;

class SolutionProjectDependency
{
public:
    std::shared_ptr<SolutionProject> target;
};

class SolutionProject
{
public:
    std::string name;
    std::string header_folder;
    std::string header_name;
    std::shared_ptr<SolutionBuildConfiguration> debug_config = std::make_shared<SolutionBuildConfiguration>("Debug");
    std::shared_ptr<SolutionBuildConfiguration> release_config = std::make_shared<SolutionBuildConfiguration>("Release");
    std::vector<std::shared_ptr<SolutionProjectDependency>> dependencies;
    std::vector<std::shared_ptr<SolutionFileItem>> source_files;
    std::vector<std::shared_ptr<SolutionFileItem>> frameworks;
};

class Solution
{
public:
    std::string name = "ClanLib";
    std::shared_ptr<SolutionBuildConfiguration> debug_config = std::make_shared<SolutionBuildConfiguration>("Debug");
    std::shared_ptr<SolutionBuildConfiguration> release_config = std::make_shared<SolutionBuildConfiguration>("Release");
    std::vector<std::shared_ptr<SolutionProject>> projects;
    std::shared_ptr<SolutionProject> aggr_target = std::make_shared<SolutionProject>();
    std::shared_ptr<SolutionFileItem> root_folder = std::make_shared<SolutionFileItem>();
    std::shared_ptr<SolutionFileItem> products_folder;
};

void flatten_tree_impl(std::vector<std::shared_ptr<SolutionFileItem>> &out, const std::shared_ptr<SolutionFileItem> &item)
{
    out.push_back(item);
    for (const auto &child : item->items)
    {
        flatten_tree_impl(out, child);
    }
}

std::vector<std::shared_ptr<SolutionFileItem>> flatten_tree(const std::shared_ptr<SolutionFileItem> &root)
{
    std::vector<std::shared_ptr<SolutionFileItem>> items;
    flatten_tree_impl(items, root);
    return items;
}

bool check_ignore_list(const std::string &name)
{
    for (const char *search : {
        "Win32",
        "win32",
        "Freetype",
        "freetype",
        "Linux",
        "linux",
        "X11",
        "x11",
        "GLX",
        "glx",
        "Android",
        "android",
        "WGL",
        "wgl",
        "GL1", // Not ported yet (and why bother? dinosaur tech!)
        "gl1",
        "ios", // To do: iOS need special targets in the project
        "IOS",
        "iOS",
        "AGL", // To do: rename AGL to iOS (those providers got nothing to do with 1450 Carbon Apple GL
        "agl",
        "clanapp.cpp"
    })
    {
        if (name.find(search) != std::string::npos)
            return true;
    }

    return false;
}

void add_source_files(std::string path, std::shared_ptr<Solution> solution, std::shared_ptr<SolutionProject> project, std::shared_ptr<SolutionFileItem> folder, bool folder_ignored = false)
{
    std::vector<std::string> subfolders;
    std::vector<std::string> files;
    
    DIR *dir = opendir(path.c_str());
    if (dir)
    {
        while (true)
        {
            dirent *ent = readdir(dir);
            if (ent == 0)
                break;
            
            if (ent->d_type == DT_DIR)
            {
                std::string name = ent->d_name;
                if (name != "." && name != "..")
                    subfolders.push_back(name);
            }
            else
            {
                files.push_back(ent->d_name);
            }
        }
        closedir(dir);
    }
    
    std::sort(subfolders.begin(), subfolders.end());
    std::sort(files.begin(), files.end());
    
    for (const auto &name : subfolders)
    {
        auto subfolder = std::make_shared<SolutionFileItem>();
        subfolder->path = name;
        subfolder->parent = folder;
        folder->items.push_back(subfolder);
        add_source_files(path + "/" + name, solution, project, subfolder, folder_ignored || check_ignore_list(name));
    }
    
    for (const auto &name : files)
    {
        bool is_cpp = name.length() > 4 && name.substr(name.length() - 4) == ".cpp";
        bool is_mm = name.length() > 3 && name.substr(name.length() - 3) == ".mm";
        bool is_h = name.length() > 2 && name.substr(name.length() - 2) == ".h";
        
        if (!is_cpp && !is_mm && !is_h)
            continue;
        
        auto item = std::make_shared<SolutionFileItem>();
        item->type = SolutionFileItemType::item;
        item->path = name;
        item->parent = folder;
        folder->items.push_back(item);
        
        if ((is_cpp || is_mm) && !folder_ignored && !check_ignore_list(name))
            project->source_files.push_back(item);
    }
}

std::shared_ptr<SolutionProject> add_project(std::string name, std::string header_name, std::shared_ptr<Solution> solution, std::shared_ptr<SolutionFileItem> sources, std::shared_ptr<SolutionFileItem> api)
{
    auto sources_subfolder = std::make_shared<SolutionFileItem>();
    sources_subfolder->path = name;
    sources_subfolder->parent = sources;
    sources->items.push_back(sources_subfolder);
    
    auto api_subfolder = std::make_shared<SolutionFileItem>();
    api_subfolder->path = name;
    api_subfolder->parent = api;
    api->items.push_back(api_subfolder);

    auto proj = std::make_shared<SolutionProject>();
    proj->name = "clan" + name;
    proj->header_folder = name;
    proj->header_name = header_name;
    add_source_files("Sources/API/" + name, solution, proj, api_subfolder);
    add_source_files("Sources/" + name, solution, proj, sources_subfolder);
    solution->projects.push_back(proj);
    return proj;
}

void add_main_api_headers(std::shared_ptr<SolutionFileItem> api)
{
    std::vector<std::string> files;
    
    DIR *dir = opendir("Sources/API");
    if (dir)
    {
        while (true)
        {
            dirent *ent = readdir(dir);
            if (ent == 0)
                break;
            
            if (ent->d_type != DT_DIR)
            {
                files.push_back(ent->d_name);
            }
        }
        closedir(dir);
    }
    
    std::sort(files.begin(), files.end());

    for (const auto &name : files)
    {
        bool is_h = name.length() > 2 && name.substr(name.length() - 2) == ".h";
        
        if (!is_h)
            continue;
            
        auto item = std::make_shared<SolutionFileItem>();
        item->type = SolutionFileItemType::item;
        item->path = name;
        item->parent = api;
        api->items.push_back(item);
    }
}

std::shared_ptr<Solution> generate_solution()
{
    std::shared_ptr<Solution> solution = std::make_shared<Solution>();
    
    auto sources = std::make_shared<SolutionFileItem>();
    auto api = std::make_shared<SolutionFileItem>();
    auto frameworks = std::make_shared<SolutionFileItem>();
    
    sources->path = "Sources";
    
    api->path = "API";
    
    frameworks->name = "Frameworks";
    
    sources->items.push_back(api);
    api->parent = sources;

    solution->products_folder = std::make_shared<SolutionFileItem>();
    solution->products_folder->name = "Products";
    
    solution->root_folder->items.push_back(sources);
    sources->parent = solution->root_folder;
    
    solution->root_folder->items.push_back(frameworks);
    frameworks->parent = solution->root_folder;
    
    solution->root_folder->items.push_back(solution->products_folder);
    solution->products_folder->parent = solution->root_folder;
    
    auto clanCore = add_project("Core", "core.h", solution, sources, api);
    auto clanDisplay = add_project("Display", "display.h", solution, sources, api);
    auto clanGL = add_project("GL", "gl.h", solution, sources, api);
    auto clanNetwork = add_project("Network", "network.h", solution, sources, api);
    auto clanSound = add_project("Sound", "sound.h", solution, sources, api);
    auto clanUI = add_project("UI", "ui.h", solution, sources, api);
    auto clanApp = add_project("App", "application.h", solution, sources, api);
    
    add_main_api_headers(api);

    return solution;
}

class XcodeProjWriter
{
public:
    XcodeProjWriter(std::shared_ptr<Solution> solution, std::string filename) : solution(std::move(solution)), output(filename)
    {
    }
    
    void write()
    {
        output << "// !$*UTF8*$!" << std::endl;
        output << "{" << std::endl;
        output << "   archiveVersion = 1;" << std::endl;
        output << "   classes = {" << std::endl;
        output << "   };" << std::endl;
        output << "   objectVersion = 46;" << std::endl;
        output << "   objects = {" << std::endl;
        
        pbx_build_file_section();
        pbx_container_item_proxy_section();
        pbx_file_reference_section();
        pbx_frameworks_build_phase_section();
        pbx_group_section();
        pbx_native_target_section();
        pbx_project_section();
        pbx_shell_script_section();
        pbx_sources_section();
        pbx_target_dependency_section();
        xc_build_configuration_section();
        xc_configuration_list_section();
        
        output << "   };" << std::endl;
        output << "   rootObject = " << id("PBXProject", solution.get())  << " /* Project object */;" << std::endl;
        output << "}" << std::endl;
    }
    
private:
    void pbx_build_file_section()
    {
        output << std::endl;
        output << "/* Begin PBXBuildFile section */" << std::endl;
        
        for (const auto &project : solution->projects)
        {
            for (const auto &item : project->source_files)
            {
                output << id("PBXBuildFile", item.get()) << " /* " << item->path << " in " << item->parent.lock()->path << " */ = {isa = PBXBuildFile; fileRef = " << id("PBXFileReference", item.get()) << " /* " << item->path << " */; };" << std::endl;
            }
        }
        
        output << "/* End PBXBuildFile section */" << std::endl;
    }

    void pbx_container_item_proxy_section()
    {
        output << std::endl;
        output << "/* Begin PBXContainerItemProxy section */" << std::endl;
        
        for (const auto &project : solution->projects)
        {
            output << id("PBXContainerItemProxy", project.get()) << " /* PBXContainerItemProxy */ = {" << std::endl;
            output << "    isa = PBXContainerItemProxy;" << std::endl;
            output << "    containerPortal = " << id("PBXProject", solution.get()) << " /* Project object */;" << std::endl;
            output << "    proxyType = 1;" << std::endl;
            output << "    remoteGlobalIDString = " << id("PBXNativeTarget", project.get()) << ";" << std::endl;
            output << "    remoteInfo = " << project->name << ";" << std::endl;
            output << "};" << std::endl;
        }
        
        output << "/* End PBXContainerItemProxy section */" << std::endl;
    }
    
    void pbx_file_reference_section()
    {
        output << std::endl;
        output << "/* Begin PBXFileReference section */" << std::endl;
        
        for (const auto &item : flatten_tree(solution->root_folder))
        {
            if (item->type == SolutionFileItemType::folder) continue;
            
            std::string file_type;
            if (item->path.length() > 4 && item->path.substr(item->path.length() - 4) == ".cpp")
                file_type = "sourcecode.cpp.cpp";
            else if (item->path.length() > 3 && item->path.substr(item->path.length() - 3) == ".mm")
                file_type = "sourcecode.mm.mm";
            else
                file_type = "sourcecode.c.h";

            output << id("PBXFileReference", item.get()) << " /* " << item->path << " */ = {isa = PBXFileReference; lastKnownFileType = " << file_type << "; path = \"" << item->path << "\"; sourceTree = \"<group>\"; };" << std::endl;
        }
        
        for (const auto &project : solution->projects)
        {
            output << id("PBXFileReference", project.get()) << " /* lib" << project->name << ".a */ = {isa = PBXFileReference; explicitFileType = archive.ar; includeInIndex = 0; path = lib" << project->name << ".a; sourceTree = BUILT_PRODUCTS_DIR; };" << std::endl;
        }
        
        output << "/* End PBXFileReference section */" << std::endl;
    }
    
    void pbx_frameworks_build_phase_section()
    {
        output << std::endl;
        output << "/* Begin PBXFrameworksBuildPhase section */" << std::endl;
        
        for (const auto &project : solution->projects)
        {
            output << id("PBXFrameworksBuildPhase", project.get()) << " /* Frameworks */ = {" << std::endl;
            output << "    isa = PBXFrameworksBuildPhase;" << std::endl;
            output << "    buildActionMask = 2147483647;" << std::endl;
            output << "    files = (" << std::endl;
            for (const auto &framework : project->frameworks)
            {
                output << "        " << id("PBXBuildFile", framework.get()) << " /* " << framework->path << " in " << framework->parent.lock()->path << " */," << std::endl;
            }
            output << "    );" << std::endl;
            output << "    runOnlyForDeploymentPostprocessing = 0;" << std::endl;
            output << "};" << std::endl;
        }
        
        output << "/* End PBXFrameworksBuildPhase section */" << std::endl;
    }
    
    void pbx_group_section()
    {
        output << std::endl;
        output << "/* Begin PBXGroup section */" << std::endl;
        
        for (const auto &item : flatten_tree(solution->root_folder))
        {
            if (item->type == SolutionFileItemType::item) continue;
            
            output << id("PBXGroup", item.get()) << " = {" << std::endl;
            output << "    isa = PBXGroup;" << std::endl;
            
            if (item == solution->root_folder)
            {
                output << "    usesTabs = 1;" << std::endl;
            }
            
            output << "    children = (" << std::endl;
            
            for (const auto &child : item->items)
            {
                if (child->type == SolutionFileItemType::item)
                    output << "        " << id("PBXFileReference", child.get()) << " /* " << child->path << " */," << std::endl;
                else
                    output << "        " << id("PBXGroup", child.get()) << " /* " << child->path << " */," << std::endl;
            }
            
            if (item == solution->products_folder)
            {
                for (const auto &project : solution->projects)
                {
                    output << "        " << id("PBXFileReference", project.get()) << " /* lib" << project->name << ".a */," << std::endl;
                }
            }
            
            output << "    );" << std::endl;
            if (!item->name.empty())
                output << "    name = \"" << item->name << "\";" << std::endl;
            if (!item->path.empty())
                output << "    path = \"" << item->path << "\";" << std::endl; // Only in folders linked to physical folder. relative to parent solution folder
            output << "    sourceTree = \"<group>\";" << std::endl;
            output << "};" << std::endl;
        }
        
        output << "/* End PBXGroup section */" << std::endl;
    }
    
    void pbx_aggregate_target_section()
    {
        output << std::endl;
        output << "/* Begin PBXAggregateTarget section */" << std::endl;

        output << id("PBXAggregateTarget", solution->aggr_target.get()) << " /* " << solution->name << " */ = {" << std::endl;
        output << "    isa = PBXAggregateTarget;" << std::endl;
        output << "    buildConfigurationList = " << id("XCConfigurationList", solution->aggr_target.get()) << " /* Build configuration list for PBXAggregateTarget \"" << solution->name << "\" */;" << std::endl;
        output << "    buildPhases = (" << std::endl;
        output << "    );" << std::endl;
        output << "    buildRules = (" << std::endl;
        output << "    );" << std::endl;
        output << "    dependencies = (" << std::endl;
        for (const auto &project : solution->projects)
        {
            output << "        " << id("PBXTargetDependency", project.get()) << " /* " << project->name << " */," << std::endl;
        }
        output << "    );" << std::endl;
        output << "    name = " << solution->name << ";" << std::endl;
        output << "    productName = " << solution->name << ";" << std::endl;
        output << "};" << std::endl;
        
        output << "/* End PBXAggregateTarget section */" << std::endl;
    }
    
    void pbx_native_target_section()
    {
        output << std::endl;
        output << "/* Begin PBXNativeTarget section */" << std::endl;

        for (const auto &project : solution->projects)
        {
            output << id("PBXNativeTarget", project.get()) << " /* " << project->name << " */ = {" << std::endl;
            output << "    isa = PBXNativeTarget;" << std::endl;
            output << "    buildConfigurationList = " << id("XCConfigurationList", project.get()) << " /* Build configuration list for PBXNativeTarget \"" << project->name << "\" */;" << std::endl;
            output << "    buildPhases = (" << std::endl;
            output << "        " << id("PBXSourcesBuildPhase", project.get()) << " /* Sources */," << std::endl;
            output << "        " << id("PBXFrameworksBuildPhase", project.get()) << " /* Frameworks */," << std::endl;
            output << "        " << id("PBXShellScriptBuildPhase", project.get()) << " /* ShellScript */," << std::endl;
            output << "    );" << std::endl;
            output << "    buildRules = (" << std::endl;
            output << "    );" << std::endl;
            output << "    dependencies = (" << std::endl;
            output << "    );" << std::endl;
            output << "    name = " << project->name << ";" << std::endl;
            output << "    productName = " << project->name << ";" << std::endl;
            output << "    productReference = " << id("PBXFileReference", project.get()) << " /* lib" << project->name << ".a */;" << std::endl;
            output << "    productType = \"com.apple.product-type.library.static\";" << std::endl;
            output << "};" << std::endl;
        }
        
        output << "/* End PBXNativeTarget section */" << std::endl;
    }
    
    void pbx_project_section()
    {
        output << std::endl;
        output << "/* Begin PBXProject section */" << std::endl;
        
        output << id("PBXProject", solution.get()) << " /* Project object */ = {" << std::endl;
        output << "    isa = PBXProject;" << std::endl;
        output << "    attributes = {" << std::endl;
        output << "        LastUpgradeCheck = 0610;" << std::endl;
        output << "    };" << std::endl;
        output << "    buildConfigurationList = " << id("XCConfigurationList", solution.get()) << " /* Build configuration list for PBXProject \"ClanLib\" */;" << std::endl;
        output << "    compatibilityVersion = \"Xcode 3.2\";" << std::endl;
        output << "    developmentRegion = English;" << std::endl;
        output << "    hasScannedForEncodings = 0;" << std::endl;
        output << "    knownRegions = (" << std::endl;
        output << "        en," << std::endl;
        output << "    );" << std::endl;
        output << "    mainGroup = " << id("PBXGroup", solution->root_folder.get()) << ";" << std::endl;
        output << "    productRefGroup = " << id("PBXGroup", solution->products_folder.get()) << " /* Products */;" << std::endl;
        output << "    projectDirPath = \"\";" << std::endl;
        output << "    projectRoot = \"\";" << std::endl;
        output << "    targets = (" << std::endl;
        for (const auto &project : solution->projects)
        {
            output << "        " << id("PBXNativeTarget", project.get()) << " /* " << project->name << " */," << std::endl;
        }
        output << "    );" << std::endl;
        output << "};" << std::endl;
        
        output << "/* End PBXProject section */" << std::endl;
    }
    
    void pbx_shell_script_section()
    {
        output << std::endl;
        output << "/* Begin PBXShellScriptBuildPhase section */" << std::endl;

        for (const auto &project : solution->projects)
        {
            output << id("PBXShellScriptBuildPhase", project.get()) << " /* ShellScript */ = {" << std::endl;
            output << "    isa = PBXShellScriptBuildPhase;" << std::endl;
            output << "    buildActionMask = 2147483647;" << std::endl;
            output << "    files = (" << std::endl;
            output << "    );" << std::endl;
            output << "    inputPaths = (" << std::endl;
            output << "    );" << std::endl;
            output << "    outputPaths = (" << std::endl;
            output << "    );" << std::endl;
            output << "    runOnlyForDeploymentPostprocessing = 0;" << std::endl;
            output << "    shellPath = /bin/sh;" << std::endl;
            output << "    shellScript = \""
                "if [ -d \\\"Build/include/ClanLib/" << project->header_folder <<"\\\" ]; then "
                "rm -rf Build/include/ClanLib/" << project->header_folder << "/; "
                "fi \\n"
                "mkdir -p Build/include/ClanLib/ \\n"
                "cp -r \\\"${PROJECT_DIR}/Sources/API/" << project->header_folder << "\\\" \\\"Build/include/ClanLib/" << project->header_folder << "\\\" \\n"
                "cp \\\"${PROJECT_DIR}/Sources/API/" << project->header_name << "\\\" \\\"Build/include/ClanLib/" << project->header_name << "\\\" \\n"
                "\";" << std::endl;
            output << "};" << std::endl;
        }
        
        output << "/* End PBXShellScriptBuildPhase section */" << std::endl;
    }
    
    void pbx_sources_section()
    {
        output << std::endl;
        output << "/* Begin PBXSourcesBuildPhase section */" << std::endl;
        
        for (const auto &project : solution->projects)
        {
            output << id("PBXSourcesBuildPhase", project.get()) << " /* Sources */ = {" << std::endl;
            output << "    isa = PBXSourcesBuildPhase;" << std::endl;
            output << "    buildActionMask = 2147483647;" << std::endl;
            output << "    files = (" << std::endl;
            
            for (const auto &file : project->source_files)
            {
                output << "        " << id("PBXBuildFile", file.get()) << " /* " << file->path << " in " << file->parent.lock()->path << " */," << std::endl;
            }

            output << "    );" << std::endl;
            output << "    runOnlyForDeploymentPostprocessing = 0;" << std::endl;
            output << "};" << std::endl;
        }
        
        output << "/* End PBXSourcesBuildPhase section */" << std::endl;
    }
    
    void pbx_target_dependency_section()
    {
        output << std::endl;
        output << "/* Begin PBXTargetDependency section */" << std::endl;
        
        for (const auto &project : solution->projects)
        {
            for (const auto &dependency : project->dependencies)
            {
                output << id("PBXTargetDependency", dependency.get()) << " /* PBXTargetDependency */ = {" << std::endl;
                output << "    isa = PBXTargetDependency;" << std::endl;
                output << "    target = " << id("PBXNativeTarget", dependency->target.get()) << " /* " << dependency->target->name << " */;" << std::endl;
                output << "    targetProxy = " << id("PBXContainerItemProxy", dependency->target.get()) << " /* PBXContainerItemProxy */;" << std::endl;
                output << "};" << std::endl;
            }
        }
        
        output << "/* End PBXTargetDependency section */" << std::endl;
    }
    
    void xc_build_configuration_section()
    {
        output << std::endl;
        output << "/* Begin XCBuildConfiguration section */" << std::endl;
        
        output << id("XCBuildConfiguration", solution->debug_config.get()) << " /* " << solution->debug_config->name << " */ = {" << std::endl;
        output << "    isa = XCBuildConfiguration;" << std::endl;
        output << "    buildSettings = {" << std::endl;
        output << "        ALWAYS_SEARCH_USER_PATHS = NO;" << std::endl;
        output << "        CLANG_CXX_LANGUAGE_STANDARD = \"c++14\";" << std::endl;
        output << "        CLANG_CXX_LIBRARY = \"libc++\";" << std::endl;
        output << "        CLANG_ENABLE_OBJC_ARC = YES;" << std::endl;
        output << "        GCC_OPTIMIZATION_LEVEL = 0;" << std::endl;
        output << "        GCC_PREPROCESSOR_DEFINITIONS = (" << std::endl;
        output << "                \"DEBUG=1\"," << std::endl;
        output << "                \"$(inherited)\"," << std::endl;
        output << "        );" << std::endl;
        output << "        ONLY_ACTIVE_ARCH = YES;" << std::endl;
        output << "        USER_HEADER_SEARCH_PATHS = Sources;" << std::endl;
        output << "        DEPLOYMENT_LOCATION = YES;" << std::endl;
        output << "        DSTROOT = Build;" << std::endl;
        output << "    };" << std::endl;
        output << "    name = " << solution->debug_config->name << ";" << std::endl;
        output << "};" << std::endl;
        
        output << id("XCBuildConfiguration", solution->release_config.get()) << " /* " << solution->release_config->name << " */ = {" << std::endl;
        output << "    isa = XCBuildConfiguration;" << std::endl;
        output << "    buildSettings = {" << std::endl;
        output << "        ALWAYS_SEARCH_USER_PATHS = NO;" << std::endl;
        output << "        CLANG_CXX_LANGUAGE_STANDARD = \"c++14\";" << std::endl;
        output << "        CLANG_CXX_LIBRARY = \"libc++\";" << std::endl;
        output << "        CLANG_ENABLE_OBJC_ARC = YES;" << std::endl;
        output << "        USER_HEADER_SEARCH_PATHS = Sources;" << std::endl;
        output << "        DEPLOYMENT_LOCATION = YES;" << std::endl;
        output << "        DSTROOT = Build;" << std::endl;
        output << "    };" << std::endl;
        output << "    name = " << solution->release_config->name << ";" << std::endl;
        output << "};" << std::endl;
        
        for (const auto &project : solution->projects)
        {
            output << id("XCBuildConfiguration", project->debug_config.get()) << " /* " << project->debug_config->name << " */ = {" << std::endl;
            output << "    isa = XCBuildConfiguration;" << std::endl;
            output << "    buildSettings = {" << std::endl;
            output << "        PRODUCT_NAME = \"$(TARGET_NAME)\";" << std::endl;
            output << "        INSTALL_PATH = /lib/osx;" << std::endl;
            output << "        SDKROOT = macosx;" << std::endl;
            output << "    };" << std::endl;
            output << "    name = " << project->debug_config->name << ";" << std::endl;
            output << "};" << std::endl;
            
            output << id("XCBuildConfiguration", project->release_config.get()) << " /* " << project->release_config->name << " */ = {" << std::endl;
            output << "    isa = XCBuildConfiguration;" << std::endl;
            output << "    buildSettings = {" << std::endl;
            output << "        PRODUCT_NAME = \"$(TARGET_NAME)\";" << std::endl;
            output << "        INSTALL_PATH = /lib/osx;" << std::endl;
            output << "        SDKROOT = macosx;" << std::endl;
            output << "    };" << std::endl;
            output << "    name = " << project->release_config->name << ";" << std::endl;
            output << "};" << std::endl;
        }
        
        output << "/* End XCBuildConfiguration section */" << std::endl;
    }
    
    void xc_configuration_list_section()
    {
        output << std::endl;
        output << "/* Begin XCConfigurationList section */" << std::endl;
        
        output << id("XCConfigurationList", solution.get()) << " /* Build configuration list for PBXProject \"" << solution->name << "\" */ = {" << std::endl;
        output << "    isa = XCConfigurationList;" << std::endl;
        output << "    buildConfigurations = (" << std::endl;
        output << "        " << id("XCBuildConfiguration", solution->debug_config.get()) << " /* " << solution->debug_config->name << " */," << std::endl;
        output << "        " << id("XCBuildConfiguration", solution->release_config.get()) << " /* " << solution->release_config->name << " */," << std::endl;
        output << "    );" << std::endl;
        output << "    defaultConfigurationIsVisible = 0;" << std::endl;
        output << "    defaultConfigurationName = " << solution->release_config->name << ";" << std::endl;
        output << "};" << std::endl;
        
        for (const auto &project : solution->projects)
        {
            output << id("XCConfigurationList", project.get()) << " /* Build configuration list for PBXNativeTarget \"" << project->name << "\" */ = {" << std::endl;
            output << "    isa = XCConfigurationList;" << std::endl;
            output << "    buildConfigurations = (" << std::endl;
            output << "        " << id("XCBuildConfiguration", project->debug_config.get()) << " /* " << project->debug_config->name << " */," << std::endl;
            output << "        " << id("XCBuildConfiguration", project->release_config.get()) << " /* " << project->release_config->name << " */," << std::endl;
            output << "    );" << std::endl;
            output << "    defaultConfigurationIsVisible = 0;" << std::endl;
            output << "    defaultConfigurationName = " << project->release_config->name << ";" << std::endl;
            output << "};" << std::endl;
        }
        
        output << "/* End XCConfigurationList section */" << std::endl;
    }

    std::string generate_id()
    {
        unsigned int i = next_id++;
        std::string id = "B00BBABE1337F00D";
        for (int j = 0; j < 8; j++)
        {
            unsigned int v = (i >> (28 - j * 4)) & 0xf;
            if (v < 10)
                id.push_back('0' + v);
            else
                id.push_back('A' + v - 10);
        }
        return id;
    }
    
    std::string id(const char *obj_type, void *object)
    {
        auto &id = object_ids[obj_type][object];
        if (id.empty())
            id = generate_id();
        return id;
    }

    unsigned int next_id = 1;
    std::shared_ptr<Solution> solution;
    std::ofstream output;
    std::map<std::string, std::map<void*, std::string>> object_ids;
};

class XcodeWorkspaceWriter
{
public:
    XcodeWorkspaceWriter(const std::string &xcodeproj, const std::string &filename) : xcodeproj(xcodeproj), output(filename) { }
    
    void write()
    {
        output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        output << "<Workspace" << std::endl;
        output << "   version = \"1.0\">" << std::endl;
        output << "   <FileRef" << std::endl;
        output << "      location = \"self:" << xcodeproj << "\">" << std::endl;
        output << "   </FileRef>" << std::endl;
        output << "</Workspace>" << std::endl;
    }
    
    std::string xcodeproj;
    std::ofstream output;
};

int main(int argc, const char * argv[])
{
    std::cout << std::endl;
    std::cout << "Generating Xcode project files.." << std::endl;

    std::shared_ptr<Solution> solution = generate_solution();
    
    mkdir((solution->name + ".xcodeproj").c_str(), 0777);
    mkdir((solution->name + ".xcodeproj/project.xcworkspace").c_str(), 0777);
    
    XcodeProjWriter writer(solution, solution->name + ".xcodeproj/project.pbxproj");
    writer.write();
    
    XcodeWorkspaceWriter workspace_writer(solution->name + ".xcodeproj", solution->name + ".xcodeproj/project.xcworkspace/contents.xcworkspacedata");
    workspace_writer.write();

    std::cout << "Finished generating files." << std::endl;
    std::cout << std::endl;
    std::cout << "Please open ClanLib.xcodeproj in Xcode to build Clanlib." << std::endl;
    std::cout << std::endl;
    
    return 0;
}
