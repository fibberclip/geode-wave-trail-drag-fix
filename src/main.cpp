#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <unordered_map>
#include <iostream>

using namespace geode::prelude;

// Static map to associate CCMotionStreak instances with their states
static std::unordered_map<CCMotionStreak*, bool> streakStates;

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = 0.2f;
        bool isCutting = false;
    };

    virtual void update(float delta) {
        // Check if this streak is active based on our custom state
        if (streakStates[this]) {
            m_fields->elapsedTime += delta;

            // Log for debugging
            std::cout << "Elapsed Time: " << m_fields->elapsedTime << std::endl;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval;

                // Log cutting state
                std::cout << "Cutting: " << m_fields->isCutting << std::endl;

                if (m_fields->isCutting) {
                    this->stopStroke();
                    std::cout << "Stopped Stroke!" << std::endl;
                } else {
                    this->resumeStroke();
                    std::cout << "Resumed Stroke!" << std::endl;
                }

                m_fields->isCutting = !m_fields->isCutting;
            }
        } else {
            // Directly stop the stroke if state is inactive
            this->stopStroke();
            std::cout << "Trail stopped due to inactive state!" << std::endl;
        }

        CCMotionStreak::update(delta);
    }
};

class $modify(PlayerObject) {
    void activateStreak() {
        PlayerObject::activateStreak(); // Call the original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = true; // Enable cutting logic
                // Ensure the streak starts with resumed stroke state
                streak->resumeStroke();
                std::cout << "Streak activated!" << std::endl;
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak(); // Call the original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false; // Disable cutting logic
                // Ensure the trail stops when reset
                streak->stopStroke();
                std::cout << "Streak reset, stopped trail!" << std::endl;
            }
        }
    }
};
