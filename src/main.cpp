class $modify(PlayerObject) {
    void update(float delta) {
        PlayerObject::update(delta); // Call the original update method

        // Check if the player is in a ground gamemode and touching the ground
        if ((m_hasGroundParticles || m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4) &&
            !m_isShip && !m_isSwing && !m_isDart) {
            // Ground mode and touching the ground
            if (m_regularTrail) {
                auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                if (streak) {
                    streakStates[streak] = false; // Disable cutting logic
                    streak->stopStroke();        // Ensure the trail stops
                }
            }
        } else {
            // In an air gamemode or not touching the ground
            if (m_regularTrail) {
                auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                if (streak) {
                    streakStates[streak] = true; // Enable cutting logic
                }
            }
        }
    }

    void activateStreak() {
        PlayerObject::activateStreak(); // Call the original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = true; // Enable cutting logic initially
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak(); // Call the original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false; // Disable cutting logic
            }
        }
    }
};
