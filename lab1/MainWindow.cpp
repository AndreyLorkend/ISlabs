#include "MainWindow.h"
#include <fstream>

int MainWindow::getIndex(std::unordered_set< char> S,  char K)
{
    int index = 0;
    for (auto u : S) {
        if (u == K)
            return index;
        index++;
    }
    return -1;
}

void MainWindow::fillArray()
{
    srand(time(0));
    for (int i = 0; i < ARR_LENGTH; i++) {
        arr[i] = rand() % 21 - 10;
    }
}

void MainWindow::fillAsciiTable()
{
    srand(12476);
    while (asciiTable.size() < 256) {
         char symbol = rand() % 256;
        asciiTable.insert(symbol);
    }
    std::unordered_set< char>::iterator startSymbol = asciiTable.begin();
    std::unordered_set< char>::iterator endSymbol = asciiTable.end();

    //int i = 0;
    //for (startSymbol; startSymbol != endSymbol;) {
    //    if (i < 16) {
    //        std::cout << *startSymbol;
    //        i++;
    //        startSymbol++;
    //    }
    //    else {
    //        std::cout << "\n";
    //        i = 0;
    //    }
    //}

    //std::cout << "\n\n";

    //for (int i = 0; i < 256; i++) {
    //    if (i % 16 != 0) {
    //        std::cout << (char)i;
    //    } else {
    //        std::cout << (char)i << "\n";
    //    }
    //}
}

void MainWindow::generateKey()
{
    srand(124369);
    while (keySet.size() < 16) {
        char symbol = rand() % 256;
        keySet.insert(symbol);
    }
}

MainWindow::MainWindow()
{
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    isExitFlagSet = false;
    fdwMode = 0;
    fillAsciiTable();
    generateKey();

    CryptoPP::AutoSeededRandomPool rng{};
    rng.GenerateBlock(gostKey.data(), gostKey.size());
    rng.GenerateBlock(gostIV.data(), gostIV.size());

    filenameIn = "otchet.pdf";
    filenameOut = "encoded_otcher.bin";
}

void MainWindow::printArray(COORD outCoord)
{
    DWORD cWritten = 0;
    std::string outText;
    std::list<CHAR*>textLines;
    char txt[10];
    for (int i = 0; i < ARR_LENGTH; i++) {
        wsprintf(txt, "  %d", arr[i]);
        outText.append(txt);
    }

    textLines.push_back(outText.data());

    for (auto& [key, value] : winBoxes) {
        value->handleWinElement("INITIAL_DATA", TEXT_BOX_ELEMENT, textLines, hStdout);
    }
}

VOID MainWindow::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
    switch (mer.dwEventFlags)
    {
    case 0:
        if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            for (auto &[key, value] : winBoxes) {
                value->checkClick(mer, this);
            }
        }
        break;
    default:
        break;
    }
}

VOID MainWindow::SetConsoleAppearance(WORD wAttributes)
{
    CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;
    COORD coordStart = { 0, 0 };
    DWORD dwNumWritten = 0;
    DWORD dwScrSize;

    if (!GetConsoleScreenBufferInfo(hStdout, &csbiScreenInfo))
        ErrorExit((LPSTR)"GetConsoleScreenBufferInfo");
    dwScrSize = csbiScreenInfo.dwMaximumWindowSize.X * csbiScreenInfo.dwMaximumWindowSize.Y;
    FillConsoleOutputAttribute(hStdout, wAttributes, dwScrSize, coordStart, &dwNumWritten);
    SetConsoleTextAttribute(hStdout, wAttributes);
}

VOID MainWindow::DrawInterface()
{
    // Adding LEGEND
    std::list<CHAR*> textLines;
    CHAR outText1[64];
    CHAR outText2[64];
    CHAR outText3[64];
    CHAR outText4[64];

    wsprintf(outText1, "<Close> for exit from program. ");
    textLines.push_back(outText1);

    wsprintf(outText2, "<Show data> click to print data from numeric array.");
    textLines.push_back(outText2);

    wsprintf(outText3, "<Handle data> click to handle data in numeric array. ");
    textLines.push_back(outText3);

    wsprintf(outText4, "<Fill array> fills original array.");
    textLines.push_back(outText4);

    // Drawing another elements
    for (auto& [key, value] : winBoxes) {
        value->drawElements(hStdout);
    }

    for (auto& [key, value] : winBoxes) {
        value->handleWinElement("LEGEND", TEXT_BOX_ELEMENT, textLines, hStdout);
    }

    textLines.clear();
}

