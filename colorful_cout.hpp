#pragma once
#ifndef _WIN64
#ifndef _WIN32
#error "Platform isn't supported."
#endif
#endif

#include <iostream>
#include <string>
#include <Windows.h>
namespace colorful {
	namespace detail {
		inline HANDLE get_handle(std::ostream &ostream) {
			if (&ostream == &std::cout)
				return GetStdHandle(STD_OUTPUT_HANDLE);
			else if (&ostream == &std::cerr)
				return GetStdHandle(STD_ERROR_HANDLE);
			else
				throw std::runtime_error("Unsupported ostream used.");
		}
		inline WORD get_attributes(HANDLE stream_handle) {
			CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
			if (!GetConsoleScreenBufferInfo(stream_handle, &screen_buffer_info))
				throw std::runtime_error("Unable to obtain console attributes: "
										 + std::to_string(GetLastError()));
			return screen_buffer_info.wAttributes;
		}
		inline void set_attributes(HANDLE stream_handle, WORD attributes) {
			if (!SetConsoleTextAttribute(stream_handle, attributes))
				throw std::runtime_error("Unable to change console attributes: "
										 + std::to_string(GetLastError()));
		}

		inline void change_color(std::ostream &ostream, unsigned short value, unsigned short mask = 0xff) {
			set_attributes(
				get_handle(ostream),
				(get_attributes(get_handle(ostream)) & ~mask) | (value & mask)
				);
		}
		inline void change_foreground(std::ostream &ostream, unsigned short value) {
			change_color(ostream, value, 0x0f);
		}
		inline void change_background(std::ostream &ostream, unsigned short value) {
			change_color(ostream, value << 4, 0xf0);
		}

		struct foreground_t { unsigned short value; };
		inline std::ostream &operator<<(std::ostream &ostream, foreground_t color) {
			change_foreground(ostream, color.value);
			return ostream;
		}

		struct background_t { unsigned short value; };
		inline std::ostream &operator<<(std::ostream &ostream, background_t color) {
			change_background(ostream, color.value);
			return ostream;
		}

		namespace color {
			enum color : unsigned short {
				black = 0,
				blue = 1,
				green = 2,
				aqua = 3,
				red = 4,
				purple = 5,
				yellow = 6,
				white = 7,

				grey = black + 8,
				blue_light = blue + 8,
				green_light = green + 8,
				aqua_light = aqua + 8,
				red_light = red + 8,
				purple_light = purple + 8,
				yellow_light = yellow + 8,
				white_bright = white + 8
			};
		}
	}

	namespace foreground {
		inline detail::foreground_t color(unsigned short color) { return { color }; }
		inline std::ostream&black(std::ostream &ostream) { return ostream << color(detail::color::black); }
		inline std::ostream&blue(std::ostream &ostream) { return ostream << color(detail::color::blue); }
		inline std::ostream&green(std::ostream &ostream) { return ostream << color(detail::color::green); }
		inline std::ostream&aqua(std::ostream &ostream) { return ostream << color(detail::color::aqua); }
		inline std::ostream&red(std::ostream &ostream) { return ostream << color(detail::color::red); }
		inline std::ostream&purple(std::ostream &ostream) { return ostream << color(detail::color::purple); }
		inline std::ostream&yellow(std::ostream &ostream) { return ostream << color(detail::color::yellow); }
		inline std::ostream&white(std::ostream &ostream) { return ostream << color(detail::color::white); }
		inline std::ostream&grey(std::ostream &ostream) { return ostream << color(detail::color::grey); }
		inline std::ostream&blue_light(std::ostream &ostream) { return ostream << color(detail::color::blue_light); }
		inline std::ostream&green_light(std::ostream &ostream) { return ostream << color(detail::color::green_light); }
		inline std::ostream&aqua_light(std::ostream &ostream) { return ostream << color(detail::color::aqua_light); }
		inline std::ostream&red_light(std::ostream &ostream) { return ostream << color(detail::color::red_light); }
		inline std::ostream&purple_light(std::ostream &ostream) { return ostream << color(detail::color::purple_light); }
		inline std::ostream&yellow_light(std::ostream &ostream) { return ostream << color(detail::color::yellow_light); }
		inline std::ostream&white_bright(std::ostream &ostream) { return ostream << color(detail::color::white_bright); }
	}
	namespace f { using namespace foreground; }

	namespace background {
		inline detail::background_t color(unsigned short color) { return { color }; }
		inline std::ostream &black(std::ostream &ostream) { return ostream << color(detail::color::black); }
		inline std::ostream &blue(std::ostream &ostream) { return ostream << color(detail::color::blue); }
		inline std::ostream &green(std::ostream &ostream) { return ostream << color(detail::color::green); }
		inline std::ostream &aqua(std::ostream &ostream) { return ostream << color(detail::color::aqua); }
		inline std::ostream &red(std::ostream &ostream) { return ostream << color(detail::color::red); }
		inline std::ostream &purple(std::ostream &ostream) { return ostream << color(detail::color::purple); }
		inline std::ostream &yellow(std::ostream &ostream) { return ostream << color(detail::color::yellow); }
		inline std::ostream &white(std::ostream &ostream) { return ostream << color(detail::color::white); }
		inline std::ostream &grey(std::ostream &ostream) { return ostream << color(detail::color::grey); }
		inline std::ostream &blue_light(std::ostream &ostream) { return ostream << color(detail::color::blue_light); }
		inline std::ostream &green_light(std::ostream &ostream) { return ostream << color(detail::color::green_light); }
		inline std::ostream &aqua_light(std::ostream &ostream) { return ostream << color(detail::color::aqua_light); }
		inline std::ostream &red_light(std::ostream &ostream) { return ostream << color(detail::color::red_light); }
		inline std::ostream &purple_light(std::ostream &ostream) { return ostream << color(detail::color::purple_light); }
		inline std::ostream &yellow_light(std::ostream &ostream) { return ostream << color(detail::color::yellow_light); }
		inline std::ostream &white_bright(std::ostream &ostream) { return ostream << color(detail::color::white_bright); }
	}
	namespace b { using namespace background; }
}
namespace cf { using namespace colorful; }