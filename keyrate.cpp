#include <windows.h>
#include <iostream>

#include <charconv>
#include <optional>
std::optional<DWORD> parse_dword(const char *in) {
    DWORD out;
    auto end = std::strchr(in, '\0');
    auto &&[ptr, ec] = std::from_chars(in, end, out);
    if (ec == std::errc() && ptr == end)
        return out;
    else
        return std::nullopt;
}

int main(int argc, char* argv[]) {
    FILTERKEYS keys = { sizeof(FILTERKEYS) };
    if (!SystemParametersInfo(SPI_GETFILTERKEYS, 0, (LPVOID) &keys, 0)) {
        std::cerr << "Error: System call failed. Unable to set keyrate.\n";
        return 1;
    }

    if (argc == 3) {
        auto new_delay = parse_dword(argv[1]);
        auto new_repeat = parse_dword(argv[2]);
        if (!new_delay || !new_repeat) {
            std::cerr << "Error: Unsupported Parameters.\n"
                << "  Make sure you suply numbers as <delay ms> and <repeat ms> values.\n";
            return 2;
        }
        std::cout << "Replace current values with\n"
            << "    Delay: " << keys.iDelayMSec << " -> " << *new_delay << '\n'
            << "    Rate: " << keys.iRepeatMSec << " -> " << *new_repeat << '\n'
            << "You sure, you want to replace the values? [Y/n]: ";
        char answer = '\0';
        std::cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            keys.iDelayMSec = *new_delay;
            keys.iRepeatMSec = *new_repeat;
            keys.dwFlags = FKF_FILTERKEYSON | FKF_AVAILABLE;
            if (!SystemParametersInfo(SPI_SETFILTERKEYS, 0, (LPVOID) &keys, 0)) {
                std::cerr << "Error: System call failed. Unable to set keyrate.\n";
                return 1;
            } else
                std::cout << "Success.\n";
        } else
            std::cout << "Abort.\n";
        return 0;
    } else if (argc == 1) {
        std::cout << "Current values are:\n"
            << "    Delay: " << keys.iDelayMSec << '\n'
            << "    Rate: " << keys.iRepeatMSec << '\n'
            << "To change the values, use \"keyrate <delay ms> <repeat ms>\"\n";
        return 0;
    } else {
        std::cout << "Error: Unsupported Parameters.\n"
            << "  Usage: keyrate <delay ms> <repeat ms>\n";
        return 2;
    }
}
