#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

struct Pixel {
    int rowOffset;
    int colOffset;
    char ch;
};

class Shape {
public:
    virtual ~Shape() = default;
    virtual std::vector<Pixel> pixels() const = 0;
    virtual void advanceFrame() {}
};

class CharShape : public Shape {
    char ch;
public:
    explicit CharShape(char c) : ch{c} {}
    std::vector<Pixel> pixels() const override {
        return { Pixel{0, 0, ch} };
    }
};

class RectShape : public Shape {
    int h, w;
    char ch;
public:
    RectShape(int height, int width, char c) : h{height}, w{width}, ch{c} {}
    std::vector<Pixel> pixels() const override {
        std::vector<Pixel> pixels;
        pixels.reserve(h * w);
        for (int r = 0; r < h; ++r)
            for (int c = 0; c < w; ++c)
                pixels.push_back(Pixel{r, c, ch});
        return pixels;
    }
};

class BitmapShape: public Shape {
    std::vector<Pixel> data;
public:
    BitmapShape(std::vector<Pixel> pixels): data{std::move(pixels)}{}
    std::vector<Pixel> pixels() const override {
        return data;
    }
};

class AnimatedShape : public Shape {
    std::vector<std::vector<Pixel>> frames;
    int current = 0;
public:
    explicit AnimatedShape(std::vector<std::vector<Pixel>> f)
        : frames{std::move(f)} {}

    std::vector<Pixel> pixels() const override {
        if (frames.empty()) return {};
        return frames[current];
    }

    void advanceFrame() override {
        if (!frames.empty())
            current = (current + 1) % static_cast<int>(frames.size());
    }
};

#endif
