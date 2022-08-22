#include <iostream>
#include <string>

static const int
	BLACK(30), RED(31), GREEN(32), ORANGE(33),
	BLUE(34), PURPLE(35), CYAN(36), LIGHT_GRAY(36),
	DARK_GRAY(90), LIGHT_RED(91), LIGHT_GREEN(92), YELLOW(93),
	LIGHT_BLUE(94), PINK(95), AQUA(96), WHITE(97);

static const int
	NORMAL(0),
	BOLD(1),
	UNDERLINE(4);

class Logger {
	public:
		Logger() {}
	
		void warn(std::string warning) {ansiprint(warning, YELLOW, NORMAL);}
		void err(std::string error) {ansiprint(error, RED, BOLD);}
		void info(std::string info) {ansiprint(info, WHITE, NORMAL);}

	private:
		 void ansiprint(std::string text, int color, int mode) {
			std::cout << "\e[" << mode << ";" << color << "m" << text << "\e[0m" << std::endl;
		}
};

