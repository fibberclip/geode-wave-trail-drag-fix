#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <unordered_map>

using namespace geode::prelude;

static std::unordered_map<CCMotionStreak*, bool> streakStates;

static double cutFreq = 0.2; 
static std::string cuttingMode = "stopStroke"; 

$execute {
    listenForSettingChanges("cutting-freq", [](double value) {
        cutFreq = value; 
        CCLOG("Cutting Frequency updated: %f", cutFreq);
    });

    listenForSettingChanges("cutting-mode", [](std::string value) {
        cuttingMode = value; 
        CCLOG("Cutting Mode updated: %s", cuttingMode.c_str());
    });
}

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;
        bool isCutting = false;
    };

    virtual void update(float delta) {

        if (streakStates[this]) {
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= cutFreq) { 
                m_fields->elapsedTime -= cutFreq;

                if (m_fields->isCutting) {

                    if (cuttingMode == "stopStroke") {
                        this->stopStroke();
                    } else if (cuttingMode == "reset") {
                        this->reset();
                    }
                } else {
                    this->resumeStroke();
                }

                m_fields->isCutting = !m_fields->isCutting;
            }
        }

        CCMotionStreak::update(delta);
    }
};

class $modify(PlayerObject) {
    void activateStreak() {
        PlayerObject::activateStreak();

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = true; 
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak();

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false; 
            }
        }
    }
};
