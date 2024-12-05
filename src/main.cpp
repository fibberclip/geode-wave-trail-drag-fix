#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <unordered_map>

using namespace geode::prelude;

// Map to track the active state of each CCMotionStreak
static std::unordered_map<CCMotionStreak*, bool> trailStates;

// Hook into CCMotionStreak to apply the cutting effect
class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for cutting
        bool isCutting = false;      // Indicates if the trail is being cut
    };

    virtual void update(float delta) {
        // Check if this streak is active
        if (!trailStates[this]) {
            CCMotionStreak::update(delta); // Default behavior
            return;
        }

        // Apply the cutting effect
        m_fields->elapsedTime += delta;

        if (m_fields->elapsedTime >= m_fields->cutInterval) {
            m_fields->elapsedTime -= m_fields->cutInterval;

            if (m_fields->isCutting) {
                this->resumeStroke(); // Resume the trail
            } else {
                this->stopStroke(); // Temporarily cut the trail
            }

            m_fields->isCutting = !m_fields->isCutting; // Toggle state
        }

        CCMotionStreak::update(delta); // Default behavior
    }
};

// Hook into PlayerObject to track when the trail starts/stops
class $modify(PlayerObject) {
    void activateStreak() {
        // Get the current streak
        auto streak = this->getMotionStreak();
        if (streak) {
            trailStates[streak] = true; // Mark the trail as active
        }

        PlayerObject::activateStreak(); // Call original
    }

    void resetStreak() {
        // Get the current streak
        auto streak = this->getMotionStreak();
        if (streak) {
            trailStates[streak] = false; // Mark the trail as inactive
        }

        PlayerObject::resetStreak(); // Call original
    }
};
