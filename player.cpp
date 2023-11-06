#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <iomanip>
namespace fs = std::filesystem;

void displayPlaylist(const std::string& username, const std::string& playlistName) {
    std::string playlistPath = username + "/" + playlistName + ".txt";
    std::ifstream playlistFile(playlistPath);

    if (playlistFile.is_open()) {
        std::cout << "\nSongs in '" << playlistName << "':" << std::endl;
        std::cout << " +----+-------------------------------+" << std::endl;
        std::cout << " |  # |            Song Name            |" << std::endl;
        std::cout << " +----+-------------------------------+" << std::endl;

        std::string song;
        int songNumber = 1;
        while (std::getline(playlistFile, song)) {
            // Use setw to format the output with fixed widths
            std::cout << " | " << std::setw(2) << songNumber << " | " << std::setw(30) << song << " |" << std::endl;
            songNumber++;
        }

        std::cout << " +----+-------------------------------+" << std::endl;
        playlistFile.close();
    } else {
        std::cout << "Playlist not found or could not be opened." << std::endl;
    }
}

// Function to display the playlist management menu
void menu(const std::string& username) {
    while (true) {
        // List the user's playlists (names without extensions)
        std::vector<std::string> playlists;
        std::cout << " Your playlists:" << std::endl;
        std::cout << " +----+-------------------------------+" << std::endl;
        std::cout << " |  # |          Playlist Name          |" << std::endl;
        std::cout << " +----+-------------------------------+" << std::endl;

        int index = 1; // To display a playlist index
        for (const auto& entry : fs::directory_iterator(username)) {
            if (fs::is_regular_file(entry) && entry.path().filename().stem() != "user_info") {
                std::string playlistName = entry.path().filename().stem();
                playlists.push_back(playlistName);
                // Use setw to format the output with fixed widths
                std::cout << " | " << std::setw(2) << index << " | " << std::setw(30) << playlistName << " |" << std::endl;
                index++;
            }
        }

        std::cout << " +----+-------------------------------+" << std::endl;

        std::cout << "\nPlaylist Management Menu" << std::endl;
        std::cout << "1. Create a new playlist" << std::endl;
        std::cout << "2. Delete a playlist" << std::endl;
        std::cout << "3. Display songs" << std::endl;
        std::cout << "4. Back to the main menu" << std::endl;
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            // Implement creating a new playlist
            std::string playlistName;
            std::cout << "Enter the name of the new playlist: ";
            std::cin.ignore(); // Clear the newline character from the previous input
            std::getline(std::cin, playlistName);

            // Create a vector to store songs
            std::vector<std::string> songs;
            std::string song;
            while (true) {
                std::cout << "Enter a song (or 'done' to finish adding songs): ";
                std::getline(std::cin, song);
                if (song == "done") {
                    break;
                }
                songs.push_back(song);
            }

            // Create a new playlist text file and save songs
            std::ofstream playlistFile(username + "/" + playlistName + ".txt");
            for (const std::string& song : songs) {
                playlistFile << song << std::endl;
            }
            playlistFile.close();

            std::cout << "New playlist '" << playlistName << "' created." << std::endl;
        } else if (choice == 2) {
            // Implement deleting a playlist
            std::string playlistToDelete;
            std::cout << "Enter the name of the playlist to delete: ";
            std::cin.ignore(); // Clear the newline character from the previous input
            std::getline(std::cin, playlistToDelete);

            for (const auto& playlist : playlists) {
                if (playlist == playlistToDelete) {
                    std::string playlistPath = username + "/" + playlistToDelete + ".txt";
                    if (fs::remove(playlistPath)) {
                        std::cout << "Playlist '" << playlistToDelete << "' deleted." << std::endl;
                    } else {
                        std::cout << "Failed to delete the playlist. It may not exist." << std::endl;
                    }
                    break;
                }
            }
        } else if (choice == 3) {
            std::string playlistName;
            std::cout << "Enter the name of the playlist to display: ";
            std::cin.ignore(); // Clear the newline character from the previous input
            std::getline(std::cin, playlistName);
            displayPlaylist(username, playlistName);
        } else if (choice == 4) {
            return; // Return to the main menu
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

// Function to create a new user account
void signup() {
    std::string username, password;
    std::cout << "Enter your username: ";
    std::cin >> username;
    std::cout << "Enter your password: ";
    std::cin >> password;

    // Create a folder for the user if it doesn't exist
    if (!fs::exists(username)) {
        fs::create_directory(username);
    }

    // Save user information to a text file
    std::ofstream userInfoFile(username + "/user_info.txt");
    userInfoFile << "Username: " << username << "\nPassword: " << password << "\n";
    userInfoFile.close();

    std::cout << "Account created successfully." << std::endl;
}

// Function to login a user
std::string login() {
    std::string username, password;
    std::cout << "Enter your username: ";
    std::cin >> username;
    std::cout << "Enter your password: ";
    std::cin >> password;

    // Check if the user folder and user_info file exist
    if (fs::exists(username) && fs::exists(username + "/user_info.txt")) {
        // Check the password by reading from the user_info file
        std::ifstream userInfoFile(username + "/user_info.txt");
        std::string userInfo;
        while (std::getline(userInfoFile, userInfo)) {
            if (userInfo.find("Password: " + password) != std::string::npos) {
                std::cout << "Login successful." << std::endl;
                menu(username);
                return username;
            }
        }
        userInfoFile.close();
        std::cout << "Incorrect password. Please try again." << std::endl;
    } else {
        std::cout << "User does not exist. Please sign up." << std::endl;
    }

    return "";
}

int main() {
    while (true) {
        std::cout << "\nMusic Playlist Management System" << std::endl;
        std::cout << "1. Sign up" << std::endl;
        std::cout << "2. Log in" << std::endl;
        std::cout << "3. Quit" << std::endl;
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            signup();
        } else if (choice == 2) {
            std::string username = login();
        } else if (choice == 3) {
            std::cout << "Goodbye!" << std::endl;
            break;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}
