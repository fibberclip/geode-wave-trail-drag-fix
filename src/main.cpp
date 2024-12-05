#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for the trail cutting
        bool isCutting = false;      // Indicates whether the trail is currently being cut
        bool isTrailActive = false;  // Tracks whether the trail is naturally active
    };

    virtual void reset() {
        // Called when the trail is reset (e.g., stops existing naturally)
        m_fields->isTrailActive = false;
        m_fields->elapsedTime = 0.0f; // Reset timer
        m_fields->isCutting = false; // Reset cutting state
        CCMotionStreak::reset();
    }

    virtual void startStroke() {
        // Called when the trail starts existing naturally
        m_fields->isTrailActive = true;
        CCMotionStreak::startStroke();
    }

    virtual void update(float delta) {
        if (m_fields->isTrailActive) {
            // Only run cutting logic if the trail is active
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

                if (m_fields->isCutting) {
                    this->stopStroke(); // Stop the trail
                } else {
                    this->resumeStroke(); // Resume the trail
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip cutting state
            }
        }

        // Always update the base functionality
        CCMotionStreak::update(delta);
    }
};
