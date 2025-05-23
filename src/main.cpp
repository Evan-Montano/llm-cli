// src/main.cpp
#include <iostream>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include "train.hpp"
#include "chat.hpp"

using namespace ftxui;

void PrintHeader() {
	std::cout << "=====================================\n";
    std::cout << "               DownsGPT\n";
    std::cout << "      Custom LLM Engine v1.0.0\n";
    std::cout << "      Developed by Evan, 2025 \n";
    std::cout << "=====================================\n\n";
}

int main() {
	auto screen = ScreenInteractive::TerminalOutput();

	int selected = 0;
	std::vector<std::string> entries = {
		"Chat with DownsGPT",
		"Train Modal",
		"Exit"
	};

	Component menu = Radiobox(&entries, &selected);
	Component confirm_button = Button("Confirm", screen.ExitLoopClosure());

	auto component_container = Container::Vertical({
		menu,
		confirm_button
	});

	Component renderer = Renderer(component_container, [&] {
		return vbox({
			text("Select an option:"),
			separator(),
			menu->Render(),
			separator(),
			confirm_button->Render() | center
		});
	});

	screen.Loop(renderer);

	switch (selected) {
		case 0:
			PrintHeader();
			InitializeChatModule();
			break;
		case 1:
			PrintHeader();
			InitializeTrainModule();
			break;
		case 2:
			return 0;
			break;
		default:
			break;
	}

	std::cout << "\n\nPress Enter to exit...\n";
	std::cin.get();
	return 0;
}
