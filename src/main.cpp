#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = 0.2f;
        bool isCutting = false;
    };

    virtual void update(float delta) {
        // Avoid crashes by ensuring `CCMotionStreak` is initialized properly
        if (!this->isStartingPositionInitialized()) {
            CCMotionStreak::update(delta); // Call base class if not active
            return;
        }

        // Cutting logic
        m_fields->elapsedTime += delta;

        if (m_fields->elapsedTime >= m_fields->cutInterval) {
            m_fields->elapsedTime -= m_fields->cutInterval;

            if (m_fields->isCutting) {
                this->resumeStroke(); // Resume the trail
            } else {
                this->stopStroke(); // Stop the trail briefly
            }

            m_fields->isCutting = !m_fields->isCutting; // Toggle cutting state
        }

        // Always call the base update to ensure the trail works normally
        CCMotionStreak::update(delta);
    }
};
