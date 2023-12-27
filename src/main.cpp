#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <matdash/console.hpp>
#include <cocos2d.h>
#include <filesystem>

using namespace cocos2d;
namespace fs = std::filesystem;

fs::path getGDPath() {
    return fs::path(CCFileUtils::sharedFileUtils()->getWritablePath());
}

int getFolderContentLength(fs::path folder) {
    auto dirIter = fs::directory_iterator(folder);
    int count = 0;
    for (auto& p : dirIter) {
        std::cout << std::to_string(count) << std::endl;
        std::cout << p.path() << std::endl;
        if (p.is_directory()) count++;
    }
    return count - 1;
}

void copyFileToBackup(fs::path input, int backupIdx) {
    auto gdPath = getGDPath();
    auto backupPath = gdPath / fs::path("backups") / fs::path("backup" + std::to_string(backupIdx));
    std::cout << backupPath << std::endl;
    fs::create_directories(backupPath);
    fs::copy_file(gdPath / input.filename(), backupPath / input.filename(), fs::copy_options::overwrite_existing);
}

void mod_main(HMODULE) {
    // this creates a console window whenever the mod is injected
    // which is very useful for debugging, but make sure to remove
    // on release builds! :D
    matdash::create_console();
    int backupIdx = getFolderContentLength(getGDPath() / fs::path("backups")) + 1;
    std::string backupFiles[4] = {"CCLocalLevels.dat", "CCLocalLevels2.dat", "CCGameManager.dat", "CCGameManager2.dat"};
    for (auto& f : backupFiles) {
        copyFileToBackup(f, backupIdx);
    }
    // TODO: compress this, handle that later
}