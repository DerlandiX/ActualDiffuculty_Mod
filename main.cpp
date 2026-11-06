#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <vector>

using namespace geode::prelude;

class $modify(ActualDifficultyHUD, PlayLayer) {
    struct Fields {
        std::vector<double> m_clickTimestamps;
        cocos2d::CCSprite* m_difficultySprite = nullptr;
        int m_currentActiveTier = -1;
    };

    // Part 1: Initialize the HUD counter layer on screen
    bool init(GJGameLevel* level, bool useReplay, bool dontPlay) {
        if (!PlayLayer::init(level, useReplay, dontPlay)) return false;

        auto* uiLayer = this->m_uiLayer;
        if (!uiLayer) return true;

        // Start with the PILL capsule frame by default
        m_fields->m_difficultySprite = CCSprite::createWithSpriteFrameName("pill_na.png");
        if (m_fields->m_difficultySprite) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            
            // Set position layout in the top right corner
            m_fields->m_difficultySprite->setPosition({ winSize.width - 40.f, winSize.height - 40.f });
            m_fields->m_difficultySprite->setScale(0.6f);
            m_fields->m_difficultySprite->setID("actual-difficulty-counter"_spr);
            
            uiLayer->addChild(m_fields->m_difficultySprite, 100);
        }

        return true;
    }

    // Part 2: Record timestamps when clicks happen
    void handleButton(bool down, int button, bool isPlayer1) {
        PlayLayer::handleButton(down, button, isPlayer1);
        
        if (down && isPlayer1 && !m_fields->m_isDead) {
            double timeHead = m_soundManager->getBackgroundMusicTime();
            m_fields->m_clickTimestamps.push_back(timeHead);
        }
    }

    // Part 3: Calculate the live actual difficulty tier and update the face sprite
    void update(float dt) {
        PlayLayer::update(dt);

        if (!m_fields->m_difficultySprite) return;

        double totalDuration = m_levelLength;
        if (totalDuration <= 0.0) return;

        double overallCPS = m_fields->m_clickTimestamps.size() / totalDuration;
        int calculatedTier = 0; // 0 = pill_na.png

        if (m_fields->m_clickTimestamps.empty()) {
            calculatedTier = 0;
        } else if (overallCPS >= 25.0) {
            calculatedTier = 12; // ultimate_obsidian.png (CYCLOLCYC / OPEN WINDOW)
        } else if (overallCPS >= 12.0) {
            calculatedTier = 11; // impossible_demon.png (ILL Layers)
        } else {
            calculatedTier = 10; // extreme_demon.png (Standard fallbacks)
        }

        // Only switch frames when the tier shifts to keep game execution smooth
        if (calculatedTier != m_fields->m_currentActiveTier) {
            m_fields->m_currentActiveTier = calculatedTier;
            std::string textureFrameName = "pill_na.png";

            if (calculatedTier == 12) {
                textureFrameName = "ultimate_obsidian.png";
            } else if (calculatedTier == 11) {
                textureFrameName = "impossible_demon.png";
            } else if (calculatedTier == 10) {
                textureFrameName = "extreme_demon.png";
            }

            auto* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
            auto* newFrame = frameCache->spriteFrameByName(textureFrameName.c_str());
            if (newFrame) {
                m_fields->m_difficultySprite->setDisplayFrame(newFrame);
            }
        }
    }
};
