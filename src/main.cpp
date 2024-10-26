#include <iostream>
#include <fstream>
#include <stdexcept>
struct TGAHeader {
    char idLength;
    char colorMapType;
    char imageType;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char pixelDepth;
    char imageDescriptor;

    TGAHeader()= default;
    TGAHeader(const TGAHeader& other) {
        idLength = other.idLength;
        colorMapType = other.colorMapType;
        imageType = other.imageType;
        colorMapOrigin = other.colorMapOrigin;
        colorMapLength = other.colorMapLength;
        colorMapDepth = other.colorMapDepth;
        xOrigin = other.xOrigin;
        yOrigin = other.yOrigin;
        width = other.width;
        height = other.height;
        pixelDepth = other.pixelDepth;
        imageDescriptor = other.imageDescriptor;
    }
};
struct Pixel {
    unsigned char BLUE;
    unsigned char GREEN;
    unsigned char RED;
};
struct TGAImage {
    TGAHeader header{};
    std::vector<Pixel> data;
};

TGAHeader readHeader(std::ifstream& file) {
    TGAHeader header{};
    file.read((&header.idLength), 1);
    file.read((&header.colorMapType), 1);
    file.read((&header.imageType), 1);
    file.read(reinterpret_cast<char*>(&header.colorMapOrigin), 2);
    file.read(reinterpret_cast<char*>(&header.colorMapLength), 2);
    file.read((&header.colorMapDepth), 1);
    file.read(reinterpret_cast<char*>(&header.xOrigin), 2);
    file.read(reinterpret_cast<char*>(&header.yOrigin), 2);
    file.read(reinterpret_cast<char*>(&header.width), 2);
    file.read(reinterpret_cast<char*>(&header.height), 2);
    file.read((&header.pixelDepth), 1);
    file.read((&header.imageDescriptor), 1);
    return header;
}
void writeHeader(TGAHeader& header, std::ofstream& file) {
    file.write((&header.idLength), 1);
    file.write((&header.colorMapType), 1);
    file.write((&header.imageType), 1);
    file.write(reinterpret_cast<char*>(&header.colorMapOrigin), 2);
    file.write(reinterpret_cast<char*>(&header.colorMapLength), 2);
    file.write((&header.colorMapDepth), 1);
    file.write(reinterpret_cast<char*>(&header.xOrigin), 2);
    file.write(reinterpret_cast<char*>(&header.yOrigin), 2);
    file.write(reinterpret_cast<char*>(&header.width), 2);
    file.write(reinterpret_cast<char*>(&header.height), 2);
    file.write((&header.pixelDepth), 1);
    file.write((&header.imageDescriptor), 1);
}
TGAImage readImage(const std::string& filePath) {
    TGAImage image;
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    image.header = readHeader(file);
    while (file){
        Pixel BGR{};
        file.read(reinterpret_cast<char*>(&BGR), 3);
        if (file.gcount() == 3) {
            image.data.push_back(BGR);
        }
    }
    file.close();
    return image;
}

void writeImageData(TGAImage image, const std::string& filename){
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Couldn't open file to write image data.");
    }
    file.write((&image.header.idLength), 1);
    file.write((&image.header.colorMapType), 1);
    file.write((&image.header.imageType), 1);
    file.write(reinterpret_cast<char*>(&image.header.colorMapOrigin), 2);
    file.write(reinterpret_cast<char*>(&image.header.colorMapLength), 2);
    file.write((&image.header.colorMapDepth), 1);
    file.write(reinterpret_cast<char*>(&image.header.xOrigin), 2);
    file.write(reinterpret_cast<char*>(&image.header.yOrigin), 2);
    file.write(reinterpret_cast<char*>(&image.header.width), 2);
    file.write(reinterpret_cast<char*>(&image.header.height), 2);
    file.write((&image.header.pixelDepth), 1);
    file.write((&image.header.imageDescriptor), 1);

    for (auto & pixel : image.data) {
        file.write(reinterpret_cast<char*>(&pixel.BLUE), 1);
        file.write(reinterpret_cast<char*>(&pixel.GREEN), 1);
        file.write(reinterpret_cast<char*>(&pixel.RED), 1);
    }
    file.close();
}

void writeImageData(TGAImage image, std::ofstream& file){
    if (!file) {
        throw std::runtime_error("Couldn't open file to write image data.");
    }
    file.write((&image.header.idLength), 1);
    file.write((&image.header.colorMapType), 1);
    file.write((&image.header.imageType), 1);
    file.write(reinterpret_cast<char*>(&image.header.colorMapOrigin), 2);
    file.write(reinterpret_cast<char*>(&image.header.colorMapLength), 2);
    file.write((&image.header.colorMapDepth), 1);
    file.write(reinterpret_cast<char*>(&image.header.xOrigin), 2);
    file.write(reinterpret_cast<char*>(&image.header.yOrigin), 2);
    file.write(reinterpret_cast<char*>(&image.header.width), 2);
    file.write(reinterpret_cast<char*>(&image.header.height), 2);
    file.write((&image.header.pixelDepth), 1);
    file.write((&image.header.imageDescriptor), 1);

    for (auto & pixel : image.data) {
        file.write(reinterpret_cast<char*>(&pixel.BLUE), 1);
        file.write(reinterpret_cast<char*>(&pixel.GREEN), 1);
        file.write(reinterpret_cast<char*>(&pixel.RED), 1);
    }
}

