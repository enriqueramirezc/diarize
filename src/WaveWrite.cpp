#include "WaveWrite.hpp"

// Write a canonical 44-byte header PCM WAV file with interleaved samples.
int WaveWrite::write(const std::string& output, const std::vector<int16_t>& samples,
                      uint32_t sampleRate, uint16_t numChannels, uint16_t bitsPerSample) {
    if (numChannels < 1) {
        std::cerr << "WaveWrite::write invalid channel count: " << numChannels << std::endl;
        return 0;
    }

    std::ofstream file(output, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 0;
    }

    const uint16_t audioFormat = 1; // PCM
    const uint16_t blockAlign = static_cast<uint16_t>(numChannels * (bitsPerSample / 8));
    const uint32_t byteRate = sampleRate * blockAlign;
    const uint32_t dataSize = static_cast<uint32_t>(samples.size() * sizeof(int16_t));
    const uint32_t fmtSize = 16;
    const uint32_t riffSize = 4 + (8 + fmtSize) + (8 + dataSize);

    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&riffSize), 4);
    file.write("WAVE", 4);

    file.write("fmt ", 4);
    file.write(reinterpret_cast<const char*>(&fmtSize), 4);
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);
    file.write(reinterpret_cast<const char*>(samples.data()), dataSize);

    file.close();

    std::cout << "WAV file written successfully." << std::endl;
    return 1;
}
