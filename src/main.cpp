#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <matdash/console.hpp>
#include <cocos2d.h>
#include <filesystem>
#include <zip.h>

using namespace cocos2d;
namespace fs = std::filesystem;

fs::path getGDPath() {
    return fs::path(CCFileUtils::sharedFileUtils()->getWritablePath());
}

fs::path getBackupName(std::tm tm) {
    auto gdPath = getGDPath();
    std::stringstream timestampStr;
    auto timestamp = std::put_time(&tm, "%Y-%m-%d %Hh%Mm%Ss");
    timestampStr << timestamp;;
    return gdPath / fs::path("backups") / fs::path("backup " + timestampStr.str());
}

void copyFileToBackup(fs::path input, std::tm tm) {
    auto gdPath = getGDPath();
    auto backupPath = getBackupName(tm);
    std::cout << backupPath << std::endl;
    fs::create_directories(backupPath);
    fs::copy_file(gdPath / input.filename(), backupPath / input.filename(), fs::copy_options::overwrite_existing);
}

void compressAndDelete(std::vector<const char*> &backupFiles, fs::path backupPath) {
    auto name = backupPath.replace_extension("zip");
    struct zip_t *zip = zip_open(name.string().c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
    {
        for (auto& f : backupFiles) {
            zip_entry_open(zip, f);
            {
                zip_entry_fwrite(zip, (getGDPath() / fs::path(f)).string().c_str());
            }
            zip_entry_close(zip);
        }
    }
    zip_close(zip);
}

void mod_main(HMODULE) {
    // this creates a console window whenever the mod is injected
    // which is very useful for debugging, but make sure to remove
    // on release builds! :D
    // matdash::create_console();
    std::vector<const char*> backupFiles = {"CCLocalLevels.dat", "CCLocalLevels2.dat", "CCGameManager.dat", "CCGameManager2.dat"};

    std::time_t now_tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now_tt);

    // there is no need for this now
    // for (auto& f : backupFiles) {
    //     copyFileToBackup(f, tm);
    // }
    compressAndDelete(backupFiles, getBackupName(tm));
}