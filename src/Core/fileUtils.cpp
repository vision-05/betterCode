//! @file
//! This file handles all of the file stream functionality of the program

#include "fileUtils.hpp"

int better::saveFile(immer::flex_vector<immer::flex_vector<char>> contents, std::string filename) {
    std::string buffer {""};
    std::ofstream file;
    file.open(filename, std::ios::out);

    for(int i{}; i < contents.size(); ++i) {
        for(int j{}; j < contents[i].size(); ++j) {
            buffer.push_back(contents[i][j]);
        }
        buffer.push_back('\n');
        file << buffer;
        buffer = "";
    }

    return 0;
}

immer::flex_vector<immer::flex_vector<char>> better::readFile(std::string filename) {
    immer::flex_vector_transient<immer::flex_vector<char>> textContents {};
    std::string buffer {};

    std::ifstream infile {};
    infile.open(filename);
    if(!infile) {
        textContents.push_back({});
        return textContents.persistent();
    }

    while(std::getline(infile, buffer)) {
        textContents.push_back(stringToVector(buffer));
        buffer = "";
    }

    if(textContents[textContents.size() - 1].size() == 1 && textContents[textContents.size() - 1][0] == '\n') {
        textContents.take(textContents.size() - 2);
    }
    if(textContents.size() == 0) {
        textContents.push_back({});
    }

    return textContents.persistent();
}

immer::flex_vector<char> better::stringToVector(std::string string) {
    immer::flex_vector_transient<char> vector {};
    for (const char character : string) {
        if(character == '\t') {
            for(int i{}; i < 4; ++i) {
                vector.push_back(' ');
            }
        }
        else {
            vector.push_back(character);
        }
    }
    return vector.persistent();
}

better::Text better::filedialog::mouseButtonDown(better::Text text, SDL_Event event) {
    
}

better::Text better::filedialog::keyDown(better::Text text, SDL_Event event) {
    
}

better::Text better::filedialog::mouseMotion(better::Text text, SDL_Event event) {
    
}

better::Text better::filedialog::mouseWheel(better::Text text, SDL_Event event) {
    
}

std::filesystem::path better::fileDialog(std::optional<std::filesystem::path> folderPath) {
    SDL_Window* window = SDL_CreateWindow("File", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 560, 0);
    std::string menu {"Back"};
    SDL_Rect screen {.x = 0, .y = 16, .w = 800, .h = 560 - 16};

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    std::vector<std::filesystem::directory_entry> files {};
    std::vector<std::filesystem::directory_entry> folders {};

    std::filesystem::path path {std::filesystem::current_path()}; //start with current working directory
    if(folderPath != std::nullopt) {
        path = *folderPath;
    }
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

    better::Text text
    {
        immer::flex_vector<immer::flex_vector<char>>(), //textEdit
        {0,0}, //cursor
        { //data start
            {false, false, false, false}, //menusToDraw
            false, //isShift
            false, //isCaps
            false, //isScroll
            false, //isCtrl
            false, //clearHistory
            0, //switchEditor
            -1, //index
            std::vector<std::string>(), //menu
            std::string(), //filename
            35, //textHeight
            100 //textWidth
        }, //data end
        0, //topLineNumber
        0, //topColumnNumber
        {0,0}, //highlightStart
        {0,0} //highlightEnd
    };
    
    std::vector<better::Text> texts {text};
    
    for(const std::filesystem::directory_entry& entry : folders) {
        text.textEdit = text.textEdit.push_back(better::stringToVector(folderString + entry.path().filename().string()));
    }
    for(const std::filesystem::directory_entry& entry : files) {
        text.textEdit = text.textEdit.push_back(better::stringToVector(fileString + entry.path().filename().string()));
    }

    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
    better::renderText(surface, text.textEdit, text.topLineNumber, text.topColumnNumber, 35, 100, text.highlightStart, text.highlightEnd, 0x222222FF, 0x5588AAFF, 0x222222FF, 0x5588AAFF, 0x5588AAFF, 0);
    better::drawMenuBar(surface, menu, 0xDDDDDDFF, 0x666666FF, 800, 0);
    SDL_UpdateWindowSurface(window);
    SDL_FillRect(surface, &screen, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));

    SDL_Event event;

    while(!isFound) {
        if(SDL_WaitEvent(&event)) {
            if(event.type == SDL_MOUSEWHEEL) {
                better::Text newText = better::scroll(text, event);
                text.textEdit = newText.textEdit;
                text.cursor = newText.cursor;
                text.topLineNumber = newText.topLineNumber;
                text.topColumnNumber = newText.topColumnNumber;
                text.data.isScroll = true;
                better::renderText(surface, text.textEdit, text.topLineNumber, text.topColumnNumber, 35, 100, text.highlightStart, text.highlightEnd, 0x222222FF, 0x5588AAFF, 0x222222FF, 0x5588AAFF, 0x5588AAFF, 0);
                SDL_UpdateWindowSurface(window);
                SDL_FillRect(surface, &screen, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN) {
                text.cursor = better::findCursorPos(text.topLineNumber, text.topColumnNumber, event, 0);
                if((text.cursor.row > text.textEdit.size() - 1) && text.cursor.row > -1) {
                    continue;
                }
                if(text.cursor.row < 0) {
                    if(text.cursor.column > 4) {
                        continue;
                    }
                    else {
                        path = path.parent_path();
                        text.textEdit = {};
                        text.cursor = {0,0};
                        text.topLineNumber = 0;
                        text.topColumnNumber = 0;
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
                            text.textEdit = text.textEdit.push_back(better::stringToVector(folderString + entry.path().string()));
                        }
                        for(const std::filesystem::directory_entry& entry : files) {
                            text.textEdit = text.textEdit.push_back(better::stringToVector(fileString + entry.path().filename().string()));
                        }
                        text.topColumnNumber = 0;
                        text.topLineNumber = 0;

                        better::renderText(surface, text.textEdit, text.topLineNumber, text.topColumnNumber, 35, 100, text.highlightStart, text.highlightEnd, 0x222222FF, 0x5588AAFF, 0x222222FF, 0x5588AAFF, 0x5588AAFF, 0);
                        SDL_UpdateWindowSurface(window);
                        SDL_FillRect(surface, &screen, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
                        continue;
                    }
                }
                if(text.cursor.row < folders.size()) {
                    path = std::filesystem::path(folders[text.cursor.row].path().string()); //change line to append on windows
                    text.textEdit = {};
                    text.cursor = {0,0};
                    text.topColumnNumber = 0;
                    text.topLineNumber = 0;
                    text.highlightStart = {0,0};
                    text.highlightEnd = {0,0};
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

                    better::renderText(surface, text.textEdit, text.topLineNumber, text.topColumnNumber, 35, 100, text.highlightStart, text.highlightEnd, 0x222222FF, 0x5588AAFF, 0x222222FF, 0x5588AAFF, 0x5588AAFF, 0);
                    SDL_UpdateWindowSurface(window);
                    SDL_FillRect(surface, &screen, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
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