void MainWindow::handleConsole()
{
    DWORD cNumRead = 0;
    while (!isExitFlagSet) {
        if (!ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead)) {
            ErrorExit((LPSTR)"ReadConsoleInput");
        }

        for (int i = 0; i < cNumRead; i++)
        {
            switch (irInBuf[i].EventType)
            {
            case MOUSE_EVENT: // mouse input
                MouseEventProc(irInBuf[i].Event.MouseEvent);
                break;
            default:
                break;
            }
        }
    }
}

bool MainWindow::SetConsoleParams()
{
    bool errorFlag = false;
    WORD attr = BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    SetConsoleAppearance(attr);

    if (hStdin == INVALID_HANDLE_VALUE) {
        errorFlag = ErrorExit((LPSTR)"GetStdHandle");
    }

    if (!GetConsoleMode(hStdin, &fdwSaveOldMode)) {
        errorFlag = ErrorExit((LPSTR)"GetConsoleMode");
    }

    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode)) {
        errorFlag = ErrorExit((LPSTR)"SetConsoleMode");
    }

    return errorFlag;
}

bool MainWindow::ErrorExit(LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);
    SetConsoleMode(hStdin, fdwSaveOldMode);
    return true;
}

void MainWindow::handleArrayData(COORD outCoord)
{
    std::list<CHAR*> textLines;
    CHAR outText1[32];
    CHAR outText2[32];

    wsprintf(outText1, "Negative numbers count: %d", findNegativeCount(ARR_LENGTH, arr));
    textLines.push_back(outText1);

    wsprintf(outText2, "Positive avrage arithmetic: %d", findPositiveArithmeticMean(ARR_LENGTH, arr));
    textLines.push_back(outText2);


    WinElement* element;
    for (auto& [key, value] : winBoxes) {
        value->handleWinElement("HANDLED_DATA", TEXT_BOX_ELEMENT, textLines, hStdout);
    }
    textLines.clear();
}

void MainWindow::encodePolybius()
{
    std::fstream dataFile("data.bin", std::ios_base::in | std::ios_base::binary);
    std::fstream writeFile("encoded.bin", std::ios::binary | std::ios::out);
    char temp;
    int column = 0;
    int row = 0;
    char rowSymbol;
    char columnSymbol;
    std::unordered_set<char>::iterator curentSymbol = keySet.begin();

    for (auto c : keySet) {
        std::cout << c;
    }

    std::cout << "\n";


    while (dataFile.read(&temp, sizeof( char))) {
        column = getIndex(asciiTable, static_cast<char>(temp)) % 16;
        row = getIndex(asciiTable, static_cast<char>(temp)) >> 4;

        //std::cout << row << " " << column << " : " << getIndex(asciiTable, temp) << "\n";

       std::unordered_set<char>::iterator ch = keySet.find(*curentSymbol);
       for (int i = 0; i < row; i++) {
           ch++;
       }

       char c = *ch;
       /*std::cout << c;*/
       writeFile.write((char*)&c, sizeof(char));

       ch = keySet.find(*curentSymbol);
       for (int i = 0; i < column; i++) {
           ch++;
       }
       c = *ch;
       /*std::cout << c;*/
       writeFile.write((char*)&c, sizeof(char));
    }

    std::cout << "\n";

    dataFile.close();
    writeFile.close();
    
}