TGAImage multiply(TGAImage top, TGAImage bottom) {
    TGAImage ret{};
    ret.header = top.header;
    for (int i = 0; i < top.data.size(); i++) {
        float blue_intermediate = (static_cast<float>(top.data[i].BLUE) / 255.0f) * (static_cast<float>(bottom.data[i].BLUE) / 255.0f) * 255.0f + 0.5f;
        float green_intermediate = (static_cast<float>(top.data[i].GREEN) / 255.0f) * (static_cast<float>(bottom.data[i].GREEN) / 255.0f) * 255.0f + 0.5f;
        float red_intermediate = (static_cast<float>(top.data[i].RED) / 255.0f) * (static_cast<float>(bottom.data[i].RED) / 255.0f) * 255.0f + 0.5f;
        Pixel newBGR{};
        newBGR.BLUE = static_cast<unsigned char>(blue_intermediate);
        newBGR.GREEN = static_cast<unsigned char>(green_intermediate);
        newBGR.RED = static_cast<unsigned char>(red_intermediate);
        ret.data.push_back(newBGR);
    }
    return ret;
}

float blend(float np1, float np2) {
    return (1 - (1 - np1) * (1-np2))*255.0f + 0.5f;
}
TGAImage screen(TGAImage top, TGAImage bottom) {
    TGAImage ret;
    ret.header = top.header;
    for (int i = 0; i < top.data.size(); i++) {
        float blue_intermediate = blend(static_cast<float>(top.data[i].BLUE)/255.0f, static_cast<float>(bottom.data[i].BLUE)/255.0f);
        float green_intermediate = blend(static_cast<float>(top.data[i].GREEN)/255.0f, static_cast<float>(bottom.data[i].GREEN)/255.0f);
        float red_intermediate = blend(static_cast<float>(top.data[i].RED)/255.0f, static_cast<float>(bottom.data[i].RED)/255.0f);
        Pixel newBGR{};
        newBGR.BLUE = static_cast<unsigned char>(blue_intermediate);
        newBGR.GREEN = static_cast<unsigned char>(green_intermediate);
        newBGR.RED = static_cast<unsigned char>(red_intermediate);
        ret.data.push_back(newBGR);
    }
    return ret;
}

float piecewiseOverlay(float np1, float np2) {
    return np2 <= 0.5f ? 2 * np1 * np2*255.0f + 0.5f : (1 - (2 * (1 - np1) * (1 - np2)))*255.0f + 0.5f;
}
TGAImage overlay(TGAImage top, TGAImage bottom){
    TGAImage ret;
    ret.header = bottom.header;
    float np1, np2;
    for (int i = 0; i < bottom.data.size(); i++) {
        Pixel newBGR{};
        np1 = static_cast<float>(top.data[i].BLUE)/255.0f;
        np2 = static_cast<float>(bottom.data[i].BLUE)/255.0f;
        newBGR.BLUE = static_cast<unsigned char>(piecewiseOverlay(np1, np2));
        np1 = static_cast<float>(top.data[i].GREEN)/255.0f;
        np2 = static_cast<float>(bottom.data[i].GREEN)/255.0f;
        newBGR.GREEN = static_cast<unsigned char>(piecewiseOverlay(np1, np2));
        np1 = static_cast<float>(top.data[i].RED)/255.0f;
        np2 = static_cast<float>(bottom.data[i].RED)/255.0f;
        newBGR.RED = static_cast<unsigned char>(piecewiseOverlay(np1, np2));
        ret.data.push_back(newBGR);
    }
    return ret;
}

TGAImage subtract(TGAImage top, TGAImage bottom) {
    TGAImage ret;
    ret.header = top.header;
    for (int i = 0; i < top.data.size(); i++) {
        int blue_intermediate = static_cast<int>(top.data[i].BLUE) - static_cast<int>(bottom.data[i].BLUE) > 0 ? static_cast<int>(top.data[i].BLUE) - static_cast<int>(bottom.data[i].BLUE) : 0;
        int green_intermediate = static_cast<int>(top.data[i].GREEN) - static_cast<int>(bottom.data[i].GREEN) > 0 ? static_cast<int>(top.data[i].GREEN) - static_cast<int>(bottom.data[i].GREEN) : 0;
        int red_intermediate = static_cast<int>(top.data[i].RED) - static_cast<int>(bottom.data[i].RED) > 0 ? static_cast<int>(top.data[i].RED) - static_cast<int>(bottom.data[i].RED) : 0;
        Pixel newBGR{};
        newBGR.BLUE = static_cast<unsigned char>(blue_intermediate);
        newBGR.GREEN = static_cast<unsigned char>(green_intermediate);
        newBGR.RED = static_cast<unsigned char>(red_intermediate);
        ret.data.push_back(newBGR);
    }
    return ret;
}

