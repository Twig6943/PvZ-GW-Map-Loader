#pragma once

#include <SDK/Containers.hpp>
#include <SDK/String.hpp>

namespace fb {

class Asset : public DataContainer {
public:
    String& getName() { return m_assetName; }

private:
    /* 0x10 */ String m_assetName;

};

struct LevelDescriptionInclusionCategory {
    /* 0x00 */ String        Category;
    /* 0x00 */ Array<String> Mode;
};

class LevelDescriptionAsset : public Asset {
public:
    String& getLevelName() { return m_levelName; }
    Array<LevelDescriptionInclusionCategory>& getCategories() { return m_categories; }

    LevelDescriptionInclusionCategory* getCategory(const char* category);
    bool hasCategory(const char* category);

private:
    /* 0x18 */ String m_levelName;
    /* 0x20 */ Array<LevelDescriptionInclusionCategory> m_categories;

};

class LevelReportingAsset : public Asset {
public:
    RefArray<class LevelDescriptionAsset>& getBuiltLevels() {
        return m_builtLevels;
    }

    class LevelDescriptionAsset* getLevelDescription(const char* level) {
        for (auto* desc : m_builtLevels) {
            if (strcmp(level, desc->getLevelName().asCString()) == 0) {
                return desc;
            }
        }

        return nullptr;
    }

private:
    RefArray<class LevelDescriptionAsset> m_builtLevels;
};

} // namespace fb
