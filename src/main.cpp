#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify(CCMotionStreak)
{
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for the trail cutting
        bool isCutting = false;      // Indicates whether the trail is currently being cut
    };

    virtual void update(float delta) {
        // Call the base update first to ensure proper behavior
        CCMotionStreak::update(delta);

        // Check if the trail is currently active
        if (m_uNuPoints > 0 && m_bStroke) {
            // Update elapsed time
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

                // Toggle cutting state if the trail is active
                if (m_fields->isCutting) {
                    this->resumeStroke(); // Resume the trail
                    CCLOG("Trail Resumed");
                } else {
                    this->stopStroke(); // Stop the trail temporarily
                    CCLOG("Trail Stopped");
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip the state
            }
        } else {
            // Reset cutting state if trail isn't active
            if (m_fields->isCutting) {
                this->resumeStroke();
                m_fields->isCutting = false;
                CCLOG("Trail Reset");
            }
        }
    }

    virtual void draw() {
        // Handle the trail drawing based on the stroke state
        if (m_bStroke && m_uNuPoints > 0) {
            // If the trail is active and has points, draw the trail
            CCMotionStreak::draw();
        } else {
            // If the trail is inactive, prevent flickering and let fading happen naturally
            CCLOG("Drawing is skipped, trail not active");
        }
    }
};
