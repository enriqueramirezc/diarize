#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include "Wave.hpp"
#include "ReadFile.hpp"
#include "CSVWrite.hpp"
#include "WaveWrite.hpp"

void printUsage() {
	std::cout << "Usage:" << std::endl;
	std::cout << "  program <input> <output>        write windowed CSV for <input>" << std::endl;
	std::cout << "  program -p <input>              print metadata for <input>" << std::endl;
	std::cout << "  program -m <in0> <in1> ... -o <output>   merge mono files into one multi-channel file" << std::endl;
}

// Read a WAV file's metadata and samples, or return 0 on failure.
int readWave(const std::string& path, Wave& file) {
	std::ifstream ifs(path, std::ios::binary);
	if (!ifs.is_open()) {
		std::cout << "File " << path << " not found" << std::endl;
		return 0;
	}
	if (ReadFile().fetchMetadata(file, ifs) != 1) {
		std::cout << "Unable to read metadata for " << path << std::endl;
		return 0;
	}
	if (ReadFile().fetchData(file, ifs) != 1) {
		std::cout << "Unable to read data for " << path << std::endl;
		return 0;
	}
	return 1;
}

int printMetadata(const std::string& input) {
	Wave file;
	if (readWave(input, file) != 1) {
		return 0;
	}
	file.printHeaderInfo();
	return 1;
}

int writeCsv(const std::string& input, const std::string& output) {
	Wave file;
	if (readWave(input, file) != 1) {
		return 0;
	}
	file.printHeaderInfo();

	CSVWrite csv;
	if (csv.write(output, file.data.samples, file.format.SampleRate, file.format.NumChannels) == 1) {
		std::cout << "Output successfully written to " << output << std::endl;
		return 1;
	}
	std::cout << "Unable to write output to " << output << std::endl;
	return 0;
}

// Merge N mono WAV files into one N-channel WAV file, one input per channel.
int mergeChannels(const std::vector<std::string>& inputPaths, const std::string& output) {
	if (inputPaths.size() < 2) {
		std::cout << "Merge needs at least two input files" << std::endl;
		return 0;
	}

	std::vector<Wave> inputs(inputPaths.size());
	for (std::size_t i = 0; i < inputPaths.size(); i++) {
		if (readWave(inputPaths[i], inputs[i]) != 1) {
			return 0;
		}
		if (inputs[i].format.NumChannels != 1) {
			std::cout << inputPaths[i] << " is not mono (has " << inputs[i].format.NumChannels
				<< " channels); merge inputs must be mono" << std::endl;
			return 0;
		}
	}

	uint32_t sampleRate = inputs[0].format.SampleRate;
	uint16_t bitsPerSample = inputs[0].format.BitsPerSample;
	std::size_t numFrames = inputs[0].data.samples.size();
	for (std::size_t i = 1; i < inputs.size(); i++) {
		if (inputs[i].format.SampleRate != sampleRate || inputs[i].format.BitsPerSample != bitsPerSample) {
			std::cout << "Input files must share the same sample rate and bit depth" << std::endl;
			return 0;
		}
		numFrames = std::min(numFrames, inputs[i].data.samples.size());
	}

	int numChannels = static_cast<int>(inputs.size());
	std::vector<int16_t> interleaved(numFrames * numChannels);
	for (std::size_t frame = 0; frame < numFrames; frame++) {
		for (int ch = 0; ch < numChannels; ch++) {
			interleaved[frame * numChannels + ch] = inputs[ch].data.samples[frame];
		}
	}

	WaveWrite writer;
	if (writer.write(output, interleaved, sampleRate, static_cast<uint16_t>(numChannels), bitsPerSample) == 1) {
		std::cout << "Merged " << numChannels << " channels into " << output << std::endl;
		return 1;
	}
	std::cout << "Unable to write " << output << std::endl;
	return 0;
}

int main(int argc, char *argv[]) {

	/**
	 * -p <input> print metadata
	 * <input> <output> write data to csv
	 * -m <input0> <input1> ... <inputN> -o <output> merge multiple files into one
	 */

	std::vector<std::string> args(argv + 1, argv + argc);

	if (args.empty()) {
		printUsage();
		return 0;
	}

	if (args[0] == "-p") {
		if (args.size() != 2) {
			printUsage();
			return 0;
		}
		return printMetadata(args[1]) == 1 ? 0 : 0;
	}

	if (args[0] == "-m") {
		std::vector<std::string> mergeInputs;
		std::string output;
		for (std::size_t i = 1; i < args.size(); i++) {
			if (args[i] == "-o") {
				if (i + 1 >= args.size()) {
					printUsage();
					return 0;
				}
				output = args[++i];
			} else {
				mergeInputs.push_back(args[i]);
			}
		}
		if (output.empty() || mergeInputs.empty()) {
			printUsage();
			return 0;
		}
		return mergeChannels(mergeInputs, output) == 1 ? 0 : 0;
	}

	if (args.size() != 2) {
		printUsage();
		return 0;
	}

	writeCsv(args[0], args[1]);
	return 0;
}
