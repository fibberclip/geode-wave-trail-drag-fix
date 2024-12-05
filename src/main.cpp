#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <unordered_map>

using namespace geode::prelude;

// Hook into CCMotionStreak to apply the cutting effect
class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for cutting
        bool isCutting = false;      // Indicates if the trail is currently hidden
    };

    virtual void update(float delta) {
        // Check if the trail is active using m_bStroke
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

            m_fields->isCutting = !m_fields->isCutting; // Toggle cutting state
        }

        CCMotionStreak::update(delta); // Default behavior
    }
};

// Hook into PlayerObject to ensure trails work as expected
class $modify(PlayerObject) {
    void activateStreak() {
        if (m_regularTrail) {
            m_regularTrail->m_fields->elapsedTime = 0.0f; // Reset cutting timer
            m_regularTrail->m_fields->isCutting = false;  // Ensure trail starts visible
        }

        if (m_shipStreak) {
            m_shipStreak->m_fields->elapsedTime = 0.0f; // Reset cutting timer
            m_shipStreak->m_fields->isCutting = false;  // Ensure trail starts visible
        }

        PlayerObject::activateStreak(); // Call original
    }

    void resetStreak() {
        if (m_regularTrail) {
            m_regularTrail->m_fields->elapsedTime = 0.0f; // Reset cutting timer
            m_regularTrail->m_fields->isCutting = false;  // Reset to visible state
        }

        if (m_shipStreak) {
            m_shipStreak->m_fields->elapsedTime = 0.0f; // Reset cutting timer
            m_shipStreak->m_fields->isCutting = false;  // Reset to visible state
        }

        PlayerObject::resetStreak(); // Call original
    }
};
