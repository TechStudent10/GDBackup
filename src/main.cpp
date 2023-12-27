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

void copyFileToBackup(fs::path input, std::tm tm) {
    auto gdPath = getGDPath();
    std::stringstream timestampStr;
    auto timestamp = std::put_time(&tm, "%Y-%m-%d %Hh%Mm%Ss");
    timestampStr << timestamp;;
    auto backupPath = gdPath / fs::path("backups") / fs::path("backup " + timestampStr.str());
    std::cout << backupPath << std::endl;
    fs::create_directories(backupPath);
    fs::copy_file(gdPath / input.filename(), backupPath / input.filename(), fs::copy_options::overwrite_existing);
}

void mod_main(HMODULE) {
    // this creates a console window whenever the mod is injected
    // which is very useful for debugging, but make sure to remove
    // on release builds! :D
    matdash::create_console();
    std::string backupFiles[4] = {"CCLocalLevels.dat", "CCLocalLevels2.dat", "CCGameManager.dat", "CCGameManager2.dat"};

    std::time_t now_tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now_tt);

    for (auto& f : backupFiles) {
        copyFileToBackup(f, tm);
    }
    // TODO: compress this, handle that later
}