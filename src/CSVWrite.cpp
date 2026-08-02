#include "CSVWrite.hpp"

int CSVWrite::write(const std::string& output, const std::vector<int16_t>& samples,
                     uint32_t sampleRate, int numChannels) { // Mono for now
    if (numChannels != 1) {
        std::cerr << "CSVWrite::write only supports mono audio for now" << std::endl;
        return 0;
    }

    // Create and open a CSV file
    std::ofstream file(output);

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 0;
    }

    // Write header
    file << "Time,Min,Max,numSample\n";

    const std::size_t N = 100; // samples per window
    for (std::size_t i = 0; i < samples.size(); i += N) {
      std::size_t end = std::min(i + N, samples.size());

      auto minVal = samples[i];
      auto maxVal = samples[i];
      for (std::size_t j = i + 1; j < end; j++) {
        if (samples[j] < minVal) minVal = samples[j];
        if (samples[j] > maxVal) maxVal = samples[j];
      }

     double time = static_cast<double>(i) / sampleRate;
      file << time << "," << minVal << "," << maxVal << "," << i << "\n";
    }

    // Close the file
    file.close();

    std::cout << "CSV file written successfully." << std::endl;
    return 1;
}