int main() {
    std::string filepath1, filepath2, filepath3;
    TGAImage combination;
    TGAImage image, image1, image2, image3;
    try{
        // part 1
        filepath1 = "input/layer1.tga";
        filepath2 = "input/pattern1.tga";
        writeImageData(multiply(readImage(filepath1), readImage(filepath2)), "output/part1.tga");
        //part 2
        filepath1 = "input/layer2.tga";
        filepath2 = "input/car.tga";
        writeImageData(subtract(readImage(filepath1), readImage(filepath2)), "output/part2.tga");
        //part 3
        filepath1 = "input/layer1.tga";
        filepath2 = "input/pattern2.tga";
        combination = multiply(readImage(filepath1), readImage(filepath2));
        filepath3 = "input/text.tga";
        writeImageData(screen(combination, readImage(filepath3)), "output/part3.tga");
        //part 4
        filepath1 = "input/layer2.tga";
        filepath2 = "input/circles.tga";
        combination = multiply(readImage(filepath1), readImage(filepath2));
        filepath3 = "input/pattern2.tga";
        writeImageData(subtract(combination, readImage(filepath3)), "output/part4.tga");
        //part 5
        filepath1 = "input/layer1.tga";
        filepath2 = "input/pattern2.tga";
        writeImageData(overlay(readImage(filepath1), readImage(filepath2)), "output/part5.tga");
        //part 6
        filepath1 = "input/car.tga";
        image = readImage(filepath1);
        std::ofstream outFile("output/part6.tga", std::ios::binary);
        if (!outFile) {
            throw std::runtime_error("Couldn't open file for reading and/or writing");
        }
        writeHeader(image.header, outFile);
        int newGreen;
        for (auto & pixel : image.data) {
            newGreen = (int)pixel.GREEN + 200 > 255 ? 255 : (int)pixel.GREEN + 200;
            outFile.write(reinterpret_cast<char*>(&pixel.BLUE), 1);
            outFile.write(reinterpret_cast<char*>(&newGreen), 1);
            outFile.write(reinterpret_cast<char*>(&pixel.RED), 1);
        }
        outFile.close();
        //part 7
        filepath1 = "input/car.tga";
        image = readImage(filepath1);
        outFile.open("output/part7.tga", std::ios::binary);

        writeHeader(image.header, outFile);
        int newRed, newBlue;
        for (auto & pixel : image.data) {
            newRed = (int)pixel.RED * 4 > 255 ? 255 : (int)pixel.RED * 4;
            newBlue = 0;
            outFile.write(reinterpret_cast<char*>(&newBlue), 1);
            outFile.write(reinterpret_cast<char*>(&pixel.GREEN), 1);
            outFile.write(reinterpret_cast<char*>(&newRed), 1);
        }
        outFile.close();
        //part 8
        filepath1 = "input/car.tga";
        image = readImage(filepath1);
        outFile.open("output/part8_r.tga", std::ios::binary);

        writeHeader(image.header, outFile);
        for (auto & pixel : image.data) {
            outFile.write(reinterpret_cast<char*>(&pixel.RED), 1);
            outFile.write(reinterpret_cast<char*>(&pixel.RED), 1);
            outFile.write(reinterpret_cast<char*>(&pixel.RED), 1);
        }
        outFile.close();
        outFile.open("output/part8_g.tga", std::ios::binary);
        writeHeader(image.header, outFile);
        for (auto & pixel : image.data) {
            outFile.write(reinterpret_cast<char*>(&pixel.GREEN), 1);
            outFile.write(reinterpret_cast<char*>(&pixel.GREEN), 1);
            outFile.write(reinterpret_cast<char*>(&pixel.GREEN), 1);
        }
        outFile.close();
        outFile.open("output/part8_b.tga", std::ios::binary);
        writeHeader(image.header, outFile);
        for (auto & pixel : image.data) {
            outFile.write(reinterpret_cast<char*>(&pixel.BLUE), 1);
            outFile.write(reinterpret_cast<char*>(&pixel.BLUE), 1);
            outFile.write(reinterpret_cast<char*>(&pixel.BLUE), 1);
        }
        outFile.close();
        //part 9
        image1 = readImage("input/layer_red.tga");
        image2 = readImage("input/layer_green.tga");
        image3 = readImage("input/layer_blue.tga");
        outFile.open("output/part9.tga", std::ios::binary);
        writeHeader(image1.header, outFile);
        for (int i = 0; i < image1.data.size(); i++) {
            outFile.write(reinterpret_cast<char*>(&image3.data[i].BLUE), 1);
            outFile.write(reinterpret_cast<char*>(&image2.data[i].GREEN), 1);
            outFile.write(reinterpret_cast<char*>(&image1.data[i].RED), 1);
        }
        outFile.close();
        //part 10
        image1 = readImage("input/text2.tga");
        TGAImage reversed;
        reversed.header = image1.header;
        reversed.data = image1.data;
        std::reverse(reversed.data.begin(), reversed.data.end());
        writeImageData(reversed, "output/part10.tga");







    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}
