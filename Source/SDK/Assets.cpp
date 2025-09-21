#include <SDK/Assets.hpp>

fb::LevelDescriptionInclusionCategory* fb::LevelDescriptionAsset::getCategory(const char* category) {
    for (auto& cat : m_categories) {
        if (strcmp(category, cat.Category.asCString()) == 0) {
            return &cat;
        }
    }
    
    return nullptr;
}

bool fb::LevelDescriptionAsset::hasCategory(const char* category) {
    for (auto& cat : m_categories) {
        if (strcmp(category, cat.Category.asCString()) == 0) {
            return true;
        }
    }

    return false;
}