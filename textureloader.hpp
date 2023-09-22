#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <string>

class Palette;
class Texture;

class TextureLoader
{
public:
    static TextureLoader &getInstance();

    void loadPaletteFromFile(std::string filePath);
    Texture *newTextureFromFile(std::string filePath);

private:
    TextureLoader();
    ~TextureLoader();

};

#endif // IMAGELOADER_HPP
