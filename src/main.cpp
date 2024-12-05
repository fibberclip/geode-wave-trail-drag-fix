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
        getFields()->isTrailActive = true; // Set the trail as active
        PlayerObject::activateStreak(); // Call the original method
    }

    void toggleStreak(bool active) {
        // Manually handle streak state, since deactivateStreak isn't usable
        getFields()->isTrailActive = active;
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
        if (!player) {
            CCMotionStreak::update(delta);
            return;
        }

        // Check trail activation status
        auto& playerFields = player->getFields(); // Correct way to access fields
        if (!playerFields.isTrailActive) {
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
