// src/main.cpp
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;
using json = nlohmann::json;

void printHeader() {
	std::cout << "=====================================\n";
    std::cout << "               DownsGPT\n";
    std::cout << "      Custom LLM Engine v1.0.0\n";
    std::cout << "      Developed by Evan, 2025 \n";
    std::cout << "=====================================\n";
	std::cout << "Make a selection:\n";
}


int main() {
	// Init Project
	//printHeader();


	// Create JSON object
	// json tree;
	// tree["a"]["char"] = "a";
	// tree["a"]["next_chars"]["b"] = 3;
	// tree["a"]["next_chars"]["c"] = 1;
	// tree["a"]["children"]["b"]["char"] = "b";
	// tree["a"]["children"]["b"]["next_chars"]["x"] = 2;

	// // Write JSON to file
	// std::ofstream outfile("../data/character_modal.json");
	// outfile << tree.dump(4); // Pretty print with indent = 4
	// outfile.close();

	// std::cout << "JSON written to ../data/character_modal.json";

	// Read from JSON file
	// std::ifstream inFile("../data/character_modal.json");
	// json loadedTree;
	// inFile >> loadedTree;
	// inFile.close();

	// std::cout << "Loaded char at a: " << loadedTree["a"]["char"] << "\n";
	// std::cout << "Next char count for 'b': " << loadedTree["a"]["next_chars"]["b"] << "\n";


	// std::cout << "\n\nPress Enter to exit...\n";
	// std::cin.get();
	// return 0;

	auto screen = ScreenInteractive::TerminalOutput();

	int selected = 0;
	std::vector<std::string> entries = {
		"Option 1",
		"Option 2",
		"Exit"
	};

	// 1. Create components
	Component menu = Radiobox(&entries, &selected); // Fixed component name
	Component confirm_button = Button("Confirm", screen.ExitLoopClosure());

	// 2. Group interactive components in a container
	auto component_container = Container::Vertical({
		menu,
		confirm_button
	});

	// 3. Create renderer linked to the container
	Component renderer = Renderer(component_container, [&] {
		return vbox({
		text("Select an option:"),
		separator(),
		menu->Render(),
		separator(),
		confirm_button->Render() | center
		});
	});

	// 4. Run the UI loop
	screen.Loop(renderer);

	// Handle selection
	if (selected == 2) {
		return 0;
	}

	std::cout << "\n\nPress Enter to exit...";
	std::cin.get();
	return 0;
}
