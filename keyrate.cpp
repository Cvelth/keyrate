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

#include "colorful_cout.hpp"
int replace_values(std::optional<DWORD> const &new_delay, std::optional<DWORD> const &new_repeat) {
	using namespace cf::f;
	if (!new_delay) {
		std::cerr << red << "Error" << white << ": Unsupported Parameter.\n"
			<< "  Make sure you suply number as <" << yellow << "delay ms" << white << "> value.\n";
		return 2;
	}
	if (!new_repeat) {
		std::cerr << red << "Error" << white << ": Unsupported Parameter.\n"
			<< "  Make sure you suply number as <" << yellow << "repeat_rate ms" << white << "> value.\n";
		return 2;
	}

	FILTERKEYS keys = { sizeof(FILTERKEYS) };
	if (!SystemParametersInfo(SPI_GETFILTERKEYS, 0, (LPVOID) &keys, 0)) {
		std::cerr << "Error: System call failed. Unable to set keyrate.\n";
		return 1;
	}

	std::cout << white << "Replace current values with\n"
		<< white << "    Delay: " << yellow << keys.iDelayMSec << white << " -> " << green << *new_delay << '\n'
		<< white << "    Rate: " << yellow << keys.iRepeatMSec << white << " -> " << green << *new_repeat << '\n'
		<< white << "You sure, you want to replace the values? ["
		<< green << "Y" << white << "/" << yellow << "n" << white << "]: ";
	char answer = '\0';
	std::cin >> answer;
	if (answer == 'y' || answer == 'Y') {
		keys.iDelayMSec = *new_delay;
		keys.iRepeatMSec = *new_repeat;
		keys.dwFlags = FKF_FILTERKEYSON | FKF_AVAILABLE;
		if (!SystemParametersInfo(SPI_SETFILTERKEYS, 0, (LPVOID) &keys, 0)) {
			std::cerr << red << "Error" << white << ": System call failed. Unable to set keyrate.\n";
			return 1;
		} else
			std::cout << green << "Success.\n";
	} else
		std::cout << red << "Abort.\n";
	return 0;
}

int current_values() {
	using namespace cf::f;
	FILTERKEYS keys = { sizeof(FILTERKEYS) };
	if (!SystemParametersInfo(SPI_GETFILTERKEYS, 0, (LPVOID) &keys, 0)) {
		std::cerr << red << "Error" << white << ": System call failed. Unable to set keyrate.\n";
		return 1;
	}

	std::cout << "Current values are:\n"
		<< "    Delay: " << yellow << keys.iDelayMSec << white << '\n'
		<< "    Rate: " << yellow << keys.iRepeatMSec << white << '\n'
		<< "To change the values, use \"" << blue_light << "keyrate" << white << " <" << yellow << "delay ms" << white << "> <" << yellow << "repeat ms" << white << ">\"\n";
	return 0;
}

int main(int argc, char* argv[]) {
	if (argc == 3)
		return replace_values(parse_dword(argv[1]), parse_dword(argv[2]));
	else if (argc == 1)
		return current_values();
	else {
		std::cout << "Error: Unsupported Parameters.\n"
			<< "  Usage: keyrate <delay ms> <repeat ms>\n";
		return 2;
	}
}
