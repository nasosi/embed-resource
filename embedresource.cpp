#include <iostream>
#include <filesystem>
#include <fstream>

int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s {sym} {rsrc}\n\n"
                        "  Creates {sym}.c from the contents of {rsrc}\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    std::filesystem::path dst{argv[1]};
	std::filesystem::path src{argv[2]};

	std::string sym = src.filename().string();
	std::replace(sym.begin(), sym.end(), '.', '_');
	std::replace(sym.begin(), sym.end(), '-', '_');

	std::filesystem::create_directories(dst.parent_path());

    std::ofstream ofs{dst};

    std::ifstream ifs{src};

    ofs << "#include <stdlib.h>" << std::endl;
    ofs << "const char _resource_" << sym << "[] = {" << std::endl;

    size_t lineCount = 0;
    while (!ifs.eof())
    {
        char c;
        ifs.get(c);
        ofs << "0x" << std::hex << (c&0xff) << ", ";
        if (++lineCount == 10) {
            ofs << std::endl;
            lineCount = 0;
        }
    }


    ofs << "};" << std::endl;
    ofs << "const size_t _resource_" << sym << "_len = sizeof(_resource_" << sym << ");";

    return EXIT_SUCCESS;
}

