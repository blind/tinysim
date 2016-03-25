#include "TinyScreen.h"

#include <Wire.h>
#include <SPI.h>

#include <cstdio>

TinyScreen::TinyScreen(uint8_t type) {
    _addr = 0;
    _cursorX = 0;
    _cursorY = 0;
    _fontHeight = 0;
    _fontFirstCh = 0;
    _fontLastCh = 0;
    _bitDepth = 0;
    _flipDisplay = 0;
    _mirrorDisplay = 0;
    _colorMode = 0;
    _externalIO = 0;
    _type = 0;
}

void TinyScreen::startData(void) {
    writeGPIO(GPIO_RegData, GPIO_DATA_START);
}

void TinyScreen::startCommand(void) {
    writeGPIO(GPIO_RegData, GPIO_CMD_START);
}

void TinyScreen::endTransfer(void) {
    writeGPIO(GPIO_RegData, GPIO_TRANSFER_END);
}

void TinyScreen::begin(void) {
    setBrightness(5);
    writeRemap();
    clearWindow(0, 0, 96, 64);
    on();
}

void TinyScreen::on(void) {
    startCommand();
    SPI.transfer(0xAF);//display on
    endTransfer();
}

void TinyScreen::off(void) {
    startCommand();
    SPI.transfer(0xAE);//display off
    endTransfer();
    writeGPIO(GPIO_RegData,~GPIO_SHDN);//bost converter off
}

void TinyScreen::setFlip(uint8_t flip) {
    _flipDisplay = flip;
    writeRemap();
}

void TinyScreen::setMirror(uint8_t mirror) {
    _mirrorDisplay = mirror;
    writeRemap();
}

void TinyScreen::setBitDepth(uint8_t depth) {
    _bitDepth = depth;
    writeRemap();
}

void TinyScreen::setColorMode(uint8_t mode) {
    _colorMode = mode;
    writeRemap();
}

void TinyScreen::setBrightness(unsigned char brightness) {
    if (brightness > 15)
        brightness = 15;

    startCommand();
    SPI.transfer(0x87);//set master current
    SPI.transfer(brightness);
    endTransfer();
}

void TinyScreen::writeRemap(void) {
    uint8_t remap = (1 << 5) | (1 << 2);

    if (_flipDisplay)
        remap |= ((1 << 4) | (1 << 1));

    if (_mirrorDisplay)
        remap ^= (1 << 1);

    if (_bitDepth)
        remap |= (1 << 6);

    if (_colorMode)
        remap ^= (1 << 2);

    startCommand();
    SPI.transfer(0xA0);//set remap
    SPI.transfer(remap);
    endTransfer();
}

void TinyScreen::drawPixel(uint8_t x, uint8_t y, uint16_t color) {
    printf("TODO: drawPixel\n");
}

void TinyScreen::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t r, uint8_t g, uint8_t b) {
    printf("TODO: drawLine\n");
}

void TinyScreen::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color) {
    printf("TODO: drawLine\n");
}

void TinyScreen::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t f, uint8_t r, uint8_t g, uint8_t b) {
    printf("TODO: drawRect\n");
}

void TinyScreen::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t f, uint16_t color) {
    printf("TODO: drawRect\n");
}

void TinyScreen::clearWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    if (x > xMax || y > yMax)
        return;

    uint8_t x2 = x + w - 1;
    uint8_t y2 = y + h - 1;

    if (x2 > xMax)
        x2 = xMax;

    if (y2 > yMax)
        y2 = yMax;

    startCommand();
    SPI.transfer(0x25);//clear window
    SPI.transfer(x);
    SPI.transfer(y);
    SPI.transfer(x2);
    SPI.transfer(y2);
    endTransfer();
}

void TinyScreen::clearScreen(void) {
    printf("TODO: clearScreen\n");
}

void TinyScreen::writePixel(uint16_t color) {
    printf("TODO: writePixel\n");
}

void TinyScreen::writeBuffer(uint8_t *buffer, int count) {
    for (int i = 0; i < count; i++) {
        SPDR = buffer[i];
    }
}

void TinyScreen::setX(unsigned char x, unsigned char end) {
    if (x > xMax)
        x = xMax;

    if (end > xMax)
        end = xMax;

    startCommand();
    SPI.transfer(0x15);//set column
    SPI.transfer(x);
    SPI.transfer(end);
    endTransfer();
}

void TinyScreen::setY(unsigned char y, unsigned char end) {
    if (y > yMax)
        y = yMax;

    if (end > yMax)
        end = yMax;

    startCommand();
    SPI.transfer(0x75);//set row
    SPI.transfer(y);
    SPI.transfer(end);
    endTransfer();
}

void TinyScreen::goTo(uint8_t x, uint8_t y) {
    setX(x, xMax);
    setY(y, yMax);
}

uint8_t TinyScreen::getButtons(uint8_t buttonMask) {
    printf("TODO: getButtons\n");
    return 0;
}

uint8_t TinyScreen::getButtons(void) {
    return getButtons(TSButtonUpperLeft | TSButtonUpperRight | TSButtonLowerLeft | TSButtonLowerRight);
}

void TinyScreen::writeGPIO(uint8_t regAddr, uint8_t regData) {
    Wire.beginTransmission(GPIO_ADDR);
    Wire.write(regAddr);
    Wire.write(regData);
    Wire.endTransmission();
}

void TinyScreen::setFont(const FONT_INFO& fontInfo) {
    printf("TODO: setFont\n");
}

uint8_t TinyScreen::getFontHeight(const FONT_INFO& fontInfo) {
    return fontInfo.height;
}

uint8_t TinyScreen::getFontHeight() {
    printf("TODO: getFontHeight\n");
    return 0;
}

uint8_t TinyScreen::getPrintWidth(char *st) {
    printf("TODO: getPrintWidth\n");
    return 0;
}

void TinyScreen::setCursor(uint8_t x, uint8_t y) {
    printf("TODO: setCursor\n");
}

void TinyScreen::fontColor(uint16_t fg, uint16_t bg) {
    printf("TODO: fontColor\n");
}

size_t TinyScreen::write(uint8_t ch) {
    printf("TODO: write\n");
    return 1;
}

void TinyScreen::initDMA(void) {
    /* Nothing to do */
}

uint8_t TinyScreen::getReadyStatusDMA(void) {
    /* Always ready */
    return 1;
}

void TinyScreen::writeBufferDMA(uint8_t *buffer, int count) {
    writeBuffer(buffer, count);
}
