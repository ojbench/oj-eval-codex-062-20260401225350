#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <string>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

// A simple file-backed storage without space reclamation.
// Stores a fixed-size T in binary and reserves 'info_len' ints as header.

template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    explicit MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = ) {
        if (FN != ) file_name = FN;
        file.open(file_name, std::ios::out | std::ios::binary | std::ios::trunc);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // Read the nth (1-based) int from the header into tmp
    void get_info(int &tmp, int n) {
        if (n > info_len || n <= 0) return;
        file.open(file_name, std::ios::in | std::ios::binary);
        file.seekg(static_cast<std::streamoff>(n - 1) * sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // Write tmp into the nth (1-based) int in the header
    void write_info(int tmp, int n) {
        if (n > info_len || n <= 0) return;
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(static_cast<std::streamoff>(n - 1) * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // Append object t to the end of file and return the starting byte index
    int write(T &t) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) {
            // If file does not exist yet, create and initialize header
            initialise(file_name);
            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        }
        file.seekp(0, std::ios::end);
        std::streamoff pos = file.tellp();
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
        return static_cast<int>(pos);
    }

    // Overwrite the object at the given byte index with t
    void update(T &t, const int index) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(static_cast<std::streamoff>(index), std::ios::beg);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    // Read the object at the given byte index into t
    void read(T &t, const int index) {
        file.open(file_name, std::ios::in | std::ios::binary);
        file.seekg(static_cast<std::streamoff>(index), std::ios::beg);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    // No space reclamation required in easy version
    void Delete(int /*index*/) {
        // Intentionally left blank
    }
};

#endif //BPT_MEMORYRIVER_HPP
