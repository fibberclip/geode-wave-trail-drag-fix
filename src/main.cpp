#include <Geode/Bindings.hpp>
#include <Geode/Modify.hpp>
#include <random>

using namespace geode::prelude;

class $modify(PlayerObject) {
    void update(float delta) {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        // Retrieve the setting (will return a shared_ptr to a Setting)
        auto setting = Mod::get()->getSetting("skip_probability");

        // Ensure the setting is valid and retrieve its value
        if (setting && setting->getType() == geode::SettingType::Float) {
            float skipProbability = setting->asFloat(); // Convert it to a float

            // Randomly skip trail updates based on probability
            if (dist(rng) < skipProbability) {
                return; // Skip trail updates for this frame
            }
        }

        PlayerObject::update(delta); // Call the original update logic
    }
};
