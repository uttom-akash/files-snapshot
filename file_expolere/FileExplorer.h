#if !defined(FILE_EXPLORER)
#define FILE_EXPLORER

#include <filesystem>
#include <vector>
#include <iostream>
#include <queue>
#include <regex>

class FileExplorer
{
public:
    static std::vector<std::filesystem::path> getAllFiles(std::filesystem::path root)
    {
        auto rootIterator = std::filesystem::recursive_directory_iterator(root);
        std::vector<std::filesystem::path> filePaths;

        for (auto filepath : rootIterator)
        {
            if (filepath.is_directory())
            {
                continue;
            }
            else if (isIgnored(filepath.path().string()))
            {
                continue;
            }
            else
            {
                filePaths.push_back(filepath);
            }
        }

        return filePaths;
    }

    static std::vector<std::string> getAllDirsOfOneLevel(std::filesystem::path root)
    {
        std::vector<std::string> filePaths;
        try
        {
            auto iterator = std::filesystem::directory_iterator(root);
            for (auto filepath : iterator)
            {
                if (filepath.is_directory())
                {
                    filePaths.push_back(filepath.path().string());
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return filePaths;
    }

    static bool isIgnored(std::string path)
    {
        std::vector<std::regex> ignoredList = {std::regex(".*\\.git.*"),
                                               std::regex(".*\\.snap.*"),
                                               std::regex(".*\\.snap2.*"),
                                               std::regex(".*\\.vscode.*"),
                                               std::regex(".*storage.*"),
                                               std::regex(".*\\.exe")};
        for (auto ignoredPath : ignoredList)
        {
            if (std::regex_match(path, ignoredPath))
            {
                return true;
            }
        }

        return false;
    }

    static std::filesystem::path currentWorkingDirectory()
    {
        return std::filesystem::current_path();
    }

    static std::string searchSnapDir()
    {
        auto cwd = currentWorkingDirectory();
        auto dirs = getAllDirsOfOneLevel(cwd.string());
        std::regex pattren(".*\\.snap$");

        for (auto dir : dirs)
        {
            if (std::regex_match(dir, pattren))
            {
                return dir;
            }
        }
        return "";
    }
};

#endif // FILE_EXPLORER