void MainWindow::decodePolybius()
{
    char temp[2];
    int column = 0;
    int row = 0;
    int index = 0;

    std::unordered_set<char>::iterator startSymbol = asciiTable.begin();

    std::fstream dataFile("encoded.bin", std::ios_base::in | std::ios_base::binary);
    std::fstream writeFile("decoded.bin", std::ios::binary | std::ios::out);

    while (dataFile.read(temp, 2*sizeof(char))) {
        row = getIndex(keySet, static_cast< char>(temp[0]));
        column = getIndex(keySet, static_cast< char>(temp[1]));

        if (row == 1) {
            row--;
        }

        if (column == 0) {
            column++;
        }

        index = (row * 16) + (column);

        std::unordered_set< char>::iterator ch = asciiTable.find(*startSymbol);
        for (int i = 0; i < index; i++) {
            ch++;
        }

        //std::cout << row << " " << column << " : " << index << "\n";
        /*std::cout << (char)*ch;*/
        writeFile.write((char*)&ch, sizeof(char));
    }
    dataFile.close();
    writeFile.close();
}

void MainWindow::encodeGOST(const gost_key_t& key, const gost_iv_t& iv, const std::string& filename_in, const std::string& filename_out)
{
    CryptoPP::CFB_Mode<CryptoPP::GOST>::Encryption cipher{};
    cipher.SetKeyWithIV(key.data(), key.size(), iv.data());
    std::cout << key.data() << std::endl;
    std::cout << iv.data() << std::endl;

    std::ifstream in{ filename_in, std::ios::binary };
    std::ofstream out{ filename_out, std::ios::binary };

    CryptoPP::FileSource{ in, /*pumpAll=*/true,
                         new CryptoPP::StreamTransformationFilter{
                             cipher, new CryptoPP::FileSink{out}} };
    filenameIn = "encoded_otcher.bin";
    filenameOut = "decoded_otchet.pdf";
    in.close();
    out.close();
}

void MainWindow::decodeGOST(const gost_key_t& key, const gost_iv_t& iv, const std::string& filename_in, const std::string& filename_out)
{
    CryptoPP::CFB_Mode<CryptoPP::GOST>::Decryption cipher{};
    cipher.SetKeyWithIV(key.data(), key.size(), iv.data());

    std::ifstream in{ filename_in, std::ios::binary };
    std::ofstream out{ filename_out, std::ios::binary };

    CryptoPP::FileSource{ in, /*pumpAll=*/true,
                         new CryptoPP::StreamTransformationFilter{
                             cipher, new CryptoPP::FileSink{out}} };
    in.close();
    out.close();
}

gost_key_t MainWindow::getGostKey()
{
    return gostKey;
}

gost_iv_t MainWindow::getGostIV()
{
    return gostIV;
}

std::string MainWindow::getFilenameIn()
{
    return filenameIn;
}

std::string MainWindow::getFileNameOut()
{
    return filenameOut;
}

void MainWindow::Show()
{
    if (!SetConsoleParams()) {
        CreateElements();
        DrawInterface();
        handleConsole();
    }

    SetConsoleMode(hStdin, fdwSaveOldMode);
    system("cls");
}

HANDLE MainWindow::getHandleStdOut()
{
    return hStdout;
}

void MainWindow::setExitFlag(bool flag)
{
    isExitFlagSet = flag;
}

void MainWindow::CreateElements()
{
    //TEXT_BOX
    winBoxes.insert(std::pair<std::string, WinBox*>("TEXT_BOX_1", new WinBox(COORD(1, 1))));
    winBoxes["TEXT_BOX_1"]->addElementWidthVerticalAlignment(new TextBox(8, 110, "|Legend|", LEGEND));
    winBoxes["TEXT_BOX_1"]->addElementWidthVerticalAlignment(new TextBox(7, 110, "|Output|", OUTPUT));
    //BUTTON_BOX
    winBoxes.insert(std::pair<std::string, WinBox*>("BUTTON_BOX_1", new WinBox(COORD(1, 17))));
    //winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Show data", SHOW_DATA));
    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Encode by PolybiusSquare", ENCODE_POLYBIUS));
    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Decode by PolybiusSquare", DECODE_POLYBIUS));

    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Encode by GOST", ENCODE_GOST));
    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Decode by GOST", DECODE_GOST));

    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Close", CLOSE));
    //winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Fill array", FILL_ARRAY));
    //winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Handle data", HANDLE_DATA));
}
