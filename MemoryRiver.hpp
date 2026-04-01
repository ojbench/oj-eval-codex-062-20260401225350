#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <string>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    explicit MemoryRiver(const string& fn) : file_name(fn) {}

    void initialise(string FN = string()) {
        if (!FN.empty()) file_name = FN;
        file.open(file_name, std::ios::out | std::ios::binary | std::ios::trunc);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    void get_info(int &tmp, int n) {
        if (n > info_len || n <= 0) return;
        file.open(file_name, std::ios::in | std::ios::binary);
        file.seekg(static_cast<std::streamoff>(n - 1) * sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    void write_info(int tmp, int n) {
        if (n > info_len || n <= 0) return;
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(static_cast<std::streamoff>(n - 1) * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    int write(T &t) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) {
            initialise(file_name);
            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        }
        file.seekp(0, std::ios::end);
        std::streamoff pos = file.tellp();
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
        return static_cast<int>(pos);
    }

    void update(T &t, const int index) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(static_cast<std::streamoff>(index), std::ios::beg);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    void read(T &t, const int index) {
        file.open(file_name, std::ios::in | std::ios::binary);
        file.seekg(static_cast<std::streamoff>(index), std::ios::beg);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    void Delete(int) {
        // no-op for easy version
    }
};

#endif //BPT_MEMORYRIVER_HPP
