#include "CSVWrite.hpp"

int CSVWrite::write(const std::string& output, const std::vector<int16_t>& samples,
                     uint32_t sampleRate, int numChannels) {
    if (numChannels < 1) {
        std::cerr << "CSVWrite::write invalid channel count: " << numChannels << std::endl;
        return 0;
    }

    // Create and open a CSV file
    std::ofstream file(output);

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 0;
    }

    // samples are interleaved per frame (frame = one sample per channel)
    const std::size_t numFrames = samples.size() / numChannels;

    // Write header
    file << "Time";
    for (int ch = 0; ch < numChannels; ch++) {
      file << ",Ch" << ch << "Min,Ch" << ch << "Max";
    }
    file << ",numSample\n";

    const std::size_t N = 100; // frames per window
    for (std::size_t i = 0; i < numFrames; i += N) {
      std::size_t end = std::min(i + N, numFrames);

      std::vector<int16_t> minVal(numChannels);
      std::vector<int16_t> maxVal(numChannels);
      for (int ch = 0; ch < numChannels; ch++) {
        minVal[ch] = samples[i * numChannels + ch];
        maxVal[ch] = samples[i * numChannels + ch];
      }

      for (std::size_t j = i; j < end; j++) {
        for (int ch = 0; ch < numChannels; ch++) {
          int16_t sample = samples[j * numChannels + ch];
          if (sample < minVal[ch]) minVal[ch] = sample;
          if (sample > maxVal[ch]) maxVal[ch] = sample;
        }
      }

      double time = static_cast<double>(i) / sampleRate;
      file << time;
      for (int ch = 0; ch < numChannels; ch++) {
        file << "," << minVal[ch] << "," << maxVal[ch];
      }
      file << "," << i << "\n";
    }

    // Close the file
    file.close();

    std::cout << "CSV file written successfully." << std::endl;
    return 1;
}
