#include "fileUtils.hpp"

int better::saveFile(immer::flex_vector<immer::flex_vector<char>> contents, std::string filename) {
    return 0;
}

immer::flex_vector<immer::flex_vector<char>> better::readFile(std::string filename) {
    immer::flex_vector_transient<immer::flex_vector<char>> textContents {};
    std::string buffer {};

    std::ifstream infile {};
    infile.open(filename);

    while(std::getline(infile, buffer)) {
        textContents.push_back(stringToVector(buffer));
        buffer = "";
    }

    if(textContents[textContents.size() - 1].size() == 1 && textContents[textContents.size() - 1][0] == '\n') {
        textContents.take(textContents.size() - 2);
    }

    return textContents.persistent();
}

immer::flex_vector<char> better::stringToVector(std::string string) {
    immer::flex_vector_transient<char> vector {};
    for (const char character: string) {
        vector.push_back(character);
    }
    return vector.persistent();
}

std::filesystem::path better::fileDialog() {
    SDL_Window* window = SDL_CreateWindow("File", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 560, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    std::vector<std::filesystem::directory_entry> files {};
    std::vector<std::filesystem::directory_entry> folders {};

    std::filesystem::path path {std::filesystem::current_path()}; //start with current working directory
    for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
        if(std::filesystem::is_regular_file(entry.status())) {
            files.push_back(entry); //push back all the files to list of files
        }
        else if(std::filesystem::is_directory(entry.status())) {
            folders.push_back(entry); //push back all the folders to list of folders
        }
    }

    const std::string folderString {"Folder: "};
    const std::string fileString {"File: "};
    std::filesystem::path filePath {}; //store the filepath to return
    bool isFound {false};

    better::Text text; //this just displays the paths
    text.cursor = {0,0};
    for(const std::filesystem::directory_entry& entry : folders) {
        text.textEdit = text.textEdit.push_back(better::stringToVector(folderString + entry.path().filename().string()));
    }
    for(const std::filesystem::directory_entry& entry : files) {
        text.textEdit = text.textEdit.push_back(better::stringToVector(fileString + entry.path().filename().string()));
    }
    text.topColumnNumber = 0;
    text.topLineNumber = 0;

    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
    better::renderText(surface, text.textEdit, text.topLineNumber, text.topColumnNumber);
    SDL_UpdateWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));

    SDL_Event event;

    while(!isFound) {
        if(SDL_WaitEvent(&event)) {
            if(event.type == SDL_MOUSEWHEEL) {
                text = better::scroll(text, event, 35, 100);
                better::renderText(surface, text.textEdit, text.topLineNumber, text.topColumnNumber);
                SDL_UpdateWindowSurface(window);
                SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN) {
                text.cursor = better::findCursorPos(text.topLineNumber, text.topColumnNumber, event);
                if(text.cursor.row > text.textEdit.size() - 1) {
                    continue;
                }
                if(text.cursor.row < folders.size()) {
                    path.append(folders[text.cursor.row].path().string());
                    text = {};
                    files = {};
                    folders = {};

                    for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
                        if(std::filesystem::is_regular_file(entry.status())) {
                            files.push_back(entry);
                        }
                    else if(std::filesystem::is_directory(entry.status())) {
                            folders.push_back(entry);
                        }
                    }

                    text.cursor = {0,0};
                    for(const std::filesystem::directory_entry& entry : folders) {
                        text.textEdit = text.textEdit.push_back(better::stringToVector(folderString + entry.path().filename().string()));
                    }
                    for(const std::filesystem::directory_entry& entry : files) {
                        text.textEdit = text.textEdit.push_back(better::stringToVector(fileString + entry.path().filename().string()));
                    }
                    text.topColumnNumber = 0;
                    text.topLineNumber = 0;

                    better::renderText(surface, text.textEdit, text.topLineNumber, text.topColumnNumber);
                    SDL_UpdateWindowSurface(window);
                    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
                }
                else {
                    isFound = true;
                    filePath = files[text.cursor.row - folders.size()].path();
                    break;
                }
            }
        }
        
    }
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    return filePath;
}