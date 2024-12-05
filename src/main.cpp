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

    virtual void reset() override {
        // Called when the trail is reset (stops existing naturally)
        m_fields->isTrailActive = false;
        m_fields->elapsedTime = 0.0f; // Reset the timer
        m_fields->isCutting = false;  // Reset cutting state
        CCMotionStreak::reset();
    }

    void resumeStreak() override {
        // Called when the trail is resumed
        m_fields->isTrailActive = true;
        CCMotionStreak::resumeStreak();
    }

    void stopStreak() override {
        // Called when the trail is stopped
        m_fields->isTrailActive = false;
        CCMotionStreak::stopStreak();
    }

    virtual void update(float delta) override {
        if (m_fields->isTrailActive) {
            // Only run cutting logic if the trail is active
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

                // Toggle cutting state
                if (m_fields->isCutting) {
                    CCMotionStreak::resumeStreak(); // Resume the trail
                } else {
                    CCMotionStreak::stopStreak(); // Stop the trail for a bit
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip cutting state
            }
        }

        // Always update the base functionality
        CCMotionStreak::update(delta);
    }
};
