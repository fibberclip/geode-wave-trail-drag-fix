#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

// Intercept PlayerObject to track streak activation
class $modify (PlayerObject) {
    struct Fields {
        bool isTrailActive = false; // Tracks whether the trail is active
    };

    void activateStreak() {
        m_fields->isTrailActive = true; // Set the trail as active
        PlayerObject::activateStreak(); // Call the original method
    }

    void deactivateStreak() {
        m_fields->isTrailActive = false; // Set the trail as inactive
        PlayerObject::deactivateStreak(); // Call the original method
    }
};

// Modify CCMotionStreak to apply trail cutting
class $modify (CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.4f;    // Interval for the trail cutting (default: 0.4s)
        bool isCutting = false;      // Indicates whether the trail is currently being cut
    };

    virtual void update(float delta) {
        // Access the parent object (assumed to be PlayerObject)
        auto player = dynamic_cast<PlayerObject*>(this->getParent());
        if (!player || !player->m_fields->isTrailActive) {
            // Skip cutting logic if the trail isn't active
            CCMotionStreak::update(delta);
            return;
        }

        // Update trail cutting logic
        m_fields->elapsedTime += delta;

        if (m_fields->elapsedTime >= m_fields->cutInterval) {
            m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

            // Toggle cutting state
            if (m_fields->isCutting) {
                this->resumeStroke(); // Resumes the trail
            } else {
                this->stopStroke(); // Stops the trail for a bit
            }

            m_fields->isCutting = !m_fields->isCutting; // Flip the state
        }

        // Update the trail's behavior, applying the delta time
        CCMotionStreak::update(delta);
    }
};
