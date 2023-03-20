#pragma once

class TextObject
{
public:
    TextObject() {}
    std::string text;
    float4 color;
    float2 position;
    float scale;
    bool draw = true;
};

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    float2 Size;       // Size of glyph
    float2 Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

static class FontManager
{
public:
    static void InitFreetype();

    static std::map<char, Character> Characters;
};