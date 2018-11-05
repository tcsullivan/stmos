/**
 * @file rba.cpp
 * A "really basic archiver"
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

int main(int argc, char *argv[])
{
	// check args
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " archive files...\n";
		return 0;
	}

	std::ofstream archive (argv[1], std::ios::out);
	for (int i = 2; i < argc; i++) {
		// attempt to read file contents
		std::string fileName (argv[i]);
		std::ifstream contents (fileName, std::ios::in | std::ios::ate);
		std::string shortName;
		if (auto pos = fileName.rfind('/'); pos != std::string::npos)
			shortName = fileName.substr(pos + 1);
		else
			shortName = fileName;

		if (contents.good()) {
			uint32_t size = contents.tellg();
			contents.seekg(0);
			std::cout << "Adding file " << fileName <<" ("
				<< size << " bytes)...\n";

			// read in file
			char *buffer = new char[size];
			contents.read(buffer, size);

			// write name size/text
			uint32_t nameSize = shortName.size();
			archive.write((char *)&nameSize, sizeof(uint32_t));
			archive.write(shortName.data(), nameSize);
			// write data size/content
			archive.write((char *)&size, sizeof(uint32_t));
			archive.write(buffer, size);

			delete[] buffer;
		} else {
			std::cout << "Failed to add file " << fileName
				<< ", continuing...\n";
		}
	}

	std::cout << "Created archive " << argv[1] << '\n';
	return 0;
}
