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
    file << "Time,Channel1,numSample\n";

    // Write data rows
    for (std::size_t i = 0; i < samples.size(); i++) {
        double time = static_cast<double>(i) / sampleRate;
        file << time << "," << samples[i] << "," << i << "\n";
    }

    // Close the file
    file.close();

    std::cout << "CSV file written successfully." << std::endl;
    return 1;
}
