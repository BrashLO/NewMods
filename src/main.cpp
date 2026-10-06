#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <algorithm>

using namespace geode::prelude;

class $modify(NewModsMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        static bool checked = false;
        if (checked) return true;
        checked = true;

        auto self = Mod::get();
        bool firstRun = !self->hasSavedValue("known-mods");
        auto known = self->getSavedValue<std::vector<std::string>>("known-mods");

        std::vector<std::string> current;
        std::string newList;

        for (auto mod : Loader::get()->getAllMods()) {
            auto id = mod->getID();
            if (id == "geode.loader") continue;
            current.push_back(id);

            if (!firstRun && std::find(known.begin(), known.end(), id) == known.end()) {
                newList += "- " + mod->getName() + " (" + mod->getVersion().toVString() + ")\n";
            }
        }

        self->setSavedValue("known-mods", current);

        if (!newList.empty()) {
            Loader::get()->queueInMainThread([newList]() {
                FLAlertLayer::create("New Mods Installed", newList, "OK")->show();
            });
        }

        return true;
    }
};