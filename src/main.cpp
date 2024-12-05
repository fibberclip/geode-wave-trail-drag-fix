#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <unordered_map>

using namespace geode::prelude;

// Map to track additional fields for each streak
static std::unordered_map<CCMotionStreak*, bool> trailStates;

// Hook into CCMotionStreak to apply the cutting effect
class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for cutting
        bool isCutting = false;      // Indicates if the trail is being cut
    };

    virtual void update(float delta) {
        // Check if this streak is active using the m_bStroke field
        if (!m_bStroke) {
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

// Hook into PlayerObject to initialize/reset streaks
class $modify(PlayerObject) {
    void activateStreak() {
        auto streak = this->getMotionStreak(); // Obtain streak (if available)
        if (streak) {
            trailStates[streak] = true; // Mark trail as active
        }

        PlayerObject::activateStreak(); // Call original
    }

    void resetStreak() {
        auto streak = this->getMotionStreak(); // Obtain streak (if available)
        if (streak) {
            trailStates[streak] = false; // Mark trail as inactive
        }

        PlayerObject::resetStreak(); // Call original
    }
};
