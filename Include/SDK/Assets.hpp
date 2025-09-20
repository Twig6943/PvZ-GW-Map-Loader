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

private:
    String m_levelName;
    Array<LevelDescriptionInclusionCategory> m_categories;

};

} // namespace fb
